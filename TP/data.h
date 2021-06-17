#ifndef DATA_H
#define DATA_H

// Correspond à la couche 6 du modèle OSI (couche Présentation)

/**
 *	\def	MAX_BUFF	taille maximale d'une chaine échangée
 */
#define MAX_BUFF	1024

/**
 *	\typedef	buffer_t	: chaîne de caractères utilisée pour l'émission/réception
 */
typedef char buffer_t[MAX_BUFF];

#include "session.h" // Potentiellement
// Définit ici puisqu'on utilise buffer_t dans reqRep et que data utilise une structure de reqRep
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
