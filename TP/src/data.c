/**
 * Couche 6
 */

#include <string.h>
#include <stdio.h>
#include "data.h"
#include "session.h"

/**
*	\fn		void str2req(buffer_t buffer, requete_t *req)
*	\brief	Sérialise la requête
*	\param	buffer : buffer contenant le message à transmettre par la requête
*	\param	req : requête à sérialiser
*/
void req2str(const requete_t *req, buffer_t buffer) {
	sprintf(buffer, FMT_REQ_SERIAL_SEND, req->reqNum, req->reqSizeBuff, req->reqBuff);
}

/**
*	\fn		void str2req(buffer_t buffer, requete_t *req)
*	\brief	Désérialise la requête
*	\param	buffer : buffer contenant le message à transmettre par la requête
*	\param	req : requête à sérialiser
*/
void str2req(const buffer_t buffer, requete_t *req) {
	sscanf(buffer, FMT_REQ_SERIAL_RECV, &(req->reqNum), &(req->reqSizeBuff), req->reqBuff);
}

/**
*	\fn		short startWith(char *str, char *pre)
*	\brief	Vérifie si une chaine commence avec une autre
*	\param	str : chaine de caractére à vérifier
*	\param	pre : prefixe à trouver
*	\return booléen
*/
short strStartWith(char *str, char *pre)
{
    return strncmp(pre, str, strlen(pre)) == 0;
}

/**
*	\fn		void envoyerRequete(int socketEchange, requete_t req)
*	\brief	Envoi d'une requête sur une socket
*	\param	socketEchange : socket d'échange à utiliser pour l'envoi
*	\param	req : requête à sérialiser avant envoi
*	\return -1 if error else 0
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
		if (strStartWith(input,CMD_BYE) == 1) req.reqNum = CMD_BYE_NUM;
		else if (strStartWith(input,CMD_CONNECT) == 1) req.reqNum = CMD_CONNECT_NUM;
		else if (strStartWith(input,CMD_LIST) == 1) req.reqNum = CMD_LIST_NUM;
		else if (strStartWith(input,CMD_TALK) == 1) req.reqNum = CMD_TALK_NUM;
		else if (strStartWith(input,CMD_PRIVATE) == 1) req.reqNum = CMD_PRIVATE_NUM;
		else req.reqNum = CMD_UNKNOW_NUM;
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
