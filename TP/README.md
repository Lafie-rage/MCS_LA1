# MINI CHAT USING STREAM SOCKETS

This project consists on a simple mini chat that allows you to communicate with any connected user via private message or to send message to every connected users.  
This a school projet whose aim is to make us use the concepts that we've seen during our lessons.  
Therefore, we had to create an internet connection via socket between a server and a multitude of clients while respecting an OSI model-based architecture.  

The whole projet is done using C and is intetended to work on Linux-based OS only and maybe even only on Ubuntu.

## COMMANDS

Here is the list of the commands that you can use, how they must be spelled and what are their purpose:  
- **/login <username>** : Log you in with the server. You'll be known as the username you've provided. If this username is already taken, you'll be ask to provide another one.  
- **/list** : List connected users  
- **/talk**  : Select the general channel. Since this command, all your messages will be broadcasted to all connected users.  
- **/private <username>**  : Select the private channel between you and the user. Since this command, all your messages will be send to the user you've chosen.
If the user you've chosen is disconnected, you'll receive an error message and you won't be moved to the private channel with him.
- **bye** : Close connection with server.
  
## Using the project

First of all, you need to build the project. To do that, you can simply use the makefile in the root of the project.  

Then you'll have to run separatly the client and the server. You must always run the server first otherwise, the clients will try to connect a non existant server and they will shutdown.  
You can specify on which address and port you want to bind your server this way :

```
./server <any IPv4 that your computer can use> <any unused port>
```

If you specify a different port from 50000 (which is the default port for the server), you'll have to specify the address of the server when you run the client the same way as for the server :
  
```
./client <server's IPv4> <server's port>
```
  
After having run the server and the clients, you can log you in each client by using the command **/login**. You'll have to provide a username with the command. This username must not be used by any of the clients that are already logged in the server.

After being logged, you can know type message or use the other commands.
To send a message, you just need to type it. Your message cannot start with "/" otherwise it would be recognized as a command.
The default destination of your message will be to everyone, but you can switch to private message by using **/private**. You'll have to provide the username of the client with the one you want to talk.
And you can swap back to the default destination mode by using **/talk**.

When you want to disconnect a client, you can use "/bye".
  
## Contributors

[ElColeoptero](https://github.com/elColeoptero)  
[Lafie-rage](https://github.com/Lafie-rage)  
