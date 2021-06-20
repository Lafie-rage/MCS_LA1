#include <stdlib.h>
#include "user.h"

user_t retrieveUserBySocket(users_t users, int socket) {
  int i = 0;
  while(users.userList[i].socket != socket && i++ < users.size);
  //if(i == users.size) return NULL_USER;
  return users.userList[i];
}
