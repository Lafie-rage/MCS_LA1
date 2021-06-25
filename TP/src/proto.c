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

typedef struct info_thread_server {
	int socket;
	int id;
	int shmId;
} info_thread_server_t;

//// PUBLIC FUNCTIONS ////

void creerProcService(int socketDial, int shmId, int userId);

void dialClt2srv(int socketAppel);

//// END PUBLIC FUNCTIONS ////

//// STATIC FUNCTIONS ////

static void *dialClt2RCV(void *vargp);

static void *dialClt2SND(void *vargp);

static void *dialSrv2Clt(void *vargp);

static void traitementMessage(int socketDial, requete_t req, users_t *users, int userId);

static void traitementCommandes(int socketDial, requete_t req, users_t *users, int userId);

static void traitementLogin(int socket, requete_t req, users_t *users, int userId);

static void traitementList(int socket, users_t *users);

static void traitementTalk(int socket, users_t *users, int userId);

static void traitementPrivate(int socket, requete_t req, users_t *users, int userId);

static void traitementBye(int socket, users_t *users, int userId);

static void listUserToString(const users_t *users, char *stringListUser);

//// END STATIC FUNCTIONS ////



void creerProcService(int socketDial, int shmId, int userId) {

	pthread_t thread_id_server;
	info_thread_server_t info;
	info.socket = socketDial;
	info.id = userId;
	info.shmId = shmId;
	pthread_create(&thread_id_server, NULL, dialSrv2Clt, (void *)&info);
}

void dialClt2srv(int socketAppel) {
	pthread_t thread_id_RCV,thread_id_SND;
	pthread_create(&thread_id_RCV, NULL, dialClt2RCV,(void *) &socketAppel);
	pthread_create(&thread_id_SND, NULL, dialClt2SND,(void *) &socketAppel);
	pthread_join(thread_id_RCV, NULL);
	pthread_cancel(thread_id_SND);
	// Fermer la socket d'appel
	close(socketAppel);
}

static void *dialSrv2Clt(void *vargp) {
	// Récupération des infos du client
	int socketDial = ((info_thread_server_t *)vargp)->socket;
	int userId = ((info_thread_server_t *)vargp)->id;
	int shmId = ((info_thread_server_t *)vargp)->shmId;
	users_t *users;
	users = shmat(shmId, NULL, 0);
	char buff[MAX_BUFF];
	requete_t req;
	do {
		memset(buff, 0, MAX_BUFF);
		recevoirRequete(socketDial, &req);

    if(req.reqNum > 0) // commands
      traitementCommandes(socketDial, req, users, userId);
    else // Messages
      traitementMessage(socketDial, req, users, userId);

    //envoyerRequete(socketDial, "OK");

	} while (req.reqNum != CMD_BYE_NUM);
	puts("COUCOU");
	// Fermer la socket de dialogue
	CHECK(close(socketDial),"-- PB close() --");
	// Fin du processus de service
	pthread_exit(NULL);
}

static void *dialClt2RCV(void *vargp) {

	int socketAppel = *((int *)vargp);
  requete_t req;
	do {
		recevoirRequete(socketAppel, &req);
	} while (req.reqNum != CMD_BYE_NUM);
	pthread_exit(NULL);
}

static void *dialClt2SND(void *vargp) {
	int socketAppel = *((int *)vargp);
	char buff[MAX_BUFF];
	while(1) {
		memset(buff, 0, MAX_BUFF);
		printf ("tapez votre message : \n");
		custom_read(buff,MAX_BUFF);
		envoyerRequete(socketAppel, buff) ;
	}
	pthread_exit(NULL); // Jamais atteind
}

static void traitementCommandes(int socketDial, requete_t req, users_t *users, int userId) {
 		switch(req.reqNum){
 			case CMD_LOGIN_NUM :
 				traitementLogin(socketDial, req, users, userId);
 				break;

 			case CMD_LIST_NUM :
				traitementList(socketDial, users);
 				break;

			case CMD_TALK_NUM :
				traitementTalk(socketDial, users, userId);
				break;

			case CMD_PRIVATE_NUM :
				traitementPrivate(socketDial, req, users, userId);
				break;

			case CMD_BYE_NUM :
				traitementBye(socketDial, users, userId);
				break;

			default : // Unknow cmd ou (si on traite unkown cmd, request error)
				envoyerRequeteWithReqNum(socketDial, "", CMD_ERROR_UNKNOW_NUM);
				break;
 		}
}

static void traitementMessage(int socketDial, requete_t req, users_t *users, int userId) {
	// Retrieve sender by socketDial
	user_t sender = retrieveUserById(users, userId);
	if(!usersCompare(sender, NULL_USER)) {
		envoyerRequeteWithReqNum(socketDial, "CMD_ERROR_USER_NOT_LOOGED_IN_NUM", CMD_ERROR_USER_NOT_LOOGED_IN_NUM); // Utilisateur déconnecté ou non trouvé dans la liste
		return;
	}
	// Retrieve message from request
	buffer_t originalMessage;
	buffer_t newMessage;
	strcpy(originalMessage, req.reqBuff);

	if(sender.destinationId == EVERYONE_DESTIONATION_ID) { // everyone
		// Formating request
		sprintf(newMessage, "From %s to everyone : %s", sender.name, originalMessage);
		for(int i = 0; i < MAX_USER; i++) {
			if(users->userList[i].id != UNUSED_ID) {
				// Sending message to each client
				envoyerRequete(users->userList[i].socket, newMessage);
			}
		}
	}
	else { // Private message
		// Formating request
		user_t receiver = retrieveUserById(users, sender.destinationId);
		if(!usersCompare(receiver, NULL_USER)) {
			envoyerRequeteWithReqNum(socketDial, "CMD_ERROR_USER_DISCONNECTED_SWAP_TO_EVERYONE_NUM", CMD_ERROR_USER_DISCONNECTED_SWAP_TO_EVERYONE_NUM); // Utilisateur déconnecté après le choix de discussion
			updateUserDestination(users, sender.id, EVERYONE_DESTIONATION_ID);
			return;
		}
		sprintf(newMessage, "From %s to %s : %s", sender.name, receiver.name, originalMessage);
		// Sending to both of them
		envoyerRequete(sender.socket, newMessage);
		if(usersCompare(receiver, sender)) envoyerRequete(receiver.socket, newMessage);
	}
}

static void traitementLogin(int socket, requete_t req, users_t *users, int userId) {
	char cmd[100];
	char name[100];
	strcpy(name, "");
	sscanf(req.reqBuff, "%s%s", cmd, name);
	user_t user = retrieveUserById(users, userId);
	if(usersCompare(user, NULL_USER)) {
		envoyerRequeteWithReqNum(socket, "CMD_ERROR_ALREADY_LOGGED_IN_NUM", CMD_ERROR_ALREADY_LOGGED_IN_NUM);
		return;
	}
	// Verifie la présence de l'arguement
	if (strcmp(name, "") == 0){
		envoyerRequeteWithReqNum(socket, "CMD_ERROR_MISSING_ARGUMENT_NUM", CMD_ERROR_MISSING_ARGUMENT_NUM);
		return;
	}
	// vérifie si le pseudo est libre
	user = retrieveUserByName(users, name);
	if (usersCompare(user, NULL_USER)){
		envoyerRequeteWithReqNum(socket, "CMD_ERROR_USERNAME_ALREADY_TAKEN_NUM", CMD_ERROR_USERNAME_ALREADY_TAKEN_NUM);
		return;
	}
	// verifie qu'il reste de la place
	if (users->size == MAX_USER){
		envoyerRequeteWithReqNum(socket, "CMD_ERROR_SERVER_FULL_NUM", CMD_ERROR_SERVER_FULL_NUM);
	}
	//ajoute l'user à la table
	if (addUser(users, name, socket, userId) == -1 ){
		envoyerRequeteWithReqNum(socket, "CMD_ERROR_UNKNOW_ERROR_NUM", CMD_ERROR_UNKNOW_ERROR_NUM); // logiquement impossible
		return;
	}
	//renvoie le nom qui lui a été affecté
	envoyerRequeteWithReqNum(socket, name, CMD_LOGIN_NUM);
}

static void listUserToString(const users_t *users, char *stringListUser) {
  int i;
	char currentUser[MAX_SIZE_NAME+2];
	strcpy(stringListUser,"Connected users :\n");
	for (i = 0; i < MAX_USER; ++i) {
		if(users->userList[i].socket != UNUSED_SOCKET){
			sprintf(currentUser,PATTERN_USER_TO_STRING,users->userList[i].name);
			strcat(stringListUser,currentUser);
		}
	}

}

static void traitementList(int socket, users_t *users) {
	char stringListUser[23+(MAX_SIZE_NAME+2)*MAX_USER];
	listUserToString(users,stringListUser);
	envoyerRequeteWithReqNum(socket, stringListUser,CMD_LIST_NUM);
}

static void traitementTalk(int socket, users_t *users, int userId) {
	user_t sender = retrieveUserById(users, userId);
	if(!usersCompare(sender, NULL_USER)) {
		envoyerRequeteWithReqNum(socket, "CMD_ERROR_USER_NOT_LOOGED_IN_NUM", CMD_ERROR_USER_NOT_LOOGED_IN_NUM); // Utilisateur déconnecté ou non trouvé dans la liste
		return;
	}

	if(!updateUserDestination(users, userId, EVERYONE_DESTIONATION_ID)) {  // Impossible de mettre à jour l'utilisateur
		envoyerRequeteWithReqNum(socket, "CMD_ERROR_UNKNOW_ERROR_NUM", CMD_ERROR_UNKNOW_ERROR_NUM);
		return;
	}

	envoyerRequeteWithReqNum(socket, "", CMD_TALK_NUM);
}

static void traitementPrivate(int socket, requete_t req, users_t *users, int userId) {
	char cmd[100];
	char name[100];
	user_t sender = retrieveUserById(users, userId);
	if(!usersCompare(sender, NULL_USER)) {
		envoyerRequeteWithReqNum(socket, "CMD_ERROR_USER_NOT_LOOGED_IN_NUM", CMD_ERROR_USER_NOT_LOOGED_IN_NUM); // Utilisateur déconnecté ou non trouvé dans la liste
		return;
	}
	strcpy(name, "");
	sscanf(req.reqBuff, "%s%s", cmd, name);
	// Verifie la présence de l'arguement
	if (strcmp(name, "") == 0){
		envoyerRequeteWithReqNum(socket, "CMD_ERROR_MISSING_ARGUMENT_NUM",CMD_ERROR_MISSING_ARGUMENT_NUM);
		return;
	}
	// Vérifie que l'utilisateur est connecté
	user_t receiver = retrieveUserByName(users, name);
	if (!usersCompare(receiver, NULL_USER)){
		envoyerRequeteWithReqNum(socket, "CMD_ERROR_USER_DISCONNECTED_NUM", CMD_ERROR_USER_DISCONNECTED_NUM);
		return;
	}
	if(!updateUserDestination(users, userId, receiver.id)) { // Impossible de mettre à jour l'utilisateur
		envoyerRequeteWithReqNum(socket, "CMD_ERROR_UNKNOW_ERROR_NUM", CMD_ERROR_UNKNOW_ERROR_NUM);
		return;
	}

	envoyerRequeteWithReqNum(socket, receiver.name, CMD_PRIVATE_NUM);
}

static void traitementBye(int socket, users_t *users, int userId) {
	user_t sender = retrieveUserById(users, userId);
	if(!usersCompare(sender, NULL_USER)) {
		envoyerRequeteWithReqNum(socket, "CMD_ERROR_USER_NOT_LOOGED_IN_NUM", CMD_ERROR_USER_NOT_LOOGED_IN_NUM); // Utilisateur déconnecté ou non trouvé dans la liste
		return;
	}

	if(!removeUser(users, userId)) {	// Impossible de supprimer l'utilisateur
		envoyerRequeteWithReqNum(socket, "CMD_ERROR_UNKNOW_ERROR_NUM", CMD_ERROR_UNKNOW_ERROR_NUM);
		return;
	}
	envoyerRequeteWithReqNum(socket, "", CMD_BYE_NUM);
}
