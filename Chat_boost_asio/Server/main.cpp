#include"Server.h"

void main() {
	shared_ptr<Server> server(new Server(1234));
	server->start();
	system("pause");
}