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


/* ********NEED TO FIX************
 * Communication Protocol description:
 * the server works with 2 clients, and from each client it gets 2 messages maximum:
 * 1. the client status (integer)
 * 2. depends on the status - the client's move (2 integers)
 *
 * the logic goes this way: (according to Status enum)
 * - read the first input and send it to the other client.
 * - if the first input - the status - is: 0 - stop reading.
 * 	 									   1 - read move
 * 	 									   2 - close client's sockets.
 * - in case of reading a move: server will read 2 integers, and will send them (and a
 *   separating comma) to the other client.
 *
 * all clients know and obey the protocol.
 */
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
	int port_;
	int serverSocket_;
	pthread_t thread_;
	ThreadPool* thread_pool_;
};

#endif /* SERVER_H_ */
