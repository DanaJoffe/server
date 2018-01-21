/*
 * Server.cpp
 *
 * Author1: name & ID: Dana Joffe 312129240
 * Author2: name & ID: Chaviva Moshavi 322082892
 */

#include "Server.h"

#define MAX_CONNECTED_CLIENTS 10


Server::Server(int port): port_(port), serverSocket_(0), thread_(0) {
  cout << "Server" << endl;
  this->thread_pool_ = new ThreadPool(MAX_CONNECTED_CLIENTS);
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

	struct receiveClientsArgs args;
	args.serverSocket = serverSocket_;
	args.thread_pool = thread_pool_;

  cout << "Enter exit to stop server." << endl << endl;

  //send MAX_CONNECTED_CLIENTS
	int rc = pthread_create(&thread_, NULL, tRecievePlayers, &args);
	if (rc) {
		cout << "Error: unable to create thread, " << rc << endl;
		exit(-1);
	}
    //wait for "exit" command from console
    string input = "";
    while(strcmp(input.c_str(), "exit") != 0) {
  	  input = "";
 	  getline(cin, input);
    }
}

void Server::stop() {
  //close all running games
  GameManager* gm = GameManager::getInstance();
  gm->closeGames();
  GameManager::destroyInstance();
  //close running threads in threadpool
  try {
	  this->thread_pool_->terminate();
  } catch (const char* msg) {
	  cout << msg << endl;
  }
  delete thread_pool_;
  //close thread and server socket
    pthread_cancel(thread_);
  close(serverSocket_);
  cout << "Server stopped" << endl;
}
