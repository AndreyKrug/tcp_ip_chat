#include "Client.h"

Client::Client(string address, short port, int buffers_size):
	socket(new boost::asio::ip::tcp::socket(io))
{
	this->buf_size_input = buffers_size;
	this->buf_size_output = buffers_size;
	this->address += address;
	this->port = port;

	buf_input = new char[buf_size_input];
	buf_output = new char[buf_size_output];
}


Client::~Client()
{
	delete[]buf_input;
	delete[]buf_output;
	if (thr->joinable()) {
		TerminateThread(thr->native_handle(), 0);
	}
	thr->join();
	delete thr;
}

void Client::start()
{
	boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(address), port);
	socket->async_connect(ep, std::bind(&Client::on_connect, this, std::placeholders::_1));

	thr = new std::thread(&Client::io_run, this);

	string tmp = "";
	while (true) {
		if (io.stopped()) {
			break;
		}
		std::getline(cin, tmp);
		tmp += '\0';
		if (tmp.size() > buf_size_output) { //если пользователь пишет слишком длинные сообщения, увеличим его буфер
			delete[] buf_output;
			buf_output = new char[tmp.size()];
		}
		strcpy_s(buf_output, tmp.size(), tmp.c_str());
		tmp.clear();

		if (strlen(buf_output) > 0 && buf_output[0] != '\0' && buf_output[0] != '\n') {
			socket->async_write_some(boost::asio::buffer(buf_output, std::strlen(buf_output) + 1),
				std::bind(&Client::on_write, this, std::placeholders::_1, std::placeholders::_2));
		}
		
	}
}

void Client::on_connect(const boost::system::error_code & err)
{
	if (err) {
		cout << "Connect error" << endl;
		io.stop();
		return;
	}
	socket->async_read_some(boost::asio::buffer(buf_input, buf_size_input),
		std::bind(&Client::on_read, this, std::placeholders::_1, std::placeholders::_2));
}

void Client::on_read(const boost::system::error_code & err, size_t bytes)
{
	if (err) {
		std::cout << "Error read" << std::endl;
		this->io.stop();
		return;
	}

	if (bytes < buf_size_input) {
		buf_input[bytes] = '\0'; //добавляю нуль-символ в конец сообщения на случай, если какой-то клиент шлет сообщение без нуль-символа
	}
	cout << buf_input << endl;

	socket->async_read_some(boost::asio::buffer(buf_input, buf_size_input),
		std::bind(&Client::on_read, this, std::placeholders::_1, std::placeholders::_2));
}

void Client::on_write(const boost::system::error_code & err, size_t bytes)
{
	if (err) {
		cout << "Connect error! Write error" << endl;
		this->io.stop();
		return;
	}
}

void Client::io_run()
{
	io.run();
}