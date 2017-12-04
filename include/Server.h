/*
 * Server.h
 *
 *  Created on: Dec 3, 2017
 *      Author: djoffe
 */

#ifndef SERVER_H_
#define SERVER_H_

class Server {
public:
	Server(int port);
	void start();
	void stop();
private:
	int port;
	int serverSocket; // the socket's file descriptor
	void handleTwoClients(int clientSocket, int clientSocket2);
	//CHANGE
	void handleOneClient(int clientSocket);

	int calc(int arg1, const char op, int arg2) const;
};

#endif /* SERVER_H_ */
