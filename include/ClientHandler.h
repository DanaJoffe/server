/*
 * ClientHandler.h
 *
 * Author1: name & ID: Dana Joffe 312129240
 * Author2: name & ID: Chaviva Moshavi 322082892
 */

#ifndef INCLUDE_CLIENTHANDLER_H_
#define INCLUDE_CLIENTHANDLER_H_


#include <map>
#include <pthread.h>
#include <vector>
#include <string.h>
#include <iostream>

#include "GameManager.h"
#include "threads.h"
#include "ThreadPool.h"

using namespace std;

class ClientHandler {
public:
  /*
   * constructor.
   */
	ClientHandler(ThreadPool* pool);
	/*
	 *creates task to treat client.
	 *input: client's socket
	 */
	void handle(int* clientSocket);
	/*
	 * destructor
	 */
	~ClientHandler() {}
private:
	ThreadPool* thread_pool_;

};


#endif /* INCLUDE_CLIENTHANDLER_H_ */
