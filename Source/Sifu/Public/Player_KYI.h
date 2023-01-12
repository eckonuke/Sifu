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

	//hj �� ���� �߰�
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Spring Arm
	UPROPERTY(VisibleAnywhere, Category = Camera)
		class USpringArmComponent* springArmComp;
	//Camera Component
	UPROPERTY(VisibleAnywhere, Category = Camera)
		class UCameraComponent* camComp;

	////������ �̵� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool movementEnabled = true;
	//�̵��ӵ�
	UPROPERTY(EditAnywhere, Category = PlayerSetting)
		float walkSpeed = 600;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isBlocking = false;
	//�̵�����
	FVector direction;
	//�ǰ� ��Ÿ��
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

	//�¿� �̵��Է� �̺�Ʈ ó���Լ�
	void Turn(float value);
	void LookUp(float value);
	void InputHorizontal(float value);
	void InputVertical(float value);
	void InputJump();
	void InputRun(bool run);
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
	//���ݹ��� �ִϸ��̼� �÷��� �Լ�
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

	//HJ �� ����
	UFUNCTION(BlueprintCallable)
		void OnHitDamage();
	UFUNCTION(BlueprintCallable)
		void PlayerDamage();
	UFUNCTION(BlueprintCallable)
		void PlayerDie();

	//Ÿ���� �� ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AHJ_Enemy* targetEnemy;
	//Ÿ�� ���� Ȯ��
	UPROPERTY(EditAnywhere)
		bool targetLock = false;

	//ü��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		int32 hp = 500;
	//���� Ȯ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isDead = false;
	//�Ʒ��� ������� �ӵ�
	UPROPERTY(EditAnywhere)
		float PlayerdieSpeed = 50.0f;

	//���� �޺�
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
