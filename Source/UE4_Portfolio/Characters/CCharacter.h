#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Character.h"
#include "Enums/CEStateType.h"

#include "CCharacter.generated.h"

UCLASS()
class UE4_PORTFOLIO_API ACCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	/** StateComponent를 통해 State가 바뀔때마다 Delegate를 통해 Character안의 함수를 실행 */
protected:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

protected:
	UPROPERTY(VisibleDefaultsOnly)
		class UCInventoryComponent* Inventory;

	UPROPERTY(VisibleDefaultsOnly)
		class UCPostprocessComponent* Postprocess;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State;

	/** EStateType에 따라 등록할 수 있도록 일단 nullptr로 등록해놓을 예정 */
	UPROPERTY(EditDefaultsOnly , Category = "StateDataAsset")
		TMap<EStateType, class UCMontageDataAsset*> StateMontageMap;

	/** 자식들이 재정의해서 사용할 함수들 , DataAsset State에 등록된 애니메이션 실행 */
protected:
	virtual void Hitted() {};
	virtual void Dead();
	virtual void Dodging() {};

protected:
	/** Dead할 시 Destroyed에서 자식들 모두 파괴후 본체 파괴 */
	virtual void Destroyed();
};
