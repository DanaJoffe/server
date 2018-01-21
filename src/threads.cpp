/*
 * threads.cpp
 *
 * Author1: name & ID: Dana Joffe 312129240
 * Author2: name & ID: Chaviva Moshavi 322082892
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

void* tTreatClient(void* arguments) {
  int clientSocket = *(int*)arguments;
	// receive command and arguments
	string commandName;
	vector<string> commandArgs;
	bool b = readCommand(clientSocket, &commandName, &commandArgs);
	if (b == false)
		throw "Error reading from socket";

	//execute command
	CommandManager comManager;
	comManager.executeCommand(commandName, commandArgs, clientSocket);

	return NULL;
}

void* tRunGame(void* gameName) {
	string* game_name_ptr = (string*)gameName;
	GameManager* gameManager = GameManager::getInstance();
	string game_name = *game_name_ptr;
	delete game_name_ptr;
	gameManager->runGame(game_name);
	return NULL;
}

void* tRecievePlayers(void* args) {
  struct receiveClientsArgs* arguments = (struct receiveClientsArgs*)args;
	int serverSocket = arguments->serverSocket;

	ClientHandler handler = ClientHandler(arguments->pool);

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



