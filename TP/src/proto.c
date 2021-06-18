/**
 * Couche 7
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h" // On suit l'ordre des couches de manière croissante
#include "proto.h"
#include "read_lib/read_lib.h"

// Traitement en fonction du type de requête reçue + fonctions venant de StreamCltSrv

void dialSrv2Clt(int socketDial){
	char buff[MAX_BUFF];

	do {
		memset(buff, 0, MAX_BUFF);
		recevoirMessage(socketDial, buff, MAX_BUFF) ;
		// appeler une fct avec le param buff pour connaître le numero de requete reçue
		// on réalise un switch sur ce numéro : à charque numéro correspond une fonction
		// de traitement qui génére un réponse
		// cette réponse sera envoyer après serialization
		if (strcmp(buff,"/bye")==0) envoyerMessage(socketDial, BYE) ;
		else envoyerMessage(socketDial, OK) ;
	} while (strcmp(buff,"/bye")!=0);
}

void dialClt2srv(int socketAppel) {
	char buff[MAX_BUFF];

	do {
		memset(buff, 0, MAX_BUFF);
		printf ("tapez votre message : \n"); fflush(stdout);
		//scanf("%[^ ]\n", buff);
		//gets(buff);
		custom_read(buff,MAX_BUFF);
		envoyerMessage(socketAppel, buff) ;
		recevoirMessage(socketAppel, buff, MAX_BUFF) ;
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
