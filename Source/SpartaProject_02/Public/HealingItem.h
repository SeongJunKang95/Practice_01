#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "HealingItem.generated.h"


UCLASS()
class SPARTAPROJECT_02_API AHealingItem : public ABaseItem
{
	GENERATED_BODY()
public:
	AHealingItem();


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Effects")
	UParticleSystem* HealingItemParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	USoundBase* HealingItemSound;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float HealAmount;


	virtual void ActivateItem(AActor* Activator) override;
};
