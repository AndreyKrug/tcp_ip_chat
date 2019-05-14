#include"Client.h"

void main() {

	shared_ptr<Client> client(new Client());
	client->start();
	system("pause");
}

