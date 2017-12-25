/*
 * threads.h
 *
 *  Created on: Dec 24, 2017
 *      Author: djoffe
 */

#ifndef INCLUDE_THREADS_H_
#define INCLUDE_THREADS_H_

//#include "CommandManager.h"
#include "ClientHandler.h"



#include <pthread.h>
#include <cstdlib>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <cstdlib>
#include <string.h>
#include <stdio.h>
#include <sstream>
//#include <vector>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>
//#include <map>

using namespace std;


enum Status {NO_MOVES, HAS_MOVE, END};


extern pthread_mutex_t map_mutex;

struct ThreadClientArgs {
  int clientSocketFirst;
  int clientSocketSecond;
  string gameName;
  map<string, vector<int> >* games;
};

class ClientHandler;

struct ThreadServerArgs {
  int serverSocket;
  ClientHandler* handler;
};

struct ClientMapArgs {
  int clientSocket;
  map<string, vector<int> >* games;
};

void* tRecievePlayers(void* server_socket);
string readStringFromSocket(int length, int socket);

//bool handleOneClient(int clientSocket, int waitingClient); // OLD DECLARATION

//bool readCommand(int socket, string* comName, vector<string>* args);


void* tTreatClient(void *serverArgs);
bool readCommand(int socket, string* comName, vector<string>* args);





#endif /* INCLUDE_THREADS_H_ */
