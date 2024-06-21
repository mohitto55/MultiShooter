// Fill out your copyright notice in the Description page of Project Settings.


#include "NameWidgetComponent.h"

UNameWidgetComponent::UNameWidgetComponent()
{
	
}

void UNameWidgetComponent::InitWidget()
{
	Super::InitWidget();
	NameText = Cast<UTextBlock>(GetWidget()->GetWidgetFromName("TextBlock"));
}

void UNameWidgetComponent::SetDisplayText(FText Text)
{
	if(NameText)
	{
		NameText->SetText(Text);
	}
}


