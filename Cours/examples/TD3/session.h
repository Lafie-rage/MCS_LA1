/**
 * \fn int createListeningScoket(char *adrIp, short port)
 * \brief Create listing socker.
 *
 * \param adrIp The server's socket's IP address
 * \param port The server's socket's port.
 *
 * \return The socket create using the addresse send as paramter.
 */
int createListeningScoket(char *adrIp, short port);

/**
 * \fn int connectClientToServer(char *adrIp, short port)
 * \brief Create conecction socket to the server.
 *
 * \param adrIp The server's socket's IP address to which you want the client to connect.
 * \param port The server's socket's port to which you want the client to connect.
 *
 * \return The socket create using the addresse send as paramter.
 */
int connectClientToServer(char *adrIp, short port);
