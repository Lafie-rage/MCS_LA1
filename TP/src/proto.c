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
