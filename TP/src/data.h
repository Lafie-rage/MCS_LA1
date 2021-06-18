/**
 * Couche 6
 */

#ifndef DATA_H
#define DATA_H

#define CLOSE_RQ_TYPE 10

#define CMD_BYE 		"/bye"
#define CMD_CONNECT		"/connect"
#define CMD_LIST		"/list"
#define CMD_TALK		"/talk"
#define CMD_PRIVATE		"/private"

#define CMD_BYE_NUM 		10
#define CMD_CONNECT_NUM		11
#define CMD_LIST_NUM		20
#define CMD_TALK_NUM		21
#define CMD_PRIVATE_NUM		22

/**
 *	\def	MAX_BUFF	taille maximale d'une chaine échangée
 */
#define MAX_BUFF	1024

/**
 *	\typedef	buffer_t	: chaîne de caractères utilisée pour l'émission/réception
 */
typedef char buffer_t[MAX_BUFF];

#include "session.h"
#include "reqRep.h"

/**
*	\fn		void str2req(buffer_t buffer, requete_t *req)
*	\brief	Sérialise la requête
*	\param	buffer : buffer contenant le message à transmettre par la requête
*	\param	req : requête à sérialiser
*/
void req2str(const requete_t *req, buffer_t buffer);

/**
*	\fn		void str2req(buffer_t buffer, requete_t *req)
*	\brief	Sérialise le buffer en requête
*	\param	buffer : buffer contenant le message à transmettre par la requête
*	\param	req : requête à sérialiser
*/
void str2req(const buffer_t buffer, requete_t *req);

/**
*	\fn		void envoyerRequete(int socketEchange, requete_t req)
*	\brief	Envoi d'une requête sur une socket
*	\param	socketEchange : socket d'échange à utiliser pour l'envoi
*	\param	req : requête à sérialiser avant envoi
*/
void envoyerRequete(int socketEchange, buffer_t input);

/**
*	\fn		void recevoirRequete(int socketEchange, requete_t *req)
*	\brief	Réception d'une requête sur une socket
*	\param	socketEchange : socket d'échange à utiliser pour la réception
*	\param	req : requête reçue après désérialisation du buufer
*/
void recevoirRequete(int socketEchange, requete_t *req);

#endif /* DATA_H */
