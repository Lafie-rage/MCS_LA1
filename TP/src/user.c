#include <stdlib.h>
#include "user.h"
#include <stddef.h>
#include <string.h>
#include <stdio.h>

const user_t NULL_USER = { /*! Returned on error by functions */
  "NULL USER",
  -1,
  -1
};

user_t retrieveUserBySocket(const users_t *users, int socket) {
  puts("BY SOCKET");
  int i = 0;
  while(users->userList[i].socket != socket && i < users->size) i++;
  printf("i : %d, size : %d\n", i, users->size);
  if(i == users->size) return NULL_USER;
  printf("\ni : %d, \n\tname : %s\n", i-1, users->userList[i].name);
  return users->userList[i];
}

void initUsers(users_t *users){
	users->size = 0;
	for (int i = 0; i < MAX_USER; ++i) {
    users->userList[i].socket = NOT_USE_SOCKET;
    strcpy(users->userList[i].name, "");
    users->userList[i].destinationSocket = NOT_USE_SOCKET;
  }
}
user_t retrieveUserByName(const users_t *users, const char *name){
  puts("BY NAME");
	int i = 0;
  while(strcmp(users->userList[i].name, name) && i < users->size) i++;
  printf("i : %d, size : %d\n", i, users->size);
  if(i == users->size) return NULL_USER;
  printf("\ni : %d, \n\tname : %s\n", i-1, users->userList[i].name);
  return users->userList[i];
}


int addUser(users_t *users, const char* name, int socket){
  puts("ADD");
  int i = 0;

  while(users->userList[i].socket != NOT_USE_SOCKET && i < users->size) i++;
  printf("i : %d, size : %d\n", i, users->size);
  if(i > users->size) return -1;
  printf("\ni : %d, \n\tname : %s\n", i-1, users->userList[i].name);
	strcpy(users->userList[i].name,name);
	users->userList[i].socket = socket;
	users->userList[i].destinationSocket = EVERYONE_DESTIONATION_SOCKET;
	users->size++;
	return 0;
}

int updateUserSocket(users_t *users, int userSocket, int destinationSocket) {
  int i = 0;
  while(users->userList[i].socket != userSocket && i < users->size) i++;
  printf("\ni : %d, \n\tname : %s\n", i, users->userList[i].name);
  if(i == users->size) return 0;
  users->userList[i].socket = destinationSocket;
  return 1;
}

void listUserToString(const users_t *users, char *stringListUser){

	char currentUser[MAX_SIZE_NAME+2];
	strcpy(stringListUser,"Connected users :\n");
	for (int i = 0; i < MAX_USER; ++i)
	{
		if(users->userList[i].socket != NOT_USE_SOCKET){
			sprintf(currentUser,PATTERN_USER_TO_STRING,users->userList[i].name);
			strcat(stringListUser,currentUser);
		}
	}

}

int usersCompare(user_t user1, user_t user2) {
  if (strcmp(user1.name, user2.name) != 0) return 1;
  if (user1.socket != user2.socket) return 1;
  if (user1.destinationSocket != user2.destinationSocket) return 1;
  return 0;
}
