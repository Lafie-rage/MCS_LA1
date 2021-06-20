/**
 * Couche 7
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "data.h"
#include "proto.h"
#include "read_lib/read_lib.h"


void dialSrv2Clt(int socketDial){
	char buff[MAX_BUFF];
  	requete_t req;

	do {
		memset(buff, 0, MAX_BUFF);
		recevoirRequete(socketDial, &req) ;

    if(req.reqNum > 0) // commands
      traitementCommandes(socketDial, req);
    else // Messages
      envoieMessage(socketDial, req);

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
				if (){
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

void envoieMessage(int socketDial, requete_t req) {

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

void creerProcService(int sockEcoute, int sockDial) {
	int pid;

	// Créer un processus de service pour l'affecter au service du client connecté
	CHECK(pid=fork(), "-- PB fork() --");
	if (pid == 0) {
		// Processus de service
		// fermer la socket d'écoute puisque le processus de service ne répond pas
		// aux requêtes de connexion
		CHECK(close(sockEcoute),"-- PB close() --");
		// Dialoguer avec le client
		dialSrv2Clt(sockDial);
		// Fermer la socket de dialogue
		CHECK(close(sockDial),"-- PB close() --");
		// Fin du processus de service
		exit(sockDial);
	}
}
