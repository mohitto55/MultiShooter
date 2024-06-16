// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/TestBlasterGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

void ATestBlasterGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	if(GameState)
	{
		if(HasAuthority())
		{
			int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
			if(GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1,
						10,
						FColor::Cyan,
						FString::Printf(TEXT("Players in Game : %d"), NumberOfPlayers)
						);
				APlayerState* PlayerState = NewPlayer->GetPlayerState<APlayerState>();
				if(PlayerState)
				{
					FString PlayerName = PlayerState->GetPlayerName();
					GEngine->AddOnScreenDebugMessage(-1,
						50,
						FColor::Cyan,
						FString::Printf(TEXT("%s has joind Game!"), *PlayerName));
				}
			}
		}
	}
}

void ATestBlasterGameMode::BeginPlay()
{
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(-1,
						10,
						FColor::Cyan,
						FString::Printf(TEXT("Play!!"))
						);
}
