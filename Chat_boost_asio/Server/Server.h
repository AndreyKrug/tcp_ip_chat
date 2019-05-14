#pragma once
#include"client.h"
#include<list>

using std::cout;
using std::cin;
using std::endl;
using std::shared_ptr;


class Server
{
	static int id; //выдается каждому клиенту при подключении

	boost::asio::io_context io;
	boost::asio::ip::tcp::endpoint ep;
	boost::asio::ip::tcp::acceptor acc;


	std::list<std::shared_ptr<Client>> Clients;

	void on_read(std::shared_ptr<Client> &cl, const boost::system::error_code &err, size_t bf); //обработчик, срабатывающий после асинхронного чтения с сокета

	void on_write(std::shared_ptr<Client> &cl, const boost::system::error_code &err, size_t bf); //обработчик, срабатывающий после асинхронной записи в сокет

	void on_accept(std::shared_ptr<Client> &cl, const boost::system::error_code &err); //обработчик, срабатывающий после асинхронного подключения клиента

public:
	Server(short port=1234);
	~Server();
	void start();
};

