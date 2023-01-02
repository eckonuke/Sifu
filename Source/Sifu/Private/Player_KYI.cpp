// Fill out your copyright notice in the Description page of Project Settings.


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

	//springArm 컴포넌트 붙이기
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0, 70, 90));
	springArmComp->TargetArmLength = 400;
	springArmComp->bUsePawnControlRotation = true;

	//Camera 컴포넌트 붙이기
	camComp = CreateDefaultSubobject<UCameraComponent>(TEXT("camComp"));
	camComp->SetupAttachment(springArmComp);
	camComp->bUsePawnControlRotation = false;


	bUseControllerRotationYaw = true;

}

// Called when the game starts or when spawned
void APlayer_KYI::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlayer_KYI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	direction = FTransform(GetControlRotation()).TransformVector(direction);
	/*FVector p0 = GetActorLocation();
	FVector vt = direction * walkSpeed * DeltaTime;
	FVector p = p0 + vt;
	SetActorLocation(p);*/
	AddMovementInput(direction);
	direction = FVector::ZeroVector;
	//상태를 죽음으로 전환 hj 수정
	if (hp <= 0)PlayerDie();

	direction = FTransform(GetControlRotation()).TransformVector(direction);
	/*FVector p0 = GetActorLocation();
	FVector vt = direction * walkSpeed * DeltaTime;
	FVector p = p0 + vt;
	SetActorLocation(p);*/
	AddMovementInput(direction);
	direction = FVector::ZeroVector;

	//상태를 죽음으로 전환 hj 수정
	if(hp<=0)PlayerDie();
}

// Called to bind functionality to input
void APlayer_KYI::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APlayer_KYI::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APlayer_KYI::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Move Right / Left"), this, &APlayer_KYI::InputHorizontal);
	PlayerInputComponent->BindAxis(TEXT("Move Forward / Backward"), this, &APlayer_KYI::InputVertical);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &APlayer_KYI::InputJump);
	PlayerInputComponent->BindAction(TEXT("Punch"), IE_Pressed, this, &APlayer_KYI::PlayerDamage);
	PlayerInputComponent->BindAction(TEXT("Kick"), IE_Pressed, this, &APlayer_KYI::PlayerDamage);
	DECLARE_DELEGATE_OneParam(FBlock, bool);
	PlayerInputComponent->BindAction<FBlock>(TEXT("Block"), IE_Pressed, this, &APlayer_KYI::PlayerBlock, true);
	PlayerInputComponent->BindAction<FBlock>(TEXT("Block"), IE_Released, this, &APlayer_KYI::PlayerBlock, false);
}

void APlayer_KYI::Turn(float value) {
	if (movementEnabled) {
		AddControllerYawInput(value);
	}
}
void APlayer_KYI::LookUp(float value) {
	if (movementEnabled) {
		AddControllerPitchInput(value);
	}
}
void APlayer_KYI::InputHorizontal(float value) {
	direction.Y = value;
}
void APlayer_KYI::InputVertical(float value) {
	direction.X = value;
}

//적 공격
void APlayer_KYI::InputJump() {
	Jump();
}

void APlayer_KYI::OnHitDamage()
{
	if(!isBlocking) {
		//체력 감소
		hp--;
		//만약 체력이 남아있다면 
		if (hp > 0){
			//상태를 피격으로 전환
			//PlayerDamage();
		}
		else{
			//상태를 죽음으로 전환
			PlayerDie();
			//캡슐 충돌체 비활성화
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			//GetComponentByClass(UCapsuleComponent::StaticClass())
		}
	}
}

//피격 상태
void APlayer_KYI::PlayerDamage(){
	movementEnabled = false;
	TArray<AActor*> enemys;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHJ_Enemy::StaticClass(), enemys);
	for (int i = 0; i < enemys.Num(); i++)
	{
		//거리 계산 (Player - enemy)
		FVector v = GetActorLocation() - enemys[i]->GetActorLocation();
		float dist = v.Length();
		if (dist < 200)
		{
			AHJ_Enemy* e = Cast<AHJ_Enemy>(enemys[i]);
			e->fsm->OnDamageProcess();
			movementEnabled = true;
		}
	}
}

void APlayer_KYI::PlayerDie()
{
	//계속 아래로 내려가고 싶다. p=p0+vt
	FVector p0 = GetActorLocation();
	FVector p = p0 + FVector::DownVector * PlayerdieSpeed * GetWorld()->DeltaTimeSeconds;
	SetActorLocation(p);

	//1. 만약 2미터 이상 내려왔다면
	if (p.Z < -200.0f)
	{
		//2. 제거시킨다
		Destroy();
	}
}

void APlayer_KYI::PlayerBlock(bool value) {
	isBlocking = value;
	//Animation 삽입
}

//HJ가 다중 AI 위해 만듬 추후에 쓸 수 있음 쓸 예정.
void APlayer_KYI::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	AHJ_Enemy* enemy = Cast<AHJ_Enemy>(OtherActor);
	if (enemy)
	{
		//enemy->fsm->OnDamageProcess();
	}
}