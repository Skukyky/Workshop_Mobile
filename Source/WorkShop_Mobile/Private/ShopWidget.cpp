// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopWidget.h"

void UShopWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (APlayerActor* Player = Cast<APlayerActor>(GetWorld()->GetFirstPlayerController()))
	{
		PlayerActor = Player;
	}
	//---------------ShopTab---------------
	if (BTN_Boutique)
	{
		BTN_Boutique->OnClicked.AddDynamic(this, &UShopWidget::OnBoutique_Clicked);
	}
	if (BTN_Offers)
	{
		BTN_Offers->OnClicked.AddDynamic(this, &UShopWidget::OnOffers_Clicked);
	}
	if (BTN_Boost)
	{
		BTN_Boost->OnClicked.AddDynamic(this, &UShopWidget::OnBoost_Clicked);
	}
	
	//---------------LeaveBTN---------------
	if (BTN_Leave)
	{
		BTN_Leave->OnClicked.AddDynamic(this, &UShopWidget::OnLeave_Clicked);
	}

	//---------------ShopPanel---------------
	if (BTN_Shop_1)
	{
		BTN_Shop_1->OnClicked.AddDynamic(this, &UShopWidget::OnShop_1_Clicked);
	}
	if (BTN_Shop_2)
	{
		BTN_Shop_2->OnClicked.AddDynamic(this, &UShopWidget::OnShop_2_Clicked);
	}
	if (BTN_Shop_3)
	{
		BTN_Shop_3->OnClicked.AddDynamic(this, &UShopWidget::OnShop_3_Clicked);
	}
	if (BTN_Shop_4)
	{
		BTN_Shop_4->OnClicked.AddDynamic(this, &UShopWidget::OnShop_4_Clicked);
	}
	if (BTN_Shop_5)
	{
		BTN_Shop_5->OnClicked.AddDynamic(this, &UShopWidget::OnShop_5_Clicked);
	}
	if (BTN_Shop_6)
	{
		BTN_Shop_6->OnClicked.AddDynamic(this, &UShopWidget::OnShop_6_Clicked);
	}

	//---------------OffersPanel---------------
	if (BTN_Offer_1)
	{
		BTN_Offer_1->OnClicked.AddDynamic(this, &UShopWidget::OnOffer_1_Clicked);
	}
	if (BTN_Offer_2)
	{
		BTN_Offer_2->OnClicked.AddDynamic(this, &UShopWidget::OnOffer_2_Clicked);
	}
	if (BTN_Offer_3)
	{
		BTN_Offer_3->OnClicked.AddDynamic(this, &UShopWidget::OnOffer_3_Clicked);
	}

	//---------------BoostPanel---------------
	if (BTN_Boost_1)
	{
		BTN_Boost_1->OnClicked.AddDynamic(this, &UShopWidget::OnBoost_1_Clicked);
	}
	if (BTN_Boost_2)
	{
		BTN_Boost_2->OnClicked.AddDynamic(this, &UShopWidget::OnBoost_2_Clicked);
	}
	if (BTN_Boost_3)
	{
		BTN_Boost_3->OnClicked.AddDynamic(this, &UShopWidget::OnBoost_3_Clicked);
	}
	if (BTN_Boost_4)
	{
		BTN_Boost_4->OnClicked.AddDynamic(this, &UShopWidget::OnBoost_4_Clicked);
	}
	if (BTN_Boost_5)
	{
		BTN_Boost_5->OnClicked.AddDynamic(this, &UShopWidget::OnBoost_5_Clicked);
	}
	if (BTN_Boost_6)
	{
		BTN_Boost_6->OnClicked.AddDynamic(this, &UShopWidget::OnBoost_6_Clicked);
	}
	if (BTN_Boost_7)
	{
		BTN_Boost_7->OnClicked.AddDynamic(this, &UShopWidget::OnBoost_7_Clicked);
	}
	
}

void UShopWidget::OnBoutique_Clicked()
{
	if (BoutiqueSwitcher)
	{
		const int32 IndexToSet = 0;
		BoutiqueSwitcher->SetActiveWidgetIndex(IndexToSet);
	}
}

void UShopWidget::OnOffers_Clicked()
{
	if (BoutiqueSwitcher)
	{
		const int32 IndexToSet = 1;
		BoutiqueSwitcher->SetActiveWidgetIndex(IndexToSet);
	}
}

void UShopWidget::OnBoost_Clicked()
{
	if (BoutiqueSwitcher)
	{
		const int32 IndexToSet = 2;
		BoutiqueSwitcher->SetActiveWidgetIndex(IndexToSet);
	}
}

void UShopWidget::OnLeave_Clicked()
{
	RemoveFromParent();
}

void UShopWidget::OnShop_1_Clicked()
{
	if (PlayerActor)
	{
		PlayerActor->SetGem(100);
	}
}

void UShopWidget::OnShop_2_Clicked()
{
	if (PlayerActor)
	{
		PlayerActor->SetGem(500);
	}
}

void UShopWidget::OnShop_3_Clicked()
{
	if (PlayerActor)
	{
		PlayerActor->SetGem(1100);
	}
}

void UShopWidget::OnShop_4_Clicked()
{
	if (PlayerActor)
	{
		PlayerActor->SetGem(2300);
	}
}

void UShopWidget::OnShop_5_Clicked()
{
	if (PlayerActor)
	{
		PlayerActor->SetGem(6400);
	}
}

void UShopWidget::OnShop_6_Clicked()
{
	if (PlayerActor)
	{
		PlayerActor->SetGem(14000);
	}
}

void UShopWidget::OnOffer_1_Clicked()
{
}

void UShopWidget::OnOffer_2_Clicked()
{
	if (PlayerActor)
	{
		PlayerActor->SetGem(1000);
	}
}

void UShopWidget::OnOffer_3_Clicked()
{
}

void UShopWidget::OnBoost_1_Clicked()
{
}

void UShopWidget::OnBoost_2_Clicked()
{
}

void UShopWidget::OnBoost_3_Clicked()
{
}

void UShopWidget::OnBoost_4_Clicked()
{
}

void UShopWidget::OnBoost_5_Clicked()
{
}

void UShopWidget::OnBoost_6_Clicked()
{
}

void UShopWidget::OnBoost_7_Clicked()
{
}
