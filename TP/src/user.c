#include <stdlib.h>
#include "user.h"

#include <stddef.h>
#include <string.h>
#include <stdio.h>

void initUsers(users_t *users){
	users->size = 0;
	for (int i = 0; i < MAX_USER; ++i)
	{
		users->userList[i].socket = NOT_USE_SOCKET;
	}

}
user_t retrieveUserByName(users_t *users ,char *name){
	user_t notFound;
	notFound.socket=-1;
	for (int i = 0; i < MAX_USER; ++i)
	{
		printf("%s VS %s \n",users->userList[i].name, name);
		if(strcmp(users->userList[i].name, name) == 0 && users->userList[i].socket != NOT_USE_SOCKET){
			return users->userList[i];
		}
	}
	
	return notFound;

}

user_t retrieveUserBySocket(users_t *users, int socket) {
  int i = 0;
  while(users->userList[i].socket != socket && i++ < users->size);
  //if(i == users->size) return NULL_USER;
  return users->userList[i];
}

int addUser(users_t *users, char* name, int socket){

	for (int i = 0; i < MAX_USER; ++i)
	{
		if(users->userList[i].socket == NOT_USE_SOCKET){
			strcpy(users->userList[i].name,name);
			users->userList[i].socket = socket;
			users->userList[i].destiantionSocket = EVERYONE_DESTIONATION_SOCKET;
			users->size++;
			return 0;
		}
	}
	return -1;
}

void listUserToString(users_t *users,char *stringListUser){

	char currentUser[MAX_SIZE_NAME+2];
	strcpy(stringListUser,"Connected users :\n");
	for (int i = 0; i < MAX_USER; ++i)
	{
		if(users->userList[i].socket != NOT_USE_SOCKET){
			printf("%d --> ",i);
			printf("%s\n",users->userList[i].name);
			sprintf(currentUser,PATTERN_USER_TO_STRING,users->userList[i].name);
			strcat(stringListUser,currentUser);
		}
	}

}