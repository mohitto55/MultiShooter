// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetPacket.h"

class MovePlayer : public NetPacket
{
public:
	uint16 Speed;
	FVector Position;
	MovePlayer(uint16 PlayerID, FVector Position, uint16 Speed);

private:
	virtual char* GenerateTail(char* buf) override;
	virtual PacketID GetPacketID() override;
};
