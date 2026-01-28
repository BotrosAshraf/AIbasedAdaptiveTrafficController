// Fill out your copyright notice in the Description page of Project Settings.


#include "BP_IntersectionManager.h"

// Sets default values
ABP_IntersectionManager::ABP_IntersectionManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABP_IntersectionManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABP_IntersectionManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

