/*
 * threads.h
 *
 *  Created on: Dec 24, 2017
 *      Author: djoffe
 */

#ifndef INCLUDE_THREADS_H_
#define INCLUDE_THREADS_H_

#include <CommandManager.h>

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



enum Status {NO_MOVES, HAS_MOVE, END};
using namespace std;

//pthread_mutex_t map_mutex;

struct ThreadClientArgs {
  int clientSocket1;
  int clientSocket2;
};

struct ThreadServerArgs {
  int serverSocket;
  map<string, vector<int> >* games;
};

void* threadRunGame(void *clientArgs);
void* threadRecievePlayers(void* server_socket);
//bool handleOneClient(int clientSocket, int waitingClient); // OLD DECLARATION
bool handleOneClient(int clientSocket, map<string, vector<int> >& games);

bool readCommand(int socket, string* comName, vector<string>* args);
int findOtherPlayer(map<string, vector<int> >& games, int clientSocket);



bool is_client_closed(int cs);

string readStringFromSocket(int length, int socket);
Status PlayDecoder(string message, int* row, int* col);


#endif /* INCLUDE_THREADS_H_ */
