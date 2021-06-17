#include <string.h>
#include "data.h"
#include "session.h"

// Correspond à la couche 6 du modèle OSI (couche Présentation)


/**
*	\fn		void str2req(buffer_t buffer, requete_t *req)
*	\brief	Sérialise la requête
*	\param	buffer : buffer contenant le message à transmettre par la requête
*	\param	req : requête à sérialiser
*/
void req2str(const requete_t *req, buffer_t buffer) {
	sprintf(buff, FMT_REQ_SERIALIZE_SEND, req->reqNum, req->reqSizeBuff, req->reqBuff);
}

/**
*	\fn		void str2req(buffer_t buffer, requete_t *req)
*	\brief	Désérialise la requête
*	\param	buffer : buffer contenant le message à transmettre par la requête
*	\param	req : requête à sérialiser
*/
void str2req(const buffer_t buffer, requete_t *req) {
	sscanf(buff, FMT_REQ_SERIALIZE_RECV, req->reqNum, req->reqSizeBuff, req->reqBuff);
}

/**
*	\fn		void envoyerRequete(int socketEchange, requete_t req)
*	\brief	Envoi d'une requête sur une socket
*	\param	socketEchange : socket d'échange à utiliser pour l'envoi
*	\param	req : requête à sérialiser avant envoi
*/
void envoyerRequete(int socketEchange, buffer_t input) {
	requete_t req;
	buffer_t buffer;

	// On définit ici le type de message.
	// Chaque msg sera de type 0, les commandes auront, quant à elle, une nombre supérieur à 0 en fonction de la commande
	// Le but est de définir un ensemble basiques de commandes qui permettent de faire plus qu'envoyer de simple messages
	// Il faudra donc penser à documenter les types de commandes (numéro, nom, traitement, texte à écrire)
	// et aussi le(s) caractère(s) definissant qu'un texte n'est plus un message mais une commande
	// Ici, on a prit '/' mais ça peut-être n'importe quoi d'autre
	if(input[0]== '/') { // CMD
		if (strcmp(buff,"/bye")==0) req.reqNum = 1;
		else if (strcmp(buff,"/list")==0) req.reqNum = 2;
		else 	if (strcmp(buff,"/talk")==0) req.reqNum = 3;
		// etc...
	}
	else req.reqNum = 0; // MSG

	req.reqSizeBuff = strlen(input);
	strcpy(req.reqBuff, input);

	req2str(&req, buffer);

	envoyerMessage(socketEchange, buffer);
}

/**
*	\fn		void recevoirRequete(int socketEchange, requete_t *req)
*	\brief	Réception d'une requête sur une socket
*	\param	socketEchange : socket d'échange à utiliser pour la réception
*	\param	req : requête reçue après désérialisation du buufer
*/
void recevoirRequete(int socketEchange, requete_t *req) {
	buffer_t	buffer;

	recevoirMessage(socketEchange, buffer, MAX_BUFF);

	str2req(buffer, req);
}
