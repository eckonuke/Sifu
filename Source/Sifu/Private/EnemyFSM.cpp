// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include <Kismet/GameplayStatics.h>
#include "Player_KYI.h"
#include "HJ_Enemy.h"
#include <Components/CapsuleComponent.h>


// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	//���忡�� APlayer_KYI Ÿ�� ã�ƿ���
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), APlayer_KYI::StaticClass());
	//APlayer_KYI Ÿ������ ĳ����
	target = Cast<APlayer_KYI>(actor);
	//���� ��ü ��������
	me = Cast<AHJ_Enemy>(GetOwner());
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (mState)
	{
	case EEnemyState::Idle:
		IdleState();
		break;
	case EEnemyState::Move:
		MoveState();
		break;
	case EEnemyState::Attack:
		AttackState();
		break;
	case EEnemyState::Damage:
		DamageState();
		break;
	case EEnemyState::Die:
		DieState();
		break;
	}
}
//��� ����
void UEnemyFSM::IdleState()
{
	//1. �ð��� �귶���ϱ� 
	currentTime += GetWorld()->DeltaTimeSeconds;
	//2. ���� ��� �ð��� ��� �ð��� �ʰ��ߴٸ�
	if (currentTime > idleDelayTime)
	{
		//3. �̵� ���·� ��ȯ�ϰ� �ʹ�.
		mState = EEnemyState::Move;
		//��� �ð� �ʱ�ȭ
		currentTime = 0;
	}
}
//�̵� ����
void UEnemyFSM::MoveState()
{
	//1.Ÿ�� �������� �ʿ��ϴ�.
	FVector destination = target->GetActorLocation();
	//2.������ �ʿ��ϴ�.
	FVector dir = destination - me->GetActorLocation();
	me->AddMovementInput(dir.GetSafeNormal());

	//Ÿ��� ��������� ���� ���·� ��ȯ�ϰ� �ʹ�.
	//1. ���� �Ÿ��� ���� ���� �ȿ� ������ 
	if (dir.Size() < attackRange)
	{
		//2. ���� ���·� ��ȯ�ϰ� �ʹ�.
		mState = EEnemyState::Attack;
	}
}
//���� ����
void UEnemyFSM::AttackState()
{
	//��ǥ: ���� �ð��� �� ���� �����ϰ� �ʹ�.
	//1. �ð��� �귯�� �Ѵ�.
	currentTime += GetWorld()->DeltaTimeSeconds;
	//2. ���� �ð��� �����ϱ�
	if (currentTime > attackDelayTime)
	{
		//3. �����ϰ� �ʹ�.
		UE_LOG(LogTemp, Warning, TEXT("Attack!!!"));
		target->OnHitDamage();

		// ��� �ð� �ʱ�ȭ
		currentTime = 0;
	}
	//��ǥ: Ÿ���� ���� ������ ����� ���¸� �̵����� ��ȯ�ϰ� �ʹ�.
	//1. Ÿ����� �Ÿ��� �ʿ��ϴ�.
	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());

	//2. Ÿ����� �Ÿ��� ���ݹ����� ������ϱ�
	if (distance > attackRange)
	{
		//3. ���¸� �̵����� ��ȯ�ϰ� �ʹ�.
		mState = EEnemyState::Move;
	}
}

//�ǰ� �˸� �̺�Ʈ �Լ�
void UEnemyFSM::OnDamageProcess()
{
	//ü�� ����
	hp--;
	//���� ü���� �����ִٸ� 
	if (hp > 0)
	{
		//���¸� �ǰ����� ��ȯ
		mState = EEnemyState::Damage;
	}
	else
	{
		//���¸� �������� ��ȯ
		mState = EEnemyState::Die;
		//ĸ�� �浹ü ��Ȱ��ȭ
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	//me->Destroy();
}
//�ǰ� ����
void UEnemyFSM::DamageState()
{
	//1. �ð��� �귶���ϱ� 
	currentTime += GetWorld()->DeltaRealTimeSeconds;
	//2. ���� ��� �ð��� ��� �ð��� �ʰ��ߴٸ�
	if (currentTime > damageDelayTime)
	{
		//3. ��� ���·� ��ȯ�ϰ� �ʹ�.
		mState = EEnemyState::Idle;
		//��� �ð� �ʱ�ȭ
		currentTime = 0;
	}

}
//���� ����
void UEnemyFSM::DieState()
{
	//��� �Ʒ��� �������� �ʹ�. p=p0+vt
	FVector p0 = me->GetActorLocation();
	FVector p = p0 + FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
	me->SetActorLocation(p);

	//1. ���� 2���� �̻� �����Դٸ�
	if (p.Z < -200.0f)
	{
		//2. ���Ž�Ų��
		me->Destroy();
	}
}