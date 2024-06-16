// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/NetPacket.h"

NetPacket::NetPacket(uint16 PlayerID)
{
	_PlayerID = PlayerID;
}

char* NetPacket::GenerateSendPacket(char* buf)
{
	_PacketSize = 0;
	char* bufStart = buf;
	buf = GenerateHeader(buf);
	buf = GenerateTail(buf);
	_PacketSize = SetPacketSize(bufStart);
	return buf;
}

char* NetPacket::GenerateHeader(char* buf)
{
	// 첫 패킷 사이즈는 0이 들어간다.
	PutUint16IntoBuffer(buf, _PacketSize);
	PutInt32IntoBuffer(buf, GetPacketID());
	PutUint16IntoBuffer(buf, _PlayerID);
	FString message = FString::Printf(TEXT("Packet Size: %d"), _PacketSize);
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, message);
	return buf;
}

int NetPacket::SetPacketSize(char* bufStart)
{
	std::memcpy(bufStart, &_PacketSize, sizeof(_PacketSize));
	return _PacketSize;
}

void NetPacket:: PutUint16IntoBuffer(char*& buf, uint16 value)
{
	std::memcpy(buf, &value, sizeof(uint16));
	buf += sizeof(uint16);
	_PacketSize += sizeof(uint16);
}

void NetPacket::PutUint32IntoBuffer(char*& buf, uint32 value)
{
	std::memcpy(buf, &value, sizeof(uint32));
	buf += sizeof(uint32);
	_PacketSize += sizeof(uint32);
}

void NetPacket::PutInt32IntoBuffer(char*& buf, int32 value)
{
	std::memcpy(buf, &value, sizeof(int32));
	buf += sizeof(int32);
	_PacketSize += sizeof(int32);
}

void NetPacket::PutFloatIntoBuffer(char*& buf, float value)
{
	std::memcpy(buf, &value, sizeof(float));
	buf += sizeof(float);
	_PacketSize += sizeof(float);
}

void NetPacket::PutVectorIntoBuffer(char*& buf, FVector value)
{
	PutFloatIntoBuffer(buf, value.X);
	PutFloatIntoBuffer(buf, value.Y);
	PutFloatIntoBuffer(buf, value.Z);
}

GetScorePacket::GetScorePacket(uint16 PlayerID, int32 Score) : NetPacket(PlayerID)
{
	score = Score;
}

char* GetScorePacket::GenerateTail(char* buf)
{
	PutUint32IntoBuffer(buf, score);
	return buf;
}

PacketID GetScorePacket::GetPacketID()
{
	return PacketID::C_GETSCORE;
}
