// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

#include "GameMapsSettings.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	if(GameState)
	{
		int32 NumberOfPlayers = GameState->PlayerArray.Num();
		if(NumberOfPlayers == 2)
		{
			FTimerHandle Timer;
			GetWorld()->GetTimerManager().SetTimer(Timer,this, &ThisClass::Travel, 3, false);
		}
		GEngine->AddOnScreenDebugMessage(-1, 3,
			FColor::Emerald, FString::Printf(TEXT("Player Num : %d"), UGameplayStatics::GetNumPlayerControllers(GetWorld())));
	}
}

void ALobbyGameMode::Travel()
{
	UWorld* World = GetWorld();
	
	if(World)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10,
		FColor::Emerald, TEXT("TRAVEL"));
		World->ServerTravel(FString("/Game/Maps/BlasterMap?listen"));
	}
}
