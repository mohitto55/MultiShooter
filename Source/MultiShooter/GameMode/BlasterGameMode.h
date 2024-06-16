// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BlasterGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MULTISHOOTER_API ABlasterGameMode : public AGameMode
{
	GENERATED_BODY()
	DECLARE_MULTICAST_DELEGATE(GAME_END)
public:
	ABlasterGameMode();
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	// virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	// virtual void PostLogin(APlayerController* NewPlayer) override;
	// virtual void StartPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UFUNCTION(Server, Reliable)
	void ActorDie(AActor* actor);
	void GameEnd();
	GAME_END _GameEnd;
private:
	int LiveActors;
};
