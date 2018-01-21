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

using namespace std;

extern pthread_mutex_t threads_mutex;





class ClientHandler {
public:
  /*
   * constructor.
   */
	ClientHandler(ThreadPool* pool);
	/*
	 *creates thread to treat client.
	 *input: client's socket
	 */
	void handle(int* clientSocket);
	/*
	 * destructor
	 */
	~ClientHandler() {
		cout<<"delete handler"<<endl;
	}
private:
	ThreadPool* pool_;

};

/*
 * accepts new clients and sends them to client handler.
 * this function will be called in a new thread.
 */
//void* tRecievePlayers(void* args);





#endif /* INCLUDE_CLIENTHANDLER_H_ */
