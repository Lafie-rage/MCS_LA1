#include <stdlib.h>
#include "user.h"
#include <stddef.h>
#include <string.h>
#include <stdio.h>

const user_t NULL_USER = { /*! Returned on error by functions */
  -1,
  "NULL USER",
  -1,
  -1
};

void initUsers(users_t *users){
	users->size = 0;
	for (int i = 0; i < MAX_USER; ++i) {
    users->userList[i].socket = UNUSED_SOCKET;
    strcpy(users->userList[i].name, "");
    users->userList[i].destinationId = UNUSED_ID;
    users->userList[i].id = UNUSED_ID;
  }
}

user_t retrieveUserById(const users_t *users, int userId) {
  int i = 0;
  for(i = 0; i < MAX_USER; i++) {
    if(users->userList[i].id == userId) return users->userList[i];
  }
  return NULL_USER;
}

user_t retrieveUserByName(const users_t *users, const char *name){
	int i;
  for(i = 0; i < MAX_USER; i++) {
    if(!strcmp(users->userList[i].name, name)) return users->userList[i];
  }
  return NULL_USER;
}


int addUser(users_t *users, const char* name, int socket,  int userId){
  int i;
  for(i = 0; i < MAX_USER; i++) {
    if(users->userList[i].id == UNUSED_ID) {
      strcpy(users->userList[i].name,name);
      users->userList[i].socket = socket;
      users->userList[i].destinationId = EVERYONE_DESTIONATION_ID;
      users->userList[i].id = userId;
      users->size++;
      return 0;
    }
  }
  return -1;
}

int updateUserDestination(users_t *users, int userId, int destinationId) {
  int i;
  for(i = 0; i < MAX_USER; i++) {
    if(users->userList[i].id == userId) {
      users->userList[i].destinationId = destinationId;
      return 1;
    }
  }
  return 0;
}

int usersCompare(user_t user1, user_t user2) {
  if (user1.id != user2.id) return 1;
  if (strcmp(user1.name, user2.name) != 0) return 1;
  if (user1.socket != user2.socket) return 1;
  if (user1.destinationId != user2.destinationId) return 1;
  return 0;
}

int removeUser(users_t *users, int userId) {
  int i;
  for (i = 0; i < MAX_USER; i++) {
		if(users->userList[i].id == userId){
      users->userList[i].socket = UNUSED_SOCKET;
      strcpy(users->userList[i].name, "");
      users->userList[i].destinationId = UNUSED_ID;
      users->userList[i].id = UNUSED_ID;
      users->size--;
      return 1;
		}
	}
  return 0;
}
