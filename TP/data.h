#ifndef DATA_H
#define DATA_H
#include "reqRep.h"

/**
*	\fn		void str2req(buffer_t buffer, requete_t *req)
*	\brief	Sérialise le buffer en requête
*	\param	buffer : buffer contenant le message à transmettre par la requête
*	\param	req : requête à sérialiser
*/
void str2req(buffer_t buffer, requete_t *req);

/**
*	\fn		void envoyerRequete(int socketEchange, requete_t req)
*	\brief	Envoi d'une requête sur une socket
*	\param	socketEchange : socket d'échange à utiliser pour l'envoi
*	\param	req : requête à sérialiser avant envoi
*/
void envoyerRequete(int socketEchange, requete_t req);

/**
*	\fn		void recevoirRequete(int socketEchange, requete_t *req)
*	\brief	Réception d'une requête sur une socket
*	\param	socketEchange : socket d'échange à utiliser pour la réception
*	\param	req : requête reçue après désérialisation du buufer
*/
void recevoirRequete(int socketEchange, requete_t *req);

#endif /* DATA_H */
