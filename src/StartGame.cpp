/*
 * Start.cpp
 *
 * Author1: name & ID: Dana Joffe 312129240
 * Author2: name & ID: Chaviva Moshavi 322082892
 */

#include "StartGame.h"

void StartGame::execute(vector<string>& args, int client_socket) {
  GameManager* gameManager = GameManager::getInstance();
  int result;
  result = gameManager->addGameWithPlayer(args[0], client_socket);

  int ret;
  if (result < 0) {
    ret = -1;
  } else {
    ret = 1;
  }

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
