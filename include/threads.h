/*
 * threads.h
 *
 *  Created on: Dec 24, 2017
 *      Author: djoffe
 */


#ifndef INCLUDE_THREADS_H_
#define INCLUDE_THREADS_H_



#include "CommandManager.h"


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
#include <vector>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <poll.h>
#include <map>



using namespace std;


enum Status {NO_MOVES, HAS_MOVE, END};

//extern

//static
extern pthread_mutex_t map_mutex;

struct ThreadClientArgs {
  int clientSocketFirst;
  int clientSocketSecond;
  string gameName;
  map<string, vector<int> >* games;
};


struct ClientMapArgs {
  int clientSocket;
};

/*
 * read string of specified length from socket.
 * input: length - length of string, socket - socket to read from.
 * output: string read from socket
 */
string readStringFromSocket(int length, int socket);
/*
 * read command from client. will read name of command and arguments of command.
 * ouput: true if succeeded, else false
 */
bool readCommand(int socket, string* comName, vector<string>* args);
/*
 * treat specific client. read command from client and execute it.
 * this function will be called in a new thread.
 * input: client socket
 */
void* tTreatClient(void *clientMapArgs);



#endif /* INCLUDE_THREADS_H_ */
