// Fill out your copyright notice in the Description page of Project Settings.


#include "SpartaPlayerController.h"
#include "SpartaGameInstance.h"
#include "EnhancedInputSubsystems.h" // Enhanced Input System의 Local Player Subsystem을 사용하기 위해 포함
#include "SpartaGameState.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"


ASpartaPlayerController::ASpartaPlayerController()
    : InputMappingContext(nullptr),
    MoveAction(nullptr),
    JumpAction(nullptr),
    LookAction(nullptr),
    SprintAction(nullptr),
    HUDWidgetClass(nullptr),
    HUDWidgetInstance(nullptr),
    MainMenuWidgetClass(nullptr),
    MainMenuWidgetInstance(nullptr)
{
}

void ASpartaPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // 현재 PlayerController에 연결된 Local Player 객체를 가져옴    
    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    {
        // Local Player에서 EnhancedInputLocalPlayerSubsystem을 획득
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            if (InputMappingContext)
            {
                // Subsystem을 통해 우리가 할당한 IMC를 활성화
                // 우선순위(Priority)는 0이 가장 높은 우선순위
                Subsystem->AddMappingContext(InputMappingContext, 0);
            }
        }
    }

    //게임 실행 시 메뉴 레벨에서 메뉴 UI 먼저 표시
    FString CurrentMapName = GetWorld()->GetMapName();
    if (CurrentMapName.Contains("MenuLevel"))
    {
        ShowMainMenu(false);
    }

}

UUserWidget* ASpartaPlayerController::GetHUDWidget() const
{
    return HUDWidgetInstance;
}

//메뉴 UI 표시
void ASpartaPlayerController::ShowMainMenu(bool bIsRestart)
{
    //HUD가 켜져 있다면 닫기
    if (HUDWidgetInstance)
    {
        HUDWidgetInstance->RemoveFromParent();
        HUDWidgetInstance = nullptr;
    }
    //이미 메뉴가 떠 있으면 제거
    if (MainMenuWidgetInstance)
    {
        MainMenuWidgetInstance->RemoveFromParent();
        MainMenuWidgetInstance = nullptr;
    }
    //메뉴 UI 생성
    if (MainMenuWidgetClass)
    {
        MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
        if (MainMenuWidgetInstance)
        {
            MainMenuWidgetInstance->AddToViewport();

            bShowMouseCursor = true; // 마우스 포커스가 UI에서만 보이게 하기
            SetInputMode(FInputModeUIOnly()); // Ui 만 사용할 수 있도록 처리
        }
        if (UTextBlock* ButtonText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("StartButtonText"))))
        {
           if (bIsRestart)
           {
             ButtonText->SetText(FText::FromString(TEXT("Restart")));
           }
           else
           {
             ButtonText->SetText(FText::FromString(TEXT("Start")));
           }
        }

    }
   
}

// 게임 HUD 표시
void ASpartaPlayerController::ShowGameHUD()
{
    // HUD가 켜져 있다면 닫기
    if (HUDWidgetInstance)
    {
        HUDWidgetInstance->RemoveFromParent();
        HUDWidgetInstance = nullptr;
    }
    //이미 메뉴가 떠 있으면 제거
    if (MainMenuWidgetInstance)
    {
        MainMenuWidgetInstance->RemoveFromParent();
        MainMenuWidgetInstance = nullptr;
    }
    if (HUDWidgetClass)
    {
        HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
        if (HUDWidgetInstance)
        {
            HUDWidgetInstance->AddToViewport();

            bShowMouseCursor = false;
            SetInputMode(FInputModeGameOnly());

            ASpartaGameState* SpartaGameState = GetWorld() ? GetWorld()->GetGameState<ASpartaGameState>() : nullptr;
            if (SpartaGameState)
            {
                SpartaGameState->UpdateHUD(); // 업데이트해서 갱신 시켜주겠다.
            }
            
        }
    }
}

// 게임 시작 - BasicLevel 오픈, GameInstance 데이터 리셋
void ASpartaPlayerController::StartGame()
{
    if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(UGameplayStatics::GetGameInstance(this)))
    {
        SpartaGameInstance->CurrentLevelIndex = 0;
        SpartaGameInstance->TotalScore = 0;

        UGameplayStatics::OpenLevel(GetWorld(), FName("BasicLevel"));
          
    }
}