/**
 * Couche 7
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "data.h"
#include "users.h"
#include "proto.h"
#include "read_lib/read_lib.h"


void dialSrv2Clt(int socketDial, users_t *users){
	char buff[MAX_BUFF];
  	requete_t req;

	do {
		memset(buff, 0, MAX_BUFF);
		recevoirRequete(socketDial, &req) ;

    if(req.reqNum > 0) // commands
      traitementCommandes(socketDial, req, users);
    else // Messages
      traitementMessage(socketDial, req, users);

    envoyerRequete(socketDial, "OK");

	} while (strcmp(buff,"/bye")!=0);
}

void traitementCommandes(int socketDial, requete_t req) {


		char param1[100];
		char param2[100];
		strcpy(param2,"");

 		switch(req.reqNum){
 			case CMD_CONNECT_NUM :
 				sscanf(req.reqBuff,"%s%s",param1,param2);
 				// Verifie la présence de l'arguement
 				if (strcmp(param2,"") == 0){
					envoyerRequeteWithReqNum(socketDial, "CMD_ERROR_MISSING_ARGUMENT",CMD_ERROR_MISSING_ARGUMENT);
					return;
				}
				// vérifie si le pseudo est libre
				user_t user = retrieveUserByName(sender.destiantionSocket);
				if (user.socket == NULL){
					envoyerRequeteWithReqNum(socketDial, "CMD_ERROR_USERNAME_ALREADY_TAKEN",CMD_ERROR_USERNAME_ALREADY_TAKEN);
					return;
				}
				//ajoute l'user à la table 
				//renvoie le nom qui lui a été affecté 
				envoyerRequeteWithReqNum(socketDial, param2,CMD_CONNECT_NUM);

 				break;
 			case 11 :
 				break;
 		}

		
		printf("reqPram : %s\n",param2);


}

void traitementMessage(int socketDial, requete_t req, users_t *users) {
	// Retrieve sender by socketDial
	user_t sender = retrieveUserBySocket(socketDial, users);
	// Retrieve message from request
	buffer_t message = req.reqBuff;

	if(sender.destiantionSocket == 0) { // everyone
		// Formating request
		sprintf(message, "From %s to everyone : %s", sender.name, message);
		for(int i = 0; i < users.size: i++) {
			// Sending message to each client
			envoyerRequete(users[i].socket, message);
		}
	}
	else { // Private message
		// Formating request
		user_t receiver = retrieveUserBySocket(sender.destiantionSocket);
		sprintf(message, "From %s to %s : %s", sender.name, receiver.name, message)
		// Sending to both of them
		envoyerRequete(sender.socket, message);
		envoyerRequete(receiver.socket, message);
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
	} while (strcmp(buff,"BYE")!=0) ;
}

void creerProcService(int sockEcoute, int sockDial, users_t *users) {
	int pid;

	// Créer un processus de service pour l'affecter au service du client connecté
	CHECK(pid=fork(), "-- PB fork() --");
	if (pid == 0) {
		// Processus de service
		// fermer la socket d'écoute puisque le processus de service ne répond pas
		// aux requêtes de connexion
		CHECK(close(sockEcoute),"-- PB close() --");
		// Dialoguer avec le client
		dialSrv2Clt(sockDial, users);
		// Fermer la socket de dialogue
		CHECK(close(sockDial),"-- PB close() --");
		// Fin du processus de service
		exit(sockDial);
	}
}
