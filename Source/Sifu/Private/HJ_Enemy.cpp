// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ_Enemy.h"
#include "EnemyFSM.h"

// Sets default values 
AHJ_Enemy::AHJ_Enemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//1.���̷�Ż�޽� ������ �ε�
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'"));;
	//1-1.������ �ε� �����ϸ�
	if (tempMesh.Succeeded())
	{
		//1-2. ������ �Ҵ�
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		//1-2.�޽� ��ġ �� ȸ�� ����
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));
	}
	//EnemyFSM ������Ʈ �߰�
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
