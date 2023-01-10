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

	//Spring Arm
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* springArmComp;
	//Camera Component
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* camComp;

	////공격중 이동 중지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool movementEnabled = true;
	//이동속도
	UPROPERTY(EditAnywhere, Category = PlayerSetting)
	float walkSpeed = 600;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isBlocking = false;
	//이동방향
	FVector direction;
	//피격 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* stomach;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* head2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* head3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* head4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* falldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* death;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* block;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* punch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* jab;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* uppercut;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* kick;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* highKick;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* lowKick;

	//좌우 이동입력 이벤트 처리함수
	void Turn(float value);
	void LookUp(float value);
	void InputHorizontal(float value);
	void InputVertical(float value);
	void InputJump();
	void AttackPunch();
	void AttackKick();
	UFUNCTION(BlueprintCallable)
	void saveAttackCombo();
	UFUNCTION(BlueprintCallable)
	void punchCombo();
	UFUNCTION(BlueprintCallable)
	void kickCombo();
	UFUNCTION(BlueprintCallable)
	void ResetCombo();
	UFUNCTION(BlueprintCallable)
	void PlayerBlock(bool value);
	UFUNCTION(BlueprintCallable)
	void setTarget();
	//공격받은 애니메이션 플레이 함수
	UFUNCTION(BlueprintCallable)
	void HurtAnim0();
	UFUNCTION(BlueprintCallable)
	void HurtAnim1();
	UFUNCTION(BlueprintCallable)
	void HurtAnim2();
	UFUNCTION(BlueprintCallable)
	void HurtAnim3();
	UFUNCTION(BlueprintCallable)
	void HurtAnim4();
	
	//HJ 가 수정
	UFUNCTION(BlueprintCallable)
	void OnHitDamage();
	UFUNCTION(BlueprintCallable)
	void PlayerDamage();
	UFUNCTION(BlueprintCallable)
	void PlayerDie();

	//타게팅 된 적
	UPROPERTY(EditAnywhere)
	class AHJ_Enemy* targetEnemy;

	//체력
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 hp = 5;
	//죽음 확인
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isDead = false;
	//아래로 사라지는 속도
	UPROPERTY(EditAnywhere)
	float PlayerdieSpeed = 50.0f;

	//공격 콤보
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int punchCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsAttacking = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool saveAttack = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int kickCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool kickorPunch = true;
	
};
