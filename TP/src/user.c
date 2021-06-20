#include "user.h"

#include <stddef.h>
#include <string.h>
user_t findUserByName(users_t users ,char *name){

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
  while(users[i].socket != socket && i++ < users.size);
  if(i == users.size) return NULL; // User not found
  return users[i];
}
