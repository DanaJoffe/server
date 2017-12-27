/*
 * Join.cpp
 *
 *  Created on: Dec 22, 2017
 *      Author: chaviva
 */

#include "JoinGame.h"

void JoinGame::execute(vector<string>& args, int client_socket) {

  GameManager* gameManager = GameManager::getInstance();
  map<string, vector<int> >& games = *gameManager->getGames();
  string game_name;
  vector<int> game_clients;
  map<string, vector<int> >::iterator it;
  int result;

  //find game
  pthread_mutex_lock(&map_mutex);
  if (gameManager->isGameExist(args[0]) && gameManager->playersAmount(args[0]) == 1) {
	  game_name = args[0];
	  gameManager->addPlayerToGame(game_name, client_socket);
	  game_clients = gameManager->getPlayers(game_name);
  }
  pthread_mutex_unlock(&map_mutex);

  //inform client if player can join requested game
  if (game_clients.empty()) {
	  cout << "inform client game doesn't exist" <<endl;

    result = -1;
  } else {
	  cout << "inform client game exists" <<endl;
    result = 1;
  }
  int n = write(client_socket, &result, sizeof(result));
  if (n == -1) {
    cout << "Error writing result to socket" << endl;
  }

  //close socket if didn't succeed to join game
  if (result == -1) {
    close(client_socket);
  //else, start game
  } else {
    //send players their colors
    int color = 1;
    n = write(game_clients[0], &color, sizeof(color));
    if (n == -1) {
      cout << "Error writing color to socket" << endl;
    }
    color = 2;
    n = write(game_clients[1], &color, sizeof(color));
    if (n == -1) {
      cout << "Error writing color to socket" << endl;
    }

    //run game
	GameManager* game_manager = GameManager::getInstance();
	game_manager->RunGame(game_name);
  }
}

