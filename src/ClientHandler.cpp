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
	cout << "ClientHandler::handle" <<endl;

	struct ClientMapArgs* clientMapArgs = new struct ClientMapArgs();
	clientMapArgs->clientSocket = clientSocket;
	clientMapArgs->games = this->getGames();

	pthread_t thread;
	int rc = pthread_create(&thread, NULL, tTreatClient, clientMapArgs);
	if (rc) {
		cout << "Error: unable to create thread, " << rc << endl;
		exit(-1);
	}
	return;
}

void ClientHandler::closeClients() {
	//close all sockets in games.
}
map<string, vector<int> >* ClientHandler::getGames() {
	return this->games;
}
