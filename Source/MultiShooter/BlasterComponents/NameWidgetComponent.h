// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "NameWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class MULTISHOOTER_API UNameWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	UNameWidgetComponent();
private:
	UPROPERTY(VisibleAnywhere)
	UTextBlock* NameText;
public:
	virtual void InitWidget() override;

	virtual void SetDisplayText(FText Text);
};
