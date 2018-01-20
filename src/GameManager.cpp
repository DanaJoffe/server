/*
 * GameManager.cpp
 *
 * Author1: name & ID: Dana Joffe 312129240
 * Author2: name & ID: Chaviva Moshavi 322082892
 */

#include <GameManager.h>

pthread_mutex_t GameManager::instance_lock;
pthread_mutex_t GameManager::threads_lock;
GameManager* GameManager::instance = 0;

GameManager* GameManager::getInstance() {
	if(!instance) {
	  pthread_mutex_lock(&instance_lock);
	  if (!instance) {
	    instance = new GameManager();
	  }
		pthread_mutex_unlock(&instance_lock);
	}
  return instance;
}

GameManager::GameManager(): games(new map<string, vector<int> >) {};

void GameManager::destroyInstance() {
  if (instance != 0) {
    pthread_mutex_lock(&instance_lock);
    if (instance != 0) {
      delete instance;
      instance = 0;
    }
    pthread_mutex_unlock(&instance_lock);
  }
}

GameManager::~GameManager() {
  pthread_mutex_lock(&map_mutex);
  delete games;
  pthread_mutex_unlock(&map_mutex);
}

void GameManager::refreshGameList() {
	map<string, vector<int> >::iterator it;
	string gameName;
	vector<int> clients;
	unsigned i;

    pthread_mutex_lock(&map_mutex);
	map<string, vector<int> > games_copy(*this->games);
    pthread_mutex_unlock(&map_mutex);

	for (it = games_copy.begin(); it!=games_copy.end(); ++it) {
		clients = it->second;
		gameName = it->first;
		for (i = 0; i<clients.size(); i++) {
			if (this->is_client_closed(clients[i])) {
				this->deleteGame(gameName);
				break;
			}
		}
	}
}

bool GameManager::doesGameExist(const string& gameName) {

	map<string, vector<int> >::iterator it;

  pthread_mutex_lock(&map_mutex);
	map<string, vector<int> > games_copy(*this->games);
  pthread_mutex_unlock(&map_mutex);

  //find game
	it = games_copy.find(gameName);

  //if didn't find game
	if (it == games_copy.end()) {
		return false;
	}

	// check if players are still connected. if not - delete game
	vector<int> clients = it->second;
	for (unsigned i = 0; i < clients.size(); i++) {
	   // if player is disconnected - delete game.
	   if (this->is_client_closed(clients[i])) {
		   this->deleteGame(gameName);
		   return false;
     }
	}
	// game has been found and players still connected
	return true;
}

int GameManager::playersAmount(const string& gameName) {
	map<string, vector<int> >::iterator it;

  pthread_mutex_lock(&map_mutex);
  map<string, vector<int> > games_copy(*this->games);
  pthread_mutex_unlock(&map_mutex);

  //find game
	it = games_copy.find(gameName);
	//if didn't find game, return -1
	if (it == games_copy.end()) {
		return -1;
	}
	//return number of players
	return it->second.size();
}

int GameManager::addGameWithPlayer(const string& gameName, int playerSocket) {
	if (this->doesGameExist(gameName)) {
		return -1;
	}
	vector<int> players_sockets;
	players_sockets.push_back(playerSocket);

	pair<map<string, vector<int> >::iterator,bool> result;
  pthread_mutex_lock(&map_mutex);
	result = this->games->insert(make_pair(gameName, players_sockets));
	if (result.second == false) {
	   pthread_mutex_unlock(&map_mutex);
	   return -1;
	}
	pthread_mutex_unlock(&map_mutex);
	return 1;
}

bool GameManager::addPlayerToGame(const string& gameName, int playerSocket) {
	if (!this->doesGameExist(gameName) || this->playersAmount(gameName) == 2) {
	  return false;
	}
	//game exists and has 1 player - add the second player
	map<string, vector<int> >::iterator it;
  pthread_mutex_lock(&map_mutex);
	it = this->games->find(gameName);
	it->second.push_back(playerSocket);
  pthread_mutex_unlock(&map_mutex);
  return true;
}

void GameManager::deleteGame(const string& gameName) {
	 vector<int> clients = this->getPlayers(gameName);
	 for (unsigned i = 0; i < clients.size(); i++) {
		 if (!this->is_client_closed(clients[i])) {
			 this->informPlayerGameClosed(clients[i]);
		 }
		 close(clients[i]);
	 }
	 map<string, pthread_t>::iterator it;
	 //delete gameName from games
	 pthread_mutex_lock(&map_mutex);
	 this->games->erase(gameName);
   pthread_mutex_unlock(&map_mutex);
   //delete gameName from threads
   pthread_mutex_lock(&threads_lock);
   it = threads.find(gameName);
   if (it != threads.end()) {
     this->threads.erase(gameName);
   }
   pthread_mutex_unlock(&threads_lock);
}

void GameManager::informPlayerGameClosed(int clientSocket) {
	int num = -2;
	int n;

	//ignore borken pipe, this way if write doesn't succeed write will return -1
	signal(SIGPIPE, SIG_IGN);
	if (!this->is_client_closed(clientSocket)) {
	  n = write(clientSocket, &num, sizeof(num));
	}
	if (n == -1) {
	  return;
	}
  if (!this->is_client_closed(clientSocket)) {
    n = write(clientSocket, &num, sizeof(num));
  }
	if (n == -1) {
	 return;
	}
}

map<string, vector<int> > GameManager::getGames() {
	this->refreshGameList();
	map<string, vector<int> > games_copy(*this->games);
	return games_copy;
}

vector<int> GameManager::getPlayers(const string& gameName) {
	//find game
  vector<int> players;
	map<string, vector<int> >::iterator it;
  pthread_mutex_lock(&map_mutex);
	it = this->games->find(gameName);
	players = it->second;
  pthread_mutex_unlock(&map_mutex);
	//return players
	return players;
}

void GameManager::RunGame(const string& gameName) {
	vector<int> clients = this->getPlayers(gameName);
	if (clients.size() != 2) {
		throw "Error in GameManager::RunGame. player is missing";
	}

	//save game thread
    saveThreadOfGame(gameName);

	int clientSocket1 = clients[0];
	int clientSocket2 = clients[1];

	while (true) {

		//handle first client
		bool isClient1Connected = handleOneClient(clientSocket1, clientSocket2, gameName);
		if (!isClient1Connected) {
			return;
		}

		//handle second client
		bool isClient2Connected = handleOneClient(clientSocket2, clientSocket1, gameName);
		if (!isClient2Connected) {
			return;
		}
	}
	return;
}

void GameManager::closeGames() {
    closeGameThreads();
	map<string, vector<int> >::iterator it;
	string gameName;
    pthread_mutex_lock(&map_mutex);
	for (it = this->games->begin(); it!=this->games->end(); ++it) {
		gameName = it->first;
	  pthread_mutex_unlock(&map_mutex);
		this->deleteGame(gameName);
	  pthread_mutex_lock(&map_mutex);
	}
	pthread_mutex_unlock(&map_mutex);
}

void GameManager::closeGameThreads() {
  map<string, pthread_t>::iterator it;
  pthread_mutex_lock(&threads_lock);
  for (it = this->threads.begin(); it!=this->threads.end(); ++it) {
    pthread_cancel(it->second);
  }
  pthread_mutex_unlock(&threads_lock);
}

void GameManager::saveThreadOfGame(const string& gameName) {
  pthread_mutex_lock(&threads_lock);
  threads[gameName] = pthread_self();
  pthread_mutex_unlock(&threads_lock);
}

bool GameManager::handleOneClient(int clientSocket, int waitingClient, const string& gameName) {
	// receive command and arguments
	string commandName;
	vector<string> args;

	bool b = readCommand(clientSocket, &commandName, &args);
	if (b == false) {
		if (!this->is_client_closed(waitingClient)) {
		  cout << "Client disconnected" << endl;
		  deleteGame(gameName);
		}
		return false;
	}
	if (strcmp(commandName.c_str(), "close") == 0) {
		closeGame(gameName);
		return false;
	} else if (strcmp(commandName.c_str(), "play") == 0) {
		bool b = playTurn(args, waitingClient, clientSocket);
		if (b == false) {
		  deleteGame(gameName);
			return false;
		}
	}
	return true;
}

void GameManager::closeGame(const string& gameName) {
	this->deleteGame(gameName);
}

bool GameManager::playTurn(vector<string>& args, int waitingPlayer, int currentPlayer) {
	int row = atoi(args[0].c_str());
	int col = atoi(args[1].c_str());

	if (is_client_closed(waitingPlayer)) {
		cout << "Client disconnected" << endl;
		return false;
	}
	// writing row to waitingClient socket
	int n = write(waitingPlayer, &row, sizeof(row));
	if (n == -1) {
		throw "Error writing size to socket";
	}
	if (is_client_closed(waitingPlayer)) {
		cout << "Client disconnected" << endl;
		return false;
	}
	// writing col to waitingClient socket
	n = write(waitingPlayer, &col, sizeof(col));
	if (n == -1) {
		throw "Error writing size to socket";
	}
	return true;
}


bool GameManager::is_client_closed(int cs) {
	pollfd pfd;
	pfd.fd = cs;
	pfd.events = POLLIN | POLLHUP | POLLRDNORM;
	pfd.revents = 0;

	// call poll with a timeout of 100 ms
	if(poll(&pfd, 1, 100) > 0) {
		// if result > 0, this means that there is either data available on the
		// socket, or the socket has been closed
		char buffer[32];
		if(recv(cs, buffer, sizeof(buffer), MSG_PEEK | MSG_DONTWAIT) == 0) {
			// if recv returns zero, that means the connection has been closed:
			return true;
		}
	}
	return false;
}
