#ifndef REQ_REP_H
#define REQ_REP_H

// Format de requête pour la sérialisation
#define FMT_REQ_SERIAL_SEND "%hd|%hd|%s"
// Fromat de la chaîne à désérailiser
#define FMT_REQ_SERIAL_RECV "%hd|%hd|%[^\t\n]"


typedef enum { CMD, MSG } reqType_t;

/**
 *	\struct		requete
 *	\brief		Définition d'une structure requête avec tous les attributs
 *				protocolaire
 */
typedef struct requete {
	short	reqNum;		/** numéro de requête -- toujours présent. Définit le type de requête (message ou type de commande) */
	buffer_t reqBuff;	/** corps de la requête */
	short	reqSizeBuff;/** taille du corps */
} requete_t;

#endif /* REQ_REP_H */
