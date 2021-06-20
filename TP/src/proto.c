/**
 * Couche 7
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "data.h" // On suit l'ordre des couches de manière croissante
#include "proto.h"
#include "read_lib/read_lib.h"

// Traitement en fonction du type de requête reçue + fonctions venant de StreamCltSrv

void dialSrv2Clt(int socketDial){
	char buff[MAX_BUFF];
  	requete_t req;

	do {
		memset(buff, 0, MAX_BUFF);
		recevoirRequete(socketDial, &req) ;
		// appeler une fct avec le param buff pour connaître le numero de requete reçue
		// on réalise un switch sur ce numéro : à charque numéro correspond une fonction
		// de traitement qui génére un réponse
		// cette réponse sera envoyer après serialization
    if(req.reqNum > 0) // commands
      traitementCommandes(socketDial, req);
    else // Messages
      envoieMessage(socketDial, req);
/*
    if(req.reqNum == BYE_REQ_TYPE) envoyerRequete(socketDial, BYE) ;
		else envoyerRequete(socketDial, OK) ;*/
	} while (strcmp(buff,"/bye")!=0);
}

void traitementCommandes(int socketDial, requete_t req) {

 
		char param1[100];
		char param2[100];
		strcpy(param2,"");sscanf(req.reqBuff,"%s%s",param1,param2);
		printf("reqPram : %s\n",param2);
		if (strcmp(param2,"") == 0){
			envoyerRequete(socketDial, "Argument Manquant");
		}
		else{
				envoyerRequete(socketDial, param2);
		}

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
