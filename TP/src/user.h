#ifndef USER_H
#define USER_H

#define MAX_SIZE_NAME 100
#define MAX_USER 10

typedef struct User {
   char  name[MAX_SIZE_NAME];
   int   socket;
   char  talkTo[MAX_SIZE_NAME];
} user_t; 

typedef struct Users
{
	user_t userList[MAX_CLIENT];
	int nbUser;
}users_t;

#endif /* USER_H */