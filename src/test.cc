#include <iostream>
#include <glog/logging.h>

int main(int argc, char* argv[]) {
	google::InitGoogleLogging(argv[0]);
	LOG(INFO) << "log from glog";
	std::cout << "hello world"<< std:: endl;
	return 0;
}