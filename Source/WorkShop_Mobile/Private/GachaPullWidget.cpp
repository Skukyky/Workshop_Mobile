#include "GachaPullWidget.h"
#include "BannerWidget.h"
#include "CharacterStructure.h"
#include "GachaCharacterShowcase.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/WidgetSwitcher.h"
#include "Engine/Engine.h" // Pour GEngine

void UGachaPullWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("GachaPullWidget::NativeConstruct"));

    if (GoldBanner)
    {
        GoldBanner->SetParentGachaWidget(this);
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("GoldBanner parent set"));
    }
    else
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("GoldBanner is NULL"));
    }

    if (UnitBanner)
    {
        UnitBanner->SetParentGachaWidget(this);
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("UnitBanner parent set"));
    }
    else
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("UnitBanner is NULL"));
    }

    if (BTN_BackToBanner)
    {
        BTN_BackToBanner->OnCustomButtonClicked.AddDynamic(this, &UGachaPullWidget::HandleBackToBannerClicked);
    }
    
    if (BTN_CharacterNext)
    {
        BTN_CharacterNext->OnCustomButtonClicked.AddDynamic(this, &UGachaPullWidget::OnCharacterNextClicked);
    }
}

void UGachaPullWidget::NativePreConstruct()
{
    Super::NativePreConstruct();

    if (ScrollBoxBanner)
    {
        ScrollBoxBanner->SetScrollOffset(200);
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("ScrollBoxBanner offset set to 200"));
    }
}

void UGachaPullWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
}

void UGachaPullWidget::AddHistoryItem(FName CharacterID)
{
    if (!CharacterDataTable || !UGP_GachaHistory)
        return;

    FCharacterStructure* Data = CharacterDataTable->FindRow<FCharacterStructure>(CharacterID, TEXT(""));
    if (!Data)
        return;

    // Création de la vraie image visible
    UImage* NewImage = NewObject<UImage>(this);
    if (!NewImage)
        return;

    if (Data->Photo)
    {
        NewImage->SetBrushFromTexture(Data->Photo);
    }

    NewImage->SetDesiredSizeOverride(FVector2D(512.f, 512.f));
    NewImage->SetVisibility(ESlateVisibility::Visible);
    NewImage->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f)); // Opacité pleine

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

        if (GEngine)
        {
            FString Msg = FString::Printf(TEXT("AddHistoryItem: Added character %s at row %d, col %d"), *CharacterID.ToString(), Row, Col);
            GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, Msg);
        }
    }

    // Si on ajoute le premier enfant ET on est dans un single pull (nombre total attendu = 10), on ajoute 9 images invisibles
    if (NewIndex == 0 && CurrentPulledCharacters.Num() == 1)
    {
        for (int i = 0; i < 9; ++i)
        {
            UImage* DummyImage = NewObject<UImage>(this);
            if (DummyImage)
            {
                DummyImage->SetBrushFromTexture(nullptr);
                DummyImage->SetDesiredSizeOverride(FVector2D(512.f, 512.f));
                DummyImage->SetVisibility(ESlateVisibility::Visible);
                DummyImage->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.f)); // transparent, invisible

                int32 DummyIndex = UGP_GachaHistory->GetChildrenCount();
                UUniformGridSlot* DummySlot = UGP_GachaHistory->AddChildToUniformGrid(DummyImage);
                if (DummySlot)
                {
                    int32 ColDummy = DummyIndex % 5;
                    int32 RowDummy = DummyIndex / 5;
                    DummySlot->SetRow(RowDummy);
                    DummySlot->SetColumn(ColDummy);
                    DummySlot->SetHorizontalAlignment(HAlign_Fill);
                    DummySlot->SetVerticalAlignment(VAlign_Fill);

                    if (GEngine)
                    {
                        FString Msg = FString::Printf(TEXT("AddHistoryItem: Added invisible dummy at row %d, col %d"), RowDummy, ColDummy);
                        GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, Msg);
                    }
                }
            }
        }
    }
}



void UGachaPullWidget::ShowPullHistory(const TArray<FName>& PulledCharacters)
{
    if (!UGP_GachaHistory || !WS_GachaPull)
        return;

    CurrentPulledCharacters = PulledCharacters;
    CurrentShowcaseIndex = 0;

    UGP_GachaHistory->ClearChildren();

    if (PulledCharacters.Num() == 1)
    {
        AddHistoryItem(PulledCharacters[0]);
    }
    else
    {
        for (const FName& CharID : PulledCharacters)
        {
            AddHistoryItem(CharID);
        }
    }

    WS_GachaPull->SetActiveWidgetIndex(2);
    UpdateShowcaseCharacter();
}


void UGachaPullWidget::HandleBackToBannerClicked()
{
    if (WS_GachaPull)
    {
        WS_GachaPull->SetActiveWidgetIndex(0);
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Switched to banner tab (index 0)"));
    }
}

void UGachaPullWidget::UpdateShowcaseCharacter()
{
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("UpdateShowcaseCharacter called, index %d"), CurrentShowcaseIndex));

    if (CharacterShowcaseActor)
    {
        CharacterShowcaseActor->Destroy();
        CharacterShowcaseActor = nullptr;
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Destroyed previous CharacterShowcaseActor"));
    }

    if (CurrentShowcaseIndex >= CurrentPulledCharacters.Num())
    {
        WS_GachaPull->SetActiveWidgetIndex(1);
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("All characters shown, switching to history tab (index 1)"));
        return;
    }

    if (!CharacterShowcaseActorClass)
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("CharacterShowcaseActorClass not set!"));
        return;
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("GetWorld() returned nullptr"));
        return;
    }

    // Spawn l'acteur à 300 unités plus haut en Z
    FVector SpawnLocation = FVector(0.f, 0.f, 100000.f);
    FRotator SpawnRotation = FRotator::ZeroRotator;
    FTransform SpawnTransform(SpawnRotation, SpawnLocation);

    CharacterShowcaseActor = World->SpawnActor<AGachaCharacterShowcase>(CharacterShowcaseActorClass, SpawnTransform);
    if (CharacterShowcaseActor)
    {
        CharacterShowcaseActor->CharacterDataTable = CharacterDataTable;
        CharacterShowcaseActor->SetCharacterByRowName(CurrentPulledCharacters[CurrentShowcaseIndex]);
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Spawned CharacterShowcaseActor for character %s"), *CurrentPulledCharacters[CurrentShowcaseIndex].ToString()));
    }
    else
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to spawn CharacterShowcaseActor"));
    }
}


void UGachaPullWidget::OnCharacterNextClicked()
{
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("OnCharacterNextClicked called"));

    // Si mode single pull avec placeholders, ignorer le next (pas d’autre perso)
    if (bSinglePullWithPlaceholders)
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Single pull mode active - Next ignored"));
        return;
    }

    if (CurrentShowcaseIndex + 1 < CurrentPulledCharacters.Num())
    {
        ++CurrentShowcaseIndex;
        UpdateShowcaseCharacter();
    }
    else
    {
        WS_GachaPull->SetActiveWidgetIndex(1);
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Reached end of showcase list, switching to history tab (index 1)"));

        if (CharacterShowcaseActor)
        {
            CharacterShowcaseActor->Destroy();
            CharacterShowcaseActor = nullptr;
            if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Destroyed CharacterShowcaseActor at end of showcase"));
        }
    }
}


void UGachaPullWidget::ShowPullResultsWithShowcase(const TArray<FName>& PulledCharacters)
{
    if (!UGP_GachaHistory || !WS_GachaPull)
        return;

    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("ShowPullResultsWithShowcase called with %d characters"), PulledCharacters.Num()));

    CurrentPulledCharacters = PulledCharacters;
    CurrentShowcaseIndex = 0;

    UGP_GachaHistory->ClearChildren();
    for (const FName& CharID : PulledCharacters)
    {
        AddHistoryItem(CharID);
    }

    WS_GachaPull->SetActiveWidgetIndex(2);
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Switched to showcase tab (index 2)"));

    UpdateShowcaseCharacter();
}
