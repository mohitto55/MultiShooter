// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientSocket.h"
// THIRD_PARTY_INCLUDES_START
// #pragma push_macro("check")
// #undef check
// #include <boost/asio.hpp>
// #include <boost/thread.hpp>
// #include <boost/bind.hpp>
// #include <boost/date_time/posix_time/posix_time.hpp>
// #pragma pop_macro("check")
// THIRD_PARTY_INCLUDES_END

ClientSocket::ClientSocket()
{
	_IsConnected = false;
	if(ConnectServer())
	{
		Thread = FRunnableThread::Create(this, TEXT("Network Thread"));
		_IsConnected = true;
	}
}
ClientSocket::~ClientSocket()
{
	if (Thread)
	{
		// 강제 종료시 클라이언트 소켓 닫기
		Exit();
		// 스레드 종료
		Thread->WaitForCompletion();
		Thread->Kill();
		delete Thread;
	}
}

#pragma endregion

bool ClientSocket::Init()
{
	UE_LOG(LogNet, Warning, TEXT("Thread has been initialized"));
    
	// Socket 연결을 하는 코드를 넣으면 된다. (너무 길어서 뺐다)
	return true;
}

uint32 ClientSocket::Run()
{
	char RecvBuff[BUFSIZE];
	while (1)
	{
		int RecvLen = recv(Socket, reinterpret_cast<char*>(RecvBuff), BUFSIZE, 0);
		if (RecvLen != SOCKET_ERROR)
		{
			for (int i = 0; i < RecvLen; ++i)
				buffer.push(RecvBuff[i]);
		}
		else
		{
			buffer.push(2);
			buffer.push(SOCKET_ERROR);
			return 0;
		}
	}
}

void ClientSocket::Exit()
{
	if (Socket)
	{
		// Socket 연결을 끊고 Winsock 사용을 종료
		closesocket(Socket);
		WSACleanup();
	}
	_IsConnected = false;
}

bool ClientSocket::ConnectServer()
{
	// UE_LOG(LogTemp, Warning, TEXT("0"));
	//
	// WSADATA wsaData;
	// int nRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
	// if (nRet != 0)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("1"));
	//
	// 	return false;
	// }
	//
	// Socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	// if (Socket == INVALID_SOCKET)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("2"));
	// 	return false;
	// }
	//
	// SOCKADDR_IN stServerAddr;
	//
	// stServerAddr.sin_family = AF_INET;
	// // htons 빅엔디안 형식으로 변경
	// stServerAddr.sin_port = htons(PORT);
	//
	// stServerAddr.sin_addr.s_addr = inet_addr(TCHAR_TO_ANSI(ToCStr(SERVER_IP)));
	//
	// nRet = connect(Socket, (sockaddr*)&stServerAddr, sizeof(sockaddr));
	//
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Trying to connect.")));
	//
	// if (nRet == SOCKET_ERROR)
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Fail")));
	// 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, SERVER_IP);
	// 	UE_LOG(LogTemp, Warning, TEXT("3"));
	//
	// 	return false;
	// }
	// else
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Success!")));
	// 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
	// 									 FString::Printf(TEXT("%i.%i"), wsaData.wVersion >> 8, wsaData.wVersion & 0xFF));
	// 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, SERVER_IP);
	//
	// 	UE_LOG(LogNet, Warning, TEXT("Socket Initialized"));
	// 	return true;
	// }
	return true;
}

bool ClientSocket::Send(const int SendSize, void* SendData)
{
	// char buff[BUFSIZE];
	// memcpy(buff, SendData, SendSize);
	// int nSendLen =  send(Socket, buff, buff[0], 0);
	//UE_LOG(LogNet, Display, TEXT("Send Packet SIZE %d"), nSendLen);
	return true;
}
