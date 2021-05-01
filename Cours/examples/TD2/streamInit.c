#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <signal.h>
#include <wait.h>
#include <pthread.h>

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
#define MAX_CLIENT_THREADS 10

typedef struct thread_data {
  int dialogSocket;
  struct sockaddr_in addrClient;
} t_thread_data;

void client(int c, char **v);
void *thread_dialog(void *arg);
void server_thread(int c, char **v);
void server_fork(int c, char **v);
void deroute(int sigNum);
void installSigServ(int sigNum);
void dialogServer2Client(int dialogSocket, struct sockaddr_in addrClient);
void dialClient2Server(int callingSocket, struct sockaddr_in addrServer);

int listeningSocket; // Socket d'écoute en mode stream
pthread_t threads[10];
int nbThreads;

int main(int c, char **v) {
  #ifdef CLIENT
    client(c, v);
  #endif
  #ifdef SERVER_FORK
    installSigServ(SIGCHLD);
    installSigServ(SIGINT);
    server_fork(c, v);
  #endif
  #ifdef SERVER_THREAD
    installSigServ(SIGINT);
    server_thread(c, v);
  #endif
  return 0;
}

void *thread_dialog(void *arg) {
  t_thread_data threadData = *((t_thread_data *) arg);
  dialogServer2Client(threadData.dialogSocket, threadData.addrClient); // On ne passe pas par adresse &addrClient pour éviter sa modification si on acceptait plusieurs clients à la suite...
  close(threadData.dialogSocket);
  PAUSE("SERVER-close()");
  pthread_exit(0);
}

// Serveur utilisant des threads
void server_thread(int c, char **v) {
  int dialogSocket; // Socket de dialogue avec le client
  struct sockaddr_in addrServer; // adressage d'un socket INET
  struct sockaddr_in addrClient; // adressage du client connecté
  socklen_t lenSockClient = sizeof(addrClient);
  t_thread_data threadData;

  /* Création du socket */
  // Mode STREAM => mode connecté
  // Dernier arg => protocole de transport. Si 0 => Selection auto par l'OS.
  CHECK(listeningSocket = socket(PF_INET, SOCK_STREAM, 0), "Problème creation socket d'écoute"); //AF_INET ou PF_INET même chose
  // Il faut associer un adressage INET au socket
  PAUSE("SERVER-socket()");
  /* Préparation de l'adresse INET */
  addrServer.sin_family = AF_INET; //sin pour socket internet
  addrServer.sin_addr.s_addr = INADDR_ANY; // INADDR_ANY => 0.0.0.0 : toute interface réseau; On peut aussi mettre l'adresse d'une interface directement.
  addrServer.sin_port = htons(PORT_SRV); // Conversion du port en network order
  // Si on utilise pas htons, le port vu dans netstat ne sera plus celui entré dans le code
  // Il ne lit pas les octets de la même manière.
  //addrServer.sin_port = 50000;
  memset(&addrServer.sin_zero, 0, 8); // Zone de bourrage

  // Bind marche aussi bien pour les socket inet qu'unix ou ipx...
  // Or les structure d'addressages ne sont pas les mêmes. Il faut donc cast sockaddr_in en sockaddr puis envoyer la taille de la structure d'addressage en argument.
  CHECK(bind(listeningSocket, (struct sockaddr *)&addrServer, sizeof(addrServer)),"Problème bind socket d'écoute");
  PAUSE("SERVER-bind()");
  // Mise en écoute //
  // Puisque c'est un socket stream, on doit le mettre en écoute avec listen
  CHECK(listen(listeningSocket, 5), "Problème lors de la mise en écoute du socket serveur"); // FD du socket et nb de client max à mettre en attente
  // Le socket n'est pas disponible avant le listen...
  PAUSE("SERVER-listen()");
  while(1) { // écoute permanente (deamon)
    // Attente d'une connexion et dialogue avec le client
    CHECK(dialogSocket = accept(listeningSocket, (struct sockaddr *)&addrClient, &lenSockClient), "Problème lors de l'acceptation d'une connexion côté serveur");
    PAUSE("SERVER-accept()");

    threadData.dialogSocket = dialogSocket;
    threadData.addrClient = addrClient;

    CHECK(pthread_create(threads + nbThreads, NULL, thread_dialog, &threadData), "SERVER-phtread_create()");
    nbThreads++;
  }
}

// Serveur utilisant des processus fils
void server_fork(int c, char **v) {
  int dialogSocket; // Socket de dialogue avec le client
  int childPid;
  struct sockaddr_in addrServer; // adressage d'un socket INET
  struct sockaddr_in addrClient; // adressage du client connecté
  socklen_t lenSockClient = sizeof(addrClient);

  /* Création du socket */
  // Mode STREAM => mode connecté
  // Dernier arg => protocole de transport. Si 0 => Selection auto par l'OS.
  CHECK(listeningSocket = socket(PF_INET, SOCK_STREAM, 0), "Problème creation socket d'écoute"); //AF_INET ou PF_INET même chose
  // Il faut associer un adressage INET au socket
  PAUSE("SERVER-socket()");
  /* Préparation de l'adresse INET */
  addrServer.sin_family = AF_INET; //sin pour socket internet
  addrServer.sin_addr.s_addr = INADDR_ANY; // INADDR_ANY => 0.0.0.0 : toute interface réseau; On peut aussi mettre l'adresse d'une interface directement.
  addrServer.sin_port = htons(PORT_SRV); // Conversion du port en network order
  // Si on utilise pas htons, le port vu dans netstat ne sera plus celui entré dans le code
  // Il ne lit pas les octets de la même manière.
  //addrServer.sin_port = 50000;
  memset(&addrServer.sin_zero, 0, 8); // Zone de bourrage

  // Bind marche aussi bien pour les socket inet qu'unix ou ipx...
  // Or les structure d'addressages ne sont pas les mêmes. Il faut donc cast sockaddr_in en sockaddr puis envoyer la taille de la structure d'addressage en argument.
  CHECK(bind(listeningSocket, (struct sockaddr *)&addrServer, sizeof(addrServer)),"Problème bind socket d'écoute");
  PAUSE("SERVER-bind()");
  // Mise en écoute //
  // Puisque c'est un socket stream, on doit le mettre en écoute avec listen
  CHECK(listen(listeningSocket, 5), "Problème lors de la mise en écoute du socket serveur"); // FD du socket et nb de client max à mettre en attente
  // Le socket n'est pas disponible avant le listen...
  PAUSE("SERVER-listen()");
  while(1) { // écoute permanente (deamon)
    // Attente d'une connexion et dialogue avec le client
    CHECK(dialogSocket = accept(listeningSocket, (struct sockaddr *)&addrClient, &lenSockClient), "Problème lors de l'acceptation d'une connexion côté serveur");
    PAUSE("SERVER-accept()");

    // Exemple traitement de la connexion par un processus fils
    CHECK(childPid=fork(), "SERVER : Problème création processus fils");
    if(childPid == 0) {
      dialogServer2Client(dialogSocket, addrClient); // On ne passe pas par adresse &addrClient pour éviter sa modification si on acceptait plusieurs clients à la suite...
      close(dialogSocket);
      PAUSE("SERVER-close()");
      exit(0);
    }
    close(dialogSocket); // Pas besoin de se socket, seul le fils l'utilise
    // Le père continue d'accepter des connexions...
  }
}

void deroute(int sigNum) {
  int status;
  int i;
  switch (sigNum) {
    case SIGCHLD:
      wait(&status); // Attente des fils avant fin...
      break;
    case SIGINT:
      #ifdef SERVER_THREAD
        for(i = 0; i < nbThreads; i++)
          pthread_join(threads[i], NULL);
      #endif
      close(listeningSocket);
      printf("Fin du serveur [%d]\n", getpid());
      exit(0); // fermeture après fermeture du socket.
      break;
  }
}

void installSigServ(int sigNum) {
  struct sigaction newAction;
  newAction.sa_handler = deroute;
  newAction.sa_flags = SA_RESTART;
  sigemptyset(&newAction.sa_mask);
  CHECK(sigaction(sigNum, &newAction, NULL), "SERVER : Problème application du sigaction");
}

void dialogServer2Client(int dialogSocket, struct sockaddr_in addrClient) {
  char buffer[MAX_BUFFER_SIZE];
  // On vide le buffer pour éviter les résidus des anciens messages si celui envoyé ne contient pas l'\0...
  memset(buffer, 0, MAX_BUFFER_SIZE);
  // On traite ici UNE requête avec/sans réponse selon le protocole applicatif
  CHECK(read(dialogSocket, buffer, MAX_BUFFER_SIZE), "Problème reception data du client");
  PAUSE("SERVER-read()");
  printf("Message reçu par le client [%s:%d] :\n\t##%s##\n", inet_ntoa(addrClient.sin_addr), ntohs(addrClient.sin_port), buffer);
  CHECK(write(dialogSocket, MSG_OK, strlen(MSG_OK)+1), "Problème envoie data du client"); // Ici sizeof car l'addresse est déjà défini
  PAUSE("SERVER-client()");
}

void client(int c, char **v) {
  int callingSocket; // Socket d'appel
  struct sockaddr_in addrServer; // adressage du serveur à contacter
  socklen_t lenAddServer = sizeof(addrServer);

  /* Création du socket */

  CHECK(callingSocket = socket(PF_INET, SOCK_STREAM, 0), "Problème creation socket d'appel");
  PAUSE("CLIENT-socket()");

  /* Préparation de l'adresse INET */
  addrServer.sin_family = AF_INET;
  addrServer.sin_addr.s_addr = inet_addr(IP_LOOP);
  addrServer.sin_port = htons(PORT_SRV);
  memset(&addrServer.sin_zero, 0, 8);
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
