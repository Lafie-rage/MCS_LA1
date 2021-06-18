/**
 * Couche 7
 */

#ifndef PROTO_H
#define PROTO_H

#include <stdlib.h>
#include "data.h"
#include "reqRep.h"


#define OK			"OK"
#define NOK			"Pas OK"
#define BYE			"BYE"

#define CHECK(sts, msg) if ((sts)==-1) {perror(msg); exit(-1);}

void creerProcService(int sockEcoute, int sockDial);

void dialClt2srv(int socketAppel);

void dialSrv2Clt(int socketDial);

void traitementCommandes(int socketDial, requete_t req);

void envoieMessage(int socketDial, requete_t req);

#endif /* PROTO_H */
