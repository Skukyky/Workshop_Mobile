#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "CharacterProgress.h"
#include "GachaPullWidget.generated.h"

class UTextBlock;
class APlayerActor;
class AGachaCharacterShowcase;
class UImage;
class UBTNCustomWidget;
class UBannerWidget;
class UWidgetSwitcher;
class UUniformGridPanel;
class UScrollBox;

UCLASS()
class WORKSHOP_MOBILE_API UGachaPullWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    
    bool bSinglePullWithPlaceholders = false;
    
    TArray<FName> CurrentPulledCharacters;
    int32 CurrentShowcaseIndex = 0;

    UPROPERTY()
    AGachaCharacterShowcase* CharacterShowcaseActor = nullptr;

    UPROPERTY(EditAnywhere, Category="Showcase")
    TSubclassOf<AGachaCharacterShowcase> CharacterShowcaseActorClass = nullptr;

    UPROPERTY(meta = (BindWidget))
    UBTNCustomWidget* BTN_BackToBanner;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* MoneyText;

    UPROPERTY(meta = (BindWidget))
    UImage* MoneyImage;

    UPROPERTY(meta = (BindWidget))
    UImage* FollowerImage;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* FollowerText;

    UPROPERTY(meta = (BindWidget))
    UBTNCustomWidget* BTN_Back;

    UPROPERTY(meta = (BindWidget))
    UBTNCustomWidget* BTN_CharacterNext;

    UPROPERTY(meta = (BindWidget))
    UImage* CharacterImage;
    
    UPROPERTY(meta = (BindWidget))
    UBannerWidget* GoldBanner;

    UPROPERTY(meta = (BindWidget))
    UBannerWidget* UnitBanner;

    UPROPERTY(meta = (BindWidget))
    UScrollBox* ScrollBoxBanner;

    UPROPERTY(meta = (BindWidget))
    UWidgetSwitcher* WS_GachaPull;

    UPROPERTY(meta = (BindWidget))
    UUniformGridPanel* UGP_GachaHistory;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gacha")
    UDataTable* CharacterDataTable;

    UPROPERTY()
    TMap<FName, FCharacterProgress> CharactersProgress;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gacha")
    float ScrollLocation = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gacha")
    APlayerActor* PlayerREF = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gacha")
    float ScrollDirection = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gacha")
    bool SnappingToMenu = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gacha")
    float SnapThreshold = 50.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gacha")
    float SnapInterpSpeed = 10.f;

    void AddHistoryItem(FName CharacterID);

    void ShowPullHistory(const TArray<FName>& PulledCharacters);

    void ShowPullResultsWithShowcase(const TArray<FName>& PulledCharacters); // nouvelle méthode pour showcase

    void UpdateShowcaseCharacter();

    UFUNCTION()
    void HandleBackToBannerClicked();

    UFUNCTION()
    void OnCharacterNextClicked();

    UFUNCTION()
    void HandleBackClicked();

protected:

    virtual void NativeConstruct() override;

    virtual void NativePreConstruct() override;

    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
