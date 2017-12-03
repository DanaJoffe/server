/*
 * socket.h
 *
 *  Created on: Dec 3, 2017
 *      Author: chaviva
 */

#ifndef SOCKET_H_
#define SOCKET_H_

struct sockaddr {
  unsigned short  sa_family;
  char sa_data[14];
};

struct sockaddr_in {
  short int sin_family;
  unsigned short int  sin_port;
  struct in_addr  sin_addr;
  unsigned char sin_zero[8];
};

struct hostent {
  char *h_name;
  char **h_aliases;
  int h_addrtype;
  int h_length;
  char **h_addr_list;

#define h_addr h_addr_list[0];
};

#endif /* SOCKET_H_ */
