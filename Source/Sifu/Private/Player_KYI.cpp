// Fiout your copyright notice in the Description page of Project Settings.


#include "Player_KYI.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <Components/SphereComponent.h>
#include "HJ_Enemy.h"
#include "EnemyFSM.h"
#include <Engine/SkeletalMesh.h>
#include <GameFramework/Character.h>
#include <Kismet/GameplayStatics.h>
#include <Components/CapsuleComponent.h>

#include <Components/SphereComponent.h>
#include "HJ_Enemy.h"
#include "EnemyFSM.h"
#include <Engine/SkeletalMesh.h>
#include <GameFramework/Character.h>
#include <Kismet/GameplayStatics.h>
#include <Components/CapsuleComponent.h>
#include <Animation/AnimMontage.h>

// Sets default values
APlayer_KYI::APlayer_KYI()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/Mannequin/Character/Mesh/UE4_Mannequin.UE4_Mannequin'"));
	if (tempMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	//springArm ������Ʈ ���̱�
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0, 70, 90));
	springArmComp->TargetArmLength = 400;
	springArmComp->bUsePawnControlRotation = true;

	//Camera ������Ʈ ���̱�
	camComp = CreateDefaultSubobject<UCameraComponent>(TEXT("camComp"));
	camComp->SetupAttachment(springArmComp);
	camComp->bUsePawnControlRotation = false;
	bUseControllerRotationYaw = true;

	//�ִϸ��̼� �������Ʈ ��������
	ConstructorHelpers::FClassFinder<UPlayerAnim> tempAnim(TEXT("AnimBlueprint'/Game/Mannequin/Animations/ThirdPerson_AnimBP.ThirdPerson_AnimBP_C'"));
	if (tempAnim.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(tempAnim.Class);
	}

	//�ǰ� �ִϸ��̼�
	ConstructorHelpers::FObjectFinder<UAnimMontage> tempStomach(TEXT("AnimMontage'/Game/Mannequin/Animations/h2H_Anim/Big_Stomach_Hit_Montage.Big_Stomach_Hit_Montage'"));
	if (tempStomach.Succeeded()) {
		stomach = tempStomach.Object;
	}
	ConstructorHelpers::FObjectFinder<UAnimMontage> tempHead2(TEXT("AnimMontage'/Game/Mannequin/Animations/h2H_Anim/Head_Hit_2_Montage.Head_Hit_2_Montage'"));
	if (tempHead2.Succeeded()) {
		head2 = tempHead2.Object;
	}
	ConstructorHelpers::FObjectFinder<UAnimMontage> tempHead3(TEXT("AnimMontage'/Game/Mannequin/Animations/h2H_Anim/Head_Hit_3_Montage.Head_Hit_3_Montage'"));
	if (tempHead3.Succeeded()) {
		head3 = tempHead3.Object;
	}
	ConstructorHelpers::FObjectFinder<UAnimMontage> tempHead4(TEXT("AnimMontage'/Game/Mannequin/Animations/h2H_Anim/Head_Hit_4_Montage.Head_Hit_4_Montage'"));
	if (tempHead4.Succeeded()) {
		head4 = tempHead4.Object;
	}
	ConstructorHelpers::FObjectFinder<UAnimMontage> tempFallDown(TEXT("AnimMontage'/Game/Mannequin/Animations/h2H_Anim/Sweep_Fall_Montage.Sweep_Fall_Montage'"));
	if (tempFallDown.Succeeded()) {
		falldown = tempFallDown.Object;
	}
	//����
	ConstructorHelpers::FObjectFinder<UAnimMontage> tempDeath(TEXT("AnimMontage'/Game/Mannequin/Animations/h2H_Anim/Death.Death'"));
	if (tempDeath.Succeeded()) {
		death = tempDeath.Object;
	}
	//���
	ConstructorHelpers::FObjectFinder<UAnimMontage> tempBlock(TEXT("AnimMontage'/Game/Mannequin/Animations/h2H_Anim/Block_Hit_Montage.Block_Hit_Montage'"));
	if (tempBlock.Succeeded()) {
		block = tempBlock.Object;
	}
	//���� �ִϸ��̼� �ָ�
	ConstructorHelpers::FObjectFinder<UAnimMontage> tempPunch(TEXT("AnimMontage'/Game/Mannequin/Animations/h2H_Anim/regular-punch.regular-punch'"));
	if (tempPunch.Succeeded()) {
		punch = tempPunch.Object;
	}
	ConstructorHelpers::FObjectFinder<UAnimMontage> tempJab(TEXT("AnimMontage'/Game/Mannequin/Animations/h2H_Anim/Lead_Jab_Montage.Lead_Jab_Montage'"));
	if (tempJab.Succeeded()) {
		jab = tempJab.Object;
	}
	ConstructorHelpers::FObjectFinder<UAnimMontage> tempUpper(TEXT("AnimMontage'/Game/Mannequin/Animations/h2H_Anim/Uppercut_Montage.Uppercut_Montage'"));
	if (tempUpper.Succeeded()) {
		uppercut = tempUpper.Object;
	}
	//���� �ִϸ��̼� ������
	ConstructorHelpers::FObjectFinder<UAnimMontage> tempKick(TEXT("AnimMontage'/Game/Mannequin/Animations/h2H_Anim/kick-regular_Montage.kick-regular_Montage'"));
	if (tempKick.Succeeded()) {
		kick = tempKick.Object;
	}
	ConstructorHelpers::FObjectFinder<UAnimMontage> tempHighKick(TEXT("AnimMontage'/Game/Mannequin/Animations/h2H_Anim/kick-up_Montage.kick-up_Montage'"));
	if (tempHighKick.Succeeded()) {
		highKick = tempHighKick.Object;
	}
	ConstructorHelpers::FObjectFinder<UAnimMontage> tempLowKick(TEXT("AnimMontage'/Game/Mannequin/Animations/h2H_Anim/Leg_Sweep_Montage.Leg_Sweep_Montage'"));
	if (tempLowKick.Succeeded()) {
		lowKick = tempLowKick.Object;
	}
}

// Called when the game starts or when spawned
void APlayer_KYI::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void APlayer_KYI::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (movementEnabled) {
		direction = FTransform(GetControlRotation()).TransformVector(direction);
		AddMovementInput(direction);
		direction = FVector::ZeroVector;
	}
}

void APlayer_KYI::NotifyActorBeginOverlap(AActor* OtherActor) {
	Super::NotifyActorBeginOverlap(OtherActor);
}

// Called to bind functionality to input
void APlayer_KYI::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Turn Right / Left Mouse"), this, &APlayer_KYI::Turn);
	PlayerInputComponent->BindAxis(TEXT("Look Up / Down Mouse"), this, &APlayer_KYI::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Move Right / Left"), this, &APlayer_KYI::InputHorizontal);
	PlayerInputComponent->BindAxis(TEXT("Move Forward / Backward"), this, &APlayer_KYI::InputVertical);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &APlayer_KYI::InputJump);
	PlayerInputComponent->BindAction(TEXT("Punch"), IE_Pressed, this, &APlayer_KYI::AttackPunch);
	PlayerInputComponent->BindAction(TEXT("Kick"), IE_Pressed, this, &APlayer_KYI::AttackKick);
	DECLARE_DELEGATE_OneParam(FBlock, bool);
	PlayerInputComponent->BindAction<FBlock>(TEXT("Block"), IE_Pressed, this, &APlayer_KYI::PlayerBlock, true);
	PlayerInputComponent->BindAction<FBlock>(TEXT("Block"), IE_Released, this, &APlayer_KYI::PlayerBlock, false);
}

void APlayer_KYI::Turn(float value) {
	AddControllerYawInput(value);
}
void APlayer_KYI::LookUp(float value) {
	AddControllerPitchInput(value);
}
void APlayer_KYI::InputHorizontal(float value) {
	direction.Y = value;
}
void APlayer_KYI::InputVertical(float value) {
	direction.X = value;
}
void APlayer_KYI::InputJump() {
	Jump();
}

void APlayer_KYI::setTarget() {
	FVector startPos = camComp->GetComponentLocation();
	FVector endPos = startPos + camComp->GetForwardVector()*5000;
	FHitResult hitinfo;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	bool bHit = GetWorld()->LineTraceSingleByChannel(hitinfo, startPos, endPos, ECC_Visibility, params);
	if (bHit) {
		UE_LOG(LogTemp, Warning, TEXT("Hit Something"));
		AHJ_Enemy* enem = Cast<AHJ_Enemy>(hitinfo.GetActor());
		if (enem) {
			targetEnemy = enem;
			UE_LOG(LogTemp,Warning, TEXT("%s"), *targetEnemy->GetName());
		}
	}
}

//���� ���
void APlayer_KYI::PlayerBlock(bool value) {
	if(!isDead) 
		isBlocking = value;
}

//�÷��̾ ������ �޾Ҵ�
void APlayer_KYI::OnHitDamage() {
	if (!isDead) {
		if (!isBlocking) {
			//ü�� ����
			hp--;
			//���࿡ ü���� ���ٸ�
			if (hp <= 0) {
				isDead = true;
				//���¸� �������� ��ȯ
				PlayerDie();
				//ĸ�� �浹ü ��Ȱ��ȭ
				GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				//GetComponentByClass(UCapsuleComponent::StaticClass())
			}
		}
	}
}

//�÷��̾ ���� Ÿ��
void APlayer_KYI::PlayerDamage() {
	TArray<AActor*> enemys;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHJ_Enemy::StaticClass(), enemys);
	for (int i = 0; i < enemys.Num(); i++)
	{
		//�Ÿ� ��� (Player - enemy)
		FVector v = GetActorLocation() - enemys[i]->GetActorLocation();
		float dist = v.Length();
		if (dist < 300) {
			AHJ_Enemy* e = Cast<AHJ_Enemy>(enemys[i]);
			e->fsm->OnDamageProcess();
		}
	}
}

//�÷��̾� ����
void APlayer_KYI::PlayerDie()
{
	PlayAnimMontage(death);
}

void APlayer_KYI::AttackPunch() {
	movementEnabled = false;
	kickorPunch = true;
	if (IsAttacking) {
		saveAttack = true;
	} else{
		IsAttacking = true;
		punchCombo();
	}
}

void APlayer_KYI::AttackKick() {
	movementEnabled = false;
	kickorPunch = false;
	if (IsAttacking) {
		saveAttack = true;
	}
	else {
		IsAttacking = true;
		kickCombo();
	}
}

//�÷��̾� ���� �޺�
void APlayer_KYI::saveAttackCombo() {
	if (saveAttack) {
		saveAttack = false;
		if (kickorPunch) {
			punchCombo();
		}
		else {
			kickCombo();
		}
	}
}

//�÷��̾� �޺� �ִϸ��̼� ����ġ
void APlayer_KYI::punchCombo() {
	switch (punchCount)
	{
		case 0:
		punchCount = 1;
		PlayerDamage();
		PlayAnimMontage(punch);
		break;
		case 1:
		punchCount = 2;
		PlayerDamage();
		PlayAnimMontage(jab);
		break;
		case 2:
		punchCount = 0;
		PlayerDamage();
		PlayAnimMontage(uppercut);
		break;
	}
}
void APlayer_KYI::kickCombo() {
	switch (kickCount)
	{
	case 0:
		kickCount = 1;
		PlayerDamage();
		PlayAnimMontage(kick);
		break;
	case 1:
		kickCount = 2;
		PlayerDamage();
		PlayAnimMontage(highKick);
		break;
	case 2:
		kickCount = 0;
		PlayerDamage();
		PlayAnimMontage(lowKick);
		break;
	}
}
//�÷��̾� �����޺� �缳��
void APlayer_KYI::ResetCombo() {
	IsAttacking = false;
	punchCount = 0;
	saveAttack = false;
	kickCount = 0;
	movementEnabled = true;
}

//Stomach hit
void APlayer_KYI::HurtAnim0() {
	if (!isDead) {
	PlayAnimMontage(stomach);
	OnHitDamage();
	ResetCombo();
	}
}
//head hit2
void APlayer_KYI::HurtAnim1() {
	if (!isDead) {
	PlayAnimMontage(head2);
	OnHitDamage();
	ResetCombo();
	}
}
//head hit3
void APlayer_KYI::HurtAnim2() {
	if (!isDead) {
	PlayAnimMontage(head3);
	OnHitDamage();
	ResetCombo();
	}
}
//head hit4
void APlayer_KYI::HurtAnim3() {
	if(!isDead)
	PlayAnimMontage(head4);
	OnHitDamage();
	ResetCombo();
}
//fall down 
void APlayer_KYI::HurtAnim4() {
	if (!isDead) {
	PlayAnimMontage(falldown);
	OnHitDamage();
	ResetCombo();
	}
}