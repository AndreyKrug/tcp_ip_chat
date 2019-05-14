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
	int buf_size_input;	//������ ������ �������� ���������
	int buf_size_output; //������ ������ ��������� ���������

	char *buf_input; //����� ��� �������� ���������
	char *buf_output;//����� ��������� ���������

	boost::asio::io_context io;
	shared_ptr<boost::asio::ip::tcp::socket> socket;

	string address; //ip-����� ��� �����������
	short port;		//���� ��� �����������

	std::thread *thr;

	void on_connect(const boost::system::error_code &err); //���������� �����������
	void on_read(const boost::system::error_code &err, size_t bytes);
	void on_write(const boost::system::error_code &err, size_t bytes);
	void io_run();
public:
	Client(string address="127.0.0.1", short port=1234, int buffers_size=1024);
	~Client();
	void start();
};

