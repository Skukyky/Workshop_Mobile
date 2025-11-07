// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "WorkRoomSettingWidget.generated.h"

class ARoomWorking;
class AWorker;
class UBTNCustomWidget;
class APlayerActor;
/**
 * 
 */
UCLASS()
class WORKSHOP_MOBILE_API UWorkRoomSettingWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(Meta = (BindWidget))
	UScrollBox* ScrollBox;

	UPROPERTY(Meta = (BindWidget))
	UUniformGridPanel* GridPanel;

	UPROPERTY(Meta = (BindWidget))
	UBTNCustomWidget* GetGold;

	UPROPERTY(Meta = (BindWidget))
	UBTNCustomWidget* Exit;

	UPROPERTY(Meta = (BindWidget))
	UBTNCustomWidget* UpgradeGold;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* GoldPerSecond;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* AllGoldStocked;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* NeedGold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APlayerActor* PlayerActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ARoomWorking* RoomWorking;

	UFUNCTION()
	void Refresh();

	UFUNCTION()
	void RefreshStat();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UBTNCustomWidget> Button;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UBTNCustomWidget*> CustomButtonForWorker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* BackgroundTextureIsEmpty;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AWorker* Worker;

	UPROPERTY()
	float MoneySecond;

	UFUNCTION()
	void OnGoldClicked();

	UFUNCTION()
	void ActualiseMoney();
	
	UFUNCTION()
	void OnUpgradeCliqued();

	UFUNCTION()
	void OnExitClicked();

	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

	
};
