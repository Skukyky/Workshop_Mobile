// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <string>

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "TutorialWidget.generated.h"


class AGachaCharacterShowcase;
class UBTNCustomWidget;
/**
 * 
 */
UCLASS()
class WORKSHOP_MOBILE_API UTutorialWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Blueprintable)
	UAnimationAsset* AnimationAsset = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<AGachaCharacterShowcase> CharacterShowcaseActorClass;
	
	UPROPERTY()
	AGachaCharacterShowcase* CharacterShowcaseActor = nullptr;

	UPROPERTY(EditAnywhere, Blueprintable)
	USkeletalMesh* SkeletalMeshActor = nullptr;

	UPROPERTY(meta = (BindWidget))
	UBTNCustomWidget* BTN_Click;

	UPROPERTY(meta = (BindWidget))
	UImage* GeraudImage;

	UPROPERTY(meta = (BindWidget))
	UImage* ImageTuto;

	UPROPERTY(meta = (BindWidget))
	UImage* ImageArrow;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_Tuto;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TutorialNumber = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> TutorialTexts;

	UFUNCTION()
	void HandleClicked();

	virtual void NativeConstruct() override;
	
	
};
