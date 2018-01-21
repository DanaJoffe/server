/*
 * structs.h
 *
 *  Created on: Jan 20, 2018
 *      Author: djoffe
 */

#ifndef INCLUDE_STRUCTS_H_
#define INCLUDE_STRUCTS_H_

#include "ThreadPool.h"
#include "ClientHandler.h"

//struct of parameters to pass to tReceivePlayers
struct receiveClientsArgs {
  int serverSocket;
//  ClientHandler handler;
  ThreadPool * pool;
};





#endif /* INCLUDE_STRUCTS_H_ */
