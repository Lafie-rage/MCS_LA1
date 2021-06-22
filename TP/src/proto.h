/**
 * Couche 7
 */

#ifndef PROTO_H
#define PROTO_H

#include <stdlib.h>

#include <sys/types.h>
#include <sys/shm.h>
#include <pthread.h>

#include "data.h"
#include "reqRep.h"
#include "user.h"


#define OK			"OK"
#define NOK			"Pas OK"
#define BYE			"BYE"

#define CHECK(sts, msg) if ((sts)==-1) {perror(msg); exit(-1);}

void creerProcService(int sockEcoute, int sockDial, int shmid, int user_id);

void dialClt2srv(int socketAppel);

void dialSrv2Clt(int socketDial, users_t *users, int user_id);

void traitementCommandes(int socketDial, requete_t req, users_t *users, int user_id);

void traitementMessage(int socketDial, requete_t req, users_t *users, int user_id);

#endif /* PROTO_H */
