#ifndef ERROR_H
#define ERROR_H

#include <string>
#include "defines.h"

class Error {
	private:
		std::string tagToStr(short tag);
	public:
		std::string msg;
		Error(std::string m);
		Error(short tag, short near);
};

#endif