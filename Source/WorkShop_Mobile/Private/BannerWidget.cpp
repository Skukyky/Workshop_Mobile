#include "BannerWidget.h"
#include "GachaPullWidget.h"
#include "Components/Image.h"
#include "PlayerActor.h"
#include "Kismet/GameplayStatics.h"

void UBannerWidget::SetParentGachaWidget(UGachaPullWidget* Parent)
{
    ParentGachaWidget = Parent;
}

void UBannerWidget::NativePreConstruct()
{
    Super::NativePreConstruct();
    if (ImageBackground && BackgroundTexture)
    {
        FSlateBrush NewBrush;
        NewBrush.SetResourceObject(BackgroundTexture);
        NewBrush.DrawAs = ESlateBrushDrawType::RoundedBox;
        NewBrush.ImageSize = FVector2D(BackgroundTexture->GetSizeX(), BackgroundTexture->GetSizeY());
        ImageBackground->SetBrush(NewBrush);
    }

    BTN_Pull->BackgroundTexture = ButtonPullTexture;
    BTN_Pull->Refresh();
    BTN_PullMulti->BackgroundTexture = ButtonPull10Texture;
    BTN_PullMulti->Refresh();
}

void UBannerWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Récupérer la référence au PlayerActor
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        PlayerActorRef = Cast<APlayerActor>(PC->GetPawn());
    }

    if (BTN_Pull)
    {
        BTN_Pull->OnCustomButtonClicked.AddDynamic(this, &UBannerWidget::HandlePullClicked);
    }

    if (BTN_PullMulti)
    {
        BTN_PullMulti->OnCustomButtonClicked.AddDynamic(this, &UBannerWidget::HandlePullMultiClicked);
    }
}

void UBannerWidget::HandlePullClicked()
{
    if ((!bMoneyFollower && ParentGachaWidget && ParentGachaWidget->PlayerREF && ParentGachaWidget->PlayerREF->GetMoney() >= PriceMoney)
           || (bMoneyFollower && ParentGachaWidget->PlayerREF->GetPoolResource() >= PriceFollower))
    {
        if (bMoneyFollower)
        {
            ParentGachaWidget->PlayerREF->SetPoolResource(- PriceFollower);
        }
        else
        {
            ParentGachaWidget->PlayerREF->SetMoney(- PriceMoney); 
        }
        
        FName PulledCharacter = PerformSinglePull();
        if (PulledCharacter == NAME_None)
        {
            return;
        }

        TArray<FName> PullResults;
        PullResults.Add(PulledCharacter);

        AddPulledWorkersToPlayer(PullResults);

        if (ParentGachaWidget)
        {
            ParentGachaWidget->ShowPullResultsWithShowcase(PullResults);
            ParentGachaWidget->BTN_Back->SetVisibility(ESlateVisibility::Collapsed);
            ParentGachaWidget->FollowerText->SetText(FText::FromString(FString::FromInt(ParentGachaWidget->PlayerREF->GetPoolResource())));
            ParentGachaWidget->MoneyText->SetText(FText::FromString(FString::FromInt(ParentGachaWidget->PlayerREF->GetMoney())));
            ParentGachaWidget->FollowerImage->SetVisibility(ESlateVisibility::Collapsed);
            ParentGachaWidget->MoneyImage->SetVisibility(ESlateVisibility::Collapsed);
            ParentGachaWidget->FollowerText->SetVisibility(ESlateVisibility::Collapsed);
            ParentGachaWidget->MoneyText->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
    else
    {
        
    }
    
}

void UBannerWidget::HandlePullMultiClicked()
{
    if ((!bMoneyFollower && ParentGachaWidget && ParentGachaWidget->PlayerREF && ParentGachaWidget->PlayerREF->GetMoney() >= PriceMoney*10)
        || (bMoneyFollower && ParentGachaWidget->PlayerREF->GetPoolResource() >= PriceFollower*10))
    {
        if (bMoneyFollower)
        {
            ParentGachaWidget->PlayerREF->SetPoolResource( - PriceFollower*10);
        }
        else
        {
            ParentGachaWidget->PlayerREF->SetMoney(- PriceMoney*10); 
        }
        
        if (!CharacterDataTable)
        {
            return;
        }

        TArray<FName> PullResults;
        for (int i = 0; i < 10; ++i)
        {
            FName PulledCharacter = PerformSinglePull();
            if (PulledCharacter != NAME_None)
            {
                PullResults.Add(PulledCharacter);
            }
        }

        // Trouver un épique dans le résultat
        int32 EpicIndexInResult = -1;
        for (int32 Index = 0; Index < PullResults.Num(); ++Index)
        {
            FCharacterStructure* CharData = CharacterDataTable->FindRow<FCharacterStructure>(PullResults[Index], TEXT(""));
            if (CharData && CharData->Rarity == ECharacterRarity::Epique)
            {
                EpicIndexInResult = Index;
                break;
            }
        }

        // Si aucun épique trouvé, en ajouter un aléatoirement
        if (EpicIndexInResult == -1)
        {
            TArray<FName> EpicCharacters;
            TArray<FName> RowNames = CharacterDataTable->GetRowNames();
            for (const FName& RowName : RowNames)
            {
                FCharacterStructure* CharData = CharacterDataTable->FindRow<FCharacterStructure>(RowName, TEXT(""));
                if (CharData && CharData->Rarity == ECharacterRarity::Epique)
                {
                    EpicCharacters.Add(RowName);
                }
            }
            if (EpicCharacters.Num() > 0)
            {
                int32 EpicIndex = FMath::RandRange(0, EpicCharacters.Num() - 1);
                PullResults[PullResults.Num() - 1] = EpicCharacters[EpicIndex];
            }
        }

        AddPulledWorkersToPlayer(PullResults);

        if (ParentGachaWidget)
        {
            ParentGachaWidget->ShowPullResultsWithShowcase(PullResults);
            ParentGachaWidget->BTN_Back->SetVisibility(ESlateVisibility::Collapsed);
            ParentGachaWidget->FollowerText->SetText(FText::FromString(FString::FromInt(ParentGachaWidget->PlayerREF->GetPoolResource())));
            ParentGachaWidget->MoneyText->SetText(FText::FromString(FString::FromInt(ParentGachaWidget->PlayerREF->GetMoney())));
            ParentGachaWidget->FollowerImage->SetVisibility(ESlateVisibility::Collapsed);
            ParentGachaWidget->MoneyImage->SetVisibility(ESlateVisibility::Collapsed);
            ParentGachaWidget->FollowerText->SetVisibility(ESlateVisibility::Collapsed);
            ParentGachaWidget->MoneyText->SetVisibility(ESlateVisibility::Collapsed);
        }
    }else
    {
       
    }
}


void UBannerWidget::AddPulledWorkersToPlayer(const TArray<FName>& PullResults)
{
    if (!PlayerActorRef)
    {
        UE_LOG(LogTemp, Error, TEXT("PlayerActorRef is null in AddPulledWorkersToPlayer"));
        return;
    }
    
    for (const FName& CharacterID : PullResults)
    {
        PlayerActorRef->AddWorkerToInventory(CharacterID, 1);
    }
}

FName UBannerWidget::PerformSinglePull()
{
    if (!CharacterDataTable) return NAME_None;
    
    EpicCounter = EpicCounter % 10;
    LegendaryCounter = LegendaryCounter % 80;

    TArray<FName> RowNames = CharacterDataTable->GetRowNames();
    float TotalWeightedDropRate = 0.f;
    TArray<FName> ValidCharacterRowNames;

    for (const FName& RowName : RowNames)
    {
        FCharacterStructure* Data = CharacterDataTable->FindRow<FCharacterStructure>(RowName, TEXT(""));
        if (!Data) continue;

        bool bForceLegendary = bUseLegendaryPity && (LegendaryCounter >= 79) && (Data->Rarity == ECharacterRarity::Legendary);
        bool bForceEpic = (EpicCounter >= 9) && (Data->Rarity == ECharacterRarity::Epique);

        float Weight = 0.f;
        if (bForceLegendary)
        {
            Weight = 100000.f;
        }
        else if (bForceEpic)
        {
            Weight = 99999.f;
        }
        else
        {
            float* DropRate = DropRatesByRarity.Find(Data->Rarity);
            Weight = DropRate ? *DropRate : 0.f;
        }

        if (Weight > 0.f)
        {
            TotalWeightedDropRate += Weight;
            ValidCharacterRowNames.Add(RowName);
        }
    }

    if (TotalWeightedDropRate <= 0.f || ValidCharacterRowNames.Num() == 0)
        return NAME_None;

    float RandomPoint = FMath::FRandRange(0.f, TotalWeightedDropRate);
    float Accumulated = 0.f;

    for (const FName& RowName : ValidCharacterRowNames)
    {
        FCharacterStructure* CharData = CharacterDataTable->FindRow<FCharacterStructure>(RowName, TEXT(""));
        if (!CharData) continue;

        float Weight = 0.f;
        if (bUseLegendaryPity && LegendaryCounter >= 79 && CharData->Rarity == ECharacterRarity::Legendary)
            Weight = 100000.f;
        else if (EpicCounter >= 9 && CharData->Rarity == ECharacterRarity::Epique)
            Weight = 99999.f;
        else
            Weight = DropRatesByRarity.Find(CharData->Rarity) ? *DropRatesByRarity.Find(CharData->Rarity) : 0.f;

        Accumulated += Weight;

        if (RandomPoint <= Accumulated)
        {
            if (CharData->Rarity == ECharacterRarity::Legendary)
            {
                if (bUseLegendaryPity)
                {
                    LegendaryCounter = 0;
                }
            }
            else
            {
                if (bUseLegendaryPity)
                {
                    LegendaryCounter++;
                }
            }

            if (CharData->Rarity == ECharacterRarity::Epique)
            {
                EpicCounter = 0;
            }
            else
            {
                EpicCounter++;
            }

            return RowName;
        }
    }

    return NAME_None;
}