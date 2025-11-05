#include "GachaPullWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "GachaSaveGame.h" 
#include "Components/ScrollBox.h"

void UGachaPullWidget::NativeConstruct()
{
    Super::NativeConstruct();
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

    if (!ScrollBoxBanner)
        return;

    if (!ScrollBoxBanner->HasMouseCapture())
    {
        float CurrentOffset = ScrollBoxBanner->GetScrollOffset();

        if (!SnappingToMenu && FMath::Abs(CurrentOffset - ScrollLocation) > SnapThreshold)
        {
            ScrollDirection = FMath::Sign(CurrentOffset - ScrollLocation);

            // Par exemple, largeur d'une bannière + marge = BannerWidth
            const float BannerWidth = 600.f;

            // Calculer la nouvelle cible en ajoutant un pas BannerWidth selon direction
            ScrollLocation += ScrollDirection * BannerWidth;

            // Clamp ScrollLocation dans les limites scroll (0 à max scroll)
            ScrollLocation = FMath::Clamp(ScrollLocation, 0.f, ScrollBoxBanner->GetScrollOffsetOfEnd());

            ScrollBoxBanner->EndInertialScrolling();

            SnappingToMenu = true;
        }

        if (SnappingToMenu)
        {
            float InterpScrollOffset = FMath::FInterpTo(CurrentOffset, ScrollLocation, InDeltaTime, SnapInterpSpeed);

            ScrollBoxBanner->SetScrollOffset(InterpScrollOffset);

            // Lorsque proche de la cible
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





