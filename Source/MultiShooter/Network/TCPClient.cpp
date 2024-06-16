// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/TCPClient.h"

#include "ClientSocket.h"


TCPClient::TCPClient(const std::function<void(char*, uint16)> receiveCallback):
	receiveCallback(receiveCallback)
{
	rcvBuf = new char[BUFSIZE];
	io_context = new boost::asio::io_context();
	serverSocket = new boost::asio::ip::tcp::socket(*io_context);
	messageLength = 0;
}

TCPClient::~TCPClient()
{
	OnDisconnted();
}


void TCPClient::Start(const boost::asio::ip::tcp::endpoint endpoints)
{
	ConnectToServer(endpoints);
	Receive();
	io_context->run();
}

void TCPClient::ConnectToServer(const boost::asio::ip::tcp::endpoint endpoints)
{
	auto connectBinder = std::bind(&TCPClient::Connect_Completed, this, std::placeholders::_1);
	serverSocket->async_connect(endpoints,connectBinder);
}

void TCPClient::Connect_Completed(const boost::system::error_code& error)
{
	if(!error)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
			FString::Printf(TEXT("Connected And Start Receive")));
	}
}


void TCPClient::Send(char* buf, uint16 size)
{
	if(io_context->stopped() || !serverSocket->is_open())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
	FString::Printf(TEXT("Not Connected")));
		return;
	}
	serverSocket->send(boost::asio::buffer(buf, size));
}

void TCPClient::Receive()
{
	if(rcvBuf)
	{
		delete[] rcvBuf;
	}
	rcvBuf = new char[BUFSIZE];
	messageLength = 0;
	// ������ ��Ŷ ũ�� ��ŭ ��Ŷ �о����
	// receive�� ���ܼ� ���� ���� ���� �� �ִ�.
	// ��Ŷ�� ó�� ���� �켱 PacketSize�� �ڷ��� ��ŭ�� �б�
	serverSocket->async_read_some(boost::asio::buffer(rcvBuf, sizeof(uint16)),
		std::bind(&TCPClient::Receive_Header, this, std::placeholders::_1, std::placeholders::_2));
}

// ��Ŷ ������ �ڷ��� ��ŭ�� �켱 �޾ƿ���
void TCPClient::Receive_Header(const boost::system::error_code& error, std::size_t bytesReceived)
{
	GEngine->AddOnScreenDebugMessage(-1,2, FColor::Green,TEXT("Received!!!!"));
	if(!error)
	{
		std::memcpy(&messageLength, rcvBuf, sizeof(uint16));
		messageLength -= sizeof(uint16);
		// ����� �ִ� �� PacketSize�� ���� �д´�.
		// �ش� PacketSize ��ŭ ��Ŷ���� ���ۿ� �� ������ Callback�� �����Ѵ�.
		serverSocket->async_read_some(boost::asio::buffer(rcvBuf, messageLength),
			std::bind(&TCPClient::Receive_Body, this, std::placeholders::_1, std::placeholders::_2));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1,10, FColor::Red, TEXT("FAILLED"));
	}
}

void TCPClient::Receive_Body(const boost::system::error_code& error, std::size_t bytesReceived)
{
	if (!error)
	{
		if(bytesReceived != messageLength)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, TEXT("PacketSize is incorrect"));
			// ��Ŷ ũ�Ⱑ �ٸ��� �ٽ� ó������ ���ư���.
			Receive();
		}
		else
		{
			FString FormattedString = FString::Format(TEXT("Welcome to {0}!"), { bytesReceived });
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FormattedString);

			// ��Ŷ ������ ���� ��� ��Ŷ ����
			receiveCallback(rcvBuf, bytesReceived);
			Receive();
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(0, 10, FColor::Red, TEXT("FAILED"));
	}
}

void TCPClient::OnDisconnted()
{
	if(serverSocket)
	{
		serverSocket->cancel();
		serverSocket->close();
		delete serverSocket;
	}

	if(io_context)
	{
		io_context->stop();
		io_context->reset();
		delete io_context;
	}
	if(rcvBuf)
	{
		delete[] rcvBuf;
	}
}
