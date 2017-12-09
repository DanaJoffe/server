/*
 * Server.h
 *
 * Author1: name & ID: Dana Joffe 312129240
 * Author2: name & ID: Chaviva Moshavi 322082892
 */

#ifndef SERVER_H_
#define SERVER_H_

//input message status
enum Status {NO_MOVES, HAS_MOVE, END};

/*
 * Server
 */
class Server {
public:
  /*
   * constructs server with port.
   */
	Server(int port);
	/*
	 * starts server.
	 */
	void start();
	/*
	 * stops server.
	 */
	void stop();
	/*
	 * sends and receives messages to 2 clients.
	 */
	void handleTwoClients(int clientSocket1, int clientSocket2);
	/*
	 * receives messages from one client and sends them to the waiting client
	 */
	bool handleOneClient(int clientSocket, int waitingClient);
	/*
	 *checks if client disconnected
	 */
	bool is_client_closed(int cs);


private:
	int port;
	int serverSocket;
};

#endif /* SERVER_H_ */
