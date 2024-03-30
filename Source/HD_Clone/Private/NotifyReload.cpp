// Fill out your copyright notice in the Description page of Project Settings.


#include "NotifyReload.h"
#include "TPSCharacter.h"
#include "TPSWeapon.h"
void UNotifyReload::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	ATPSCharacter* myCharacter = Cast<ATPSCharacter>(MeshComp->GetOwner());

	if (myCharacter == nullptr)
		return;
	myCharacter->Reload();
}
