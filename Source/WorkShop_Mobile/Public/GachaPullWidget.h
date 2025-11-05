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

	// ScrollBox contenant les bannières
	UPROPERTY(meta = (BindWidget))
	UScrollBox* ScrollBoxBanner;
    
	// TextBlocks pour afficher les infos
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

	// Table des données pour les personnages
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gacha")
	UDataTable* CharacterDataTable;

	// Map de progression des personnages
	UPROPERTY()
	TMap<FName, FCharacterProgress> CharactersProgress;

	// Valeur du scroll actuel ciblé (pour le snap)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gacha")
	float ScrollLocation = 0.0f;

	// Direction du scroll (-1 ou 1)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gacha")
	float ScrollDirection = 0.0f;

	// Bool indiquant si on est en train de snapper
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gacha")
	bool SnappingToMenu = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gacha")
	float SnapThreshold = 50.f;  // seuil en pixels pour déclencher le snap

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gacha")
	float SnapInterpSpeed = 10.f;  // vitesse d'interpolation du snap

protected:

	virtual void NativeConstruct() override;

	virtual void NativePreConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
    
};
