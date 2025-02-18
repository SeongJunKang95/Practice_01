#include "BaseItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"



ABaseItem::ABaseItem()
{
 	
	PrimaryActorTick.bCanEverTick = false;

	// 루트 컴포넌트 생성 및 설정
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	// 충돌 컴포넌트 생성 및 설정
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	// 겹침만 감지하는 프로파일 설정
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	// 루트 컴포넌트로 설정
	Collision->SetupAttachment(Scene);

	// 스태틱 메시 컴포넌트 생성 및 설정
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Collision);
	// 메시가 불필요하게 충돌을 막지 않도록 하기 위해, 별도로 NoCollision 등으로 설정할 수 있음.

	//이벤트 바인딩
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnItemOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &ABaseItem::OnItemEndOverlap);
}




// 플레이어가 이 아이템의 범위에 들어왔을 때 호출
void ABaseItem::OnItemOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Overlap!!!")));
		// 아이템 사용 (획득) 로직 호출
		ActivateItem(OtherActor);
	}
}

// 플레이어가 이 아이템의 범위를 벗어났을 때 호출
void ABaseItem::OnItemEndOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OverlapActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodtIndex)
{
}

void ABaseItem::ActivateItem(AActor* Activator)
{
	UParticleSystemComponent* Particle = nullptr;

	if (PickupParticle)
	{
		Particle = UGameplayStatics::SpawnEmitterAtLocation(
		 GetWorld(),
		 PickupParticle,
		 GetActorLocation(),
		 GetActorRotation(),
		 true
       );
	}
		
	if (PickupSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			PickupSound,
			GetActorLocation()
		);
	}
	/*if (Particle)
	{
		FTimerHandle DestroyParticleTimerHandle;

		GetWorld()->GetTimerManager().SetTimer(
			DestroyParticleTimerHandle,
			[Particle]()
			{
			// Particle이 유효하다고 가정하고 바로 삭제
				Particle->DestroyComponent();
			},
			2.0f,
			false
		);
	}*/

	// TWeakObjectptr을 사용한 이유는 위 코드 사용을 해서 진행을 해보니깐 지뢰, 힐링 포션이 사라지는 타이밍에 레벨라운드가 바뀌면 게임이 꺼져버리는 현상이 발생
	// 이유를 찾아보니 위 코드는 (Particle이 타이머가 실행되기 전에 삭제될 경우 dangling pointer(미사용 메모리를 참조하는 포인터) 문제가 발생할 가능성이 있다고 한다)
	// 그래서  TWeakObjectPtr을 사용하여 Particle의 유효성을 확인한 후에만 삭제하도록 처리하여 안전성을 높였습니다.


	if (Particle) // Particle이 nullptr이 아닌 경우 실행
	{
		FTimerHandle DestroyParticleTimerHandle;
		// Particle을 약한 참조(Weak Pointer)로 저장
		TWeakObjectPtr<UParticleSystemComponent> WeakParticle = Particle;
		GetWorld()->GetTimerManager().SetTimer(
			DestroyParticleTimerHandle,
			[WeakParticle]() // 약한 참조를 캡처하여 사용
			{
				if (WeakParticle.IsValid()) // Particle이 아직 유효한지 확인
				{
					// 유효한 경우에만 삭제
					WeakParticle->DestroyComponent();
				}
			},
			2.0f,
			false
		);
	}

}

FName ABaseItem::GetItemType() const
{
	return ItemType;
}
// 아이템을 파괴(제거)하는 함수
void ABaseItem::DestroyItem()
{
	// AActor에서 제공하는 Destroy() 함수로 객체 제거
	Destroy();
}
