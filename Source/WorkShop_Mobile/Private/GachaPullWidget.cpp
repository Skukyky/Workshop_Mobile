#include "GachaPullWidget.h"

#include "BannerWidget.h"
#include "CharacterStructure.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/WidgetSwitcher.h"
#include "Engine/Engine.h"  // Pour GEngine

void UGachaPullWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("GachaPullWidget::NativeConstruct called"));
    }

    if (GoldBanner)
    {
        GoldBanner->SetParentGachaWidget(this);
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("ParentGachaWidget assigned to GoldBanner"));
        }
    }
    else
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("GoldBanner is null"));
        }
    }

    if (UnitBanner)
    {
        UnitBanner->SetParentGachaWidget(this);
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("ParentGachaWidget assigned to UnitBanner"));
        }
    }
    else
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("UnitBanner is null"));
        }
    }
}


void UGachaPullWidget::NativePreConstruct()
{
    Super::NativePreConstruct();

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("GachaPullWidget::NativePreConstruct called"));
    }

    if (ScrollBoxBanner)
    {
        ScrollBoxBanner->SetScrollOffset(200);
    }
}

void UGachaPullWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (!ScrollBoxBanner)
        return;

    if (!ScrollBoxBanner->HasMouseCapture())
    {
        float CurrentOffset = ScrollBoxBanner->GetScrollOffset();

        if (!SnappingToMenu && FMath::Abs(CurrentOffset - ScrollLocation) > SnapThreshold)
        {
            ScrollDirection = FMath::Sign(CurrentOffset - ScrollLocation);

            const float BannerWidth = 600.f;

            ScrollLocation += ScrollDirection * BannerWidth;

            ScrollLocation = FMath::Clamp(ScrollLocation, 0.f, ScrollBoxBanner->GetScrollOffsetOfEnd());

            ScrollBoxBanner->EndInertialScrolling();

            SnappingToMenu = true;
        }

        if (SnappingToMenu)
        {
            float InterpScrollOffset = FMath::FInterpTo(CurrentOffset, ScrollLocation, InDeltaTime, SnapInterpSpeed);
            ScrollBoxBanner->SetScrollOffset(InterpScrollOffset);

            if (FMath::IsNearlyEqual(InterpScrollOffset, ScrollLocation, 1.f))
            {
                ScrollBoxBanner->SetScrollOffset(ScrollLocation);
                SnappingToMenu = false;
            }
        }
    }
    else
    {
        SnappingToMenu = false;
    }
}

void UGachaPullWidget::AddHistoryItem(FName CharacterID)
{
    if (!CharacterDataTable || !UGP_GachaHistory)
    {
        if (GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AddHistoryItem: Missing DataTable or GachaHistory panel"));
        return;
    }

    FCharacterStructure* Data = CharacterDataTable->FindRow<FCharacterStructure>(CharacterID, TEXT(""));
    if (!Data)
    {
        if (GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("AddHistoryItem: Character %s not found in DataTable"), *CharacterID.ToString()));
        return;
    }

    UImage* NewImage = NewObject<UImage>(this);
    if (!NewImage)
    {
        if (GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AddHistoryItem: Failed to create UImage"));
        return;
    }

    if (Data->Photo)
    {
        NewImage->SetBrushFromTexture(Data->Photo);
    }

    // Taille souhaitée
    NewImage->SetDesiredSizeOverride(FVector2D(512.f, 512.f));

    int32 NewIndex = UGP_GachaHistory->GetChildrenCount();
    UUniformGridSlot* NewSlot = UGP_GachaHistory->AddChildToUniformGrid(NewImage);
    if (NewSlot)
    {
        int32 Col = NewIndex % 5;
        int32 Row = NewIndex / 5;
        NewSlot->SetRow(Row);
        NewSlot->SetColumn(Col);

        NewSlot->SetHorizontalAlignment(HAlign_Fill);
        NewSlot->SetVerticalAlignment(VAlign_Fill);
    }
}


void UGachaPullWidget::ShowPullHistory(const TArray<FName>& PulledCharacters)
{
    if (!UGP_GachaHistory || !WS_GachaPull)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ShowPullHistory: Missing GachaHistory panel or WidgetSwitcher"));
        }
        return;
    }

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
            FString::Printf(TEXT("ShowPullHistory called with %d pulled characters"), PulledCharacters.Num()));
    }

    UGP_GachaHistory->ClearChildren();

    for (const FName& CharID : PulledCharacters)
    {
        AddHistoryItem(CharID);
    }

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Switching WidgetSwitcher to index 1 (history)"));
    }

    WS_GachaPull->SetActiveWidgetIndex(1);
}
