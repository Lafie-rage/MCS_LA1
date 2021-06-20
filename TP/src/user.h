#ifndef USER_H
#define USER_H

#define MAX_SIZE_NAME 100
#define MAX_USER 2

#define EVERYONE_DESTIONATION_SOCKET 0
#define NOT_USE_SOCKET 0

#define PATTERN_USER_TO_STRING "\t%s\n"

typedef struct User {
   char name[MAX_SIZE_NAME];
   int  socket;
   int  destiantionSocket;
} user_t;

typedef struct Users
{
	user_t userList[MAX_USER];
	int size;
} users_t;

void initUsers(users_t *users);

user_t retrieveUserBySocket(users_t *users, int socket);

user_t retrieveUserByName(users_t *users ,char *name);

int addUser(users_t *users, char *name, int socket);

void listUserToString(users_t *users,char *stringListUser);
#endif /* USER_H */
