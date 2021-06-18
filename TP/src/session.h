#ifndef SESSION_H
#define SESSION_H

#include "data.h"
/**
*	\fn		creerSocketEcoute (char *adrIP, short port)
*	\brief	Création d'une socket d'écoute avec l'adressage fourni en paramètre
*	\param	adrIP : adresse IP du serveur à mettre en écoute
*	\param	port  : port TCP du serveur à mettre en écoute
*	\result	socket créée avec l'adressage fourni en paramètre
*/
int creerSocketEcoute (char *adrIP, short port) ;

/**
*	\fn		connecterClt2Srv (char *adrIP, short port)
*	\brief	Crétaion d'une socket d'appel et connexion au seveur dont
*			l'adressage est fourni en paramètre
*	\param	adrIP : adresse IP du serveur à connecter
*	\param	port  : port TCP du serveur à connecter
*	\result	socket connectée au serveur fourni en paramètre
*/
int connecterClt2Srv (char *adrIP, short port) ;

/**
*	\fn		int accepterClt (int sockEcoute, struct sockaddr_in *addrClt)
*	\brief	Acceptation d'une demande de connexion d'un client
*	\param	sockEcoute : socket d'écoute de réception de la demande
*	\result	socket connectée par le serveur avec un client
*/
int accepterClt (int sockEcoute);

/**
*	\fn		void envoyerMessage(int socketEchange, char *msg)
*	\brief	Envoi d'un message sur une socket
*	\param	socketEchange : socket d'échange à utiliser pour l'envoi
*	\param	msg : message à envoyer
*/
void envoyerMessage(int socketEchange, char *msg) ;

/**
*	\fn		void recevoirMessage(int socketEchange, char *msg, int msgSize)
*	\brief	Réception d'un message sur une socket
*	\param	socketEchange : socket d'échange à utiliser pour la réception
*	\param	msg : message reçu
*	\param	msgSize : taille de l' espace mémoire préalablement alloué à msg
*/
void recevoirMessage(int socketEchange, char *msg, int msgSize) ;

#endif /* SESSION_H */
