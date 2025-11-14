#pragma once

#include "CoreMinimal.h"
#include "BTNCustomWidget.h"
#include "Blueprint/UserWidget.h"
#include "CharacterStructure.h"
#include "GachaPullWidget.h"
#include "PlayerActor.h"
#include "Engine/DataTable.h"
#include "BannerWidget.generated.h"

class UImage;
class UGachaPullWidget;
class APlayerActor;

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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
    UTexture2D* ButtonPullTexture = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
    UTexture2D* ButtonPull10Texture = nullptr;

    UPROPERTY()
    UUserWidget* ConvertionWidgetRef;
	
    UPROPERTY(EditDefaultsOnly, Category = Widget)
    TSubclassOf<UUserWidget> WidgetConvertionReference;
    
    UPROPERTY(meta = (BindWidget))
    UBTNCustomWidget* BTN_Pull;

    UPROPERTY(meta = (BindWidget))
    UBTNCustomWidget* BTN_PullMulti;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drop Rates")
    TMap<ECharacterRarity, float> DropRatesByRarity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gacha")
    UDataTable* CharacterDataTable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gacha")
    int EpicCounter = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gacha")
    int LegendaryCounter = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pity System")
    bool bUseLegendaryPity = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    int PriceFollower = 100;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    int PriceMoney = 1000;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    bool bMoneyFollower = true;

protected:
    virtual void NativeConstruct() override;
    virtual void NativePreConstruct() override;

    UFUNCTION()
    void HandlePullClicked();

    UFUNCTION()
    void HandlePullMultiClicked();

private:
    FName PerformSinglePull();
    void AddPulledWorkersToPlayer(const TArray<FName>& PullResults);

    UPROPERTY()
    APlayerActor* PlayerActorRef = nullptr;
};