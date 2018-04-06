#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <thread>
#include <vector>

using namespace std;

void send_message(ClientSocket client){
	string message;
	
	try {
		while(true){
			cout << "Send like: 'USER your_new_username' or 'MSG new_message' "
			getline(cin, message);
			client >> message;
		}	
	} catch ( SocketException& e ) {
		cout << "Exception was caught:" << e.description() << endl;
	}
}

void receive_message(ClientSocket client) {
	string message;
	try {
		while (true) {
			client >> message;
			cout << "We received this response from the server:\n\"" << message << "\"\n";
		}
	} catch ( SocketException& e ) {
		cout << "Exception was caught:" << e.description() << endl;
	}
}

int main (int argc, char* argv[]) {
  if(argc != 3) {
    cout << "invalid number of arguments: call with `./client IP port`" << endl;
    return 1;
  }

  string host = argv[1];
  int port = atoi(argv[2]);

  std::vector<thread> v(thread::hardware_concurrency());

  try {
  	cout << "Hello, to begin, enter your name please: ";
  	getline(cin, name);
  	ClientSocket client(host, port);
  	client << name;

  	//Init Threads (Listen & Speak)
  	thread t1 = thread(receive_message, ref(client));
  	thread t2 = thread(send_message, ref(client));
  catch ( SocketException& e ) {
    cout << "Exception was caught:" << e.description() << endl;
  }

  return 0;
}
