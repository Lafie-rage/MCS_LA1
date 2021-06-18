/**
 * Couche 5
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define CHECK(sts, msg) if ((sts)==-1) {perror(msg); exit(-1);}

extern char *progName;
/**
*	\fn		creerSocketEcoute (char *adrIP, short port)
*	\brief	Création d'une socket d'écoute avec l'adressage fourni en paramètre
*	\param	adrIP : adresse IP du serveur à mettre en écoute
*	\param	port  : port TCP du serveur à mettre en écoute
*	\result	socket créée avec l'adressage fourni en paramètre
*/
int creerSocketEcoute (char *adrIP, short port) {
	int sockEcoute;					// socket d'écoute à créer
	struct sockaddr_in addrSer;		// adressage d'une socket INET

	CHECK (sockEcoute = socket (AF_INET, SOCK_STREAM, 0), "-- PB socket() --");
	// Préparer la structure d'adressage de la socket d'écoute
	addrSer.sin_family = AF_INET;
	addrSer.sin_addr.s_addr = inet_addr(adrIP);
	addrSer.sin_port = htons(port);
	memset(&addrSer.sin_zero, 0, 8);
	// Associer l'adressage INET préparé à la socket
	CHECK(bind(sockEcoute, (struct sockaddr *)&addrSer, sizeof(addrSer)),"-- PB bind() --");
	// Mise en écoute
	CHECK (listen(sockEcoute, 5), "-- PB listen() --");
	printf("[\033[32m%s:%d\033[0m] Serveur en écoute sur [\033[31m%s:%d\033[0m]\nAttente de connexion ...\n",
				basename(progName), getpid(), inet_ntoa(addrSer.sin_addr), ntohs(addrSer.sin_port));
	return sockEcoute;
}

/**
*	\fn		connecterClt2Srv (char *adrIP, short port)
*	\brief	Crétaion d'une socket d'appel et connexion au seveur dont
*			l'adressage est fourni en paramètre
*	\param	adrIP : adresse IP du serveur à connecter
*	\param	port  : port TCP du serveur à connecter
*	\result	socket connectée au serveur fourni en paramètre
*/
int connecterClt2Srv (char *adrIP, short port) {
	int socketAppel;				// socket d'appel
	struct sockaddr_in addrSer;		// adresse du serveur à connecter
	struct sockaddr_in addrLocale;
	socklen_t lenAddr = sizeof(addrLocale);

	// Créer la socket d'appel
	CHECK (socketAppel = socket (AF_INET, SOCK_STREAM, 0), "-- PB socket() --");
	// Préparer l'adresse applicative du serveur
	addrSer.sin_family = AF_INET;
	addrSer.sin_addr.s_addr = inet_addr(adrIP);
	addrSer.sin_port = htons(port);
	memset(&addrSer.sin_zero, 0, 8);
	// Se connecter au serveur avec l'adressage préalablement préparé
	CHECK(connect(socketAppel, (struct sockaddr *)&addrSer, sizeof(addrSer)),"-- PB connect() --");
	CHECK(getsockname(socketAppel, (struct sockaddr *)&addrLocale, &lenAddr),"-- PB getsockname() --");
	printf("[\033[32m%s:%d\033[0m] connecté à [\033[31m%s:%d\033[0m] via [\033[34m%s:%d\033[0m]\n",
				basename(progName), getpid(), inet_ntoa(addrSer.sin_addr), ntohs(addrSer.sin_port),
		   		inet_ntoa(addrLocale.sin_addr), ntohs(addrLocale.sin_port));
	return socketAppel;
}

/**
*	\fn		int accepterClt (int sockEcoute, struct sockaddr_in *addrClt)
*	\brief	Acceptation d'une demande de connexion d'un client
*	\param	sockEcoute : socket d'écoute de réception de la demande
*	\result	socket créée pour connecter le serveur avec un client en vue du dialogue
*/
int accepterClt (int sockEcoute) {
	int socketDial;				// socket de dialogue
	struct sockaddr_in addrClt;	// Adresse du client
	socklen_t lenSockClt;

	lenSockClt = sizeof(addrClt);
	// Accepter une demande de connexion avec renseignement de l'adresse client
	// et création d'une socket de dialogue
	CHECK(socketDial=accept(sockEcoute,(struct sockaddr *)&addrClt, &lenSockClt), "-- PB accept() --");
	return socketDial;
}

/**
*	\fn		void envoyerMessage(int socketEchange, char *msg)
*	\brief	Envoi d'un message sur une socket
*	\param	socketEchange : socket d'échange à utiliser pour l'envoi
*	\param	msg : message à envoyer
*/
void envoyerMessage(int socketEchange, char *msg) {
	struct sockaddr_in addrLocale, addrDistante;
	socklen_t lenAddr = sizeof(addrLocale);
	int nbCar;

	CHECK(getsockname(socketEchange, (struct sockaddr *)&addrLocale, &lenAddr),"-- PB getsockname() --");
	CHECK(getpeername(socketEchange, (struct sockaddr *)&addrDistante, &lenAddr),"-- PB getpeername() --");
	CHECK(nbCar=write(socketEchange, msg, strlen(msg)+1),"--PB write() --");

	fprintf(stderr, "[\033[32m%s:%d\033[0m]\tMessage envoyé à [\033[31m%s:%d\033[0m] via [\033[34m%s:%d\033[0m]\n\témis [\033[32m%d\033[0m]\t##\033[32m%s\033[0m##\n",
				basename(progName), getpid(),
				inet_ntoa(addrDistante.sin_addr), ntohs(addrDistante.sin_port),
				inet_ntoa(addrLocale.sin_addr), ntohs(addrLocale.sin_port),
				nbCar, msg);
}

/**
*	\fn		void recevoirMessage(int socketEchange, char *msg, int msgSize)
*	\brief	Réception d'un message sur une socket
*	\param	socketEchange : socket d'échange à utiliser pour la réception
*	\param	msg : message reçu
*	\param	msgSize : taille de l' espace mémoire préalablement alloué à msg
*/
void recevoirMessage(int socketEchange, char *msg, int msgSize) {
	struct sockaddr_in addrLocale, addrDistante;
	socklen_t lenAddr = sizeof(addrLocale);
	int nbCar;

	CHECK(getsockname(socketEchange, (struct sockaddr *)&addrLocale, &lenAddr),"-- PB getsockname() --");
	CHECK(getpeername(socketEchange, (struct sockaddr *)&addrDistante, &lenAddr),"-- PB getpeername() --");
	memset(msg, 0, msgSize);
	CHECK(nbCar=read(socketEchange, msg, msgSize),"-- PB read() --");

	fprintf(stderr, "[\033[32m%s:%d\033[0m]\tMessage reçu de [\033[31m%s:%d\033[0m] via [\033[34m%s:%d\033[0m]\n\treçu [\033[32m%d\033[0m]\t##\033[32m%s\033[0m##\n",
				basename(progName), getpid(),
		   		inet_ntoa(addrDistante.sin_addr), ntohs(addrDistante.sin_port),
				inet_ntoa(addrLocale.sin_addr), ntohs(addrLocale.sin_port),
		   		nbCar, msg);
}
