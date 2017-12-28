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
	/*
	 *creates thread to treat client.
	 *input: client's socket
	 */
	void handle(int clientSocket);
	/*
	 * destructor
	 */
	~ClientHandler() {}
};

/*
 * accepts new clients and sends them to client handler.
 * this function will be called in a new thread.
 */
void* tRecievePlayers1(void* server_socket);



#endif /* INCLUDE_CLIENTHANDLER_H_ */
