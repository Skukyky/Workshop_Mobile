#include "GachaInventoryWidget.h"
#include "CharacterStructure.h"
#include "GachaInventoryItemWidget.h"
#include "Components/ScrollBox.h"
#include "Kismet/GameplayStatics.h"
#include "GachaSaveGame.h"

void UGachaInventoryWidget::NativeConstruct()
{
    Super::NativeConstruct();

    UGachaSaveGame* LoadedGame = nullptr;
    if (UGameplayStatics::DoesSaveGameExist(TEXT("GachaSaveSlot"), 0))
    {
        LoadedGame = Cast<UGachaSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("GachaSaveSlot"), 0));
    }

    if (LoadedGame && InventoryScrollBox && CharacterDataTable)
    {
        PopulateInventory(LoadedGame->SavedCharactersArray);
    }
    else if (InventoryScrollBox)
    {
        InventoryScrollBox->ClearChildren();
    }
}

void UGachaInventoryWidget::PopulateInventory(const TArray<FCharacterProgress>& CharactersInventory)
{
    if (!InventoryScrollBox || !CharacterDataTable)
    {
        return;
    }

    InventoryScrollBox->ClearChildren();

    if (CharactersInventory.Num() == 0)
    {
        return;
    }

    // Copie locale pour tri
    TArray<FCharacterProgress> SortedInventory = CharactersInventory;

    // Tri par rareté décroissante, puis étoiles décroissantes, puis nom croissant
    SortedInventory.Sort([this](const FCharacterProgress& A, const FCharacterProgress& B)
    {
        FCharacterStructure* DataA = CharacterDataTable->FindRow<FCharacterStructure>(A.CharacterID, TEXT(""));
        FCharacterStructure* DataB = CharacterDataTable->FindRow<FCharacterStructure>(B.CharacterID, TEXT(""));

        int32 RarityA = DataA ? static_cast<int32>(DataA->Rarity) : 0;
        int32 RarityB = DataB ? static_cast<int32>(DataB->Rarity) : 0;

        if (RarityA != RarityB)
        {
            return RarityA > RarityB;
        }

        if (A.StarCount != B.StarCount)
        {
            return A.StarCount > B.StarCount;
        }

        return A.CharacterID.ToString() < B.CharacterID.ToString();
    });

    for (const FCharacterProgress& Progress : SortedInventory)
    {
        const FName& CharacterRowName = Progress.CharacterID;

        FCharacterStructure* CharacterData = CharacterDataTable->FindRow<FCharacterStructure>(CharacterRowName, TEXT("GachaInventoryWidget::PopulateInventory"));
        if (!CharacterData)
        {
            continue;
        }

        if (!ItemWidgetClass)
        {
            continue;
        }

        UGachaInventoryItemWidget* EntryWidget = CreateWidget<UGachaInventoryItemWidget>(this, ItemWidgetClass);
        if (!EntryWidget)
        {
            continue;
        }

        EntryWidget->InitializeWithData(*CharacterData, Progress);
        InventoryScrollBox->AddChild(EntryWidget);
    }
}
