#ifndef HEADER_H
#define HEADER_H

#include <string>
#include <vector>
#include <tuple>

class HTTP{
public:
	HTTP();
	~HTTP();
	void* getRequestHTTP(void*);
}

#endif // HEADER_H