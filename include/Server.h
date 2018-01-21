/*
 * Server.h
 *
 * Author1: name & ID: Dana Joffe 312129240
 * Author2: name & ID: Chaviva Moshavi 322082892
 */

#ifndef SERVER_H_
#define SERVER_H_

#include "threads.h"
#include "ClientHandler.h"
#include "ThreadPool.h"
#include "structs.h"


class Server {
public:
  /*
   * constructs server with port.
   */
	Server(int port);
	/*
	 * starts server.
	 */
	void start();
	/*
	 * stops server.
	 */
	void stop();

private:
	ThreadPool* thread_pool_;
	int port_;
	int serverSocket_;
	pthread_t thread_;
};

#endif /* SERVER_H_ */
