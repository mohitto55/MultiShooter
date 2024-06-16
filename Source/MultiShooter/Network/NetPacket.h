// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

enum PacketID
{
	S_GETSCORE = 0,
	C_GETSCORE = 1,
	S_AttackPlayer = 2,
	C_AttackPlayer = 3,
	S_MovePlayer = 4,
	C_MovePlayer = 5
};
class NetPacket
{
public:
	uint16 _PacketSize;
	uint16 _PlayerID;
	uint16 _BuffOffset;
	uint16 _BuffCount;
	NetPacket(uint16 PlayerID);
protected:
public:
	char* GenerateSendPacket(char* buf);
protected:
	char* GenerateHeader(char* buf);
	virtual char* GenerateTail(char* buf) = 0;
	virtual PacketID GetPacketID() = 0;
	int SetPacketSize(char* bufStart);

	
	void PutUint16IntoBuffer(char*& buf, uint16 value);
	void PutUint32IntoBuffer(char*& buf, uint32 value);
	void PutInt32IntoBuffer(char*& buf, int32 value);
	void PutFloatIntoBuffer(char*& buf, float value);
	void PutVectorIntoBuffer(char*& buf, FVector value);
};

class GetScorePacket : public NetPacket
{
public:
	int32 score;
	GetScorePacket(uint16 PlayerID, int32 Score);
private:
	virtual char* GenerateTail(char* buf) override;
	virtual PacketID GetPacketID() override;
};
