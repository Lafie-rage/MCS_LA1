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

void creerProcService(int sockDial, int shmid, int user_id);

void dialClt2srv(int socketAppel);

#endif /* PROTO_H */
