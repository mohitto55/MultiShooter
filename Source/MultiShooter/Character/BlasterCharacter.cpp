// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BlasterCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "OnlineSubsystem.h"
#include "BlasterComponents/CombatComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/Weapon.h"
#include "GameMode/BlasterGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Player/BlasterPlayerController.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineIdentityInterface.h"
// Sets default values
ABlasterCharacter::ABlasterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> ShoulderMappingRef(TEXT("/Game/Input/IMC_ShoulderPlayer.IMC_ShoulderPlayer"));
	if(ShoulderMappingRef.Object)
	{
		ShoulderContext = ShoulderMappingRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> MoveInputRef(TEXT("/Game/Input/IA_ShoulderMove.IA_ShoulderMove"));
	if(MoveInputRef.Object)
	{
		ShoulderMoveAction = MoveInputRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> LookRef(TEXT("/Game/Input/IA_Look.IA_Look"));
	if(LookRef.Object)
	{
		ShoulderLookAction = LookRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> jumpRef(TEXT("/Game/Input/IA_Jump.IA_Jump"));
	if(jumpRef.Object)
	{
		JumpAction = jumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> EquipRef(TEXT("/Game/Input/IA_Equip.IA_Equip"));
	if(EquipRef.Object)
	{
		EquipAction = EquipRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> CrouchRef(TEXT("/Game/Input/IA_Crouch.IA_Crouch"));
	if(CrouchRef.Object)
	{
		CrouchAction = CrouchRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> AimRef(TEXT("/Game/Input/IA_Aim.IA_Aim"));
	if(AimRef.Object)
	{
		AimAction = AimRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> FireRef(TEXT("/Game/Input/IA_Fire.IA_Fire"));
	if(FireRef.Object)
	{
		FireAction = FireRef.Object;
	}
	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	Combat->SetIsReplicated(true);
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

void ABlasterCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME_CONDITION(ABlasterCharacter, OverlappingWeapon, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ABlasterCharacter, OverlappingWeapon, COND_OwnerOnly);
	DOREPLIFETIME(ABlasterCharacter, Health);
}

void ABlasterCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if(Combat)
	{
		Combat->SetIsReplicated(true);
		Combat->Character = this;
	}
}

void ABlasterCharacter::PlayFireMontage(bool bAiming)
{
	if(Combat == nullptr || Combat->EquippedWeapon == nullptr) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && FireWeaponMontage)
	{
		AnimInstance->Montage_Play((FireWeaponMontage));
		FName SectionName;
		SectionName = bAiming ? FName("RifleAim") : FName("RifleHip");
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

void ABlasterCharacter::BeginPlay()
{
	Super::BeginPlay();
	if(IsLocallyControlled())
	{
		//GetStaemUserID();
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// OnlineSubsystemSteam을 가져옵니다.
			IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
			if (OnlineSubsystem)
			{
				IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface();
				if (Identity.IsValid())
				{
					// LocalUserNum은 일반적으로 0으로 설정됩니다.
					TSharedPtr<const FUniqueNetId> UserId = GetFirstSignedInUser(0);
					if (UserId.IsValid())
					{
						// Steam ID를 FString으로 변환합니다.
						// FString SteamUserId = UserId->ToString();
						// UE_LOG(LogTemp, Log, TEXT("Steam User ID: %s"), *SteamUserId);
			
					}
				}
				IOnlineUserPtr UserInterface = OnlineSubsystem->GetUserInterface();
				// FString a2 = GetPlayerState()->GetPlayerName();
				// UE_LOG(LogTemp, Log, TEXT("Steam User ID: %s"), *a2);
			}
			
			Subsystem->ClearAllMappings();
			UInputMappingContext* NewMappingContext = ShoulderContext;
			if(NewMappingContext)
			{
				Subsystem->AddMappingContext(NewMappingContext,0);	
			}
		}
	}
	UpdateHUDHealth();
	GetStaemUserID();
	if(HasAuthority())
	{
		OnTakeAnyDamage.AddDynamic(this, &ABlasterCharacter::ReceiveDamage);
	}
}

// Called every frame
void ABlasterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimOffset(DeltaTime);
}

void ABlasterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if(!IsLocallyControlled())
	{
		return;
	}
	
	if(UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &ABlasterCharacter::ShoulderMove);
		EnhancedInput->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &ABlasterCharacter::Look);
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInput->BindAction(EquipAction, ETriggerEvent::Started, this, &ABlasterCharacter::Equip);
		EnhancedInput->BindAction(CrouchAction, ETriggerEvent::Started, this, &ABlasterCharacter::OnCrouch);
		EnhancedInput->BindAction(AimAction, ETriggerEvent::Started, this, &ABlasterCharacter::OnAim);
		EnhancedInput->BindAction(AimAction, ETriggerEvent::Completed, this, &ABlasterCharacter::AimReleased);
		EnhancedInput->BindAction(FireAction, ETriggerEvent::Started, this, &ABlasterCharacter::FireButtonPressed);
		EnhancedInput->BindAction(FireAction, ETriggerEvent::Completed, this, &ABlasterCharacter::FireButtonReleased);
		// EnhancedInput->BindAction(AimAction, ETriggerEvent::Completed, this, &AMPlayerCharacter::Aiming);
		// EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		// EnhancedInput->BindAction(AttackAction, ETriggerEvent::Started, this, &AMPlayerCharacter::Attack);
	}
}

void ABlasterCharacter::ShoulderMove(const FInputActionValue& Value)
{
	const FVector2d Movement = Value.Get<FVector2d>();
	
	const FRotator ControllerRotator = Controller->GetControlRotation();
	const FRotator YawRotation(0,ControllerRotator.Yaw,0);
	
	const FVector ForwardDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	AddMovementInput(ForwardDir, Movement.X * AimMoveSpeed);
	AddMovementInput(RightDir, Movement.Y * AimMoveSpeed);
}

void ABlasterCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		FIntPoint ScreenResolution = FIntPoint(GEngine->GameViewport->Viewport->GetSizeXY());
		float ScreenWidth = static_cast<float>(ScreenResolution.X);
		float ScreenHeight = static_cast<float>(ScreenResolution.Y);
		float heightResolution = ScreenHeight / ScreenWidth;
	
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X * MouseSensitivity);
		AddControllerPitchInput(-LookAxisVector.Y * MouseSensitivity * heightResolution); 
	}
}

void ABlasterCharacter::Equip()
{
	// 서버만 로직처리
	if(Combat)
	{
		if(HasAuthority())
		{
			Combat->EquipWeapon(OverlappingWeapon);
		}
		else
		{
			ServerEquipButtonPressed();
		}
	}
}

void ABlasterCharacter::OnCrouch()
{
	if(bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}

void ABlasterCharacter::OnAim()
{
	if(Combat)
	{
		Combat->SetAiming(true);
	}
}

void ABlasterCharacter::AimReleased()
{
	if(Combat)
	{
		Combat->SetAiming(false);
	}
}

void ABlasterCharacter::AimOffset(float DeltaTime)
{
	if (Combat && Combat->EquippedWeapon == nullptr) return;
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f;
	float Speed = Velocity.Size();
	bool bIsInAir = GetCharacterMovement()->IsFalling();

	if (Speed == 0.f && !bIsInAir) // standing still, not jumping
	{
		FRotator CurrentAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation, StartingAimRotation);
		AO_Yaw = DeltaAimRotation.Yaw;
		bUseControllerRotationYaw = false;
	}
	if (Speed > 0.f || bIsInAir) // running, or jumping
	{
		StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		AO_Yaw = 0.f;
		bUseControllerRotationYaw = true;
	}

	AO_Pitch = GetBaseAimRotation().Pitch;
	if(AO_Pitch > 90.f && !IsLocallyControlled())
	{
		FVector2D InRange(270.f, 360.f);
		FVector2D OutRange(-90.f, 0.f);
		AO_Pitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AO_Pitch);
	}
}

void ABlasterCharacter::FireButtonPressed()
{
	if(Combat)
	{
		Combat->FireButtonPressed(true);
	}
}

void ABlasterCharacter::FireButtonReleased()
{
	if(Combat)
	{
		Combat->FireButtonPressed(false);
	}
}

void ABlasterCharacter::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* EventInstigator, AActor* DamageCauser)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
	UpdateHUDHealth();
	PlayHitReactMontage();
	if(Health <= 0 && BlasterPlayerController->IsDie == 0)
	{
		ABlasterGameMode* BGameMode = Cast<ABlasterGameMode>(UGameplayStatics::GetGameMode(this));
		if(BGameMode)
		{
			BGameMode->ActorDie(this);
		}
	}
}

void ABlasterCharacter::UpdateHUDHealth()
{
	BlasterPlayerController = BlasterPlayerController == nullptr ? Cast<ABlasterPlayerController>(Controller) : BlasterPlayerController;
	if(BlasterPlayerController)
	{
		BlasterPlayerController->SetHUDHealth(Health,MaxHealth);
	}
}

float ABlasterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ABlasterCharacter::PlayHitReactMontage()
{
}

void ABlasterCharacter::OnRep_OverlappingWeapon(AWeapon* LastWeapon)
{
	if(OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(true);
	}
	if(LastWeapon)
	{
		LastWeapon->ShowPickupWidget(false);
	}
}

void ABlasterCharacter::ServerEquipButtonPressed_Implementation()
{
	if(Combat)
	{
		Combat->EquipWeapon(OverlappingWeapon);
	}
}

void ABlasterCharacter::OnRep_Health()
{
	PlayHitReactMontage();
	UpdateHUDHealth();
}


void ABlasterCharacter::GetStaemUserID()
{
	// Get the OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		// Get the identity interface for Steam
		IOnlineIdentityPtr IdentityInterface = OnlineSubsystem->GetIdentityInterface();
		if (IdentityInterface.IsValid())
		{
			TSharedPtr<const FUniqueNetId> id = GetFirstSignedInUser(0);
			int ShowTime = 10;
			// const FString stateUnique = GetPlayerState()->GetUniqueId().ToString();
			// GEngine->AddOnScreenDebugMessage(-1,ShowTime, FColor::Magenta,
			// FString::Printf(TEXT("State Unique Id : %s"), *stateUnique));
			if(GetPlayerState())
			{
				GEngine->AddOnScreenDebugMessage(-1,ShowTime, FColor::Magenta,
				FString::Printf(TEXT("State Player ID : %d"), GetPlayerState()->GetPlayerId()));
				GEngine->AddOnScreenDebugMessage(-1,ShowTime, FColor::Magenta,
				FString::Printf(TEXT("State Player Name : %s"), *GetPlayerState()->GetPlayerName()));
			}
// 			FString NickName = IdentityInterface->GetPlayerNickname(0);
// 			GEngine->AddOnScreenDebugMessage(-1,ShowTime, FColor::Magenta,
// FString::Printf(TEXT("Nick Name : %s"),*NickName));
			if(id.IsValid())
			{
				FString DisplayName = IdentityInterface->GetUserAccount(*id.Get())->GetDisplayName();
				FString RealName = IdentityInterface->GetUserAccount(*id.Get())->GetRealName();
	
				GEngine->AddOnScreenDebugMessage(-1,ShowTime, FColor::Magenta,
					FString::Printf(TEXT("Display Name : %s"),*DisplayName));
				GEngine->AddOnScreenDebugMessage(-1,ShowTime, FColor::Magenta,
	FString::Printf(TEXT("RealName Name : %s"),*RealName));
			}
		}
	}
}

void ABlasterCharacter::SetOverlappingWeapon(AWeapon* Weapon)
{
	if(OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(false);
	}
	
	OverlappingWeapon = Weapon;
	if(IsLocallyControlled())
	{
		if(OverlappingWeapon)
		{
			OverlappingWeapon->ShowPickupWidget(true);
		}
	}
}

bool ABlasterCharacter::IsWeaponEquipped()
{
	return (Combat && Combat->EquippedWeapon);
}

bool ABlasterCharacter::IsAiming()
{
	return (Combat && Combat->bAiming);
}

AController* ABlasterCharacter::GetBlasterController()
{
	return GetController(); 
}
