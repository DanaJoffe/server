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
  struct treatClientArgs* args = (struct treatClientArgs*)arguments;

	// receive command and arguments
	string commandName;
	vector<string> commandArgs;
	bool b = readCommand(args->clientSocket, &commandName, &commandArgs);
	if (b == false)
		throw "Error reading from socket";

	//execute command
	CommandManager comManager;
	comManager.executeCommand(commandName, commandArgs, args->clientSocket);

	delete args;
	return NULL;
}
