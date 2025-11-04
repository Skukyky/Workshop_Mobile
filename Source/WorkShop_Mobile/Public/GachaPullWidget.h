// GachaPullWidget.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "CharacterProgress.h"
#include "GachaPullWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class WORKSHOP_MOBILE_API UGachaPullWidget : public UUserWidget
{
	GENERATED_BODY()

public:

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

protected:

	virtual void NativeConstruct() override;

private:

	UFUNCTION(BlueprintCallable)
	void OnPullButtonClicked();

	void SaveProgress();

	void LoadProgress();
};
