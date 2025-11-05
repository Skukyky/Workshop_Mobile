#pragma once

#include "CoreMinimal.h"
#include "BTNCustomWidget.h"
#include "Blueprint/UserWidget.h"
#include "CharacterStructure.h"
#include "Engine/DataTable.h"
#include "CharacterProgress.h"
#include "BannerWidget.generated.h"

USTRUCT(BlueprintType)
struct FDropRateByRarity
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drop Rate")
    ECharacterRarity Rarity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drop Rate")
    float DropRate;
};

UCLASS()
class WORKSHOP_MOBILE_API UBannerWidget : public UUserWidget
{
    GENERATED_BODY()

public:

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

    UFUNCTION()
    void HandlePullClicked();

    UFUNCTION()
    void HandlePullMultiClicked();

private:
    void SaveProgress();

    FName PerformSinglePull();
    void MergePullResults(const TArray<FName>& PullResults);
};
