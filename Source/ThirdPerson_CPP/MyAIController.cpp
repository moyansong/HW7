// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"
#include "Containers/Array.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void AMyAIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BehaviorTree);//ָ����Ϊ��

	//�������
	//APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	//if (PlayerPawn)
	//{
	//	                                        //��Blackboard���ж�Ӧ��Keys
	//	GetBlackboardComponent()->SetValueAsVector("MoveToLocation", PlayerPawn->GetActorLocation());
	//	GetBlackboardComponent()->SetValueAsObject("TargetActor", PlayerPawn);
	//}

	//����NPC
	TArray<AActor*> TargetActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), TargetActorClass, TargetActors);
	if (TargetActors.Num() > 0 && TargetActors[0])
	{
		GetBlackboardComponent()->SetValueAsVector("MoveToLocation", TargetActors[0]->GetActorLocation());
		GetBlackboardComponent()->SetValueAsObject("TargetActor", TargetActors[0]);
	}
}