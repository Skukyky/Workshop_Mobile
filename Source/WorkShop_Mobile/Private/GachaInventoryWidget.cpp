#include "GachaInventoryWidget.h"
#include "CharacterStructure.h"
#include "GachaInventoryItemWidget.h"
#include "Components/ScrollBox.h"
#include "Kismet/GameplayStatics.h"
#include "GachaSaveGame.h"
#include "Engine/Engine.h"

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
        PopulateInventory(LoadedGame->SavedCharacters);
    }
    else if (InventoryScrollBox)
    {
        InventoryScrollBox->ClearChildren();
    }
}

void UGachaInventoryWidget::PopulateInventory(const TMap<FName, FCharacterProgress>& CharactersProgress)
{
    if (!InventoryScrollBox || !CharacterDataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("InventoryScrollBox ou CharacterDataTable est nul."));
        return;
    }

    InventoryScrollBox->ClearChildren();

    if (CharactersProgress.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Aucun personnage débloqué dans l'inventaire."));
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Aucun personnage débloqué !"));
        }
        return;
    }

    for (const auto& Pair : CharactersProgress)
    {
        FName CharacterRowName = Pair.Key;
        const FCharacterProgress& Progress = Pair.Value;

        UE_LOG(LogTemp, Log, TEXT("Personnage débloqué : %s, étoiles : %d"), *CharacterRowName.ToString(), Progress.StarCount);
        if (GEngine)
        {
            FString DebugMsg = FString::Printf(TEXT("Personnage débloqué : %s, étoiles : %d"), *CharacterRowName.ToString(), Progress.StarCount);
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, DebugMsg);
        }

        FCharacterStructure* CharacterData = CharacterDataTable->FindRow<FCharacterStructure>(CharacterRowName, TEXT(""));
        if (!CharacterData)
        {
            UE_LOG(LogTemp, Warning, TEXT("CharacterData introuvable pour %s"), *CharacterRowName.ToString());
            continue;
        }

        if (ItemWidgetClass)
        {
            UGachaInventoryItemWidget* EntryWidget = CreateWidget<UGachaInventoryItemWidget>(this, ItemWidgetClass);
            if (EntryWidget)
            {
                EntryWidget->InitializeWithData(*CharacterData, Progress);
                InventoryScrollBox->AddChild(EntryWidget);
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Ajouté un widget BP dans la ScrollBox"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("ItemWidgetClass n'est pas assigné !"));
        }
    }
}
