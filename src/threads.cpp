/*
 * threads.cpp
 *
 *  Created on: Dec 24, 2017
 *      Author: djoffe
 */

#include "threads.h"



bool readCommand(int socket, string* comName, vector<string>* args) {
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

	// Read message
	string message = readStringFromSocket(length,socket);

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

	// recieve command and arguments
	string commandName;
	vector<string> args;
	bool b = readCommand(clientSocket, &commandName, &args);
	if (b == false)
		throw "Error reading from socket";

	/*// CHECK::
	cout << "commandName: " <<commandName<<endl;
	cout << "arguments: ";
	for (unsigned i = 0; i < arguments.size(); i++) {
		cout << arguments[i] << " ";
	}
	cout << endl;
	*/

	CommandManager comManager;
	comManager.executeCommand(commandName, args, *arguments->games, clientSocket);

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
void* tRecievePlayers(void *serverArgs) {
	cout << "tRecievePlayers" <<endl;

	struct ThreadServerArgs *arguments = (struct ThreadServerArgs *)serverArgs;
	int serverSocket = arguments->serverSocket;
	ClientHandler* handler = arguments->handler;

	// Define the client socket's structures
	struct sockaddr_in clientAddress;
	socklen_t clientAddressLen;

	//accept and handle one client
	while (true) {
		cout << "Waiting for client connections..." << endl;
		// Accept a new client connection
		int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLen);
		cout << "Client connected" << endl;
		if (clientSocket == -1){
			cout << "Error on accept" <<endl;
			throw "Error on accept";
		}


		cout << "tRecievePlayers: handler->handle" <<endl;
		handler->handle(clientSocket);
	}
}



