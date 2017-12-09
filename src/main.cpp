/*
 * main.cpp
 *
 *  Created on: Dec 3, 2017
 *      Author: djoffe
 */

#include "Server.h"
#include <iostream>
#include <stdlib.h>
#include <fstream>
using namespace std;


int main() {
  int port_num;
  ifstream port_file;
  port_file.open("port_number.txt");
  if(!port_file.is_open()) {
    cout << "Cannot open file with port number." << endl;
    return -1;
  }
  port_file >> port_num;
  port_file.close();
  Server server(port_num);
  try {
	server.start();
  } catch (const char *msg) {
	cout << "Cannot start server. Reason: " << msg << endl;
	exit(-1);
  }
}
