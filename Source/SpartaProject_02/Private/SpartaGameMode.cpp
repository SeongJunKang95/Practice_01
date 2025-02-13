#include "SpartaGameMode.h"
#include "SpartaCharacter.h"
#include "SpartaPlayerController.h" // PlayerController 클래스를 사용
#include "SpartaGameState.h"


ASpartaGameMode::ASpartaGameMode()
{
    DefaultPawnClass = ASpartaCharacter::StaticClass();
    PlayerControllerClass = ASpartaPlayerController::StaticClass();

    // 우리가 만든 GameState로 설정
    GameStateClass = ASpartaGameState::StaticClass();
}
