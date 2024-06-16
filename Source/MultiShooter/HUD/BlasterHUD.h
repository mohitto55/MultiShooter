// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BlasterHUD.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FHUDPackage
{
	GENERATED_BODY()
public:
	class UTexture2D* CrosshairsCenter;
	UTexture2D* CrosshairsLeft;
	UTexture2D* CrosshairsRight;
	UTexture2D* CrosshairsTop;
	UTexture2D* CrosshairsBottom;
};

UCLASS()
class MULTISHOOTER_API ABlasterHUD : public AHUD
{
	GENERATED_BODY()
public:
	virtual void DrawHUD() override;

	UPROPERTY(EditAnywhere, Category = "player Stats")
	TSubclassOf<class UUserWidget> CharacterOverlayClass;
	
	class UCharacterOverlay* CharacterOverlay;
private:
	FHUDPackage HUDPackage;
public:
	FORCEINLINE void SetHUDPackage(const FHUDPackage& Package) { HUDPackage = Package;}
	void DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter);
protected:
	virtual void BeginPlay() override;
	void AddCharacterOverlay();
};