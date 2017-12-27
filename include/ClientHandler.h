/*
 * ClientHandler.h
 *
 *  Created on: Dec 25, 2017
 *      Author: djoffe
 */

#ifndef INCLUDE_CLIENTHANDLER_H_
#define INCLUDE_CLIENTHANDLER_H_


#include <map>
#include <vector>
#include <string.h>
#include <iostream>

//#include "threads.h"
#include "CommandManager.h"


using namespace std;


class ClientHandler {
public:
  /*
   * constructor.
   */
	ClientHandler();
	void handle(int clientSocket);
	void closeClients();

	~ClientHandler() {}

};

struct ServerSocketArg {
//  ClientHandler* handler;
  int serverSocket;
};

struct ThreadServerArgs {
  int serverSocket;
//  ClientHandler* handler;
};


void* tRecievePlayers1(void* server_socket);




#endif /* INCLUDE_CLIENTHANDLER_H_ */
