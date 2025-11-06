// Fill out your copyright notice in the Description page of Project Settings.


#include "GachaCharacterShowcase.h"

#include "CharacterStructure.h"
#include "Components/SceneCaptureComponent2D.h"

// Sets default values
AGachaCharacterShowcase::AGachaCharacterShowcase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	CharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	CharacterMesh->SetupAttachment(RootComp);

	CharacterCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("CharacterCapture"));
	CharacterCapture->SetupAttachment(RootComp);
	

}

// Called when the game starts or when spawned
void AGachaCharacterShowcase::BeginPlay()
{
	Super::BeginPlay();

	CharacterCapture->ShowOnlyActorComponents(this);
	
}

// Called every frame
void AGachaCharacterShowcase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGachaCharacterShowcase::SetCharacterByRowName(FName RowName)
{
	if (!CharacterDataTable)
		return;

	FCharacterStructure* CharacterData = CharacterDataTable->FindRow<FCharacterStructure>(RowName, TEXT("AGachaCharacterShowcase::SetCharacterByRowName"));
	if (!CharacterData)
		return;

	if (CharacterData->CharacterMesh)
	{
		if (!CharacterMesh)
		{
			CharacterMesh = NewObject<USkeletalMeshComponent>(this);
			CharacterMesh->RegisterComponent();
			CharacterMesh->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		}

		CharacterMesh->SetSkeletalMesh(CharacterData->CharacterMesh);
	}
}

