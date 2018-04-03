#include "ServerSocket.h"
#include "SocketException.h"
#include <string>
#include <iostream>
#include <cstdlib>
#include <thread>

using namespace std;

int main (int argc, char* argv[]) {
  if(argc !=2) {
    cout << "invalid number of arguments: call with `./server port`" << endl;
    return 1;
  }

  ServerSocket sockets[3];

  int port = atoi(argv[1]);

  cout << "running....\n";

  try {
    ServerSocket server ( port );
	int i = 0;
    while ( true ) {

      server.accept ( sockets[i] );
	  i++;
	  std::thread t1([&](){
		  try {
			while ( true ) {
			  string data;
			  sockets[0] >> data;
			  sockets[0] << data;
			  for(int x = 0; x < sizeof(sockets); x++){
				  sockets[x] << data;
			  }
			  cout << data << endl;
			}
		  } catch ( SocketException& ) {}
	  });

	  std::thread t2([&](){
		  try {
			while (true) {
				if(i>1)
				{
				  string data;
				  sockets[1] >> data;

				  for(int x = 0; x < sizeof(sockets); x++){
					  sockets[x] << data;
				  }
				  cout << data << endl;
				}
			}
		  } catch ( SocketException& ) {}
	  });

	  t1.join();
	  t2.join();
    }
  } catch ( SocketException& e ) {
    cout << "Exception was caught:" << e.description() << "\nExiting" << endl;
  }

  return 0;
}
