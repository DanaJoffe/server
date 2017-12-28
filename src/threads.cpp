/*
 * threads.cpp
 *
 *  Created on: Dec 24, 2017
 *      Author: djoffe
 */

#include "threads.h"

pthread_mutex_t map_mutex;

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
	struct ClientMapArgs *arguments = (struct ClientMapArgs  *)clientMapArgs;
	int clientSocket = arguments->clientSocket;

	// recieve command and arguments
	string commandName;
	vector<string> args;
	bool b = readCommand(clientSocket, &commandName, &args);
	if (b == false)
		throw "Error reading from socket";

	if (args.empty()) {
		cout <<"args: none"<< endl;
	} else {
		cout <<"args: "<< args[0] << endl;
	}

	CommandManager comManager;
	comManager.executeCommand(commandName, args, clientSocket);

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
