// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <boost/system/system_error.hpp>
#include <map>
THIRD_PARTY_INCLUDES_START
#include <boost/asio.hpp>
#include <boost/asio/connect.hpp>
THIRD_PARTY_INCLUDES_END
#include "Network/NetPacket.h"
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MyNetworkSubsystem.generated.h"

class NetPacket;
/**
 * 
 */
UCLASS()
class MULTISHOOTER_API UMyNetworkSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UMyNetworkSubsystem();
	~UMyNetworkSubsystem();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Network, DisplayName = "서버 연결")
	bool ConnectNetwork;
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
public:
	std::string BufferToString(const char* buf, size_t length);
	void SendPacket(NetPacket* packet);
private:
	
	class boost::asio::thread_pool* pool;
private:
	void StartConnect();
	void ReceivePacket(char* buf, uint16 size);
	void ServerDisConnect();
	void S_AttackPlayerHandler(char* buf, uint16 size);
	class TCPClient* client;
	TQueue<NetPacket*> PacketBuf;
	/**
	 * 
	 */
	std::map<PacketID, std::function<void(char*, uint16)>> handlers;
};
