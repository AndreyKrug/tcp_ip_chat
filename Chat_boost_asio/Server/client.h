#pragma once
#include<iostream>
#include<memory>
#include<boost\asio.hpp>

//��� ������� ������������� ������� ������ ������ ��������� client
struct Client {
	int id; //id �������; �������� �������� ��� �����������
	std::shared_ptr<boost::asio::ip::tcp::socket> sock; 
	char buf[1024]; //����� ��� ������ ��������� �� �������
	Client(std::shared_ptr<boost::asio::ip::tcp::socket> &socket, int id):
		sock(socket), id(id) { }

};
