/*
 * structs.h
 *
 * Author1: name & ID: Dana Joffe 312129240
 * Author2: name & ID: Chaviva Moshavi 322082892
 */

#ifndef INCLUDE_STRUCTS_H_
#define INCLUDE_STRUCTS_H_

#include "ThreadPool.h"
#include "ClientHandler.h"

//struct of parameters to pass to tReceivePlayers
struct receiveClientsArgs {
  int serverSocket;
  ThreadPool * thread_pool;
};

#endif /* INCLUDE_STRUCTS_H_ */
