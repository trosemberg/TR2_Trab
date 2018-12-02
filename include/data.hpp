#ifndef MSGDATA_H
#define MSGDATA_H

#include <string>
#include <vector>
#include <memory>

#include <unistd.h>

typedef struct {
	std::string message;

	std::string addr_from;
	int port_from;

	std::string addr_to;
	int port_to;

	int host;
	int port;
} msgData;

#endif // MSGDATA_H