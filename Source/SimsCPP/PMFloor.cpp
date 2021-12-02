// Fill out your copyright notice in the Description page of Project Settings.


#include "PMFloor.h"

// Sets default values
APMFloor::APMFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	GeneratedFloor = CreateDefaultSubobject<UProceduralMeshComponent>("GeneratedFloor");
	SetRootComponent(GeneratedFloor);
	GeneratedFloor->bUseAsyncCooking = true;
}

// Called when the game starts or when spawned
void APMFloor::BeginPlay()
{
	Super::BeginPlay();
	GenerateFloor();

	
}

// Called every frame
void APMFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//my functions
void APMFloor::SetParameters(TArray<FVector> floorVerticies, TArray<int32> floorTriangles) {
	Vertices = floorVerticies;
	Triangles = floorTriangles;
}


void APMFloor::GenerateFloor() {
	/*if (Vertices[0] != ) {
		UE_LOG(error, Warning, TEXT("Verticies or Triangles were NULL when trying to GenerateWall()"));
	}*/

	UE_LOG(LogTemp, Warning, TEXT("GenerateFloor()"));


	TArray<FLinearColor> VertexColors;
	//VertexColors.Add(FLinearColor(0.f, 0.f, 1.f));
	//VertexColors.Add(FLinearColor(1.f, 0.f, 0.f));
	//VertexColors.Add(FLinearColor(1.f, 0.f, 0.f));
	//VertexColors.Add(FLinearColor(0.f, 1.f, 0.f));
	//VertexColors.Add(FLinearColor(0.5f, 1.f, 0.5f));
	//VertexColors.Add(FLinearColor(0.f, 1.f, 0.f));
	//VertexColors.Add(FLinearColor(1.f, 1.f, 0.f));
	//VertexColors.Add(FLinearColor(0.f, 1.f, 1.f));
	///*

	//VertexColors.Add(FLinearColor(0.f, 0.f, 1.f));
	//VertexColors.Add(FLinearColor(1.f, 0.f, 0.f));
	//VertexColors.Add(FLinearColor(1.f, 0.f, 0.f));
	//VertexColors.Add(FLinearColor(0.f, 1.f, 0.f));
	//VertexColors.Add(FLinearColor(0.5f, 1.f, 0.5f));
	//VertexColors.Add(FLinearColor(0.f, 1.f, 0.f));
	//VertexColors.Add(FLinearColor(1.f, 1.f, 0.f));
	//VertexColors.Add(FLinearColor(0.f, 1.f, 1.f));*/

	GeneratedFloor->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), VertexColors, TArray<FProcMeshTangent>(), true);
}

void APMFloor::DeleteFloor()
{
	GeneratedFloor->DestroyComponent();
}

