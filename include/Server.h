/*
 * Server.h
 *
 *  Created on: Dec 3, 2017
 *      Author: djoffe
 */

#ifndef SERVER_H_
#define SERVER_H_

enum Status {NO_MOVES, HAS_MOVE, END};

/*
 * Communication Protocol description:
 * the server works with 2 clients, and from each client it gets 2 messages maximum:
 * 1. the client status (integer)
 * 2. depends on the status - the client's move (2 integers)
 *
 * the logic goes this way: (according to Status enum)
 * - read the first input and send it to the other client.
 * - if the first input - the status - is: 0 - stop reading.
 * 	 									   1 - read move
 * 	 									   2 - close client's sockets.
 * - in case of reading a move: server will read 2 integers, and will send them (and a
 *   separating comma) to the other client.
 *
 * all clients know and obey the protocol.
 */
class Server {
public:
	Server(int port);
	void start();
	void stop();
	int serverSocket; // the socket's file descriptor
	void handleTwoClients(int clientSocket, int clientSocket2);
	bool handleOneClient(int clientSocket, int waitingClient);
	bool is_client_closed(int cs);


private:
	int port;
	int calc(int arg1, const char op, int arg2) const;
};

#endif /* SERVER_H_ */
