// Fill out your copyright notice in the Description page of Project Settings.

#include "Network/MovePlayerPacket.h"


#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineIdentityInterface.h"
MovePlayer::MovePlayer(uint16 PlayerID, FVector Position, uint16 Speed) : NetPacket(PlayerID)
{
	this->Position = Position;
	this->Speed = Speed;
}

char* MovePlayer::GenerateTail(char* buf)
{
	PutVectorIntoBuffer(buf, Position);
	PutUint16IntoBuffer(buf, Speed);
	return buf;
}

PacketID MovePlayer::GetPacketID()
{
	return PacketID::C_MovePlayer;
}
