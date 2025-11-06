#include "GachaPullWidget.h"
#include "BannerWidget.h"
#include "CharacterStructure.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/WidgetSwitcher.h"

void UGachaPullWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (GoldBanner)
    {
        GoldBanner->SetParentGachaWidget(this);
    }

    if (UnitBanner)
    {
        UnitBanner->SetParentGachaWidget(this);
    }

    if (BTN_BackToBanner)
    {
        BTN_BackToBanner->OnCustomButtonClicked.AddDynamic(this, &UGachaPullWidget::HandleBackToBannerClicked);
    }
}

void UGachaPullWidget::NativePreConstruct()
{
    Super::NativePreConstruct();

    if (ScrollBoxBanner)
    {
        ScrollBoxBanner->SetScrollOffset(200);
    }
}

void UGachaPullWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
}

void UGachaPullWidget::AddHistoryItem(FName CharacterID)
{
    if (!CharacterDataTable || !UGP_GachaHistory)
    {
        return;
    }

    FCharacterStructure* Data = CharacterDataTable->FindRow<FCharacterStructure>(CharacterID, TEXT(""));
    if (!Data)
    {
        return;
    }

    UImage* NewImage = NewObject<UImage>(this);
    if (!NewImage)
    {
        return;
    }

    if (Data->Photo)
    {
        NewImage->SetBrushFromTexture(Data->Photo);
    }

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
        return;
    }

    UGP_GachaHistory->ClearChildren();

    if (PulledCharacters.Num() == 1)
    {
        AddHistoryItem(PulledCharacters[0]);  // vraie carte visible

        for (int i = 1; i < 10; i++)
        {
            UImage* InvisibleImage = NewObject<UImage>(this);
            if (InvisibleImage)
            {
                InvisibleImage->SetOpacity(0.f);
                InvisibleImage->SetDesiredSizeOverride(FVector2D(512.f, 512.f));

                int32 NewIndex = UGP_GachaHistory->GetChildrenCount();
                UUniformGridSlot* NewSlot = UGP_GachaHistory->AddChildToUniformGrid(InvisibleImage);
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
        }
    }
    else
    {
        for (const FName& CharID : PulledCharacters)
        {
            AddHistoryItem(CharID);
        }
    }

    WS_GachaPull->SetActiveWidgetIndex(1);
}

void UGachaPullWidget::HandleBackToBannerClicked()
{
    if (WS_GachaPull)
    {
        WS_GachaPull->SetActiveWidgetIndex(0);
    }
}
