// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ_BossAnim.h"
#include "BossFSM.h"

void UHJ_BossAnim::AnimNotify_EndAppear()
{
	UE_LOG(LogTemp, Warning, TEXT("1111111111111"));
	// ���� ��ġ ������ ��ġ�� ����

	// ���� ���°� Appear �϶���
// 	FVector pos = pathArray[0]->GetActorLocation();
// 	pos.Z = 100;
// 	SetActorLocation(pos);

// 	if (bfsm->bState == EBossState::Appear)
// 	{
// 		// ����FSM �� Idle �� ���¸� �ٲ��
// 		bfsm->bState = EBossState::Idle;
// 	}

}
