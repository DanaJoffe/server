/*
 * ClientHandler.cpp
 *
 * Author1: name & ID: Dana Joffe 312129240
 * Author2: name & ID: Chaviva Moshavi 322082892
 */


#include <ClientHandler.h>

ClientHandler::ClientHandler(ThreadPool* pool): thread_pool_(pool) {}

void ClientHandler::handle(int* clientSocket) {
	this->thread_pool_->addTask(new Task(tTreatClient, clientSocket));
	return;
}






