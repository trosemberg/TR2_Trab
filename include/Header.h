#ifndef HEADER_H
#define HEADER_H

#include <string>
#include <vector>
#include <tuple>

#include "Dump.h"

#define field std::tuple< std::string, std::string >

namespace HTTP {

struct Header {
	explicit Header( std::string& str );

	std::string to_string( bool include = true );

	std::string startLine;
	std::vector< field > fields;
	std::string body;

	std::string host;
	std::string port;
};

};

#endif // HEADER_H