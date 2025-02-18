#include "HealingItem.h"
#include "SpartaCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AHealingItem::AHealingItem()
{
	HealAmount = 20.0f;
	ItemType = "Healing";
}

void AHealingItem::ActivateItem(AActor* Activator)
{
	if (Activator && Activator->ActorHasTag("Player"))

	{    UParticleSystemComponent* Particle = nullptr;

		if (HealingItemParticle)
		{
			
			Particle = UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				HealingItemParticle,
				GetActorLocation(),
				GetActorRotation(),
				false
			);
		}

		if (HealingItemSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				GetWorld(),
				HealingItemSound,
				GetActorLocation()
			);
		}


		if (ASpartaCharacter* PlayerCharacter = Cast<ASpartaCharacter>(Activator))
		{
			// 캐릭터의 체력을 회복
			PlayerCharacter->AddHealth(HealAmount);
		}

		DestroyItem();
		if (Particle)
		{
			FTimerHandle DestroyParticleTimerHandle;
			TWeakObjectPtr<UParticleSystemComponent> WeakParticle = Particle;
			GetWorld()->GetTimerManager().SetTimer(
				DestroyParticleTimerHandle,
				[WeakParticle]()
				{
					if (WeakParticle.IsValid())
					{
						WeakParticle->DestroyComponent();
					}
				},
				2.0f,
				false
			);
		}
        
	}
}