/*
 * Server.cpp
 *
 *  Created on: Dec 3, 2017
 *      Author: djoffe
 */


#include "Server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <stdio.h>

#include <poll.h>          // For poll()

using namespace std;
#define MAX_CONNECTED_CLIENTS 2


Server::Server(int port): serverSocket(0), port(port) {
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
	while (true) {
		cout << "Waiting for clients connections..." << endl;
		// Accept a new client connection
		int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLen);
		cout << "Client connected" << endl;
		if (clientSocket == -1)
					throw "Error on accept";


		cout << "Waiting for one more client..."<<endl;
		// client 2 connected
		int clientSocket2 = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLen);
		cout << "Client connected" << endl;

		if (clientSocket2 == -1)
			throw "Error on accept";

		int color = 1;
		int n = write(clientSocket, &color, sizeof(color));
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

		handleTwoClients(clientSocket, clientSocket2);//CHANGE

		// Close communication with the client
		close(clientSocket);
		close(clientSocket2);
	}
}


// Handle requests from a specific client
void Server::handleTwoClients(int clientSocket, int clientSocket2) {
	while (true) {
		bool isClient1Connected = handleOneClient(clientSocket, clientSocket2); //return msg
		if (!isClient1Connected) {
			return;
		}
		cout << "turn passes to client 2"<<endl;
		bool isClient2Connected = handleOneClient(clientSocket2, clientSocket);
		if (!isClient2Connected) {
			return;
		}
		cout << "turn passes to player 1"<<endl;
	}
}

bool Server::handleOneClient(int clientSocket, int waitingClient) {
	int status;
	int row, col;
	char sep;
	int n;
	// Read status arguments
	n = read(clientSocket, &status, sizeof(status));
	if (n == -1) {
		cout << "Error reading status" << endl;
		return false;
	}
	if (n == 0) {
		cout << "Client disconnected" << endl;
		return false;
	}
	cout << "Got status: " << status <<endl;
	//inform other player on current player's status.
	n = write(waitingClient, &status, sizeof(status));
	if (n == -1) {
		cout << "Error writing status to socket" << endl;
		return false;
	}

	if (status == NO_MOVES) {
		cout << "Player has no moves" << endl;
		return true; //current player is still in the game.

	} else if (status == HAS_MOVE) {
		// Read move arguments
		n = read(clientSocket, &row, sizeof(row));
		if (n == -1) {
			cout << "Error reading row" << endl;
			return false;
		}
		n = read(clientSocket, &sep, sizeof(sep));
		if (n == -1) {
			cout << "Error reading separator" << endl;
			return false;
		}
		n = read(clientSocket, &col, sizeof(col));
		if (n == -1) {
			cout << "Error reading col" << endl;
			return false;
		}
		cout << "Player's move is: " << row << ", " << col<< endl;


		//inform other player on current player's move.

		if (is_client_closed(waitingClient)) {
			cout << "Client disconnected" << endl;
			int a = END;
			n = write(clientSocket, &a, sizeof(a));
			return false;
		}

		n = write(waitingClient, &row, sizeof(row));
		if (n == -1) {
			cout << "Error writing row to socket" << endl;
			return false;
		}
		n = write(waitingClient, &sep, sizeof(sep));
		if (n == -1) {
			cout << "Error writing separator to socket" << endl;
			return false;
		}
		n = write(waitingClient, &col, sizeof(col));
		if (n == -1) {
			cout << "Error writing col to socket" << endl;
			return false;
		}
	} else if (status == END) {
		cout << "Finish game- closing clients' sockets" << endl;
		return false;
	}

	return true;
}



bool Server::is_client_closed(int cs)
{
	pollfd pfd;
	pfd.fd = cs;
	pfd.events = POLLIN | POLLHUP | POLLRDNORM;
	pfd.revents = 0;
	while(pfd.revents == 0)
	{
		// call poll with a timeout of 100 ms
		if(poll(&pfd, 1, 100) > 0)
		{
			// if result > 0, this means that there is either data available on the
			// socket, or the socket has been closed
			char buffer[32];
			if(recv(cs, buffer, sizeof(buffer), MSG_PEEK | MSG_DONTWAIT) == 0)
			{
				// if recv returns zero, that means the connection has been closed:
				return true;
			}
		}
	}
	return false;
}



/*
 * string input method:
 *
 *		string massage = "Server: Waiting for another player to join...";
		int n = write(clientSocket, massage.c_str(), strlen(massage.c_str())); // n needs to be checked
 *
 * in client mimush:
 * char message[80];
 *	int dummy = read(client.clientSocket, message, 80);
 *	cout << message<<endl;
 *
 *
 */


void Server::stop() {
 close(serverSocket);
}
