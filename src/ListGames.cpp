/*
 * ListGames.cpp
 *
 *  Created on: Dec 22, 2017
 *      Author: chaviva
 */

#include "ListGames.h"

void ListGames::execute(vector<string>& args, map<string, vector<int> >& games,
    int client_socket) {
  //write string of available games to send to client
  stringstream available_games;
  map<string, vector<int> >::iterator it;
  for (it = games.begin(); it != games.end(); it++) {
    if (it->second.size() == 1) {
      available_games << it->first << " "<< endl;
    }
  }
  //send size of string
  available_games.seekg(0, ios::end);
  int size = available_games.tellg();
  available_games.seekg(0, ios::beg);
  int n = write(client_socket, &size, sizeof(size));
  if (n == -1) {
    cout << "Error writing string size to socket" << endl;
  }
  //send string of available games
  string msg = available_games.str();
  n = write(client_socket, msg.c_str(), size);
  if (n == -1) {
    cout << "Error writing available games to socket" << endl;
  }
}
