// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "string.h"
THIRD_PARTY_INCLUDES_START
#include <boost/asio.hpp>
#include <boost/asio/connect.hpp>
THIRD_PARTY_INCLUDES_END
/**
 * 
 */
class UMyNetworkSubsystem;
class MULTISHOOTER_API TCPClient
{
public:
	TCPClient(const std::function<void(char*, uint16)> receiveCallback);
	~TCPClient();
private:
	char* rcvBuf;
	boost::asio::io_context* io_context;
	boost::asio::ip::tcp::socket* serverSocket;
	uint16 messageLength;
	
	std::function<void(char*, uint16)> receiveCallback;
public:
	void Start(const boost::asio::ip::tcp::endpoint endpoints);
	void ConnectToServer(const boost::asio::ip::tcp::endpoint endpoints);
	void Connect_Completed(const boost::system::error_code& error);
	void Send(char* buf, uint16 size);
	void Receive();
	void Receive_Header(const boost::system::error_code& error, std::size_t bytesReceived);
	void Receive_Body(const boost::system::error_code& error, std::size_t bytesReceived);
	void OnDisconnted();
};
