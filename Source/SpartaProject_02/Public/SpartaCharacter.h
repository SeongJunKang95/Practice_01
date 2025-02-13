// Fill out your copyright notice in the Description page of Project Settings.#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpartaCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
// Enhanced Input에서 액션 값을 받을 때 사용하는 구조체
struct FInputActionValue;

UCLASS()
class SPARTAPROJECT_02_API ASpartaCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASpartaCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camear")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camear")
	UCameraComponent* CameraComp;

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(float Amount);

protected:
	// 최대 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;
	// 현재 체력
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;

	// 입력 바인딩을 처리할 함수
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 데미지 처리 함수 - 외부로부터 데미지를 받을 때 호출됨
// 또는 AActor의 TakeDamage()를 오버라이드
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


	// IA_Move와 IA_Jump 등을 처리할 함수 원형
// Enhanced Input에서 액션 값은 FInputActionValue로 전달됩니다.
	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);

	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual void OnDeath();

private:
	// 이동 속도 관련 프로퍼티들
	
	float NormalSpeed; // 기본 걷기 속도
	
	float SprintSpeedMultiplier;  // "기본 속도" 대비 몇 배로 빠르게 달릴지 결정
	
	float SprintSpeed; 	// 실제 스프린트 속도
    
};

