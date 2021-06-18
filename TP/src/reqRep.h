#ifndef REQ_REP_H
#define REQ_REP_H

// Format de requête pour la sérialisation
#define FMT_REQ_SERIAL_SEND "%hd|%hd|%s"
// Fromat de la chaîne à désérailiser
#define FMT_REQ_SERIAL_RECV "%hd|%hd|%[^\t\n]"

// Ces deux constantes peuvent changer de forme
// Le but étant de pouvoir transmettre la requête en un seul message et donc pouvoir la sérialiser ou la désérialiser facilement
// On a utilisé le | ici mais n'importe quel délimiteur est acceptable, même plusieurs caractères à la suite
// Le but est simplement de reconnaitre la fin des formats.
// On remarque ici qu'on affiche d'abord le numéro qui donne le type de requête
// puis la taille du corps de la requête
// et finalement le corps.
// On envoie la taille d'abord pour connaitre le nombre de caractère à parcourir
// Ce n'est pas utilise pour notre utilisation mais c'est un standart dans ce type de développement.


// Deux choix de type, le premier étant plus simple que le second à mon sens
// J'ai pas trop compris à quoi il sert honnêtement vu qu'on a le type de requête par le numéro visiblement
typedef enum { CMD, MSG } reqType_t;
//typedef enum { LIST, SELECT, CHAT } reqType_t;

/**
 *	\struct		requete
 *	\brief		Définition d'une structure requête avec tous les attributs
 *				protocolaire
 *  Cette structure peut évoluer selon vos choix de protocole.
 */
typedef struct requete {
	short	reqNum;		/** numéro de requête -- toujours présent. Définit le type de requête (message ou type de commande) */
	buffer_t reqBuff;	/** sérialization de la requête : chaîne requête à émettre/recevoir */
	short	reqSizeBuff;/** taille de la requête sérializée */
} requete_t;

#endif /* REQ_REP_H */
