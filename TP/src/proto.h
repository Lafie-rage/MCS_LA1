/**
 * Couche 7
 */

#ifndef PROTO_H
#define PROTO_H

#include <stdlib.h>
#include "reqRep.h"
#include "data.h" // Potentiellement


#define OK			"OK"
#define NOK			"Pas OK"
#define BYE			"BYE"

#define CHECK(sts, msg) if ((sts)==-1) {perror(msg); exit(-1);}

#endif /* PROTO_H */
