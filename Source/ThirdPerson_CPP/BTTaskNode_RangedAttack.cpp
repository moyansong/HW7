// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_RangedAttack.h"
#include "AIController.h"
#include "ThirdPerson_CPP.h"
#include "Actor.h"
#include "Containers/Array.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Projectile.h"



EBTNodeResult::Type UBTTaskNode_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (ensure(MyController))
	{
		ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());
		if (MyPawn == nullptr)
		{
			return EBTNodeResult::Failed;
		}
		FVector MuzzleLocation = MyPawn->GetMesh()->GetSocketLocation("Muzzle_01");

		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		if (TargetActor == nullptr)
		{
			return EBTNodeResult::Failed;
		}
		FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
		FRotator MuzzleRotation = Direction.Rotation();

		if (bCanCloseAttack)
		{
			TArray<AActor*> IgnoreActor;
			IgnoreActor.Add(MyPawn);
			UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, TargetActor->GetActorLocation(), 10.f, nullptr, IgnoreActor, MyPawn);
			MyPawn->PlayAnimMontage(FireMontage, 1.f); 
		}

		if (ProjectileClass)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			Projectile->SetOwner(MyPawn);
			MyPawn->PlayAnimMontage(FireMontage, 1.f);
			return Projectile ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
		}
	}
	return EBTNodeResult::Failed;
}