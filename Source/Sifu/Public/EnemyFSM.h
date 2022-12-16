// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

//사용자 상태 정의 
UENUM(BlueprintType)
enum class EEnemyState :uint8
{
	Idle,
	Move,
	Attack,
	Damage,
	Die,
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIFU_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	//상태 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)// VisibleAnywhere, BlueprintReadOnly, Category = FSM)
	EEnemyState mState = EEnemyState::Idle;

	//대기 상태
	void IdleState();
	//이동 상태
	void MoveState();
	//공격상태
	void AttackState();
	//피격 상태
	void DamageState();
	//죽음상태
	void DieState();

	//일정 시간 기다렸다가 이동 상태로 전환하고 싶다.
	//대기 시간
	UPROPERTY(EditDefaultsOnly,Category=FSM)
	float idleDelayTime = 2;
	//경과 시간
	float currentTime = 0;
	
	//타깃
//	UPROPERTY(VisibleAnywhere,Category = FSM)
//	class ATPSPl


};
