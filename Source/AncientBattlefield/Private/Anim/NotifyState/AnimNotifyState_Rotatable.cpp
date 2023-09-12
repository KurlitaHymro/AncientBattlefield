// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/NotifyState/AnimNotifyState_Rotatable.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UAnimNotifyState_Rotatable::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	Owner = Cast<ACharacter>(MeshComp->GetOwner());
}

void UAnimNotifyState_Rotatable::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (Owner)
	{
		FVector InputVector = Owner->GetCharacterMovement()->GetLastInputVector();
		if (InputVector != FVector(0.f, 0.f, 0.f))
		{
			FRotator NewRotator = UKismetMathLibrary::MakeRotFromX(InputVector);
			FRotator CurRotator = Owner->GetActorRotation();
			if (NewRotator != CurRotator) {
				FRotator DeltaRotator = UKismetMathLibrary::RInterpTo(CurRotator, NewRotator, FrameDeltaTime, RotateSpeed);
				Owner->SetActorRotation(DeltaRotator);
			}
		}
	}
}

void UAnimNotifyState_Rotatable::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
}
