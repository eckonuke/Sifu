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

// Sets default values
APlayer_KYI::APlayer_KYI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'"));
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


	//���¸� �������� ��ȯ hj ����
	if(hp<=0)PlayerDie();

}

// Called to bind functionality to input
void APlayer_KYI::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APlayer_KYI::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APlayer_KYI::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &APlayer_KYI::InputHorizontal);
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &APlayer_KYI::InputVertical);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &APlayer_KYI::InputJump);
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


//�� ����
void APlayer_KYI::InputJump() {
	//Jump();
	TArray<AActor*> enemys;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHJ_Enemy::StaticClass(), enemys);
	for (int i = 0; i < enemys.Num(); i++)
	{
		//�Ÿ� ��� (Player - enemy)
		FVector v = GetActorLocation() - enemys[i]->GetActorLocation();
		float dist = v.Length();
		if (dist < 300)
		{
			AHJ_Enemy* e = Cast<AHJ_Enemy>(enemys[i]);
			e->fsm->OnDamageProcess();
		}
	}
}

void APlayer_KYI::OnHitDamage()
{
    //ü�� ����
	hp--;
	//���� ü���� �����ִٸ� 
	if (hp > 0)
	{
		//���¸� �ǰ����� ��ȯ
		PlayerDamage();
	}
	else
	{
		//���¸� �������� ��ȯ
		PlayerDie();

		
		//ĸ�� �浹ü ��Ȱ��ȭ
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//GetComponentByClass(UCapsuleComponent::StaticClass())
		
		
	}
}

//�ǰ� ����
void APlayer_KYI::PlayerDamage()
{
	
}


void APlayer_KYI::PlayerDie()
{
	//��� �Ʒ��� �������� �ʹ�. p=p0+vt
	FVector p0 =GetActorLocation();
	FVector p = p0 + FVector::DownVector * PlayerdieSpeed * GetWorld()->DeltaTimeSeconds;
	SetActorLocation(p);

	//1. ���� 2���� �̻� �����Դٸ�
	if (p.Z < -200.0f)
	{
		//2. ���Ž�Ų��
		Destroy();
	}
}





//HJ�� ���� AI ���� ���� ���Ŀ� �� �� ���� �� ����.
 
void APlayer_KYI::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	AHJ_Enemy* enemy = Cast<AHJ_Enemy>(OtherActor);
	if (enemy)
	{
		enemy->fsm->OnDamageProcess();
	}
}


 