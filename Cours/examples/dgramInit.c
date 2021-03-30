#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define CHECK(sts, msg) if ((sts)==-1) {perror(msg); exit(-1);}
#define PAUSE printf("Appuyez sur entrée pour continuer"); getchar();
#define MAX_BUFFER_SIZE 1024

void client(void);
void server(void);

int main(void) {
  #ifdef CLIENT
    client();
  #endif
  #ifdef SERVER
    server();
  #endif
  return 0;
}

void server(void) {
  int listeningSocket; // Socket d'écoute
  struct sockaddr_in addrServer; // adressage d'un socket INET
  struct sockaddr_in addrClient;
  socklen_t lenSockClient = sizeof(addrClient);
  char buffer[MAX_BUFFER_SIZE];

  /* Création du socket */
  // Mode DGRAM => mode non connecté
  // Dernier arg => protocole de transport. Si 0 => Selection auto par l'OS.
  CHECK(listeningSocket = socket(PF_INET, SOCK_DGRAM, 0), "Problème creation socket d'écoute"); //AF_INET ou PF_INET même chose
  // Il faut associer un adressage INET au socket

  /* Préparation de l'adresse INET */
  addrServer.sin_family = AF_INET; //sin pour socket internet
  addrServer.sin_addr.s_addr = INADDR_ANY; // INADDR_ANY => 0.0.0.0 : toute interface réseau; On peut aussi mettre l'adresse d'une interface directement.
  addrServer.sin_port = htons(50000); // Conversion du port en network order
  memset(&addrServer.sin_zero, 0, 8); // Zone de bourrage

  CHECK(bind(listeningSocket, (struct sockaddr *)&addrServer, sizeof(addrServer)),"Problème bind socket d'écoute");
  PAUSE;
  // Bind marche aussi bien pour les socket inet qu'unix ou ipx...
  // Hors les structure d'addressages ne sont pas les mêmes. Il faut donc cast sockaddr_in en sockaddr puis envoyer la taille de la structure d'addressage en argument.
  CHECK(recvfrom(listeningSocket, buffer, MAX_BUFFER_SIZE, 0, (struct sockaddr*)&addrClient, &lenSockClient), "Problème reception data du client");
  printf("Message reçu par le client %s sur le port %d :\n\t##%s##\n", inet_ntoa(addrClient.sin_addr), ntohs(addrClient.sin_port), buffer);
  // On doit reconvertir les Network Order en valeur (adresse client, port d'écoute) => inet_ntoa (network to alpha) converti l'adresse
}

void client() {
  int callingSocket; // Socket d'appel
  struct sockaddr_in addrServer; // adressage du serveur à contacter
  char buffer[] = "Salut toi, le fond de l'eau est clair par ici !";

  /* Création du socket */
  // Mode DGRAM => mode non connecté
  // Dernier arg => protocole de transport. Si 0 => Selection auto par l'OS.
  CHECK(callingSocket = socket(PF_INET, SOCK_DGRAM, 0), "Problème creation socket d'appel"); //AF_INET ou PF_INET même chose
  // Il faut associer un adressage INET au socket

  /* Préparation de l'adresse INET */
  addrServer.sin_family = AF_INET; //sin pour socket internet
  addrServer.sin_addr.s_addr = inet_addr("127.0.0.1"); // Conversion en network order
  addrServer.sin_port = htons(50000); // Conversion du port en network order
  memset(&addrServer.sin_zero, 0, 8); // Zone de bourrage

  // Pas de bind ici
  PAUSE;
  // Bind marche aussi bien pour les socket inet qu'unix ou ipx...
  // Hors les structure d'addressages ne sont pas les mêmes. Il faut donc cast sockaddr_in en sockaddr puis envoyer la taille de la structure d'addressage en argument.
  CHECK(sendto(callingSocket, buffer, strlen(buffer), 0, (struct sockaddr*)&addrServer, sizeof(addrServer)), "Problème reception data du client"); // Ici sizeof car l'addresse est déjà défini.
  printf("Message envoyé au serveur %s sur le port %d :\n\t##%s##\n", inet_ntoa(addrServer.sin_addr), ntohs(addrServer.sin_port), buffer);
  // On doit reconvertir les Network Order en valeur (adresse client, port d'écoute) => inet_ntoa (network to alpha) converti l'adresse
  PAUSE;
}
