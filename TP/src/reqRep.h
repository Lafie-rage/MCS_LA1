#ifndef REQ_REP_H
#define REQ_REP_H

// Format de requête pour la sérialisation
#define FMT_REQ_SERIAL_SEND "%hd|%hd|%s"
// Fromat de la chaîne à désérailiser
#define FMT_REQ_SERIAL_RECV "%hd|%hd|%[^\t\n]"


typedef enum { CMD, MSG } reqType_t;

/**
 *	\typedef	requete_t
 *	\brief		Définition d'une structure requête avec tous les attributs
 *				protocolaire
 */
typedef struct requete {
	short	reqNum;				/**< Type de requête (message ou type de commande)#reqNum */
	buffer_t reqBuff;		/**< Corps de la requête#reqBuff */
	short	reqSizeBuff;	/**< Taille du corps#reqSizeBuff */
} requete_t;

#endif /* REQ_REP_H */
