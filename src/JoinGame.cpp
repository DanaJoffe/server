/*
 * Join.cpp
 *
 *  Created on: Dec 22, 2017
 *      Author: chaviva
 */

#include <JoinGame.h>

void JoinGame::execute(vector<string>& args, map<string, vector<int> >& games,
    int client_socket) {
  //find game
  map<string, vector<int> >::iterator it;
  it = games.find(args[0]);
  //inform client if player can join requested game
  int result;
  if (it == games.end() || it->second.size() == 2) {
    result = -1;
  } else {
    result = 1;
  }
  int n = write(client_socket, &result, sizeof(result));
  if (n == -1) {
    cout << "Error writing result to socket" << endl;
  }
  //if player can join game
  if (result == 1) {
    //add player to game
    it->second.push_back(client_socket);
    //send players their colors
    int color = 1;
    n = write(it->second[0], &color, sizeof(color));
    if (n == -1) {
      cout << "Error writing color to socket" << endl;
    }
    color = 2;
    n = write(it->second[1], &color, sizeof(color));
    if (n == -1) {
      cout << "Error writing color to socket" << endl;
    }
  }
}
