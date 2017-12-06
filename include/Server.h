/*
 * Server.h
 *
 *  Created on: Dec 3, 2017
 *      Author: djoffe
 */

#ifndef SERVER_H_
#define SERVER_H_


enum Input {NO_MOVES, HAS_MOVE, END};

class Server {
public:
	Server(int port);
	void start();
	void stop();
	int serverSocket; // the socket's file descriptor
	void handleTwoClients(int clientSocket, int clientSocket2);

	//CHANGE
	bool handleOneClient(int clientSocket, int waitingClient);

private:
	int port;
	int calc(int arg1, const char op, int arg2) const;
};

#endif /* SERVER_H_ */
