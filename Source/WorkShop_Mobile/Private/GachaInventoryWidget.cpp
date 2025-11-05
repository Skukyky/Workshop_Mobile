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
        PopulateInventory(LoadedGame->SavedCharactersArray);  // Tableau à la place de TMap
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
        UE_LOG(LogTemp, Warning, TEXT("InventoryScrollBox ou CharacterDataTable est nul."));
        return;
    }

    InventoryScrollBox->ClearChildren();

    if (CharactersInventory.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Aucun personnage débloqué dans l'inventaire."));
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Aucun personnage débloqué !"));
        }
        return;
    }

    for (const FCharacterProgress& Progress : CharactersInventory)
    {
        const FName& CharacterRowName = Progress.CharacterID;

        UE_LOG(LogTemp, Log, TEXT("Personnage débloqué : %s, étoiles : %d"), *CharacterRowName.ToString(), Progress.StarCount);
        if (GEngine)
        {
            FString DebugMsg = FString::Printf(TEXT("Personnage débloqué : %s, étoiles : %d"), *CharacterRowName.ToString(), Progress.StarCount);
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, DebugMsg);
        }

        FCharacterStructure* CharacterData = CharacterDataTable->FindRow<FCharacterStructure>(CharacterRowName, TEXT("GachaInventoryWidget::PopulateInventory"));
        if (!CharacterData)
        {
            UE_LOG(LogTemp, Warning, TEXT("CharacterData introuvable pour %s"), *CharacterRowName.ToString());
            continue;
        }

        if (!ItemWidgetClass)
        {
            UE_LOG(LogTemp, Error, TEXT("ItemWidgetClass n'est pas assigné !"));
            continue;
        }

        UGachaInventoryItemWidget* EntryWidget = CreateWidget<UGachaInventoryItemWidget>(this, ItemWidgetClass);
        if (!EntryWidget)
        {
            UE_LOG(LogTemp, Warning, TEXT("Echec de création du widget pour %s"), *CharacterRowName.ToString());
            continue;
        }

        EntryWidget->InitializeWithData(*CharacterData, Progress);
        InventoryScrollBox->AddChild(EntryWidget);
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Ajouté %s dans la ScrollBox"), *CharacterRowName.ToString()));
        }
    }
}
