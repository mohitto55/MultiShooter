// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BlasterPlayerController.h"

#include "GameMapsSettings.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GameFramework/GameModeBase.h"
#include "HUD/BlasterHUD.h"
#include "HUD/CharacterOverlay.h"
#include "Net/UnrealNetwork.h"

void ABlasterPlayerController::SetHUDHealth(float Health, float MaxHealth)
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHudValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->HealthBar &&
		BlasterHUD->CharacterOverlay->HealthText;
	if(bHudValid)
	{
		const float HealthPercent = Health / MaxHealth;
		BlasterHUD->CharacterOverlay->HealthBar->SetPercent(HealthPercent);
		FString HealthText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));
		BlasterHUD->CharacterOverlay->HealthText->SetText(FText::FromString(HealthText));

		if(Health == 0)
		{
			SetCenterText(FText::FromString("You Died"));
		}
		else
		{
			SetCenterText(FText::FromString(""));
		}
	}

}

void ABlasterPlayerController::SetCenterText(FText text)
{
	if(BlasterHUD)
	{
		if(BlasterHUD->CharacterOverlay != nullptr)
			BlasterHUD->CharacterOverlay->DieText->SetText(text);
	}
}


void ABlasterPlayerController::ServerSetPlayerDie_Implementation()
{
	IsDie = 1;
	OnRep_IsDie();
}

void ABlasterPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	bReplicates = true;
}

void ABlasterPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABlasterPlayerController, IsDie);
	DOREPLIFETIME(ABlasterPlayerController, IsGameEnd);
}

void ABlasterPlayerController::PostNetInit()
{
	Super::PostNetInit();
}

void ABlasterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	BlasterHUD = Cast<ABlasterHUD>(GetHUD());
	
	// UMyNetworkSubsystem* NetworkSub = GetGameInstance()->GetSubsystem<UMyNetworkSubsystem>();
	//
	// if(NetworkSub->ConnectNetwork)
	// {
	// 	GetScorePacket* packet = new GetScorePacket(1,10);
	// 	NetworkSub->SendPacket(packet);
	// 	delete packet;
	// }
}

void ABlasterPlayerController::OnProssess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}


void ABlasterPlayerController::OnRep_IsDie()
{
	if(IsDie == 1)
	SetCenterText(FText::FromString("You Died!"));
}

void ABlasterPlayerController::OnRep_IsGameEnd()
{
	FString a = FString::Format(TEXT("sad : {0}"), {GetName()});
	GEngine->AddOnScreenDebugMessage(-1,13,FColor::Orange,a);
	
	if(IsDie == 0 && IsGameEnd == 1)
	{
		SetCenterText(FText::FromString("You Win!"));
	}

	
}

void ABlasterPlayerController::ServerSetGameEnd_Implementation()
{
	IsGameEnd=1;
	OnRep_IsGameEnd();
}
