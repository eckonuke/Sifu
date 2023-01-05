// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include <Kismet/GameplayStatics.h>
#include "Player_KYI.h"
#include "HJ_Enemy.h"
#include <Components/CapsuleComponent.h>
#include "HJ_EnemyAnim.h"
#include <GameFramework/CharacterMovementComponent.h>



// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


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
	me->GetCharacterMovement()->MaxWalkSpeed = 300;
	//UEnemyAnim* �Ҵ�
	anim = Cast<UHJ_EnemyAnim>(me->GetMesh()->GetAnimInstance());

	//compProjectile=CreateDefaultSubobject< UProjectileMovementComponent>(TEXT("compProject"));

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

		//Idle �ִϸ��̼� ���
	    //int32 index = FMath::RandRange(0,1);
		anim->PlayDamageAnim(TEXT("Move0"));


		//�ִϸ��̼� ���� ����ȭ 
		anim->animState = mState;
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
		anim->StopAllMontages(currentTime);
		//2. ���� ���·� ��ȯ�ϰ� �ʹ�.
		mState = EEnemyState::Attack;

		//�ִϸ��̼� ���� ����ȭ
		anim->animState = mState;
		 
		//���� �ִϸ��̼� ��� Ȱ��ȭ 
		anim->PlayDamageAnim(TEXT("Idle0"));

		//���� ���� ��ȯ �� ��� �ð��� �ٷ� �������� ó��
		currentTime = attackDelayTime;



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
		target->HurtAnim0();
		target->OnHitDamage();

		// ��� �ð� �ʱ�ȭ
		currentTime = 0;
// 		int32 index = FMath::RandRange(1,3);
// 		FString sectionName = FString::Printf(TEXT("Fight%d"),index);
// 		anim->PlayDamageAnim(TEXT("Fight"));
		anim->PlayDamageAnim(TEXT("Fight2"));


		//anim->bAttackPlay = true;
	}
	//��ǥ: Ÿ���� ���� ������ ����� ���¸� �̵����� ��ȯ�ϰ� �ʹ�.
	//1. Ÿ����� �Ÿ��� �ʿ��ϴ�.
	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());

	//2. Ÿ����� �Ÿ��� ���ݹ����� ������ϱ�
	if (distance > attackRange)
	{
		//3. ���¸� �̵����� ��ȯ�ϰ� �ʹ�.
		mState = EEnemyState::Move;
		anim->PlayDamageAnim(TEXT("Move0"));
		//�ִϸ��̼� ���� ����ȭ
		anim->animState = mState;
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

		//�÷��̾����� ������ �ڷ� �з�����
		FVector s = me->GetActorLocation()+(-me->GetActorForwardVector()*200);
		me->SetActorLocation(s);
		currentTime = 0;

		//�ǰ� �ִϸ��̼� ���
		anim->PlayDamageAnim(TEXT("Damage0"));
 		//int32 index = FMath::RandRange(0,1);
 		//FString sectionName = FString::Printf(TEXT("Damage%d"),index);
 		//anim->PlayDamageAnim(FName(*sectionName));
	}
	else
	{
		//���¸� �������� ��ȯ
		mState = EEnemyState::Die;
		//ĸ�� �浹ü ��Ȱ��ȭ
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		//���� �ִϸ��̼� ���
		anim->PlayDamageAnim(TEXT("Die"));

		////Movement �� �������� �� Component �� ����
		////AHJ_Enemy* compProjectile = Cast <AHJ_Enemy>(me->GetMesh());

		//me->compProjectile->SetUpdatedComponent(me->GetMesh());
		////�ʱ� �ӷ�
		//me->compProjectile->InitialSpeed = 2000;
		////�ִ� �ӷ�
		//me->compProjectile->MaxSpeed = 2000;
		////�ݵ� ����(ƨ��� ����)
		//me->compProjectile->bShouldBounce = true;
		////�󸶳� �� ƨ��� �� ������(0 ~ 1)
		//me->compProjectile->Bounciness = 1.0f;
		
	}
	//�ִϸ��̼� ���� ����ȭ
	anim->animState = mState;
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

		anim->PlayDamageAnim(TEXT("Fight0"));
		//�ִϸ��̼� ���� ����ȭ
		anim->animState = mState;
	}

	//p=p0+vt;
	//FVector p0 = me -> GetActorLocation();
	// 
	//FVector vt = -me-> GetActorForwardVector();
	// 
	//FVector p = p0 + vt * speed * GetWorld()->DeltaRealTimeSeconds;
	// 
	//me->SetActorLocation(p);


}
//���� ����
void UEnemyFSM::DieState()
{

	//���� ���� �ִϸ��̼��� ������ �ʾҴٸ� 
	//�ٴڱ��� �������� �ʵ��� ó��
	if ( anim->bDieDone == false )
	{
		return;
	}

	
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

