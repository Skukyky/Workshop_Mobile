// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialWidget.h"
#include "BTNCustomWidget.h"
#include "GachaCharacterShowcase.h"

void UTutorialWidget::HandleClicked()
{
	if (TutorialNumber < TutorialTexts.Num())
	{
		if (TEXT_Tuto)
		{
			TEXT_Tuto->SetText(FText::FromString(TutorialTexts[TutorialNumber]));
		}
		if (TutorialNumber == 3)
		{
			ImageArrow->SetVisibility(ESlateVisibility::Visible);
			ImageTuto->SetVisibility(ESlateVisibility::Visible);
		}else
		{
			ImageArrow->SetVisibility(ESlateVisibility::Collapsed);
			ImageTuto->SetVisibility(ESlateVisibility::Collapsed);
		}
		TutorialNumber++;
	}
	else
	{
		RemoveFromParent();
	}
}

void UTutorialWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UWorld* World = GetWorld();
	if (World && CharacterShowcaseActorClass)
	{
		FVector SpawnLocation(0.f, 0.f, 100000.f);
		FRotator SpawnRotation(0.f, 0.f, 0.f);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		CharacterShowcaseActor = World->SpawnActor<AGachaCharacterShowcase>(CharacterShowcaseActorClass, SpawnLocation, SpawnRotation, SpawnParams);
	}
	if (CharacterShowcaseActor)
	{
		if (CharacterShowcaseActor->CharacterMesh)
		{
			CharacterShowcaseActor->CharacterMesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
			if (AnimationAsset)
			{
				CharacterShowcaseActor->CharacterMesh->SetAnimation(AnimationAsset);
				CharacterShowcaseActor->CharacterMesh->Play(true); // true pour boucle
				UE_LOG(LogTemp, Log, TEXT("Animation boucle lancée sur CharacterMesh."));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Animation asset non trouvée."));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("CharacterMesh est nullptr."));
		}
	}

	if (BTN_Click)
	{
		BTN_Click->OnCustomButtonClicked.AddDynamic(this, &UTutorialWidget::HandleClicked);
	}
}