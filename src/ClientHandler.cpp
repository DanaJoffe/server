/*
 * ClientHandler.cpp
 *
 *  Created on: Dec 25, 2017
 *      Author: djoffe
 */


#include <ClientHandler.h>


void* tRecievePlayers1(void* server_socket) {
	struct ServerSocketArg* arg2 = (struct ServerSocketArg*)server_socket;
	int serverSocket = arg2->serverSocket;
	ClientHandler handler = ClientHandler();

	// Define the client socket's structures
		struct sockaddr_in clientAddress;
		socklen_t clientAddressLen;

		while (true) {
			cout << "Waiting for client connections..." << endl;
			// Accept a new client connection
			int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLen);
			cout << "Client connected" << endl;
			if (clientSocket == -1)
				throw "Error on accept";

		handler.handle(clientSocket);
	}
}

ClientHandler::ClientHandler() {}

void ClientHandler::handle(int clientSocket) {
	struct ClientMapArgs *clientMapArgs = new struct ClientMapArgs();

	clientMapArgs->clientSocket = clientSocket;


	pthread_t thread;
	int rc = pthread_create(&thread, NULL, tTreatClient, clientMapArgs);
	if (rc) {
		cout << "Error: unable to create thread, " << rc << endl;
		exit(-1);
	}
	return;
}



