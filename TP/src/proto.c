/**
 * Couche 7
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "data.h"
#include "user.h"
#include "proto.h"
#include "read_lib/read_lib.h"


void dialSrv2Clt(int socketDial, users_t *users){
	char buff[MAX_BUFF];
  	requete_t req;
	do {
		memset(buff, 0, MAX_BUFF);
		recevoirRequete(socketDial, &req);

    if(req.reqNum > 0) // commands
      traitementCommandes(socketDial, req, users);
    else // Messages
      traitementMessage(socketDial, req, users);

    //envoyerRequete(socketDial, "OK");

	} while (strcmp(buff,"/bye")!=0);
}

static void traitementLogin(int socket, requete_t req, users_t *users) {
	char cmd[100];
	char name[100];
	strcpy(name, "");
	sscanf(req.reqBuff, "%s%s", cmd, name);
	// Verifie la présence de l'arguement
	if (strcmp(name, "") == 0){
		envoyerRequeteWithReqNum(socket, "CMD_ERROR_MISSING_ARGUMENT", CMD_ERROR_MISSING_ARGUMENT);
		return;
	}
	// vérifie si le pseudo est libre
	user_t user = retrieveUserByName(users, name);
	if (!usersCompare(user, NULL_USER)){
		envoyerRequeteWithReqNum(socket, "CMD_ERROR_USERNAME_ALREADY_TAKEN", CMD_ERROR_USERNAME_ALREADY_TAKEN);
		return;
	}
	// verifie qu'il reste de la place
	if (users->size == MAX_USER){
		envoyerRequeteWithReqNum(socket, "CMD_ERROR_SERVER_FULL", CMD_ERROR_SERVER_FULL);
	}
	//ajoute l'user à la table
	if (addUser(users, name, socket) == -1 ){
		envoyerRequeteWithReqNum(socket, "CMD_ERROR_UNKNOW_ERROR", CMD_ERROR_UNKNOW_ERROR); // logiquement impossible
		return;
	}
	//renvoie le nom qui lui a été affecté
	envoyerRequeteWithReqNum(socket, name, CMD_LOGIN_NUM);
}

static void traitementList(int socket, users_t *users) {
	char stringListUser[23+(MAX_SIZE_NAME+2)*MAX_USER];
	listUserToString(users,stringListUser);
	envoyerRequeteWithReqNum(socket, stringListUser,CMD_LIST_NUM);
}

static void traitementTalk(int socket, users_t *users) {
	if(updateUserSocket(users, socket, EVERYONE_DESTIONATION_SOCKET)) {  // Impossible de mettre à jour l'utilisateur
		envoyerRequeteWithReqNum(socket, "CMD_ERROR_UNKNOW_ERROR", CMD_ERROR_UNKNOW_ERROR);
		return;
	}

	envoyerRequeteWithReqNum(socket, "", CMD_PRIVATE_NUM);
}

static void traitementPrivate(int socket, requete_t req, users_t *users) {
	char cmd[100];
	char name[100];
	strcpy(name, "");
	sscanf(req.reqBuff, "%s%s", cmd, name);
	// Verifie la présence de l'arguement
	if (strcmp(name, "") == 0){
		envoyerRequeteWithReqNum(socket, "CMD_ERROR_MISSING_ARGUMENT",CMD_ERROR_MISSING_ARGUMENT);
		return;
	}
	// Vérifie que l'utilisateur est connecté
	user_t receiver = retrieveUserByName(users, name);
	if (usersCompare(receiver, NULL_USER)){
		envoyerRequeteWithReqNum(socket, "CMD_ERROR_USER_DISCONNECTED", CMD_ERROR_USER_DISCONNECTED);
		return;
	}
	if(updateUserSocket(users, socket, receiver.socket)) { // Impossible de mettre à jour l'utilisateur
		envoyerRequeteWithReqNum(socket, "CMD_ERROR_UNKNOW_ERROR", CMD_ERROR_UNKNOW_ERROR);
		return;
	}

	envoyerRequeteWithReqNum(socket, receiver.name, CMD_PRIVATE_NUM);
}

static void traitementBye(int socket) {
	envoyerRequeteWithReqNum(socket, "", CMD_BYE_NUM);
}

void traitementCommandes(int socketDial, requete_t req, users_t *users) {
 		switch(req.reqNum){
 			case CMD_LOGIN_NUM :
 				traitementLogin(socketDial, req, users);
 				break;
 			case CMD_LIST_NUM :
				traitementList(socketDial, users);
 				break;

			case CMD_TALK_NUM :
				traitementTalk(socketDial, users);
				break;

			case CMD_PRIVATE_NUM :
				traitementPrivate(socketDial, req, users);
				break;

			case CMD_BYE_NUM :
				traitementBye(socketDial);
				break;

			default : // Unknow cmd ou (si on traite unkown cmd, request error)
				envoyerRequeteWithReqNum(socketDial, "", CMD_ERROR_UNKNOW_NUM);
				break;
 		}
}

void traitementMessage(int socketDial, requete_t req, users_t *users) {
	// Retrieve sender by socketDial
	user_t sender = retrieveUserBySocket(users, socketDial);
	// Retrieve message from request
	buffer_t originalMessage;
	buffer_t newMessage;
	strcpy(originalMessage, req.reqBuff);

	if(sender.destinationSocket == 0) { // everyone
		// Formating request
		sprintf(newMessage, "From %s to everyone : %s", sender.name, originalMessage);
		for(int i = 0; i < users->size; i++) {
			// Sending message to each client
			envoyerRequete(users->userList[i].socket, newMessage);
		}
	}
	else { // Private message
		// Formating request
		user_t receiver = retrieveUserBySocket(users, sender.destinationSocket);
		sprintf(newMessage, "From %s to %s : %s", sender.name, receiver.name, originalMessage);
		envoyerRequete(sender.socket, newMessage);
		// Sending to both of them
		envoyerRequete(receiver.socket, newMessage);
	}
}

void dialClt2srv(int socketAppel) {
	char buff[MAX_BUFF];
  requete_t req;
	do {
		memset(buff, 0, MAX_BUFF);
		printf ("tapez votre message : \n"); fflush(stdout);
		//scanf("%[^ ]\n", buff);
		//gets(buff);
		custom_read(buff,MAX_BUFF);
		envoyerRequete(socketAppel, buff) ;
		recevoirRequete(socketAppel, &req) ;
	} while (req.reqNum != CMD_BYE_NUM) ;
}

void creerProcService(int sockEcoute, int sockDial, int shmId) {
	int pid;
	// Créer un processus de service pour l'affecter au service du client connecté
	CHECK(pid=fork(), "-- PB fork() --");
	if (pid == 0) {
		users_t *users2;
		users2 = shmat(shmId, NULL, 0);
		// Processus de service
		// fermer la socket d'écoute puisque le processus de service ne répond pas
		// aux requêtes de connexion
		CHECK(close(sockEcoute),"-- PB close() --");
		// Dialoguer avec le client
		dialSrv2Clt(sockDial, users2);
		// Fermer la socket de dialogue
		CHECK(close(sockDial),"-- PB close() --");
		// Fin du processus de service
		exit(sockDial);
	}
}
