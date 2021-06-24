# MINI CHAT USING STREAM SOCKETS

This project consists on a simple mini chat that allows you to communicate with any connected user via private message or to send message to every connected users.  
This a school projet whose aim is to make us use the concepts that we've seen during our lessons.  
Therefore, we had to create an internet connection via socket between a server and a multitude of clients while respecting an OSI model-based architecture.  

The whole projet is done using C and is intetended to work on Linux-based OS only and maybe even only on Ubuntu.

## COMMANDS

Here is the list of the commands that you can use, how they must be spelled and what are their purpose:  
- **/login <username>** : Log you in with the server. You'll be known as the username you've provided. If this username is already taken, you'll be ask to provide another one.  This command can be used before logging in, obviously.
- **/list** : List connected users. This command can be used before logging in.
- **/talk**  : Select the general channel. Since this command, all your messages will be broadcasted to all connected users. This command cannot be used before logging in.
- **/private <username>**  : Select the private channel between you and the user. Since this command, all your messages will be send to the user you've chosen. This command cannot be used before logging in.
If the user you've chosen is disconnected, you'll receive an error message and you won't be moved to the private channel with him.
- **/bye** : Close connection with server. This command cannot be used before logging in.

## ERROR RETURNED BY SERVER
  
- **CMD_ERROR_UNKNOW_NUM** : The command typed is not defined.
- **CMD_ERROR_ALREADY_LOGGED_IN_NUM** : The client is already logged in.
- **CMD_ERROR_USERNAME_ALREADY_TAKEN_NUM** : The username is already taken by another client.
- **CMD_ERROR_MISSING_ARGUMENT_NUM** : An argument is missing in the command received.
- **CMD_ERROR_USER_DISCONNECTED_SWAP_TO_EVERYONE_NUM** : The client were talking to someone who is disconnected. He's know swap to the evreyone channel. It happens when the client try to send a message after using **/private** to a client.
- **CMD_ERROR_USER_DISCONNECTED_NUM** : The client wants to talk to an unlogged user. It happens when the client use **/private** providing a username of a client that's not connected at the moment.
- **CMD_ERROR_SERVER_FULL_NUM** : There is no more space to let a new client logging in.
- **CMD_ERROR_USER_NOT_LOOGED_IN_NUM** : The client try use any other command than **/login** and **/list** before being logged.
- **CMD_ERROR_UNKNOW_ERROR_NUM** : An unknown error happened.
  
## Using the project

First of all, you need to build the project. To do that, you can simply use the makefile in the root of the project.  

Then you'll have to run separatly the client and the server. You must always run the server first otherwise, the clients will try to connect a non existant server and they will shutdown.  
You can specify on which address and port you want to bind your server this way :

```
./server <any IPv4 that your computer can use> <any unused port>
```

If you specify a different address from 0.0.0.0:50000 (which is the default port for the server), you'll have to specify the address of the server when you run the client the same way as for the server :
  
```
./client <server's IPv4> <server's port>
```
  
After having run the server and the clients, you can log you in each client by using the command **/login**. You'll have to provide a username with the command. This username must not be used by any of the clients that are already logged in the server.

After being logged, you can know type message or use the other commands.
To send a message, you just need to type it. Your message cannot start with "/" otherwise it would be recognized as a command.
The default destination of your message will be to everyone, but you can switch to private message by using **/private**. You'll have to provide the username of the client with the one you want to talk.
And you can swap back to the default destination mode by using **/talk**.

When you want to disconnect a client, you can use "/bye".
  
## Protocol
  
In oder to communicate, the server and the clients are flowing a specific protocol as any dialog app.
Every request are composed of a :
- A type of request : It describes the type of message that has been sent. Typically, any message have 0 as number, any command or error have a number superior to 0. A command is a message starting with '/'.
- A body : The text typed by the client or the message that will be prompt to the client.
- The size of the body : In order to know how many character have to be read.

The type of the request depends on the request message or command you send.
Here is a table summarizing the type of request.
|**Type of request**|**Number corresponding**|
|:-:|:-:|
|A simple message like *Hello world*|0|
|Command */bye*|10|
|Command */login*|11|
|Command */list*|20|
|Command */talk*|21|
|Command */private*|22|
|Error : *CMD_ERROR_UNKNOW_NUM*|90|
|Error : *CMD_ERROR_ALREADY_LOGGED_IN_NUM*|91|
|Error : *CMD_ERROR_USERNAME_ALREADY_TAKEN_NUM*|92|
|Error : *CMD_ERROR_MISSING_ARGUMENT_NUM*|93|
|Error : *CMD_ERROR_USER_DISCONNECTED_SWAP_TO_EVERYONE_NUM*|94|
|Error : *CMD_ERROR_USER_DISCONNECTED_NUM*|95|
|Error : *CMD_ERROR_SERVER_FULL_NUM*|96|
|Error : *CMD_ERROR_USER_NOT_LOOGED_IN_NUM*|97|
|Error : *CMD_ERROR_UNKNOW_ERROR_NUM*|32767 (Maximum value of a short type number)|
  
## Contributors

[ElColeoptero](https://github.com/elColeoptero)  
[Lafie-rage](https://github.com/Lafie-rage)  
