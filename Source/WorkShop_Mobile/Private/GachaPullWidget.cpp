#include "GachaPullWidget.h"
#include "BannerWidget.h"
#include "CharacterStructure.h"
#include "GachaCharacterShowcase.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"

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
    
    if (BTN_CharacterNext)
    {
        BTN_CharacterNext->OnCustomButtonClicked.AddDynamic(this, &UGachaPullWidget::OnCharacterNextClicked);
    }
    if (BTN_Back)
    {
        BTN_Back->OnCustomButtonClicked.AddDynamic(this, &UGachaPullWidget::HandleBackClicked);
    }

    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UGachaPullWidget::UpdateValue, 0.1f, true);
}

void UGachaPullWidget::NativePreConstruct()
{
    Super::NativePreConstruct();

    if (ScrollBoxBanner)
    {
        ScrollBoxBanner->SetScrollOffset(0);
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
    }
    BTN_Back->SetVisibility(ESlateVisibility::Visible);
    FollowerImage->SetVisibility(ESlateVisibility::Visible);
    MoneyImage->SetVisibility(ESlateVisibility::Visible);
    FollowerText->SetVisibility(ESlateVisibility::Visible);
    MoneyText->SetVisibility(ESlateVisibility::Visible);
}

void UGachaPullWidget::UpdateShowcaseCharacter()
{
    if (AnimLoot)
    {
        PlayAnimation(AnimLoot);
    }
    
    if (CharacterShowcaseActor)
    {
        CharacterShowcaseActor->Destroy();
        CharacterShowcaseActor = nullptr;
    }

    if (CurrentShowcaseIndex >= CurrentPulledCharacters.Num())
    {
        WS_GachaPull->SetActiveWidgetIndex(1);
        return;
    }

    // --- Ajout modification couleur d'effet selon la rareté ---
    if (EffectImage && CharacterDataTable)
    {
        FName CharId = CurrentPulledCharacters[CurrentShowcaseIndex];
        const FCharacterStructure* Data = CharacterDataTable->FindRow<FCharacterStructure>(CharId, TEXT(""));
        if (Data)
        {
            FLinearColor RarityColor = GetColorForRarity(Data->Rarity);
            EffectImage->SetColorAndOpacity(RarityColor);
        }
        else
        {
            // Optionnel : reset à blanc si pas de data trouvée
            EffectImage->SetColorAndOpacity(FLinearColor::White);
        }
    }
    // ----------------------------------------------------------

    if (!CharacterShowcaseActorClass)
    {
        return;
    }

    UWorld* World = GetWorld();
    if (!World)
        return;

    // Spawn l'acteur à la position souhaitée (ajuste la hauteur ici)
    FVector SpawnLocation = FVector(0.f, 0.f, 300.f);
    FRotator SpawnRotation = FRotator::ZeroRotator;
    FTransform SpawnTransform(SpawnRotation, SpawnLocation);

    CharacterShowcaseActor = World->SpawnActor<AGachaCharacterShowcase>(CharacterShowcaseActorClass, SpawnTransform);
    if (CharacterShowcaseActor)
    {
        CharacterShowcaseActor->CharacterDataTable = CharacterDataTable;
        CharacterShowcaseActor->SetCharacterByRowName(CurrentPulledCharacters[CurrentShowcaseIndex]);
    }
}

void UGachaPullWidget::OnCharacterNextClicked()
{

    
    // Si mode single pull avec placeholders, ignorer le next (pas d’autre perso)
    if (bSinglePullWithPlaceholders)
    {
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

        if (CharacterShowcaseActor)
        {
            CharacterShowcaseActor->Destroy();
            CharacterShowcaseActor = nullptr;
        }
    }
}

void UGachaPullWidget::HandleBackClicked()
{
    PlayerREF->ChangeVolumeMusic(false);
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    if (PlayerController)
    {
        FInputModeGameOnly InputMode;
        PlayerController->SetInputMode(InputMode);
        PlayerController->bShowMouseCursor = true;
    }
    RemoveFromParent();
}

void UGachaPullWidget::UpdateValue()
{
    FollowerText->SetText(FText::FromString(FString::FromInt(PlayerREF->GetPoolResource())));
    MoneyText->SetText(FText::FromString(FString::FromInt(PlayerREF->GetMoney())));
}

void UGachaPullWidget::ShowPullResultsWithShowcase(const TArray<FName>& PulledCharacters)
{
    
    if (!UGP_GachaHistory || !WS_GachaPull)
        return;

    CurrentPulledCharacters = PulledCharacters;
    CurrentShowcaseIndex = 0;

    UGP_GachaHistory->ClearChildren();
    for (const FName& CharID : PulledCharacters)
    {
        AddHistoryItem(CharID);
    }

    WS_GachaPull->SetActiveWidgetIndex(2);

    UpdateShowcaseCharacter();
}

FLinearColor UGachaPullWidget::GetColorForRarity(ECharacterRarity Rarity)
{
    switch (Rarity)
    {
    case ECharacterRarity::Commun:
        return FLinearColor(0.f, 1.f, 0.045f, 1.f); // Blanc
    case ECharacterRarity::Rare:
        return FLinearColor(0.f, 0.376f, 1.f, 1.f); // Bleu
    case ECharacterRarity::Epique:
        return FLinearColor(0.244f, 0.f, 1.f, 1.f); // Violet
    case ECharacterRarity::Legendary:
        return FLinearColor(0.794f, 0.77f, 0.f, 1.f); // Jaune
    case ECharacterRarity::Secret:
        return FLinearColor(0.794f, 0.f, 0.064f, 1.f); // Rouge
    default:
        return FLinearColor(1.f, 1.f, 1.f, 1.f);
    }
}
