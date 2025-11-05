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

    if (!ScrollBoxBanner)  // Vérification de sécurité
    {
        return;
    }

    if (!ScrollBoxBanner->HasMouseCapture() && ScrollBoxBanner->GetScrollOffset() != ScrollLocation)
    {
        if (FMath::Abs(ScrollBoxBanner->GetScrollOffset() - ScrollLocation) > 300 && !SnappingToMenu)
        {
            ScrollDirection = FMath::Sign(ScrollBoxBanner->GetScrollOffset() - ScrollLocation);
            ScrollLocation = ScrollLocation + (ScrollDirection * -2000);
        }

        ScrollBoxBanner->EndInertialScrolling();

        // Interpolation
        float InterpScrollOffset = FMath::FInterpTo(
            ScrollBoxBanner->GetScrollOffset(),  // current
            ScrollLocation,                      // target
            InDeltaTime,                        // deltaTime
            50.f                               // interp speed
        );

        ScrollBoxBanner->SetScrollOffset(InterpScrollOffset);
        SnappingToMenu = true;
    }else
    {
        SnappingToMenu = false;
    }
}




