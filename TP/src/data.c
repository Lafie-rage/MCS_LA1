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
*	\fn		void envoyerRequete(int socketEchange, buffer_t input)
*	\brief	Envoi d'une requête sur une socket
*	\param	socketEchange : socket d'échange à utiliser pour l'envoi
*	\param	req : requête à sérialiser avant envoi
*/
void envoyerRequete(int socketEchange, buffer_t input) {
	int reqNum;
	if(input[0] == '/') { // CMD
		if (strStartWith(input, CMD_BYE) == 1) reqNum = CMD_BYE_NUM;
		else if (strStartWith(input, CMD_LOGIN) == 1) reqNum = CMD_LOGIN_NUM;
		else if (strStartWith(input, CMD_LIST) == 1) reqNum = CMD_LIST_NUM;
		else if (strStartWith(input, CMD_TALK) == 1) reqNum = CMD_TALK_NUM;
		else if (strStartWith(input, CMD_PRIVATE) == 1) reqNum = CMD_PRIVATE_NUM;
		else reqNum = CMD_ERROR_UNKNOW_NUM;
	}
	else reqNum = 0; // MSG
	envoyerRequeteWithReqNum(socketEchange, input, reqNum);
}

/**
*	\fn		void envoyerRequeteWithReqNum(int socketEchange, buffer_t input, int reqNum)
*	\brief	Envoi d'une requête sur une socket
*	\param	socketEchange : socket d'échange à utiliser pour l'envoi
*	\param	req : requête à sérialiser avant envoi
* 	\param	reqNum : numéro de requête
*/
void envoyerRequeteWithReqNum(int socketEchange, buffer_t input, int reqNum) {
	requete_t req;
	buffer_t buffer;
	req.reqNum = reqNum;
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
