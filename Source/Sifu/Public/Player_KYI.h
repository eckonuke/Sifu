// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Player_KYI.generated.h"

UCLASS()
class SIFU_API APlayer_KYI : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayer_KYI();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//hj 가 한출 추가
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, Category=Camera)
	class USpringArmComponent* springArmComp;
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* camComp;

	void Turn(float value);
	void LookUp(float value);

	//이동속도
	UPROPERTY(EditAnywhere, Category = PlayerSetting)
	float walkSpeed= 600;
	//이동방향
	FVector direction;

	//좌우 이동입력 이벤트 처리함수
	void InputHorizontal(float value);
	void InputVertical(float value);
	void InputJump();

	
	//HJ 가 수정
	void OnHitDamage();
	
	void PlayerDamage();

	void  PlayerDie();

	//체력
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		int32 hp = 10;

	//아래로 사라지는 속도
	UPROPERTY(EditAnywhere)
		float PlayerdieSpeed = 50.0f;
		

};
