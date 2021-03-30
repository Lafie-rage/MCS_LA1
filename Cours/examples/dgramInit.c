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

void client(int messageNb);
void lonelyClient();
void server(void);

int main(void) {
  #ifdef CLIENT
    lonelyClient();
  #endif
  #ifdef CLIENT1
    client(1);
  #endif
  #ifdef CLIENT2
    client(2);
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
  // Si on utilise pas htons, le port vu dans netstat ne sera plus celui entré dans le code
  // Il ne lit pas les octets de la même manière.
  //addrServer.sin_port = 50000;
  memset(&addrServer.sin_zero, 0, 8); // Zone de bourrage

  // Bind marche aussi bien pour les socket inet qu'unix ou ipx...
  // Or les structure d'addressages ne sont pas les mêmes. Il faut donc cast sockaddr_in en sockaddr puis envoyer la taille de la structure d'addressage en argument.
  CHECK(bind(listeningSocket, (struct sockaddr *)&addrServer, sizeof(addrServer)),"Problème bind socket d'écoute");
  while(1) { // écoute permanente (deamon)
    PAUSE;
    // On vide le buffer pour éviter les résidus des anciens messages si celui envoyé ne contient pas l'\0...
    memset(buffer, 0, MAX_BUFFER_SIZE);
    // On traite ici UNE requête avec/sans réponse selon le protocole applicatif
    CHECK(recvfrom(listeningSocket, buffer, MAX_BUFFER_SIZE, 0, (struct sockaddr*)&addrClient, &lenSockClient), "Problème reception data du client");
    printf("Message reçu par le client [%s:%d] :\n\t##%s##\n", inet_ntoa(addrClient.sin_addr), ntohs(addrClient.sin_port), buffer);
    strcpy(buffer, "OK");
    CHECK(sendto(listeningSocket, buffer, strlen(buffer)+1, 0, (struct sockaddr*)&addrClient, sizeof(addrClient)), "Problème envoie data du client"); // Ici sizeof car l'addresse est déjà défini
    // On doit reconvertir les Network Order en valeur (adresse client, port d'écoute) => inet_ntoa (network to alpha) converti l'adresse
  }
  close(listeningSocket);
}

void client(int messageNb) {
  int callingSocket; // Socket d'appel
  struct sockaddr_in addrServer; // adressage du serveur à contacter
  struct sockaddr_in myAddr;
  socklen_t lenMyAddr = sizeof(myAddr);
  char buffer1[] = "Salut toi, le fond de l'eau est clair par ici !";
  char buffer2[] = "Salut toi, le fond de l'eau est plutôt brouillé par ici !";

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
  // strlen +1 ajoute le \0
  if(messageNb == 1) {
    CHECK(sendto(callingSocket, buffer1, strlen(buffer1)+1, 0, (struct sockaddr*)&addrServer, sizeof(addrServer)), "Problème envoie data du client"); // Ici sizeof car l'addresse est déjà défini.
    // récupération de mon adresse
    CHECK(getsockname(callingSocket, (struct sockaddr *)&myAddr, &lenMyAddr), "Problème lors de la récupération de mon adresse client");
    printf("Message envoyé au serveur [%s:%d] via [%s:%d] :\n\t##%s##\n", inet_ntoa(addrServer.sin_addr), ntohs(addrServer.sin_port), inet_ntoa(myAddr.sin_addr), ntohs(myAddr.sin_port), buffer1);
    PAUSE;
    //Pour vérifier qu'on garde bien le même numéro de port au fur et à mesure des appels.
    CHECK(sendto(callingSocket, buffer1, strlen(buffer1)-10, 0, (struct sockaddr*)&addrServer, sizeof(addrServer)), "Problème envoie data au serveur"); // Ici sizeof car l'addresse est déjà défini.
    // récupération de mon adresse
    CHECK(getsockname(callingSocket, (struct sockaddr *)&myAddr, &lenMyAddr), "Problème lors de la récupération de mon adresse client");
    printf("Message envoyé au serveur [%s:%d] via [%s:%d] :\n\t##%s##\n", inet_ntoa(addrServer.sin_addr), ntohs(addrServer.sin_port), inet_ntoa(myAddr.sin_addr), ntohs(myAddr.sin_port), buffer1);
    PAUSE;
  }
  else {
    CHECK(sendto(callingSocket, buffer2, strlen(buffer2)+1, 0, (struct sockaddr*)&addrServer, sizeof(addrServer)), "Problème reception data au serveur"); // Ici sizeof car l'addresse est déjà défini.
    // récupération de mon adresse
    CHECK(getsockname(callingSocket, (struct sockaddr *)&myAddr, &lenMyAddr), "Problème lors de la récupération de mon adresse client");
    printf("Message envoyé au serveur [%s:%d] via [%s:%d] :\n\t##%s##\n", inet_ntoa(addrServer.sin_addr), ntohs(addrServer.sin_port), inet_ntoa(myAddr.sin_addr), ntohs(myAddr.sin_port), buffer2);
    PAUSE;
  }
  // On doit reconvertir les Network Order en valeur (adresse client, port d'écoute) => inet_ntoa (network to alpha) converti l'adresse
  close(callingSocket);
}

void lonelyClient() {
  int callingSocket; // Socket d'appel
  struct sockaddr_in addrServer; // adressage du serveur à contacter
  struct sockaddr_in myAddr;
  socklen_t lenMyAddr = sizeof(myAddr);
  socklen_t lenAddServer = sizeof(addrServer);
  char buffer[] = "Salut toi, le fond de l'eau est clair par ici !";

  /* Création du socket */

  CHECK(callingSocket = socket(PF_INET, SOCK_DGRAM, 0), "Problème creation socket d'appel");

  /* Préparation de l'adresse INET */
  myAddr.sin_family = AF_INET;
  myAddr.sin_addr.s_addr = INADDR_ANY;
  myAddr.sin_port = 0; // port assigné dynamiquement
  memset(&myAddr.sin_zero, 0, 8);

  CHECK(bind(callingSocket, (struct sockaddr *)&myAddr, sizeof(myAddr)),"Problème bind socket d'écoute");

  /* Préparation de l'adresse INET */
  addrServer.sin_family = AF_INET;
  addrServer.sin_addr.s_addr = inet_addr("127.0.0.1");
  addrServer.sin_port = htons(50000);
  memset(&addrServer.sin_zero, 0, 8);

  CHECK(sendto(callingSocket, buffer, strlen(buffer)+1, 0, (struct sockaddr*)&addrServer, sizeof(addrServer)), "Problème envoie data au serveur"); // Ici sizeof car l'addresse est déjà défini.
  CHECK(getsockname(callingSocket, (struct sockaddr *)&myAddr, &lenMyAddr), "Problème lors de la récupération de mon adresse client");
  printf("Message envoyé au serveur [%s:%d] via [%s:%d] :\n\t##%s##\n", inet_ntoa(addrServer.sin_addr), ntohs(addrServer.sin_port), inet_ntoa(myAddr.sin_addr), ntohs(myAddr.sin_port), buffer);
  PAUSE;
  CHECK(recvfrom(callingSocket, buffer, MAX_BUFFER_SIZE, 0, (struct sockaddr *)&addrServer, &lenAddServer), "Problème récepetion data du serveur");
  PAUSE;

  close(callingSocket);
}
