// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ_BossAnim.h"
#include "BossFSM.h"

void UHJ_BossAnim::AnimNotify_EndAppear()
{
	UE_LOG(LogTemp, Warning, TEXT("1111111111111"));
	// 보스 위치 떨어진 위치로 셋팅

	// 보스 상태가 Appear 일때만
// 	FVector pos = pathArray[0]->GetActorLocation();
// 	pos.Z = 100;
// 	SetActorLocation(pos);

// 	if (bfsm->bState == EBossState::Appear)
// 	{
// 		// 보스FSM 에 Idle 로 상태를 바꿔라
// 		bfsm->bState = EBossState::Idle;
// 	}

}
