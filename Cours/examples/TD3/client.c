#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#include "session.h"

#define CHECK(sts, msg) if ((sts)==-1) {perror(msg); exit(-1);}
#define PAUSE(msg) printf("%s\n[Appuyez sur entrée pour continuer]", msg); getchar();
#define MAX_BUFFER_SIZE 1024
#define IP_LOOP "127.0.0.1"
#define IP_LOCAL "192.168.5.70"
#define IP_NAT "10.0.3.15"
#define IP_DISTANT "192.168.5."
#define PORT_SRV 50000
#define PORT_CLT 50500
#define MSG_CLIENT "Salut toi, le fond de l'eau est clair par ici !"
#define MSG_CLIENT2 "Salut toi, le fond de l'eau est brouillé par ici !"
#define MSG_OK "OK"
#define MSG_NOT_OK "Not OK"

void dialClient2Server(int callingSocket, struct sockaddr_in addrServer);

int main(int c, char **v) {
  int callingSocket; // Socket d'appel
  struct sockaddr_in addrServer; // adressage du serveur à contacter

  callingSocket = connectClientToServer(char *adrIp, short port);

  // Connexion avec le serveur...
  CHECK(connect(callingSocket, (struct sockaddr *) &addrServer, sizeof(addrServer)), "Problème lors de la connexion au serveur");
  dialClient2Server(callingSocket, addrServer);

  close(callingSocket);
}

void dialClient2Server(int callingSocket, struct sockaddr_in addrServer) {
  struct sockaddr_in myAddr;
  socklen_t lenMyAddr = sizeof(myAddr);
  char buffer[MAX_BUFFER_SIZE];

  CHECK(write(callingSocket, MSG_CLIENT, strlen(MSG_CLIENT)+1), "Problème envoie data au serveur"); // Ici sizeof car l'addresse est déjà défini.
  PAUSE("CLIENT-write()");
  // Le client n'a pas fait de bind donc on execute getsockname pour récupérer son adresse.
  CHECK(getsockname(callingSocket, (struct sockaddr *)&myAddr, &lenMyAddr), "Problème lors de la récupération de mon adresse client");
  printf("Message envoyé au serveur [%s:%d] via [%s:%d] :\n\t##%s##\n", inet_ntoa(addrServer.sin_addr), ntohs(addrServer.sin_port), inet_ntoa(myAddr.sin_addr), ntohs(myAddr.sin_port), MSG_CLIENT);
  CHECK(read(callingSocket, buffer, MAX_BUFFER_SIZE), "Problème récepetion data du serveur");
  PAUSE("CLIENT-read()");
}
