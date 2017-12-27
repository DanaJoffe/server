/*
 * ListGames.cpp
 *
 *  Created on: Dec 22, 2017
 *      Author: chaviva
 */

#include "ListGames.h"

void ListGames::execute(vector<string>& args, int client_socket) {
	cout <<"ListGames::execute" <<endl;


  GameManager* gameManager = GameManager::getInstance();


  pthread_mutex_lock(&map_mutex);
  map<string, vector<int> >& game_list(*gameManager->getGames());//change
  pthread_mutex_unlock(&map_mutex);

  //write string of available games to send to client
  stringstream available_games;
  map<string, vector<int> >::iterator it;
  for (it = game_list.begin(); it != game_list.end(); it++) {
    if (it->second.size() == 1) {
      available_games << it->first << " "<< endl;
    }
  }
  //get size of string
  available_games.seekg(0, ios::end);
  int size = available_games.tellg();
  available_games.seekg(0, ios::beg);

  if (size == 0) {
    available_games << "There are no current available games to join." << endl;
    available_games.seekg(0, ios::end);
    size = available_games.tellg();
    available_games.seekg(0, ios::beg);
  }

  //send size of string
  int n = write(client_socket, &size, sizeof(size));
  if (n == -1) {
    cout << "Error writing string size to socket" << endl;
  }
  //send string of available games
  string msg = available_games.str();

  cout <<"ListGames::execute: sending to client: " <<msg <<endl;


  n = write(client_socket, msg.c_str(), size);
  if (n == -1) {
    cout << "Error writing available games to socket" << endl;
  }

  //close client's socket
  close(client_socket);
}
