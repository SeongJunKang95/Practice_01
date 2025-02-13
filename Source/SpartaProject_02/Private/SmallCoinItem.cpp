#include "SmallCoinItem.h"

ASmallCoinItem::ASmallCoinItem()
{
	PointValue = 50;
	ItemType = "SmallCoin";
}

void ASmallCoinItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
}