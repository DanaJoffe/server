/*
 * Join.cpp
 *
 *  Created on: Dec 22, 2017
 *      Author: chaviva
 */

#include "JoinGame.h"

void JoinGame::execute(vector<string>& args, map<string, vector<int> >& games,
    int client_socket) {

  string game_name;
  vector<int> game_clients;
  map<string, vector<int> >::iterator it;
  int result;

  //find game
  pthread_mutex_lock(&map_mutex);
  it = games.find(args[0]);
  if (it != games.end() && it->second.size() == 1) {
	  cout << "game exists!" <<endl;
	game_name = it->first.c_str();
    //add player to game
    it->second.push_back(client_socket);
    game_clients = it->second;
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

/*
 * SOME CODE I SAVED FOR JOIN FUNC
 *
	/// join ////

	//start a game between both clients

	int color = 1;
	int n = write(clientSocket, &color, sizeof(color));
	if (n == -1) {
		cout << "Error writing color to socket" << endl;
		return NULL;
	}

	color = 2;
	n = write(clientSocket2, &color, sizeof(color));
	if (n == -1) {
		cout << "Error writing color to socket" << endl;
		return NULL;
	}

	struct ThreadClientArgs args;
	args.clientSocket1 = clientSocket;
	args.clientSocket2 = clientSocket2;

	pthread_t thread;
	int rc = pthread_create(&thread, NULL, threadRunGame, &args);
	if (rc) {
		cout << "Error: unable to create thread, " << rc << endl;
		exit(-1);
	}
*/
