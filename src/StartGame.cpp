/*
 * Start.cpp
 *
 *  Created on: Dec 22, 2017
 *      Author: chaviva
 */

#include "StartGame.h"

void StartGame::execute(vector<string>& args, int client_socket) {
  GameManager* gameManager = GameManager::getInstance();
  int result;
  pthread_mutex_lock(&map_mutex);
  result = gameManager->addGame(args[0], client_socket);
  pthread_mutex_unlock(&map_mutex);

  int ret;
  if (result < 0) {
    ret = -1;
  } else {
    ret = 1;
  }
  cout << "will write result of insert" << endl;

  //write to client if succeeded to start the game
  int n = write(client_socket, &ret, sizeof(ret));
  if (n == -1) {
    cout << "Error writing return value to socket" << endl;
  }
  //if starting game didn't succeed, close client socket
  if (ret == -1) {
    close(client_socket);
  }
}
