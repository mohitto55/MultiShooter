// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <concurrent_queue.h>
// winsocket 사용을 위한 코맨트
// 주석 기록을 개체 파일 또는 실행 파일에 배치합니다.
// 여러사람이 같이 수행하는 프로젝트의 경우 이와 같은 방법을
// 사용하여 lib를 링크하는 것이 라이브러리가 링크되어있다는 사실을 알기에도
// 좋고 굳이 주석다라 설명할 필요도 없어 좋지 않나 싶다. 

#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#define BUFSIZE 4096
#define PORT 12000
#define SERVER_IP "127.0.0.1"


class MULTISHOOTER_API ClientSocket : public FRunnable
{
public:
	ClientSocket();
	virtual ~ClientSocket() override;

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;

	SOCKET Socket;

	bool ConnectServer();

	bool Send(const int SendSize, void* SendData);

	inline bool IsConnected() {return _IsConnected;};
private:
	FRunnableThread* Thread;
	bool _IsConnected;
public:
	Concurrency::concurrent_queue<char> buffer;
};
