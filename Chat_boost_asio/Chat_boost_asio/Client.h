#pragma once
#include<iostream>
#include<boost\asio.hpp>

using std::cout;
using std::cin;
using std::endl;
using std::shared_ptr;
using std::string;

class Client
{
	int buf_size_input;	//размер буфера входящих сообщений
	int buf_size_output; //размер буфера исходящих сообщений

	char *buf_input; //буфер для входящих сообщений
	char *buf_output;//буфер исходящих сообщений

	boost::asio::io_context io;
	shared_ptr<boost::asio::ip::tcp::socket> socket;

	string address; //ip-адрес для подключения
	short port;		//порт для подключения

	std::thread *thr;

	void on_connect(const boost::system::error_code &err); //обработчик подключения
	void on_read(const boost::system::error_code &err, size_t bytes);
	void on_write(const boost::system::error_code &err, size_t bytes);
	void io_run();
public:
	Client(string address="127.0.0.1", short port=1234, int buffers_size=1024);
	~Client();
	void start();
};

