#include "mininn/utils/log.h"

#include <iostream>

int main(int argc, char* argv[]) {
	LOG(INFO) << "log from my log";
	std::cout << "hello world"<< std:: endl;
	return 0;
}