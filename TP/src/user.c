#include <stdlib.h>
#include "user.h"

#include <stddef.h>
#include <string.h>
user_t retrieveUserByName(users_t users ,char *name){

	for (int i = 0; i < MAX_USER; ++i)
	{
		if(strcmp(users.userList[i].name, name) == 0){
			return users.userList[i];
		}
	}
	user_t z;
	return z;

}

user retrieveUserBySocket(users_t users, int socket) {
  int i = 0;
  while(users.userList[i].socket != socket && i++ < users.size);
  //if(i == users.size) return NULL_USER;
  return users.userList[i];
}

int addUser(users_t users, char name, int socket){
	user_t newUser;
	strcpy(newUser.name,name);
	newUser.socket = socket;
	newUser.destiantionSocket = EVERYONE_DESTIONATION_SOCKET;
	for (int i = 0; i < MAX_USER; ++i)
	{
		if(users.userList[i].socket = NOT_USE_SOCKET){
			users.userList[i] = newUser;
			return 0;
		}

	}
	return -1
}
