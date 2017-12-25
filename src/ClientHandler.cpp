/*
 * ClientHandler.cpp
 *
 *  Created on: Dec 25, 2017
 *      Author: djoffe
 */


#include <ClientHandler.h>




ClientHandler::ClientHandler() {
	this->games = new map<string, vector<int> >;
}

void ClientHandler::handle(int clientSocket) {
	struct ClientMapArgs clientMapArgs;
	clientMapArgs.clientSocket = clientSocket;
	clientMapArgs.games = this->getGames();

	pthread_t thread;
	int rc = pthread_create(&thread, NULL, tTreatClient, &clientMapArgs);
	if (rc) {
		cout << "Error: unable to create thread, " << rc << endl;
		exit(-1);
	}

	pthread_exit (NULL);
}

void ClientHandler::closeClients() {
	//close all sockets in games.
}
map<string, vector<int> >* ClientHandler::getGames() {
	return this->games;
}
