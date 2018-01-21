/*
 * ClientHandler.cpp
 *
 * Author1: name & ID: Dana Joffe 312129240
 * Author2: name & ID: Chaviva Moshavi 322082892
 */


#include <ClientHandler.h>

pthread_mutex_t threads_mutex;

void* tRecievePlayers(void* args) {
  struct receiveClientsArgs* arguments = (struct receiveClientsArgs*)args;
	int serverSocket = arguments->serverSocket;
	ThreadPool* threadPool = arguments->thread_pool;

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

	    handler.handle(&clientSocket);
	}
}

ClientHandler::ClientHandler() {}

void ClientHandler::handle(int* clientSocket) {
  pthread_t thread;
  int rc = pthread_create(&thread, NULL, tTreatClient, clientSocket);
  if (rc) {
    cout << "Error: unable to create thread, " << rc << endl;
    exit(-1);
  }
	return;
}
