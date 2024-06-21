// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BlasterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MULTISHOOTER_API ABlasterPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	void SetHUDHealth(float Health,float MaxHealth);
	void SetCenterText(FText text);
	UPROPERTY(ReplicatedUsing=OnRep_IsGameEnd)
	int IsGameEnd = 0;
	UPROPERTY(ReplicatedUsing=OnRep_IsDie)
	int IsDie = 0;

	UFUNCTION(Server, Reliable)
	void ServerSetPlayerDie();
	UFUNCTION(Server, Reliable)
	void ServerSetGameEnd();

	int32 PlayerNum = -1;
protected:
	virtual void PostInitializeComponents() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostNetInit() override;
	virtual void BeginPlay() override;
	virtual void OnProssess(APawn* InPawn);
	
	UFUNCTION()
	void OnRep_IsDie();
	
	UFUNCTION()
	void OnRep_IsGameEnd();
private:
	class ABlasterHUD* BlasterHUD;
};
