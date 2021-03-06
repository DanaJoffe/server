/*
 * Join.cpp
 *
 * Author1: name & ID: Dana Joffe 312129240
 * Author2: name & ID: Chaviva Moshavi 322082892
 */

#include "JoinGame.h"

void JoinGame::execute(vector<string>& args, int client_socket) {
  GameManager* gameManager = GameManager::getInstance();
  string* game_name = new string(args[0].c_str());
  int result;
  vector<int> game_clients;

  if (!gameManager->doesGameExist(*game_name) || gameManager->playersAmount(*game_name) != 1) {
	  result = -1;
  } else {
	  result = 1;
	  //add player to game
		bool succeeded = gameManager->addPlayerToGame(*game_name, client_socket);
		if (!succeeded) {
		  result = -1;
		} else {
	    game_clients = gameManager->getPlayers(*game_name);
		}
  }

  //inform player if succeeded in joining game
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

		//run game in new thread
		pthread_t thread;
		int rc = pthread_create(&thread, NULL, tRunGame, game_name);
		if (rc) {
		   cout << "Error: unable to create thread, " << rc << endl;
		   exit(-1);
		}
  }
}
