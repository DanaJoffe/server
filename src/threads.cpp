/*
 * threads.cpp
 *
 *  Created on: Dec 24, 2017
 *      Author: djoffe
 */

#include "threads.h"



bool readCommand(int socket, string* comName, vector<string>* args) {
	cout <<"readCommand" << endl;

	int length;
	// Read length
	int n = read(socket, &length, sizeof(length));
	if (n == -1) {
		cout << "Error reading size" << endl;
		return false;
	}
	if (n == 0) {
		cout << "Client disconnected" << endl;
		return false;
	}
	cout <<"read length: "<<length << endl;


	// Read message
	string message = readStringFromSocket(length,socket);

	cout <<"read message: "<<message<< endl;


	// Interpret
	string buf; // Have a buffer string
	stringstream msg(message); // Insert the string into a stream
	msg >> buf;
	*comName = buf;
	while (msg >> buf)
		args->push_back(buf);
	return true;
}

void* tTreatClient(void *clientMapArgs) {

	cout << "tTreatClient" <<endl;

	struct ClientMapArgs *arguments = (struct ClientMapArgs  *)clientMapArgs;
	int clientSocket = arguments->clientSocket;

	cout << "tTreatClient: clientSocket = " << clientSocket <<endl;


	// recieve command and arguments
	string commandName;
	vector<string> args;
	bool b = readCommand(clientSocket, &commandName, &args);
	if (b == false)
		throw "Error reading from socket";

	cout <<"calling command: "<< commandName << endl;
	if (args.empty()) {
		cout <<"args: none"<< endl;
	} else {
		cout <<"args: "<< args[0] << endl;
	}


	GameManager* gameManager = GameManager::getInstance();
	CommandManager comManager;
	comManager.executeCommand(commandName, args, *gameManager->getGames(), clientSocket);


	cout << "tTreatClient - finish treatment" <<endl;

	delete arguments;
	return NULL;
}

string readStringFromSocket(int length, int socket) {
	string str;
	int r;
	char letter;
	while (length >0) {
		r = read(socket, &letter, sizeof(letter));
		if (r == -1) {
			cout << "readStringFromSocket: Error reading message" << endl;
			return NULL;
		}
		str.append(1,letter);
		length -= 1;
	}
	return str;
}
//void* tRecievePlayers(void *serverArgs) {
//	cout << "tRecievePlayers" <<endl;
//
//	struct ThreadServerArgs *arguments = (struct ThreadServerArgs *)serverArgs;
//	int serverSocket = arguments->serverSocket;
//	ClientHandler* handler = arguments->handler;
//
//	// Define the client socket's structures
//	struct sockaddr_in clientAddress;
//	socklen_t clientAddressLen;
//
//	//accept and handle one client
//	while (true) {
//		cout << "Waiting for client connections..." << endl;
//		// Accept a new client connection
//		int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLen);
//		cout << "Client connected" << endl;
//		if (clientSocket == -1){
//			cout << "Error on accept" <<endl;
//			throw "Error on accept";
//		}
//
//
//		cout << "tRecievePlayers: handler->handle" <<endl;
//		handler->handle(clientSocket);
//	}
//}




