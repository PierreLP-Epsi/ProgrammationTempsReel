#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

void send_message(ClientSocket client, string user){
	string message;
	string reply;
	
	while(true){
		getline(cin, message);
		client << user + " : " + message;
		client >> reply;
		cout << reply << endl;
	}
}

int main (int argc, char* argv[]) {
  if(argc != 3) {
    cout << "invalid number of arguments: call with `./client IP port`" << endl;
    return 1;
  }

  string host = argv[1];
  int port = atoi(argv[2]);

  try {
    ClientSocket client_socket ( host, port );
    string reply;
	string user;

	try {
		client_socket << "Coucou";
		client_socket >> reply;
	} catch ( SocketException& ) {}
	
    cout << "We received this response from the server:\n\"" << reply << "\"\n";;
	
	cout << "Entrer votre pseudo : ";
	cin >> user;
	cout << "Votre pseudo est : " << user << endl;

	send_message(client_socket, user);
  }  
  
  catch ( SocketException& e ) {
    cout << "Exception was caught:" << e.description() << endl;
  }

  return 0;
}
