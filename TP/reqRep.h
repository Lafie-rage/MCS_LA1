#ifndef REQ_REP_H
#define REQ_REP_H

/**
 *	\def	MAX_BUFF	taille maximale d'une chaine échangée
 */
#define MAX_BUFF	1024

/**
 *	\typedef	buffer_t	: chaîne de caractères utilisée pour l'émission/réception
 */
typedef char buffer_t[MAX_BUFF];

typedef enum { CMD, MSG } reqType_t;
//typedef enum { LIST, SELECT, CHAT } reqType_t;

/**
 *	\struct		requete
 *	\brief		Définition d'une structure requête avec tous les attributs
 *				protocolaire
 */
typedef struct requete {
	short	reqNum;		/** numéro de requête -- toujours présent */
	buffer_t reqBuff;	/** sérialization de la requête : chaîne requête à émettre/recevoir */
	short	reqSizeBuff;/** taille de la requête sérializée */
} requete_t;

#endif /* REQ_REP_H */
