#ifndef USER_H
#define USER_H

/**
 *	\def	MAX_SIZE_NAME	Taille maximale du nom d'un utilisateur
 */
#define MAX_SIZE_NAME 100

/**
 *	\def	MAX_USER	Nombre maximal d'utilisateurs connectés en même temps sur le serveur
 */
#define MAX_USER 10

/**
 *	\def	EVERYONE_DESTIONATION_ID	Socket associé à l'envoie de message à tous les utilisateurs connectés
 */
#define EVERYONE_DESTIONATION_ID 0
#define UNUSED_SOCKET 0
#define UNUSED_ID 0

#define PATTERN_USER_TO_STRING "\t%s\n"

/**
 *	\typedef	user_t Structure représentant un utilisateur vu par le serveur
 */
typedef struct User {
   int id;
   char name[MAX_SIZE_NAME];  /**< Nom de l'utilsateur#name */
   int  socket;               /**< Socket de dialogue avec l'utilsateur#socket */
   int  destinationId;    /**< Id d'utilisateur destinationataire des messages de l'utilisateur. Si le socket vaut EVERYONE_DESTIONATION_ID ça signifie que le message est à envoyé à tout le monde#destiantionSocket */
} user_t;

/**
 *	\typedef	users_t Structure représentant une liste d'utilisateurs vue par le serveur
 */
typedef struct Users
{
	user_t userList[MAX_USER]; /**< Liste des utilsateurs#userList */
	int size;                  /**< Nombre d'utilisateurs connectés#size */
} users_t;

extern const user_t NULL_USER;  /*! Returned on error by functions */

/**
*	\fn		   void initUsers(users_t *users)
*	\brief	 Initialise la liste des utilisateurs en plaçant -1 dans la composante "socket" de chaque entité de la liste.
*	\param	 users : Liste des utilisateurs à initialisé
*/
void initUsers(users_t *users);

/**
*	\fn		   user_t retrieveUserById(users_t users, int Id)
*	\brief	 Renvoie l'utilisateur parmis la liste ayant l'Id transmit en paramètre
*	\param	 users : Liste des utilisateurs connectés au serveur
* \param	 socket : Le socket associé à l'utilisateur recherché
*	\return  L'utilisateur recherché. Renvoie l'utilisateur "NULL_USER" en cas d'échec
*/
user_t retrieveUserById(const users_t *users, int Id);

/**
*	\fn		   user_t retrieveUserByName(const users_t *users, const char *name)
*	\brief	 Renvoie l'utilisateur parmis la liste ayant le nom transmit en paramètre
*	\param	 users : Liste des utilisateurs connectés au serveur
* \param	 name : Le nom de l'utilisateur recheché
*	\return  L'utilisateur recherché. Renvoie l'utilisateur "NULL_USER" en cas d'échec
*/
user_t retrieveUserByName(const users_t *users, const char *name);

/**
*	\fn		   int addUser(users_t *users, const char *name, int socket)
*	\brief	 Ajoute un utilisateur à la liste des utilisateurs connectés et indique si l'ajout a été effectué avec siccès.
*	\param	 users : Liste des utilisateurs connectés au serveur
* \param	 name : Le nom de l'utilisateur à ajouter
* \param   socket : Le socket de l'utilisateur à ajouter
*	\return  1 si l'utilisateur a été ajouté. 0 Sinon.
*/
int addUser(users_t *users, const char *name, int socket,  int userId);

/**
*	\fn		   int updateUserDestination(users_t *users, int userSocket, int destinationSocket)
*	\brief	 Modifie le socket de destination d'un utilisateur
*	\param	 users : Liste des utilisateurs connectés au serveur
* \param   userSocket : Le socket associé à l'utilisateur qui doit être modifié
* \param	 destinationSocket : Le nouveau socket de destination de l'utilisateur
*	\return  1 si l'utilisateur a été modifié. 0 Sinon.
*/
int updateUserDestination(users_t *users, int userSocket, int destinationSocket);

/**
*	\fn		   int usersCompare(user_t user1, user_t user2)
*	\brief	 Compare deux utilisateurs en indiquant en retour s'ils sont différents.
*	\param	 user1 : Le premier utilisateur à comparer
* \param	 user2 : Le deuxième utilisateur à comparer
*	\return  0 s'ils sont égaux, 1 sinon.
*/
int usersCompare(user_t user1, user_t user2);

/**
*	\fn		   int removeUser(users_t *users, int userId)
*	\brief	 Retire un utilisateur de la liste des utilisateurs connectés.
*	\param	 users : Liste des utilisateurs connectés au serveur
* \param	 userId : L'id de l'utilsateur à retirer.
*	\return  1 si l'utilisateur a été retiré, 0 sinon.
*/
int removeUser(users_t *users, int userId);


#endif /* USER_H */
