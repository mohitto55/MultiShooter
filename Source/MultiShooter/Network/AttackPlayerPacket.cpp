// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/AttackPlayerPacket.h"


AttackPlayerPacket::AttackPlayerPacket(uint16 PlayerID, uint16 DamagedID, uint16 DamageAmount, uint16 Dist) : NetPacket(PlayerID)
{
	this->DamagedID = DamagedID;
	this->DamageAmount = DamageAmount;
	this->Dist = Dist;
}

char* AttackPlayerPacket::GenerateTail(char* buf)
{
	PutUint16IntoBuffer(buf, DamagedID);
	PutUint16IntoBuffer(buf, DamageAmount);
	PutUint16IntoBuffer(buf, Dist);
	return buf;
}

PacketID AttackPlayerPacket::GetPacketID()
{
	return PacketID::C_AttackPlayer;
}
