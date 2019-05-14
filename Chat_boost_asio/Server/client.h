#pragma once
#include<iostream>
#include<memory>
#include<boost\asio.hpp>

//под каждого подключенного клиента создаю объект структуры client
struct Client {
	int id; //id клиента; выдается сервером при подключении
	std::shared_ptr<boost::asio::ip::tcp::socket> sock; 
	char buf[1024]; //буфер для чтения сообщений от клиента
	Client(std::shared_ptr<boost::asio::ip::tcp::socket> &socket, int id):
		sock(socket), id(id) { }

};
