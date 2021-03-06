#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

#include "proto.h"

#define CHECK(sts, msg) if ((sts)==-1) {perror(msg); exit(-1);}
#define MAX_BUFF 1024
#define IP_ANY		"0.0.0.0"
#define IP_LOOP		"127.0.0.1"
#define IP_LOCAL	"172.20.30.13"
#define IP_DISTANT	"172.20.30.19"
#define PORT_SRV	50000

#include "session.h"
int sockEcoute;		// socket d'écoute de l'aaplication serveur

void installSigServer(int sigNum);
void client(char *adrIP, int port);
void serveur (char *adrIP, int port);
char * progName;

int main (__attribute__((unused)) int c, char **v) {
	progName = v[0];
#ifdef CLIENT
	if (c<3) {printf("usage %d: %s @IP port\n", c, basename(progName)); /*exit(-1);*/ client(IP_ANY, PORT_SRV);}
	else client(v[1], atoi(v[2]));
#endif

#ifdef SERVER
	installSigServer(SIGCHLD);
	installSigServer(SIGINT);
	if (c<3) {printf("usage: %s @IP port\n", basename(progName)); /*exit(-1);*/ serveur(IP_ANY, PORT_SRV);}
	else serveur(v[1], atoi(v[2]));
#endif
	return 0;
}

void serveur (char *adrIP, int port) {
	int sockDial, user_id = 0;
	users_t *users2;
	int shmId;
	CHECK(shmId = shmget(ftok("/tmp",1234),sizeof(users_t), 0666| IPC_CREAT), " --- Problème création mémoire partagée --- ");
   	users2 = shmat(shmId, NULL, 0);
   	initUsers(users2);

	sockEcoute = creerSocketEcoute(adrIP, port);
	while(1)	// daemon !
	{
		// Accepter une connexion
		sockDial = accepterClt(sockEcoute);
		// On incrémente l'identifiant
		user_id++;
		creerProcService(sockDial, shmId, user_id);
	}
	CHECK(close(sockEcoute),"-- PB close() --");
}
void client (char *adrIP, int port) {
	int sockAppel;

	sockAppel = connecterClt2Srv (adrIP, port);
	dialClt2srv(sockAppel);
}

void deroute (int sigNum) {
	int pidFils, status;
	switch (sigNum) {
	case SIGCHLD : CHECK(pidFils=wait(&status),"-- PB wait() --");
			printf("Fin du processus de service [%d], socket de dialogue n°%d\n", pidFils, WEXITSTATUS(status));
			break;
	case SIGINT	 : close(sockEcoute);
			printf("\nFin du serveur [%d] et de tous les processus de service\n", getpid());
			kill(SIGKILL, 0);
			exit(0);
			break;
	}
}

void installSigServer(int sigNum) {
	struct sigaction newAct;

	newAct.sa_handler = deroute;
	newAct.sa_flags = SA_RESTART;
	sigemptyset(&newAct.sa_mask) ;
	CHECK(sigaction(sigNum, &newAct, NULL),"-- PB sigaction() --");
}
