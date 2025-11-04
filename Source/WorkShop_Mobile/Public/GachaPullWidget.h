// GachaPullWidget.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "CharacterProgress.h"
#include "GachaPullWidget.generated.h"

class UScrollBox;
class UButton;
class UTextBlock;

UCLASS()
class WORKSHOP_MOBILE_API UGachaPullWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	UScrollBox* ScrollBoxBanner;

	UPROPERTY(meta = (BindWidget))
	UButton* BTN_PullMulti;
	
	UPROPERTY(meta = (BindWidget))
	UButton* BTN_Pull;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TypeText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* RarityText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* NameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* StatYoutubeText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* StatTikTokText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* StarText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gacha")
	UDataTable* CharacterDataTable;

	UPROPERTY()
	TMap<FName, FCharacterProgress> CharactersProgress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gacha")
	float ScrollLocation = 0.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gacha")
	float ScrollDirection = 0.0;

protected:

	virtual void NativeConstruct() override;

	virtual void NativePreConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:

	UFUNCTION(BlueprintCallable)
	void OnPullButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnPullMultiButtonClicked();

	void SaveProgress();

	void LoadProgress();

	UFUNCTION()
	void OnUserScrolled(float CurrentOffset);
};
