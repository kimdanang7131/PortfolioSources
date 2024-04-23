#include "Managers/UIManager.h"

// 정적 멤버 변수 초기화
TArray<class UCUIManagementComponent*> UIManager::UIComponents;
ACPlayerController* UIManager::PlayerController = nullptr;