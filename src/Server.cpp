/*
 * Server.cpp
 *
 * Author1: name & ID: Dana Joffe 312129240
 * Author2: name & ID: Chaviva Moshavi 322082892
 */

#include "Server.h"

#define MAX_CONNECTED_CLIENTS 10


Server::Server(int port): port(port), serverSocket(0) {
  cout << "Server" << endl;
}
void Server::start() {
	// Create a socket point
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1) {
		throw "Error opening socket";
	}
	// Assign a local address to the socket
	struct sockaddr_in serverAddress;
	bzero((void *)&serverAddress,sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(port);
	if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
		throw "Error on binding";
	}
	// Start listening to incoming connections
	listen(serverSocket, MAX_CONNECTED_CLIENTS);
	// Define the client socket's structures
	struct sockaddr_in clientAddress;
	socklen_t clientAddressLen;

	//accept and handle 2 clients
	while (true) {

		cout << "Waiting for clients' connections..." << endl;
		// Accept a new client connection
		int clientSocket1 = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLen);
		cout << "Client connected" << endl;
		if (clientSocket1 == -1)
					throw "Error on accept";

		cout << "Waiting for one more client..."<<endl;
		// client 2 connected
		int clientSocket2 = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLen);
		cout << "Client connected" << endl;

		if (clientSocket2 == -1)
			throw "Error on accept";

		int color = 1;
		int n = write(clientSocket1, &color, sizeof(color));
		if (n == -1) {
			cout << "Error writing color to socket" << endl;
			return;
		}

		color = 2;
		n = write(clientSocket2, &color, sizeof(color));
		if (n == -1) {
			cout << "Error writing color to socket" << endl;
			return;
		}

		handleTwoClients(clientSocket1, clientSocket2);

		// Close communication with the clients
		close(clientSocket1);
		close(clientSocket2);
	}
}


// Handle requests from 2 specific clients
void Server::handleTwoClients(int clientSocket1, int clientSocket2) {
	while (true) {
	  //handle first client
	  bool isClient1Connected = handleOneClient(clientSocket1, clientSocket2);
		if (!isClient1Connected) {
			return;
		}
		//handle second client
		bool isClient2Connected = handleOneClient(clientSocket2, clientSocket1);
		if (!isClient2Connected) {
			return;
		}
	}
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

	if(num1 == -1 && num2 == -1) {
		return NO_MOVES;
	} else if(num1 == -2 && num2 == -2) {
		return END;
	} else {
		*row = num1;
		*col = num2;
		return HAS_MOVE;
	}
}

bool Server::handleOneClient(int clientSocket, int waitingClient) {
//	int status;
	int row, col, size;
//	char sep;
	int n;

//	// Read status argument
//	n = read(clientSocket, &status, sizeof(status));
//	if (n == -1) {
//		cout << "Error reading status" << endl;
//		return false;
//	} else if (n == 0) {
//		cout << "Client disconnected" << endl;
//		return false;
//	}


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
//	if (move == NULL) {
//		return false;
//	}

	// Decipher status
	Status status = PlayDecoder(move, &row, &col);

	//return false if other client disconnected
	if (is_client_closed(waitingClient)) {
		cout << "Client disconnected" << endl;
	    return false;
	}

	// writing message size
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
}





//
//
//	//get player's move if status is has move
//	if (status == HAS_MOVE) {
////      // Read player's move arguments
////      n = read(clientSocket, &row, sizeof(row));
////      if (n == -1) {
////        cout << "Error reading row" << endl;
////        return false;
////      } else if (n == 0) {
////        cout << "Client disconnected" << endl;
////        return false;
////      }
////      n = read(clientSocket, &sep, sizeof(sep));
////      if (n == -1) {
////        cout << "Error reading separator" << endl;
////        return false;
////      } else  if (n == 0) {
////        cout << "Client disconnected" << endl;
////        return false;
////      }
////      n = read(clientSocket, &col, sizeof(col));
////      if (n == -1) {
////        cout << "Error reading col" << endl;
////        return false;
////      } else if (n == 0) {
////        cout << "Client disconnected" << endl;
////        return false;
////      }
//	}
//
//  //return false if other client disconnected
//  if (is_client_closed(waitingClient)) {
//    cout << "Client disconnected" << endl;
//    return false;
//  }
//
//	//inform other player on current player's status.
////	n = write(waitingClient, &status, sizeof(status));
////	if (n == -1) {
////		cout << "Error writing status to socket" << endl;
////		return false;
////	}
//
//	if (status == NO_MOVES) {
//		return true; //current player is still in the game.
//
//	} else if (status == HAS_MOVE) {
//
//	  //return false if other client disconnected
//		if (is_client_closed(waitingClient)) {
//			cout << "Client disconnected" << endl;
//			return false;
//		}
//
//	  //inform other player on current player's move.
////		n = write(waitingClient, &row, sizeof(row));
////		if (n == -1) {
////			cout << "Error writing row to socket" << endl;
////			return false;
////		}
////		n = write(waitingClient, &sep, sizeof(sep));
////		if (n == -1) {
////			cout << "Error writing separator to socket" << endl;
////			return false;
////		}
////		n = write(waitingClient, &col, sizeof(col));
////		if (n == -1) {
////			cout << "Error writing col to socket" << endl;
////			return false;
////		}
//	} else if (status == END) {
//		cout << "Finish game- closing clients' sockets" << endl;
//		return false;
//	}
//
//	return true;
//}

bool Server::is_client_closed(int cs)
{
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

void Server::stop() {
 close(serverSocket);
}
