/*
 * GameManager.h
 *
 *  Created on: Dec 25, 2017
 *      Author: djoffe
 */

#ifndef INCLUDE_GAMEMANAGER_H_
#define INCLUDE_GAMEMANAGER_H_

#include "threads.h"

//class GameManager {
//public:
//  /*
//   * constructs.
//   */
//	GameManager(){};
//	bool is_client_closed(int cs);
//	bool isGameOnList(string& comgameName);
//
//	void closeGame(string& gameName, map<string, vector<int> >& games,  int waitingClient);
//	bool playTurn(vector<string>& args, int waitingClient);
//	void RunGame(int clientSocket1 , int clientSocket2, string& gameName, map<string, vector<int> >& games);
//	bool handleOneClient(int clientSocket, int waitingclient, string& gameName, map<string, vector<int> >& games);
//
//private:
//
//};

//Singleton version
class GameManager {
public:
	static GameManager* getInstance();
	int addGame(string gameName, int playerSocket);
	int addPlayerToGame(string gameName, int playerSocket);
	void deleteGame(string gameName);

	void informPlayerGameCloses(int clientSocket);


	map<string, vector<int> >* getGames();
	bool isGameExist(string gameName);
	int playersAmount(string gameName);
	vector<int>& getPlayers(string gameName);
	void closeGames();

	void RunGame(string& gameName);


//	void RunGame(int clientSocket1 , int clientSocket2, string& gameName, map<string, vector<int> >& games);

private:
	GameManager(): games(new map<string, vector<int> >){}
	map<string, vector<int> >* games;
	static GameManager* instance;

	bool is_client_closed(int cs);
	bool isGameOnList(string& comgameName);
	void closeGame(string& gameName, map<string, vector<int> >& games,  int waitingClient);
	bool playTurn(vector<string>& args, int waitingClient);
	bool handleOneClient(int clientSocket, int waitingclient, string& gameName, map<string, vector<int> >& games);
};








#endif /* INCLUDE_GAMEMANAGER_H_ */
