/*
 * ListGames.cpp
 *
 * Author1: name & ID: Dana Joffe 312129240
 * Author2: name & ID: Chaviva Moshavi 322082892
 */

#include "ListGames.h"

void ListGames::execute(vector<string>& args, int client_socket) {
  GameManager* gameManager = GameManager::getInstance();

  map<string, vector<int> > game_list(gameManager->getGames());

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
  n = write(client_socket, msg.str().c_str(), size);
  if (n == -1) {
    cout << "Error writing available games to socket" << endl;
  }

  //close client's socket
  close(client_socket);
}
