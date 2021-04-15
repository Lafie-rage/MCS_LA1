#include "session.h"

int createListeningScoket(char *adrIp, short port) {
  int listeningSocket; // Socket de dialogue avec le client
  struct sockaddr_in addrServer; // adressage d'un socket INET

  CHECK(listeningSocket = socket(PF_INET, SOCK_STREAM, 0), "Problème creation socket d'écoute"); //AF_INET ou PF_INET même chose

  // Préparation de l'adresse INET //
  addrServer.sin_family = AF_INET;
  addrServer.sin_addr.s_addr = inet_addr(adrIP);
  addrServer.sin_port = htons(port);
  memset(&addrServer.sin_zero, 0, 8);

  // Association de l'adressage inet préparé au socket //
  CHECK(bind(listeningSocket, (struct sockaddr *)&addrServer, sizeof(addrServer)),"Problème bind socket d'écoute");

  // Mise en écoute //
  CHECK(listen(listeningSocket, 5), "Problème lors de la mise en écoute du socket serveur"); // FD du socket et nb de client max à mettre en attente
  printf("Server [\033[32m%d\O33[0m] : Waiting for connexion from client on [\033[32%s:%d\033[0m]\n",
        getpid(), inet_ntoa(addrServer.sin_addr), ntohs(addrServer.sin_port));
  return listeningSocket
}

int connectClientToServer(char *adrIp, short port) {
  int mySock;
  struct sockaddr_in addrServer;

  /* Création du socket */
  CHECK(mySock = socket(PF_INET, SOCK_STREAM, 0), "Problème creation socket d'appel");

  /* Préparation de l'adresse INET */
  addrServer.sin_family = AF_INET;
  addrServer.sin_addr.s_addr = inet_addr(adrIp);
  addrServer.sin_port = htons(port);
  memset(&addrServer.sin_zero, 0, 8);
  // Connexion avec le serveur...
  CHECK(connect(mySock, (struct sockaddr *) &addrServer, sizeof(addrServer)), "Problème lors de la connexion au serveur");

  return mySock;
}
