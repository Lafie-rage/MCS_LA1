#ifndef USER_H
#define USER_H

#define MAX_SIZE_NAME 100
#define MAX_USER 10

#define EVERYONE_DESTIONATION_SOCKET 0

typedef struct User {
   char name[MAX_SIZE_NAME];
   int  socket;
   int  destiantionSocket;
} user_t;

typedef struct Users
{
	user_t userList[MAX_USER];
	int size;
}users_t;

user_t findUserByName(users_t users ,char *name);

#endif /* USER_H */
