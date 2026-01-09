// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/AbilityTask_PlayMontageWaitEvent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "CombatFramework.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"

/**
 * 播放Montage并在过程中响应Event
 */
UAbilityTask_PlayMontageWaitEvent::UAbilityTask_PlayMontageWaitEvent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Rate = 1.f;
	bStopWhenAbilityEnds = true;
}

UAbilityTask_PlayMontageWaitEvent* UAbilityTask_PlayMontageWaitEvent::CreatePlayMontageWaitEventProxy(
	UGameplayAbility* OwningAbility,
	FName TaskInstanceName,
	FGameplayTagContainer TagFilter,
	bool bStopWhenAbilityEnds,
	UAnimMontage* MontageToPlay,
	float Rate,
	FName StartSection,
	float AnimRootMotionTranslationScale,
	float StartTimeSeconds)
{
	UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Rate(Rate);

	UAbilityTask_PlayMontageWaitEvent* MyObj = NewAbilityTask<UAbilityTask_PlayMontageWaitEvent>(OwningAbility, TaskInstanceName);
	MyObj->MontageToPlay = MontageToPlay;
	MyObj->TagFilter = TagFilter;
	MyObj->Rate = Rate;
	MyObj->StartSection = StartSection;
	MyObj->AnimRootMotionTranslationScale = AnimRootMotionTranslationScale;
	MyObj->StartTimeSeconds = StartTimeSeconds;
	MyObj->bStopWhenAbilityEnds = bStopWhenAbilityEnds;

	return MyObj;
}

void UAbilityTask_PlayMontageWaitEvent::Activate()
{
	if (Ability == nullptr)
	{
		return;
	}

	bool bPlayedMontage = false;

	if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get())
	{
		const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
		UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
		if (AnimInstance != nullptr && Rate > 0.f && StartTimeSeconds >= 0.f)
		{
			float During = ASC->PlayMontage(Ability, Ability->GetCurrentActivationInfo(), MontageToPlay, Rate, StartSection, StartTimeSeconds);
			if (During > 0.f)
			{
				float TimeOut = FMath::Clamp((During - StartTimeSeconds), 0.f, During) / Rate;
				if (ShouldBroadcastAbilityTaskDelegates() == false)
				{
					return;
				}
				EventHandle = ASC->AddGameplayEventTagContainerDelegate(TagFilter, FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnGameplayEvent));

				CancelledHandle = Ability->OnGameplayAbilityCancelled.AddUObject(this, &UAbilityTask_PlayMontageWaitEvent::OnAbilityCancelled);

				BlendingInDelegate.BindUObject(this, &UAbilityTask_PlayMontageWaitEvent::OnMontageBlendedInEnded);
				AnimInstance->Montage_SetBlendedInDelegate(BlendingInDelegate, MontageToPlay);

				BlendingOutDelegate.BindUObject(this, &UAbilityTask_PlayMontageWaitEvent::OnMontageBlendingOutStarted);
				AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, MontageToPlay);

				MontageEndedDelegate.BindUObject(this, &UAbilityTask_PlayMontageWaitEvent::OnMontageEnded);
				AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, MontageToPlay);

				GetWorld()->GetTimerManager().SetTimer(TimeOutHandle, this, &ThisClass::OnMontageTimeOut, TimeOut, false);

				ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
				if (Character && (Character->GetLocalRole() == ROLE_Authority ||
					(Character->GetLocalRole() == ROLE_AutonomousProxy && Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted)))
				{
					Character->SetAnimRootMotionTranslationScale(AnimRootMotionTranslationScale);
				}

				//UE_LOG(LogCombatFramework, Warning, TEXT("UAbilityTask_PlayMontageAndWait task activate at %d'%d''"), FDateTime::Now().GetSecond(), FDateTime::Now().GetMillisecond());
				bPlayedMontage = true;
			}
		}
		else
		{
			UE_LOG(LogCombatFramework, Warning, TEXT("UAbilityTask_PlayMontageWaitEvent call to PlayMontage failed!"));
		}
	}
	else
	{
		UE_LOG(LogCombatFramework, Warning, TEXT("UAbilityTask_PlayMontageWaitEvent called on invalid AbilitySystemComponent"));
	}

	if (!bPlayedMontage)
	{
		UE_LOG(LogCombatFramework, Warning, TEXT("UAbilityTask_PlayMontageAndWait called in Ability %s failed to play montage %s; Task Instance Name %s."), *Ability->GetName(), *GetNameSafe(MontageToPlay), *InstanceName.ToString());
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnCancelled.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}

	SetWaitingOnAvatar();
}

void UAbilityTask_PlayMontageWaitEvent::ExternalCancel()
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnCancelled.Broadcast(FGameplayTag(), FGameplayEventData());
	}
	Super::ExternalCancel();
}

void UAbilityTask_PlayMontageWaitEvent::OnDestroy(bool AbilityEnded)
{
	// This delegate, however, should be cleared as it is a multicast
	if (Ability)
	{
		Ability->OnGameplayAbilityCancelled.Remove(CancelledHandle);
		if (AbilityEnded && bStopWhenAbilityEnds)
		{
			StopPlayingMontage();
		}

		AbilitySystemComponent->RemoveGameplayEventTagContainerDelegate(TagFilter, EventHandle);
		GetWorld()->GetTimerManager().ClearTimer(TimeOutHandle);
	}

	Super::OnDestroy(AbilityEnded);
}

FString UAbilityTask_PlayMontageWaitEvent::GetDebugString() const
{
	UAnimMontage* PlayingMontage = nullptr;
	if (Ability)
	{
		const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
		UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();

		if (AnimInstance != nullptr)
		{
			PlayingMontage = AnimInstance->Montage_IsActive(MontageToPlay) ? ToRawPtr(MontageToPlay) : AnimInstance->GetCurrentActiveMontage();
		}
	}

	return FString::Printf(TEXT("PlayMontageWaitEvent. MontageToPlay: %s  (Currently Playing): %s"), *GetNameSafe(MontageToPlay), *GetNameSafe(PlayingMontage));
}

void UAbilityTask_PlayMontageWaitEvent::OnMontageBlendedInEnded(UAnimMontage* Montage)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnBlendIn.Broadcast(FGameplayTag(), FGameplayEventData());
	}
}

void UAbilityTask_PlayMontageWaitEvent::OnMontageBlendingOutStarted(UAnimMontage* Montage, bool bInterrupted)
{
	if (Ability && Ability->GetCurrentMontage() == MontageToPlay)
	{
		if (Montage == MontageToPlay)
		{
			if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get())
			{
				ASC->ClearAnimatingAbility(Ability);
			}

			// Reset AnimRootMotionTranslationScale
			ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
			if (Character && (Character->GetLocalRole() == ROLE_Authority ||
				(Character->GetLocalRole() == ROLE_AutonomousProxy && Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted)))
			{
				Character->SetAnimRootMotionTranslationScale(1.f);
			}
		}
	}

	if (bInterrupted)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnInterrupted.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}
	else
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnBlendOut.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}
}

void UAbilityTask_PlayMontageWaitEvent::OnAbilityCancelled()
{
	if (StopPlayingMontage())
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnInterrupted.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}
}

void UAbilityTask_PlayMontageWaitEvent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnCompleted.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}
}

void UAbilityTask_PlayMontageWaitEvent::OnGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		FGameplayEventData Data = *Payload;
		Data.EventTag = EventTag;

		OnReceiveEvent.Broadcast(EventTag, Data);
	}
}

void UAbilityTask_PlayMontageWaitEvent::OnMontageTimeOut()
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnTimeOut.Broadcast(FGameplayTag(), FGameplayEventData());
	}
}

bool UAbilityTask_PlayMontageWaitEvent::StopPlayingMontage()
{
	if (Ability == nullptr)
	{
		return false;
	}

	const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
	if (ActorInfo == nullptr)
	{
		return false;
	}

	UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		return false;
	}

	// The ability would have been interrupted, in which case we should automatically stop the montage
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC && Ability)
	{
		if (ASC->GetAnimatingAbility() == Ability && ASC->GetCurrentMontage() == MontageToPlay)
		{
			FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(MontageToPlay);
			if (MontageInstance)
			{
				MontageInstance->OnMontageBlendedInEnded.Unbind();
				MontageInstance->OnMontageBlendingOutStarted.Unbind();
				MontageInstance->OnMontageEnded.Unbind();
			}

			ASC->CurrentMontageStop();
			return true;
		}
	}

	return false;
}
