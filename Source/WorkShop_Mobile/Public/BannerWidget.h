#pragma once

#include "CoreMinimal.h"
#include "BTNCustomWidget.h"
#include "Blueprint/UserWidget.h"
#include "CharacterStructure.h"
#include "Engine/DataTable.h"
#include "CharacterProgress.h"
#include "BannerWidget.generated.h"

class UImage;
class UGachaPullWidget;

USTRUCT(BlueprintType)
struct FDropRateByRarity
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drop Rate")
    ECharacterRarity Rarity = ECharacterRarity::Commun;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drop Rate")
    float DropRate = 0.f;
};

UCLASS()
class WORKSHOP_MOBILE_API UBannerWidget : public UUserWidget
{
    GENERATED_BODY()

public:

    UPROPERTY()
    UGachaPullWidget* ParentGachaWidget;

    void SetParentGachaWidget(UGachaPullWidget* Parent);

    UPROPERTY(meta = (BindWidget))
    UImage* ImageBackground;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
    UTexture2D* BackgroundTexture = nullptr;

    UPROPERTY(meta = (BindWidget))
    UBTNCustomWidget* BTN_Pull;
    
    UPROPERTY(meta = (BindWidget))
    UBTNCustomWidget* BTN_PullMulti;

    /** Map editable des drop rates par rareté */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drop Rates")
    TMap<ECharacterRarity, float> DropRatesByRarity;

    /** DataTable des personnages à tirer */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gacha")
    UDataTable* CharacterDataTable;

    /** Map pour suivi (peut-être temporaire) */
    UPROPERTY()
    TMap<FName, FCharacterProgress> CharactersProgress;

    /** Inventaire principal sous forme de tableau, supporte doublons et fusion */
    UPROPERTY(BlueprintReadWrite, Category="Inventory")
    TArray<FCharacterProgress> CharactersInventory;

protected:

    virtual void NativeConstruct() override;

    virtual void NativePreConstruct() override;

    UFUNCTION()
    void HandlePullClicked();

    UFUNCTION()
    void HandlePullMultiClicked();

private:
    void SaveProgress(); 

    FName PerformSinglePull();
    void MergePullResults(const TArray<FName>& PullResults);
};
