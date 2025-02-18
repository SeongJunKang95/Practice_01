#include "CoinItem.h"
#include "Engine/World.h"
#include "SpartaGameState.h"
#include "Kismet/GameplayStatics.h"


ACoinItem::ACoinItem()
{
	PointValue = 0;
	ItemType = "DefaulfCoin";
}

void ACoinItem::ActivateItem(AActor* Activator)
{

	if (Activator && Activator->ActorHasTag("Player"))

	{
		if (CoinItemSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				GetWorld(),
				CoinItemSound,
				GetActorLocation()
			);
		}

		if (UWorld* World = GetWorld())
		{
			if (ASpartaGameState* GameState = World->GetGameState<ASpartaGameState>())
			{
				GameState->AddScore(PointValue);
				GameState->OnCoinCollected();

			}
		}

	DestroyItem();

	}
}
