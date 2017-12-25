/*
 * threads.cpp
 *
 *  Created on: Dec 24, 2017
 *      Author: djoffe
 */

#include <threads.h>

//handleTwoClients
void* threadRunGame(void *clientArgs) { //void* client_socket1, void* client_socket2) {

	struct ThreadClientArgs *args = (struct ThreadClientArgs *)clientArgs;

	int clientSocket1 = args->clientSocket1;
	int clientSocket2 = args->clientSocket2;


	map<string, vector<int> > games;

	while (true) {
		//handle first client
		bool isClient1Connected = handleOneClient(clientSocket1, games); //return msg
		if (!isClient1Connected) {
			return NULL;
		}
		//handle second client
		bool isClient2Connected = handleOneClient(clientSocket2, games);
		if (!isClient2Connected) {
			return NULL;
		}
	}
	// Close communication with the client
	close(clientSocket1);
	close(clientSocket2);
	return NULL;
}



void* threadRecievePlayers(void *serverArgs) {
	struct ThreadServerArgs *arguments = (struct ThreadServerArgs *)serverArgs;
	int serverSocket = arguments->serverSocket;

	// Define the client socket's structures
	struct sockaddr_in clientAddress;
	socklen_t clientAddressLen;

	//accept and handle one client
	while (true) {
		cout << "Waiting for client connections..." << endl;
		// Accept a new client connection
		int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLen);
		cout << "Client connected" << endl;
		if (clientSocket == -1)
			throw "Error on accept";

		// recieve command and arguments
		string commandName;
		vector<string> argss;
		bool b = readCommand(clientSocket, &commandName, &argss);
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
		comManager.executeCommand(commandName, argss, *arguments->games, clientSocket);

		/* didn't do:
		 *
		 * lock
		 * map -- change map
		 * free lock
		 *
		 */
	}
}

bool handleOneClient(int clientSocket, map<string, vector<int> >& games) {
	int size, n;

	// Read length
	n = read(clientSocket, &size, sizeof(size));
	if (n == -1) {
		cout << "Error reading size" << endl;
		return false;
	}
	if (n == 0) {
		cout << "Client disconnected" << endl;
		return false;
	}

	// Read message
	string move = readStringFromSocket(size, clientSocket);

	//return false if other client disconnected
	if (is_client_closed(findOtherPlayer(games, clientSocket))) {
		cout << "Client disconnected" << endl;
	    return false;
	}

	// Interpret
	string buf; // Have a buffer string
	stringstream msg(move); // Insert the string into a stream
	msg >> buf;
	string comName = buf;
	vector<string> args;
	while (msg >> buf)
		args.push_back(buf);

	CommandManager comManager;
	comManager.executeCommand(comName, args, games, clientSocket);

	return true;
}

int findOtherPlayer(map<string, vector<int> >& games, int clientSocket) {//NEED TO WRITE!!!
	return 0;
}


/*
 * 	// writing message size
	n = write(waitingClient, &size, sizeof(size));
	if (n == -1) {
		cout << "Error writing size to socket" << endl;
		return false;
	}
	//return false if other client disconnected
	if (is_client_closed(waitingClient)) {
	    cout << "Client disconnected" << endl;
	    return false;
	}
	//writing message
	n = write(waitingClient, move.c_str(), strlen(move.c_str()));
	if (n == -1) {
		cout << "Error writing move to socket" << endl;
		return false;
	}
 */

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

Status PlayDecoder(string message, int* row, int* col) {
	string buf; // Have a buffer string
	stringstream msg(message); // Insert the string into a stream
	vector<string> tokens; // Create vector to hold our words

	while (msg >> buf)
		   tokens.push_back(buf);

	int num1 = atoi(tokens[1].c_str());
	int num2 = atoi(tokens[2].c_str());
	*row = num1;
	*col = num2;

	if(num1 == -1 && num2 == -1) {
		return NO_MOVES;
	} else if(num1 == -2 && num2 == -2) {
		return END;
	} else {
		return HAS_MOVE;
	}
}

bool is_client_closed(int cs) {
	pollfd pfd;
	pfd.fd = cs;
	pfd.events = POLLIN | POLLHUP | POLLRDNORM;
	pfd.revents = 0;

	// call poll with a timeout of 100 ms
	if(poll(&pfd, 1, 100) > 0) {
		// if result > 0, this means that there is either data available on the
		// socket, or the socket has been closed
		char buffer[32];
		if(recv(cs, buffer, sizeof(buffer), MSG_PEEK | MSG_DONTWAIT) == 0) {
			// if recv returns zero, that means the connection has been closed:
			return true;
		}
	}
	return false;
}

