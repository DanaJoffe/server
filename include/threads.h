/*
 * threads.h
 *
 * Author1: name & ID: Dana Joffe 312129240
 * Author2: name & ID: Chaviva Moshavi 322082892
 */


#ifndef INCLUDE_THREADS_H_
#define INCLUDE_THREADS_H_



#include "CommandManager.h"
#include "structs.h"
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
#include <vector>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <poll.h>
#include <map>


using namespace std;


enum Status {NO_MOVES, HAS_MOVE, END};

/*
 * read string of specified length from socket.
 * input: length - length of string, socket - socket to read from.
 * output: string read from socket
 */
string readStringFromSocket(int length, int socket);
/*
 * read command from client. will read name of command and arguments of command.
 * output: true if succeeded, else false
 */
bool readCommand(int socket, string* comName, vector<string>* args);
/*
 * treat specific client. read command from client and execute it.
 * this function will be called in a new thread.
 * input: client socket
 */
void* tTreatClient(void* args);
/*
 * run game with the name gameName
 * intput: name of game
 */
void* tRunGame(void* gameName);

/*
 * accepts new clients and sends them to client handler.
 * this function will be called in a new thread.
 * input:args includes serversocket and threadpool
 */
void* tRecievePlayers(void* args);

#endif /* INCLUDE_THREADS_H_ */
