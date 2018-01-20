/*
 * GameManager.h
 *
 * Author1: name & ID: Dana Joffe 312129240
 * Author2: name & ID: Chaviva Moshavi 322082892
 */

#ifndef INCLUDE_GAMEMANAGER_H_
#define INCLUDE_GAMEMANAGER_H_

#include <map>
#include <csignal>
#include <vector>

#include "threads.h"

class GameManager {
public:
  /*
   * get instance of game manager.
   * game manager is singleton. only one instance of it can be created.
   */
	static GameManager* getInstance();
  /*
   * deletes instance if instance exists
   */
	static void destroyInstance();
	/*
	 * add new game with player's socket to map of games
	 * input: string gameName to be added, int playerSocket
	 * output:
	 * 1 if succeeded to add game
	 * -1 if didn't succeed to add game.
	 */
	int addGameWithPlayer(const string& gameName, int playerSocket);
	/*
	 * add new player's socket to existing game.
	 * input:
	 * string gameName - the game to add the player to
	 * int playerSocket - the player to add
	 */
	bool addPlayerToGame(const string& gameName, int playerSocket);
	/*
	 * delete game from map of games.
	 * close player's sockets if they are still open.
	 */
	void deleteGame(const string& gameName);
	/*
	 * inform player if their game was closed.
	 * input: int clientSocket - player's socket
	 */
	void informPlayerGameClosed(int clientSocket);
	/*
	 * get up to date map of games with their players' sockets
	 */
	map<string, vector<int> > getGames();
	/*
	 * check if gameName is a game on list that is currently
	 * playing or waiting to start playing.
	 * input: gameName
	 */
	bool doesGameExist(const string& gameName);
	/*
	 * get amount of players in specified game
	 * input: gameName - name of the game
	 * output:
	 * if game exists return number of players
	 * else return -1
	 */
	int playersAmount(const string& gameName);
	/*
	 * get list of players in specified game.
	 * input: gameName - name of the game
   * output: vector of players
	 */
	vector<int> getPlayers(const string& gameName);
	/*
	 * close all games on list of games.
	 */
	void closeGames();
	/*
	 * run game
	 * input: gameName - name of game to run.
	 */
	void RunGame(const string& gameName);

private:
  /*
   * constructor. it is private because game manager is singleton.
   * if no instance exists, getInstance() will create game manager.
   */
  GameManager();
  /*
   * destructor
   */
  ~GameManager();
	/*
	 * refreshes game list to be up to date.
	 * deletes games whose players left the game/ their sockets were closed.
	 */
	void refreshGameList();
	/*
	 * checks if client socket is closed.
	 * input: int cs - client socket
	 */
	bool is_client_closed(int cs);
	/*
	 * close specific game: delete game from list and close client sockets
	 * input: gameName - name of game to close
	 */
	void closeGame(const string& gameName);
	/*
	 *play turn of current player.
	 *send current player's move to waiting player.
	 *if waiting player;s socket is closed - inform current player that game is closed.
	 *input:
	 *args: player's move: x, y
	 *waitingCleint: waiting player's socket
	 *currentPlayer: current player's socket
	 *ouput: true if succeeded in writing to waiting player, else false
	 */
	bool playTurn(vector<string>& args, int waitingClient, int currentPlayer);
	/*
	 * read command from client and call relevant function to execute command.
	 * input:
	 * clientSocket: client's socket
	 * waitingclient: other client in game
	 * gameName: name of game with these clients
	 * output: true if waiting client's socket is open. else, false
	 */
	bool handleOneClient(int clientSocket, int waitingclient, const string& gameName);
  /*
   * save thread of existing game.
   */
  void saveThreadOfGame(const string& gameName);
	/*
	 * cancel all threads of current games.
	 */
  void closeGameThreads();

	//data members
  map<string, vector<int> >* games;
  map<string, pthread_t> threads;
  static GameManager* instance;
  static pthread_mutex_t instance_lock;
  static pthread_mutex_t threads_lock;
};


#endif /* INCLUDE_GAMEMANAGER_H_ */
