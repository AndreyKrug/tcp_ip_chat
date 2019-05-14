#include "Server.h"

Server::Server(short port):
	ep(boost::asio::ip::tcp::v4(), port),
	acc(boost::asio::ip::tcp::acceptor(io,ep))
{
}

int Server::id = 0;

Server::~Server()
{
}

void Server::start()
{
	shared_ptr<boost::asio::ip::tcp::socket> sock(new boost::asio::ip::tcp::socket(io));
	shared_ptr<Client> cl(new Client(sock, ++id));
	acc.async_accept(*sock, std::bind(&Server::on_accept, this, cl, std::placeholders::_1));
	io.run();
}

void Server::on_accept(std::shared_ptr<Client>& cl, const boost::system::error_code & err)
{
	if (err) {
		cout << "Client " << cl->id << " on_accept error" << endl;
	}
	else { //если небыло ошибки подключени€, запускаю асинхронную прослушку сокета клиента и добавл€ю клиента в список
		cout << "Client " << cl->id << " connect" << endl;
		Clients.push_back(cl);
		cl->sock->async_read_some(boost::asio::buffer(cl->buf), std::bind(&Server::on_read, this, cl, std::placeholders::_1, std::placeholders::_2));
	}
	//в не нависимости, была ошибка подключени€ или нет, запускаю следующую операцию асинхронного прослушивани€ порта
	std::shared_ptr<boost::asio::ip::tcp::socket> sock(new boost::asio::ip::tcp::socket(io));
	std::shared_ptr<Client> client(new Client(sock, ++id));
	acc.async_accept(*sock, std::bind(&Server::on_accept, this, client, std::placeholders::_1));
}

void Server::on_read(std::shared_ptr<Client>& cl, const boost::system::error_code & err, size_t bf)
{
	if (err) { //если ошибка у клиента, у которого было запущено async_read_some()
		cout << "Client " << cl->id << " disconnect. Error read" << endl;
		Clients.remove_if([cl](std::shared_ptr<Client> &c) {
			return cl->id == c->id;
		});
	}
	else {

		if (strlen(cl->buf) > 0 && cl->buf[0] != '\0' && cl->buf[0] != '\n') {
			for (auto i = Clients.begin(); i != Clients.end(); ++i) {
				if ((*i)->id == cl->id) {
					continue;
				}
				//отправл€ем не весь буфер, а только количество символов до символа '\0' и сам нуль-символ тоже отправл€ем
				(*i)->sock->async_write_some(boost::asio::buffer(cl->buf, std::strlen(cl->buf) + 1), 
					std::bind(&Server::on_write, this, *i, std::placeholders::_1, std::placeholders::_2));
			}
		}

		cl->sock->async_read_some(boost::asio::buffer(cl->buf), 
			std::bind(&Server::on_read, this, cl, std::placeholders::_1, std::placeholders::_2));

	}
}

void Server::on_write(std::shared_ptr<Client>& cl, const boost::system::error_code & err, size_t bf)
{
	if (err) {
		cout << "Client " << cl->id << " disconnect. Error write" << endl;
		Clients.remove_if([cl](std::shared_ptr<Client> &client)->bool {
			return cl->id == client->id;
		});
	}
}
