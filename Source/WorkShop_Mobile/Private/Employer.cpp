// Fill out your copyright notice in the Description page of Project Settings.


#include "Employer.h"

void UEmployer::NativeConstruct()
{
	Super::NativeConstruct();
	if (Close)
	{
		Close->OnReleased.AddDynamic(this, &UEmployer::ClickClose);
	}
}

void UEmployer::ClickClose()
{
	RemoveFromParent();
}