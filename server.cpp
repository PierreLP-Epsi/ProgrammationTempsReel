#include "ServerSocket.h"
#include "SocketException.h"
#include <string>
#include <iostream>
#include <thread>
#include <vector>
#include <cstdlib>
using namespace std;

class User{
	public:
		string name = "ProbablyUnnamed";
		ServerSocket *socket;

		void send(string message) {
			*this->socket << message;
		}

		void send_all(vector<User *> users, string message, int current) {
			for (int i = 0; i < users.size(); i++) {
				if (i != current) {
					users[i]->send(message);
				}
			}
		}
};

//Vector for concurrent_access
static vector<thread> threads;
static vector<User *> users;

void thread_for_clients(User user, int current) {
	try {
		while (true) {
			string data;
			*user.socket >> data;
			//Unknow User
			if (user.name == "ProbablyUnnamed") {
				string success_message;
				user.name = data;
				cout << user.name << " joined the party! " << endl;
				success_message = user.name + " has joined";
				*user.socket << success_message;
			} else {
				string message;
				string request;

				//ChangeUserName (Should be data.first4char() or like that)
				if (data.find("USER") != string::npos) {
					//Can be change as: message data.substr(data.find(" ")); to strictly respect given rules (no space allowed in username)
					message = data.substr(5);
					request = user.name + " evolved into " + message;
					user.send_all(users, request, current);
					string success_message;
					success_message = user.name + " evolved successfully!";
					*user.socket << success_message;
				} 
				//SendMessage (Should be data.first3char() or like that)
				else if (data.find("MSG") != string::npos) {
					//Same possiblity but without "retour chariot" this time
					message = data.substr(3);
					request = user.name + " says: " + message;
					user.send_all(users, request, current);
					string success_message;
					success_message = user.name + " send successfully!";
					*user.socket << success_message;
				}
				//Respect pattern
				else {
					message = data;
					request = "You try to say: " + message;
					user.send(request);
					request = "But it failed. Try to follow this condition:";
					user.send(request);
					request = "'USER new_username' or 'MSG new_message'";
					user.send(request);
					string failure_message;
					failure_message = user.name + " failed to send!";
					*user.socket << failure_message;
				}


			}
		}
	} catch (SocketException e) {
		cout << "Exception was caught:" << e.description() << "\nExiting" << endl;
	}
}

int main (int argc, char* argv[]) {
  if(argc !=2) {
    cout << "invalid number of arguments: call with `./server port`" << endl;
    return 1;
  }

  int port = atoi(argv[1]);

  cout << "running....\n";

  try {
    ServerSocket server ( port );
    while ( true ) {
      ServerSocket *new_socket = new ServerSocket();
      server.accept ( *new_socket );
      User *user = new User();
      users.push_back(user);
      user->socket = new_socket;
      threads.push_back(thread(thread_for_clients, user, users.size() - 1));
    }
  } catch ( SocketException& e ) {
  	cout << "Exception was caught:" << e.description() << "\nExiting" << endl;
  }

  return 0;
}
