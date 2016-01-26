#include "Errors.h"

#include <cstdlib>
#include <iostream>


void fatalError(std::string errorString)
{
	std::cout << errorString << std::endl;
}
