// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include <Kismet/GameplayStatics.h>
#include "Player_KYI.h"
#include "HJ_Enemy.h"
#include <Components/CapsuleComponent.h>
#include "HJ_EnemyAnim.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <Kismet/KismetMathLibrary.h>



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

	//���� �ʱ� ü���� ��������
	currHP = maxHP;

	//ó�� ��ġ ����
	originPos = me->GetActorLocation();

	//AIController
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
	case EEnemyState::ReturnPos:
		ReturnPosState();
		break;
	}
}
//��� ����
void UEnemyFSM::IdleState()
{
// 	//1. �ð��� �귶���ϱ� 
// 	currentTime += GetWorld()->DeltaTimeSeconds;
// 	//2. ���� ��� �ð��� ��� �ð��� �ʰ��ߴٸ�
// 	if (currentTime > idleDelayTime)
// 	{
// 	
// 		//3. �̵� ���·� ��ȯ�ϰ� �ʹ�.
// 		mState = EEnemyState::Move;
// 		//��� �ð� �ʱ�ȭ
// 		currentTime = 0;
// 
// 		//Idle �ִϸ��̼� ���
// 	    //int32 index = FMath::RandRange(0,1);
// 		//anim->PlayDamageAnim(TEXT("Move0"));
// 
// 
// 		//�ִϸ��̼� ���� ����ȭ 
// 		anim->animState = mState;
// 	}

//���࿡ Player �� �Ѿ� �� �� �ִ�? ( �� �þ߿� ���̸�)
	if (IsTargetTrace())
	{
		//���¸� Move �� ��ȯ
		mState = EEnemyState::Move;
		//�ִϸ��̼� ���� ����ȭ 
		anim->animState = mState;
	}

}
//�̵� ����
void UEnemyFSM::MoveState()
{
	//1. Ÿ���� ���ϴ� ������ ���ϰ�(target - me)
	FVector dir = target->GetActorLocation() - me->GetActorLocation();

	//1). ó�� ��ġ - ���� ��ġ �Ÿ��� ���Ѵ�.
	float dist = FVector::Distance(originPos, me->GetActorLocation());

	
	//2). ���࿡ dist �� moveRange ���� Ŀ���� (������ �� �ִ� �ݰ��� �Ѿ�ٸ�)
	if (dist > moveRange)
	{
		//3). ReturnPos ���·� ��ȯ 
		mState = EEnemyState::ReturnPos;
	}

	//Ÿ��� ��������� ���� ���·� ��ȯ�ϰ� �ʹ�.
	// 
	//1. ���� �Ÿ��� ���� ���� �ȿ� ������ 
	if (dir.Length() < attackRange)
	{
		//anim->StopAllMontages(currentTime);
		//2. ���� ���·� ��ȯ�ϰ� �ʹ�.
		mState = EEnemyState::Attack;

		//�ִϸ��̼� ���� ����ȭ
		anim->animState = mState;

		//���� �ִϸ��̼� ��� Ȱ��ȭ 
		anim->PlayDamageAnim(TEXT("Idle"));

		//���� ���� ��ȯ �� ��� �ð��� �ٷ� �������� ó��
		currentTime = attackDelayTime;
	}

	else
	{
		//3. �� �������� �̵��ϰ� �ʹ�.+ (����ȭ)������ �����ϵ� ũ�Ⱑ 1��ŭ�� ����?
		me->AddMovementInput(dir.GetSafeNormal());
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
		//���� �ִϸ��̼� �������� ����ϱ�
		int32 index = FMath::RandRange(0,3);
		FString sectionName = FString::Printf(TEXT("Attack%d"),index);
		anim->PlayDamageAnim(FName(*sectionName));

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
		//anim->PlayDamageAnim(TEXT("Move0"));
		//�ִϸ��̼� ���� ����ȭ
		anim->animState = mState;
	}
	
	
	
}

//�ǰ� �˸� �̺�Ʈ �Լ�
void UEnemyFSM::OnDamageProcess()
{
	//ü�� ����
	currHP--;
	//���� ü���� �����ִٸ� 
	if (currHP > 0)
	{
		//���¸� �ǰ����� ��ȯ
		mState = EEnemyState::Damage;

		//�÷��̾����� ������ �ڷ� �з�����
		FVector s = me->GetActorLocation() + (-me->GetActorForwardVector() * 100);
		me->SetActorLocation(s);
		currentTime = 0;

	}
	else
	{
		//���¸� �������� ��ȯ
		mState = EEnemyState::Die;
		//ĸ�� �浹ü ��Ȱ��ȭ
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		//���� �ִϸ��̼� ���
		anim->PlayDamageAnim(TEXT("Die0"));

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

		anim->PlayDamageAnim(TEXT("Idle0"));
		//�ִϸ��̼� ���� ����ȭ
		//anim->animState = mState;
	}



}
//���� ����
void UEnemyFSM::DieState()
{
	
	//���� ���� �ִϸ��̼��� ������ �ʾҴٸ� 
	//�ٴڱ��� �������� �ʵ��� ó��
	if ( anim->bDieDone == false )
	{
		//�÷��̾����� ������ �ڷ� �з�����
		FVector s = me->GetActorLocation() + (-me->GetActorForwardVector() * 5);
		me->SetActorLocation(s);
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

void UEnemyFSM::ReturnPosState()
{
	//1. �� ---> ó�� ��ġ�� ���ϴ� ������ ���Ѵ�.
	FVector dir = originPos - me->GetActorLocation();

	// 	3. ���࿡ ��--->ó�� ��ġ �Ÿ��� 10 ���� �۾�����
	if (dir.Length() < 10)
	{
		//4. idle ���·� ��ȯ
		mState = EEnemyState::Idle;
	}
	//�׷��� ������
	else
	{
		//�� �������� �̵��Ѵ�.
		me->AddMovementInput(dir.GetSafeNormal());
	}
}


//DamgeAnim0 �� ȣ�� �Լ�
void  UEnemyFSM::DamageAnim0()
{
	anim->PlayDamageAnim(TEXT("Damage0"));
	OnDamageProcess();
}
//DamgeAnim1 �� ȣ�� �Լ�
void  UEnemyFSM::DamageAnim1()
{
	anim->PlayDamageAnim(TEXT("Damage1"));
	OnDamageProcess();
}
//DamgeAnim2 �� ȣ�� �Լ�
void  UEnemyFSM::DamageAnim2()
{
	anim->PlayDamageAnim(TEXT("Damage2"));
	OnDamageProcess();
}
//DamgeAnim3 �� ȣ�� �Լ�
void  UEnemyFSM::DamageAnim3()
{
	anim->PlayDamageAnim(TEXT("Damage3"));
	OnDamageProcess();
}
//DamgeAnim4 �� ȣ�� �Լ�
void  UEnemyFSM::DamageAnim4()
{
	anim->PlayDamageAnim(TEXT("Damage4"));
	OnDamageProcess();
}

bool UEnemyFSM::IsTargetTrace()
{
	//1. ��---->�÷��̾ ���ϴ� ������ ���Ѵ� (Normalize)
	FVector dir = target->GetActorLocation() - me->GetActorLocation();

	//2. ���� �չ���� 1���� ���� ������ ����
	float dotvalue = FVector::DotProduct(me->GetActorForwardVector(), dir.GetSafeNormal());
	//3. 2������ ���� ���� Acos �Ѵ�.---> �� ������ ���̰�
	float degree = UKismetMathLibrary::DegAcos(dotvalue);

	//4. ���࿡ 3���� ���� ���� 30 ���� �۰� (�þ߰� 60)
	//�� - Ÿ�� ���� �Ÿ��� traceRange ���� ������
	if (degree < 30 && dir.Length() < traceRange)
	{
		//return true;

		//Enemy ----> targetLinTrace ���� !
		FHitResult hitInfo;
		FCollisionQueryParams param;
		param.AddIgnoredActor(me);

		bool bHit = GetWorld()->LineTraceSingleByChannel(
			hitInfo,
			me->GetActorLocation(), target->GetActorLocation(),
			ECollisionChannel::ECC_Visibility, param);

		//���࿡ �ε��� ���� �ִٸ�
		if (bHit)
		{
			//���࿡ �ε��� ���� �̸��� Player �� �����ϰ� �ִٸ�
			FString str = hitInfo.GetActor()->GetName();
			if (hitInfo.GetActor()->GetName().Contains(TEXT("Character")))
			{
				//true ��ȯ
				return true;
			}

		}

	}
	//false �� ��ȯ
	return false;

	
}