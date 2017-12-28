/*
 * ListGames.cpp
 *
 *  Created on: Dec 22, 2017
 *      Author: chaviva
 */

#include "ListGames.h"

void ListGames::execute(vector<string>& args, int client_socket) {
  GameManager* gameManager = GameManager::getInstance();

  pthread_mutex_lock(&map_mutex);
  map<string, vector<int> >& game_list(*gameManager->getGames());//change
  pthread_mutex_unlock(&map_mutex);

  //write string of available games to send to client
  stringstream available_games;
  map<string, vector<int> >::iterator it;
  for (it = game_list.begin(); it != game_list.end(); it++) {
    if (it->second.size() == 1) {
      available_games << it->first << endl;
    }
  }
  //check if string has available games
  available_games.seekg(0, ios::end);
  int size = available_games.tellg();
  available_games.seekg(0, ios::beg);

  stringstream msg;

  if (size == 0) {
    msg << "There are no current available games to join." << endl;
  } else {
	  stringstream title;
	  title << "Available Games:" <<endl;
	  msg << title.str();
	  msg << available_games.str();
  }

  //get size of message
  msg.seekg(0, ios::end);
  size = msg.tellg();
  msg.seekg(0, ios::beg);

  //send size of string
  int n = write(client_socket, &size, sizeof(size));
  if (n == -1) {
    cout << "Error writing string size to socket" << endl;
  }
  //send string of available games

  cout <<"ListGames::execute: sending to client: " <<msg;


  n = write(client_socket, msg.str().c_str(), size);
  if (n == -1) {
    cout << "Error writing available games to socket" << endl;
  }

  //close client's socket
  close(client_socket);
}
