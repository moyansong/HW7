// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "ThirdPerson_CPP.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AttributeComponent.h"
#include "BrainComponent.h"

// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");

	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComponent"));
}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
	/*AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC && TargetActor)
	{
		UBlackboardComponent* BBComponent = AIC->GetBlackboardComponent();
		BBComponent->SetValueAsVector("MoveToLocation", TargetActor->GetActorLocation());
		BBComponent->SetValueAsObject("TargetActor", TargetActor);
	}*/
}

// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (bShouldAttackSeenActor)
	{
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &AAICharacter::OnPawnSeen);
	}

	AttributeComponent->OnHealthChanged.AddDynamic(this, &AAICharacter::OnHealthChange);
}

void AAICharacter::OnPawnSeen(APawn* Pawn)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		UBlackboardComponent* BBComponent = AIC->GetBlackboardComponent();
		BBComponent->SetValueAsObject("TargetActor", Pawn);
	}
}

void AAICharacter::OnHealthChange(AActor* InstigatocActor, UAttributeComponent* OwningCopmonent, float NewHealth, float Damage)
{
	if (NewHealth <= 0.f)
	{
		AAIController* MyController = Cast<AAIController>(GetController());
		if (MyController)
		{
			MyController->GetBrainComponent()->StopLogic("Killed");
		}

		GetMesh()->SetAllBodiesSimulatePhysics(true);
		GetMesh()->SetCollisionProfileName("Ragdoll");

		SetLifeSpan(8.f);
	}
}

float AAICharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Cast<AAICharacter>(DamageCauser))
	{
		return Damage;
	}
	else if (AttributeComponent)
	{
		AttributeComponent->ApplyHealthChange(DamageCauser, Damage);
		
		AAIController* AIC = Cast<AAIController>(GetController());
		//APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
		if (AIC && DamageCauser)
		{
			UBlackboardComponent* BBComponent = AIC->GetBlackboardComponent();
			BBComponent->SetValueAsVector("MoveToLocation", DamageCauser->GetActorLocation());
			BBComponent->SetValueAsObject("TargetActor", DamageCauser);
		}
	}
	return Damage;
}
