/*
 * Join.cpp
 *
 *  Created on: Dec 22, 2017
 *      Author: chaviva
 */

#include "JoinGame.h"

void JoinGame::execute(vector<string>& args, int client_socket) {
  GameManager* gameManager = GameManager::getInstance();
  string game_name = args[0];
  int result;
  vector<int> game_clients;
//  pthread_mutex_lock(&map_mutex);
  if (!gameManager->doesGameExist(game_name) || gameManager->playersAmount(game_name) != 1) {
//	  pthread_mutex_unlock(&map_mutex);
	  cout << "inform client game doesn't exist or can't join game" <<endl;
	  result = -1;
  } else {
	  cout << "inform client game exists" <<endl;
	  result = 1;
	  //find game
	  pthread_mutex_lock(&map_mutex);
	  cout << "step 1" <<endl;

//	  if (gameManager->doesGameExist(game_name) && gameManager->playersAmount(game_name) == 1) {
		  cout << "step 2" <<endl;
		  gameManager->addPlayerToGame(game_name, client_socket);
		  cout << "step 3" <<endl;
		  game_clients = gameManager->getPlayers(game_name);
//	  }
	  pthread_mutex_unlock(&map_mutex);
	  cout << "step 4" <<endl;
  }
  int n = write(client_socket, &result, sizeof(result));
  if (n == -1) {
     cout << "Error writing result to socket" << endl;
  }
  cout << "step 5" <<endl;

  //close socket if didn't succeed to join game
  if (result == -1) {

	     cout << "result == -1" << endl;

	    close(client_socket);
	    //else, start game
  } else {
		//send players their colors
		int color = 1;
	     cout << "writes color 1" << endl;


		n = write(game_clients[0], &color, sizeof(color));
		if (n == -1) {
		  cout << "Error writing color to socket" << endl;
		}


	     cout << "writes color 2" << endl;


		color = 2;
		n = write(game_clients[1], &color, sizeof(color));
		if (n == -1) {
		  cout << "Error writing color to socket" << endl;
		}

		cout << "run game" << endl;
		//run game
		gameManager->RunGame(game_name);
  }
}

