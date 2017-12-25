/*
 * main.cpp
 *
 * Author1: name & ID: Dana Joffe 312129240
 * Author2: name & ID: Chaviva Moshavi 322082892
 */

#include "Server.h"
#include <iostream>
#include <stdlib.h>
#include <fstream>
using namespace std;

//run server
int main() {
try {

  //get server port number from file
  int port_num;
  ifstream port_file;
  port_file.open("port_number.txt");
  if(!port_file.is_open()) {
    cout << "Cannot open file with port number." << endl;
    return -1;
  }
  port_file >> port_num;
  port_file.close();
  //construct and run server
  Server server(port_num);
  try {
    server.start();
  } catch (const char *msg) {
    cout << "Cannot start server. Reason: " << msg << endl;
    exit(-1);
  }
  //stop server
  server.stop();

}catch (const char* msg) {
	cout << msg << endl;
	}
  return 0;
}
