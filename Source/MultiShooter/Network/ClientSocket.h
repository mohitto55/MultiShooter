// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <concurrent_queue.h>
// winsocket ����� ���� �ڸ�Ʈ
// �ּ� ����� ��ü ���� �Ǵ� ���� ���Ͽ� ��ġ�մϴ�.
// ��������� ���� �����ϴ� ������Ʈ�� ��� �̿� ���� �����
// ����Ͽ� lib�� ��ũ�ϴ� ���� ���̺귯���� ��ũ�Ǿ��ִٴ� ����� �˱⿡��
// ���� ���� �ּ��ٶ� ������ �ʿ䵵 ���� ���� �ʳ� �ʹ�. 

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
