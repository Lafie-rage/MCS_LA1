#include "user.h"

user retrieveUserBySocket(users_t users, int socket) {
  int i = 0;
  while(users[i].socket != socket && i++ < users.size);
  if(i == users.size) return NULL; // User not found
  return users[i];
}
