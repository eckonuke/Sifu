// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ_Enemy.h"
#include "EnemyFSM.h"

// Sets default values 
AHJ_Enemy::AHJ_Enemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//1.스켈레탈메시 데이터 로드
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'"));;
	//1-1.데이터 로드 성공하면
	if (tempMesh.Succeeded())
	{
		//1-2. 데이터 할당
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		//1-2.메시 위치 및 회전 설정
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));
	}
	//EnemyFSM 컴포넌트 추가
	fsm = CreateDefaultSubobject<UEnemyFSM>(TEXT("FSM"));



}

// Called when the game starts or when spawned
void AHJ_Enemy::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AHJ_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHJ_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
