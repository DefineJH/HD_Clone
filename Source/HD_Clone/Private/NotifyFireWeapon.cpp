// Fill out your copyright notice in the Description page of Project Settings.


#include "NotifyFireWeapon.h"
#include "TPSCharacter.h"
void UNotifyFireWeapon::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	ATPSCharacter* myCharacter = Cast<ATPSCharacter>(MeshComp->GetOwner());

	if (myCharacter == nullptr)
		return;
	myCharacter->FireWeapon();
}
