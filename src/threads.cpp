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

	while (true) {
		//handle first client
		bool isClient1Connected = handleOneClient(clientSocket1, clientSocket2); //return msg
		if (!isClient1Connected) {
			return NULL;
		}
		//handle second client
		bool isClient2Connected = handleOneClient(clientSocket2, clientSocket1);
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
//void* threadRecievePlayers(void* server_socket) {

	struct ThreadServerArgs *arguments = (struct ThreadServerArgs *)serverArgs;
	int serverSocket = arguments->serverSocket;
	//arguments->games

	// Define the client socket's structures
	struct sockaddr_in clientAddress;
	socklen_t clientAddressLen;

	//accept and handle 2 clients
	while (true) {
		cout << "Waiting for client connections..." << endl;
		// Accept a new client connection
		int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLen);
		cout << "Client connected" << endl;
		if (clientSocket == -1)
			throw "Error on accept";

		/////
		/* read length
		 *	read string - from socket
		 *	divide words by spaces
		 *  first- command
		 *
		 * lock
		 * map -- change map
		 * free lock
		 *
		 *	command manager- execute command
		 *
		 *
		 */

		/////


		cout << "Waiting for one more client..."<<endl;
		// client 2 connected
		int clientSocket2 = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLen);
		cout << "Client connected" << endl;

		if (clientSocket2 == -1)
			throw "Error on accept";



		/// join ////

		//start a game between both clients
		int color = 1;
		int n = write(clientSocket, &color, sizeof(color));
		if (n == -1) {
			cout << "Error writing color to socket" << endl;
			return NULL;
		}

		color = 2;
		n = write(clientSocket2, &color, sizeof(color));
		if (n == -1) {
			cout << "Error writing color to socket" << endl;
			return NULL;
		}

		struct ThreadClientArgs args;
		args.clientSocket1 = clientSocket;
		args.clientSocket2 = clientSocket2;


		/*
		 * cout << "clientSocket1: " <<clientSocket<< endl;
		cout << "clientSocket2: " <<clientSocket2<< endl;

		vector<int> vec;
		vec.push_back(clientSocket);
		vec.push_back(clientSocket2);
		arguments->games->insert(pair<string, vector<int> >("GAME",vec));
		 */


		pthread_t thread;
		int rc = pthread_create(&thread, NULL, threadRunGame, &args);
		if (rc) {
			cout << "Error: unable to create thread, " << rc << endl;
			exit(-1);
		}
	}
}

bool handleOneClient(int clientSocket, int waitingClient) {
	int row, col, size;
	int n;

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
	// Decipher status
	Status status = PlayDecoder(move, &row, &col);

	//return false if other client disconnected
	if (is_client_closed(waitingClient)) {
		cout << "Client disconnected" << endl;
	    return false;
	}

	// writing row
	n = write(waitingClient, &row, sizeof(row));
	if (n == -1) {
		cout << "Error writing size to socket" << endl;
		return false;
	}
	// writing col
	n = write(waitingClient, &col, sizeof(col));
	if (n == -1) {
		cout << "Error writing size to socket" << endl;
		return false;
	}

	if (status == NO_MOVES) {
		cout << "Player has no moves" << endl;
		return true; //current player is still in the game.
	} else if (status == HAS_MOVE) {
		cout << "Player's move is: " << row << ", " << col<< endl;
	} else if (status == END) {
		cout << "Finish game- closing clients' sockets" << endl;
		return false;
	}
	return true;


//	int size;
//	int row, col;
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
//	string move = readStringFromSocket1(size, clientSocket);
//
//
//	Status status = PlayDecoder1(move, &row, &col);
//
//	cout << "writing size: "<< size<< endl;
//
//	n = write(waitingClient, &size, sizeof(size));
//	if (n == -1) {
//		cout << "Error writing size to socket" << endl;
//		return false;
//	}
//
//	//writing message
//	cout << "writing message: "<< move.c_str()<< endl;
//
//	n = write(waitingClient, move.c_str(), strlen(move.c_str()));
//	if (n == -1) {
//		cout << "Error writing move to socket" << endl;
//		return false;
//	}
//
//
//	if (status == NO_MOVES) {
//		cout << "Player has no moves" << endl;
//		return true; //current player is still in the game.
//
//	} else if (status == HAS_MOVE) {
//		cout << "Player's move is: " << row << ", " << col<< endl;
//
//	} else if (status == END) {
//		cout << "Finish game- closing clients' sockets" << endl;
//		return false;
//	}
//	return true;
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

