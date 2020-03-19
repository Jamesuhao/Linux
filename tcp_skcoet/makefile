all:tcp_cli thread_srv 
tcp_cli:tcp_cli.cpp 
	g++ $^ -o $@ -std=c++11
thread_srv:thread_srv.cpp 
	g++ $^ -o $@ -std=c++11 -lpthread
tcp_srv:tcp_srv.cpp 
	g++ $^ -o $@ -std=c++11
process_srv:process_srv.cpp 
	g++ $^ -o $@ -std=c++11
