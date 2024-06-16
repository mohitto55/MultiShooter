// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetPacket.h"

class AttackPlayerPacket : public NetPacket
{
public:
	uint16 DamagedID;
	uint16 DamageAmount;
	uint16 Dist;
	AttackPlayerPacket(uint16 PlayerID, uint16 DamagedID, uint16 DamageAmount, uint16 Dist);

private:
	virtual char* GenerateTail(char* buf) override;
	virtual PacketID GetPacketID() override;
};
