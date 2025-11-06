#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "CharacterProgress.h"
#include "GachaPullWidget.generated.h"

class UBTNCustomWidget;
class UBannerWidget;
class UWidgetSwitcher;
class UUniformGridPanel;
class UScrollBox;
class UButton;
class UTextBlock;

UCLASS()
class WORKSHOP_MOBILE_API UGachaPullWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	UBTNCustomWidget* BTN_BackToBanner;
	
	UPROPERTY(meta = (BindWidget))
	UBannerWidget* GoldBanner;

	UPROPERTY(meta = (BindWidget))
	UBannerWidget* UnitBanner;

	// ScrollBox contenant les bannières
	UPROPERTY(meta = (BindWidget))
	UScrollBox* ScrollBoxBanner;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* WS_GachaPull;

	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* UGP_GachaHistory;
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

	// Ajoute une image à l'historique pour un personnage donné
	void AddHistoryItem(FName CharacterID);

	// Affiche l'historique des tirages et active le widget switcher sur l'onglet concerné
	void ShowPullHistory(const TArray<FName>& PulledCharacters);

	UFUNCTION()
	void HandleBackToBannerClicked();

protected:

	virtual void NativeConstruct() override;

	virtual void NativePreConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	
    
};
