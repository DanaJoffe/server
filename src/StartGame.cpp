/*
 * Start.cpp
 *
 *  Created on: Dec 22, 2017
 *      Author: chaviva
 */

#include <StartGame.h>

void StartGame::execute(vector<string>& args, map<string, vector<int> >& games,
    int client_socket) {
  //add game name to map with client_socket as player 1
  vector<int> players_sockets;
  players_sockets.push_back(client_socket);
  pair<map<string, vector<int> >::iterator,bool> result;
  result = games.insert(make_pair(args[0], players_sockets));
  int n;
  int ret;
  if (result.second == false) {
    ret = -1;
  } else {
    ret = 1;
  }
  //write to client if succeeded to start the game
  n = write(client_socket, &ret, sizeof(ret));
  if (n == -1) {
    cout << "Error writing return value to socket" << endl;
  }
}
