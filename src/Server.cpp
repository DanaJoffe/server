/*
 * Server.cpp
 *
 * Author1: name & ID: Dana Joffe 312129240
 * Author2: name & ID: Chaviva Moshavi 322082892
 */

#include "Server.h"

#define MAX_CONNECTED_CLIENTS 10


Server::Server(int port): port_(port), serverSocket_(0) {
  cout << "Server" << endl;
}


void Server::start() {
	// Create a socket point
	serverSocket_ = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket_ == -1) {
		throw "Error opening socket";
	}
	// Assign a local address to the socket
	struct sockaddr_in serverAddress;
	bzero((void *)&serverAddress,sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(port_);
	if (bind(serverSocket_, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
		throw "Error on binding";
	}
	// Start listening to incoming connections
	listen(serverSocket_, MAX_CONNECTED_CLIENTS);

	ClientHandler handler = ClientHandler();


	cout << " Server::start()" <<endl;


	struct ThreadServerArgs args;
	args.serverSocket = serverSocket_;
	args.handler = &handler;

//	map_mutex = PTHREAD_MUTEX_INITIALIZER; //check if we need this

	pthread_t thread;
	int rc = pthread_create(&thread, NULL, tRecievePlayers, &args);
	if (rc) {
		cout << "Error: unable to create thread, " << rc << endl;
		exit(-1);
	}

	string input = "";
	while(strcmp(input.c_str(), "exit") != 0) {
		getline(cin, input);
	}
	handler.closeClients();
}

//	cout << "games contain:\n";
//	map<string, vector<int> >::iterator it;
//	for (it=games.begin(); it!=games.end(); ++it) {
//		cout << it->first << " => ";
//		for (unsigned i = 0; i < it->second.size(); i++) {
//			cout << " " << it->second[i] << " ";
//	    }
//		cout << endl;
//	}

//	// Define the client socket's structures
//	struct sockaddr_in clientAddress;
//	socklen_t clientAddressLen;
//
//	//accept and handle 2 clients
//
//	while (true) {
//
//		cout << "Waiting for clients' connections..." << endl;
//		// Accept a new client connection
//		int clientSocket1 = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLen);
//		cout << "Client connected" << endl;
//		if (clientSocket1 == -1)
//					throw "Error on accept";
//
//		cout << "Waiting for one more client..."<<endl;
//		// client 2 connected
//		int clientSocket2 = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLen);
//		cout << "Client connected" << endl;
//
//		if (clientSocket2 == -1)
//			throw "Error on accept";
//
//
//		//start a game between both clients
//		int color = 1;
//		int n = write(clientSocket1, &color, sizeof(color));
//		if (n == -1) {
//			cout << "Error writing color to socket" << endl;
//			return;
//		}
//
//		color = 2;
//		n = write(clientSocket2, &color, sizeof(color));
//		if (n == -1) {
//			cout << "Error writing color to socket" << endl;
//			return;
//		}
//
//		//run a game
//		handleTwoClients(clientSocket1, clientSocket2);
//
//		// Close communication with the clients
//		close(clientSocket1);
//		close(clientSocket2);
//	}


// Handle requests from 2 specific clients --> run a game
//void Server::handleTwoClients(int clientSocket1, int clientSocket2) {
//	while (true) {
//	  //handle first client
//	  bool isClient1Connected = handleOneClient(clientSocket1, clientSocket2);
//		if (!isClient1Connected) {
//			return;
//		}
//		//handle second client
//		bool isClient2Connected = handleOneClient(clientSocket2, clientSocket1);
//		if (!isClient2Connected) {
//			return;
//		}
//	}
//}

//string readStringFromSocket(int length, int socket) {
//	string str;
//	int r;
//	char letter;
//	while (length >0) {
//		r = read(socket, &letter, sizeof(letter));
//		if (r == -1) {
//			throw "readStringFromSocket: Error reading message";
//		}
//		str.append(1,letter);
//		length -= 1;
//	}
//	return str;
//}

//Status PlayDecoder(string message, int* row, int* col) {
//	string buf; // Have a buffer string
//	stringstream msg(message); // Insert the string into a stream
//	vector<string> tokens; // Create vector to hold our words
//
//	while (msg >> buf)
//		   tokens.push_back(buf);
//
//	int num1 = atoi(tokens[1].c_str());
//	int num2 = atoi(tokens[2].c_str());
//
//	if(num1 == -1 && num2 == -1) {
//		return NO_MOVES;
//	} else if(num1 == -2 && num2 == -2) {
//		return END;
//	} else {
//		*row = num1;
//		*col = num2;
//		return HAS_MOVE;
//	}
//}

//
//bool Server::handleOneClient(int clientSocket, int waitingClient) {
//	int row, col, size;
//	int n;
//
//	// Read length
//	n = read(clientSocket, &size, sizeof(size));
//	if (n == -1) {
//		cout << "Error reading size" << endl;
//		return false;
//	}
//	if (n == 0) {
//		cout << "Client disconnected" << endl;
//		return false;
//	}
//
//	// Read message
//	string move = readStringFromSocket(size, clientSocket);
//	// Decipher status
//	Status status = PlayDecoder(move, &row, &col);
//	//return false if other client disconnected
//	if (is_client_closed(waitingClient)) {
//		cout << "Client disconnected" << endl;
//	    return false;
//	}
//	// writing message size
//	n = write(waitingClient, &size, sizeof(size));
//	if (n == -1) {
//		cout << "Error writing size to socket" << endl;
//		return false;
//	}
//
//	//return false if other client disconnected
//	if (is_client_closed(waitingClient)) {
//	    cout << "Client disconnected" << endl;
//	    return false;
//	}
//
//	//writing message
//	n = write(waitingClient, move.c_str(), strlen(move.c_str()));
//	if (n == -1) {
//		cout << "Error writing move to socket" << endl;
//		return false;
//	}
//
//	if (status == NO_MOVES) {
//		cout << "Player has no moves" << endl;
//		return true; //current player is still in the game.
//	} else if (status == HAS_MOVE) {
//		cout << "Player's move is: " << row << ", " << col<< endl;
//	} else if (status == END) {
//		cout << "Finish game- closing clients' sockets" << endl;
//		return false;
//	}
//
//	return true;
//}


//bool Server::is_client_closed(int cs)
//{
//	pollfd pfd;
//	pfd.fd = cs;
//	pfd.events = POLLIN | POLLHUP | POLLRDNORM;
//	pfd.revents = 0;
//
//	// call poll with a timeout of 100 ms
//	if(poll(&pfd, 1, 100) > 0) {
//		// if result > 0, this means that there is either data available on the
//		// socket, or the socket has been closed
//		char buffer[32];
//		if(recv(cs, buffer, sizeof(buffer), MSG_PEEK | MSG_DONTWAIT) == 0) {
//			// if recv returns zero, that means the connection has been closed:
//			return true;
//		}
//	}
//	return false;
//}

void Server::stop() {
 close(serverSocket_);
}
