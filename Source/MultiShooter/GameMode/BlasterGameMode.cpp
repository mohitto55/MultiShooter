// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterGameMode.h"

#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSubsystem.h"
#include "Character/BlasterCharacter.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Net/Subsystems/NetworkSubsystem.h"
#include "Player/BlasterPlayerController.h"
#include "Subsystem/MyNetworkSubsystem.h"
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

void ABlasterGameMode::BeginPlay()
{
	Super::BeginPlay();
	UMultiplayerSessionsSubsystem* sub = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UMultiplayerSessionsSubsystem>();
	if(sub)
	{
		sub->MultiplayerOnDestroySessionComplete.AddDynamic(this, &ABlasterGameMode::OnDestroySession);
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
				//NewPlayer->PlayerState = GameState->PlayerArray[GameState->PlayerArray.Num()-1];

				// if (PlayerState)
				// {
				// 	FUniqueNetIdRepl UniqueIdRepl = PlayerState->GetUniqueId();
				// 	if (UniqueIdRepl.IsValid())
				// 	{
				// 		TSharedPtr<const FUniqueNetId> UniqueId = UniqueIdRepl.GetUniqueNetId();
				// 		if (UniqueId.IsValid())
				// 		{
				// 			FString UniqueIdStr = UniqueId->ToString();
				// 			GEngine->AddOnScreenDebugMessage(-1,10,FColor::Red, UniqueIdStr);
				// 			UE_LOG(LogTemp, Log, TEXT("Player Unique ID: %s"), *UniqueIdStr);
				// 		}
				// 	}
				// }
				

				
				APlayerState* PlayerState = NewPlayer->GetPlayerState<APlayerState>();
				if(PlayerState)
				{
					FString PlayerName = PlayerState->GetPlayerName();
					GEngine->AddOnScreenDebugMessage(-1,
						50,
						FColor::Cyan,
						FString::Printf(TEXT("%s has joind Game!"),  *PlayerName));
					
				}
			}
		}
		UMyNetworkSubsystem* Sub = 	UGameplayStatics::GetGameInstance(this)->GetSubsystem<UMyNetworkSubsystem>();
		if(Sub)
		{
			IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
			if(APlayerState* State = NewPlayer->GetPlayerState<APlayerState>())
			{
				
			}
			IOnlineIdentityPtr IdentiyPtr = OnlineSub->GetIdentityInterface();
			ABlasterPlayerController* BlasterController = Cast<ABlasterPlayerController>(NewPlayer);
			
			// BlasterController->PlayerNum = GameState->PlayerArray.Num()-1;
			// FUniqueNetIdPtr UniqueID = OnlineSub->GetIdentityInterface()->GetUniquePlayerId(BlasterController->PlayerNum);
			// FString LocalUserNum = FString::Format(TEXT("Local User Num : {0}"), {BlasterController->PlayerNum});
			// FString a = FString::Format(TEXT("Online Unique ID : {0}"),{UniqueID.Get()->ToString()});
			// // FString FInternalID = UniqueID->ToString();
			// GEngine->AddOnScreenDebugMessage(-1,50, FColor::White, LocalUserNum);
			// GEngine->AddOnScreenDebugMessage(-1,50, FColor::White, a);
			//GEngine->AddOnScreenDebugMessage(-1,50, FColor::White, FInternalID);
		
		
			// GetScorePacket* Packet = new GetScorePacket(0, NewController->GetUniqueID(), 100);
			// Sub->SendPacket(Packet);
			// delete Packet;
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
	if(!HasAuthority() && LiveActors <= 0) return;
	
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

		IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
		if (OnlineSubsystem)
		{
			IOnlineSessionPtr Sessions = OnlineSubsystem->GetSessionInterface();
			if (Sessions.IsValid())
			{
				FName SessionName = NAME_GameSession; // 기본 세션 이름, 필요에 따라 수정 가능
				Sessions->EndSession(SessionName);
			}
		}
	}
}

void ABlasterGameMode::GameEnd()
{
	GEngine->AddOnScreenDebugMessage(-1,3,FColor::Orange, TEXT("GameEnd"));

	UMultiplayerSessionsSubsystem* sub = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UMultiplayerSessionsSubsystem>();
	if(sub)
	{
		sub->DestroySession();
	}
}

void ABlasterGameMode::OnDestroySession(bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1,3,FColor::Orange, TEXT("O11"));

	if(bWasSuccessful)
	{
		GEngine->AddOnScreenDebugMessage(-1,3,FColor::Orange, TEXT("OnDestroySession"));
		ReturnToMainMenuHost();
	}
}

// void ABlasterGameMode::GoStartMap(bool bWasSuccessful)
// {
// 	if(HasAuthority() && bWasSuccessful)
// 	{
// 		UWorld* World = GetWorld();
// 		if(World)
// 		{
// 			bUseSeamlessTravel = true;
// 			World->ServerTravel(FString("/Game/Maps/GameStartupMap?listen"));
// 		}
// 	}
// }

