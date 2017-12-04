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
using namespace std;
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
	while (true) {
		cout << "Waiting for client connections..." << endl;
		// Accept a new client connection
		int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLen);
		cout << "Client connected" << endl;
		if (clientSocket == -1)
					throw "Error on accept";

		/////CHANGE
		cout << "Waiting for another player to join..."<<endl;

		//inform client 1
//		string massage = "Server: Waiting for another player to join...";
//		int n = write(clientSocket, massage.c_str(), strlen(massage.c_str())); // n needs to be checked






		// client 2 connected
		int clientSocket2 = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLen);
		cout << "Client connected" << endl;

		if (clientSocket2 == -1)
			throw "Error on accept";


		int color = 1;
		int n1 = write(clientSocket, &color, sizeof(color)); // n needs to be checked
		color = 2;
		n1 = write(clientSocket2, &color, sizeof(color)); // n needs to be checked


//		//inform client 2 that game starts
//		string massag = "Server: Welcome! lets start the game";
//		n = write(clientSocket2, massag.c_str(), strlen(massag.c_str())); //n needs to be checked



//		//inform client 1 that game starts
//		massage = "Server: player 2 has connected! game starts";
//		n = write(clientSocket, &massage, sizeof(massage)); // n needs to be checked


		handleTwoClients(clientSocket, clientSocket2);//CHANGE



		// Close communication with the client
		close(clientSocket);
		close(clientSocket2);
	}
}


// Handle requests from a specific client
void Server::handleTwoClients(int clientSocket, int clientSocket2) {
	while (true) {
//		char* op;
		int result = 0;

//		string result = "waiting for client 1 exercise";
//		int n = write(clientSocket2, &result, sizeof(result)); // n needs to be checked
		handleOneClient(clientSocket); //return msg

//		cout <<"op: "<<op<<endl;


		//if msg = end - close server
		//else - contine & send msg to other client

		//write result to other client
		cout << "turn passes to client 2"<<endl;
		int n = write(clientSocket2, &result, sizeof(result));

		if (n == -1) {
			cout << "Error writing to socket" << endl;
			return;
		}


//		result = "waiting for client 2 exercise";
//		n = write(clientSocket, &result, sizeof(result)); // n needs to be checked
		handleOneClient(clientSocket2);

//		cout <<"op: "<<op<<endl;

		//if msg = end - close server
		//else - contine & send msg to other client
		result = 0;
			//write result to other client
		cout << "turn passes to player 1"<<endl;
		n = write(clientSocket, &result, sizeof(result));
		//		int n = write(clientSocket2, op, sizeof(char));


		if (n == -1) {
			cout << "Error writing to socket" << endl;
			return;
		}



//		result = 0;
//			//write result to other client
//			n = write(clientSocket, &result, sizeof(result));
//			cout << "send result to client 2"<<endl;
//
//			if (n == -1) {
//				cout << "Error writing to socket" << endl;
//				return;
//			}

	}
}


void Server::handleOneClient(int clientSocket) {
	int arg1, arg2;
	char op;
	// Read new exercise arguments
	int n = read(clientSocket, &arg1, sizeof(arg1));
	if (n == -1) {
		cout << "Error reading arg1" << endl;
		return;
	}
	if (n == 0) {
		cout << "Client disconnected" << endl;
		return;
	}
	n = read(clientSocket, &op, sizeof(op));
	if (n == -1) {
		cout << "Error reading operator" << endl;
		return;
	}
	n = read(clientSocket, &arg2, sizeof(arg2));
	if (n == -1) {
		cout << "Error reading arg2" << endl;
		return;
	}
	cout << "Got exercise: " << arg1 << op << arg2 <<endl;
	int result = calc(arg1, op, arg2);
	// Write the result back to the client
	cout << "send result to client" <<endl;
	n = write(clientSocket, &result, sizeof(result));


	return;


	if (n == -1) {
		cout << "Error writing to socket" << endl;
		return;
	}


//			//CHANGE
//			//write result to other client
//			n = write(clientSocket2, &result, sizeof(result));
//			cout << "send result to client 2"<<endl;
//
//			if (n == -1) {
//				cout << "Error writing to socket" << endl;
//				return;
//			}
}

//char* Server::handleOneClient(int clientSocket) {
//	int arg1, arg2;
//	char* op;
//	// Read new exercise arguments
//	int n = read(clientSocket, &arg1, sizeof(arg1));
//	if (n == -1) {
//		cout << "Error reading arg1" << endl;
//		return NULL;
//	}
//	if (n == 0) {
//		cout << "Client disconnected" << endl;
//		return NULL;
//	}
//	n = read(clientSocket, op, sizeof(char));
//	if (n == -1) {
//		cout << "Error reading operator" << endl;
//		return NULL;
//	}
//	n = read(clientSocket, &arg2, sizeof(arg2));
//	if (n == -1) {
//		cout << "Error reading arg2" << endl;
//		return NULL;
//	}
//	cout << "Got exercise: " << arg1 << *op << arg2 <<endl;
//	int result = calc(arg1, *op, arg2);
//	// Write the result back to the client
//	cout << "send result to client" <<endl;
//	n = write(clientSocket, &result, sizeof(result));
//
//
//	return op;
//
//
//	if (n == -1) {
//		cout << "Error writing to socket" << endl;
//		return NULL;
//	}
//
//
////			//CHANGE
////			//write result to other client
////			n = write(clientSocket2, &result, sizeof(result));
////			cout << "send result to client 2"<<endl;
////
////			if (n == -1) {
////				cout << "Error writing to socket" << endl;
////				return;
////			}
//}





int Server::calc(int arg1, const char op, int arg2) const {
	switch (op) {
	case '+':
		return arg1 + arg2;
	case '-':
		return arg1 - arg2;
	case '*':
		return arg1 * arg2;
	case '/':
		return arg1 / arg2;
	default:
		cout << "Invalid operator" << endl;
		return 0;
	}
}
void Server::stop() {
 close(serverSocket);
}
