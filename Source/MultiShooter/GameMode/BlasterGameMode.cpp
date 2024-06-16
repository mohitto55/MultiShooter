// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterGameMode.h"

#include "MultiplayerSessionsSubsystem.h"
#include "Character/BlasterCharacter.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/BlasterPlayerController.h"
#include "Tests/AutomationCommon.h"

/**
 * 
 */
ABlasterGameMode::ABlasterGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PawnClassRef(
		TEXT("/Game/Blueprints/Character/BP_BlasterCharacter.BP_BlasterCharacter_C"));
	if (PawnClassRef.Class)
	{
		DefaultPawnClass = PawnClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerRef(
		TEXT("/Script/CoreUObject.Class'/Script/MultiShooter.BlasterPlayerController'"));
	if (PlayerControllerRef.Class)
	{
		PlayerControllerClass = PlayerControllerRef.Class;
	}
}

void ABlasterGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
                                FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
	LiveActors++;
}

void ABlasterGameMode::PostLogin(APlayerController* NewPlayer)
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

void ABlasterGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}

void ABlasterGameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ABlasterGameMode::ActorDie_Implementation(AActor* actor)
{
	if(!HasAuthority()) return;
	
	LiveActors--;
	FString Txt = FString::Format(TEXT("Last Person : {0}"), {LiveActors} );
	GEngine->AddOnScreenDebugMessage(-1,3,FColor::Orange, Txt);
	ABlasterCharacter* Blaster = Cast<ABlasterCharacter>(actor);
	Cast<ABlasterPlayerController>(Blaster->GetController())->ServerSetPlayerDie();
	if(LiveActors <= 0)
	{
		FTimerHandle handle = GetWorld()->GetTimerManager().GenerateHandle(3);
		GetWorldTimerManager().SetTimer(handle, this, &ABlasterGameMode::GameEnd,5,false);

		// 0명 이하이면 게임 종료 알림
		for (FConstPlayerControllerIterator Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
		{
			ABlasterPlayerController* BController = Cast<ABlasterPlayerController>(*Iter);
			if(BController)
			{
				BController->ServerSetGameEnd();
				GEngine->AddOnScreenDebugMessage(-1,3,FColor::Orange,
					FString::Format(TEXT("Index : {0}, All P : {1}"), { BController->GetName(), GetWorld()->GetNumPlayerControllers()}));
			}
		}
	}
}


void ABlasterGameMode::GameEnd()
{
	GEngine->AddOnScreenDebugMessage(-1,3,FColor::Orange, TEXT("GameEnd"));
	
	if(HasAuthority())
	{
		UMultiplayerSessionsSubsystem* sub = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UMultiplayerSessionsSubsystem>();
		if(sub)
		{
			sub->DestroySession();
		}
		else
		{
			UWorld* World = GetWorld();
			FURL URL(TEXT("/Game/Maps/GameStartupMap?listen"));
			FString Error;
			FWorldContext context;
			GEngine->Browse(context, URL, Error);
		}
		// UWorld* World = GetWorld();
		// if(World)
		// {
		// 	bUseSeamlessTravel = true;
		// 	World->ServerTravel(FString("/Game/Maps/GameStartupMap?listen"));
		// }
	}
}

