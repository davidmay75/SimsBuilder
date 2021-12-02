// Fill out your copyright notice in the Description page of Project Settings.


#include "PMWall.h"

// Sets default values
APMWall::APMWall()
{
	GeneratedWall = CreateDefaultSubobject<UProceduralMeshComponent>("GeneratedWall");
	SetRootComponent(GeneratedWall);
	GeneratedWall->bUseAsyncCooking = true;
}

// Called when the game starts or when spawned
void APMWall::BeginPlay()
{
	Super::BeginPlay();
	GenerateWall();	
}

// Called every frame
void APMWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//my functions
void APMWall::SetParameters(TArray<FVector> wallVerticies, TArray<int32> wallTriangles) {
	Vertices = wallVerticies;
	Triangles = wallTriangles;
}


void APMWall::GenerateWall() {
	/*if (Vertices[0] != ) {
		UE_LOG(error, Warning, TEXT("Verticies or Triangles were NULL when trying to GenerateWall()"));
	}*/

	TArray<FLinearColor> VertexColors;
	VertexColors.Add(FLinearColor(0.f, 0.f, 1.f));
	VertexColors.Add(FLinearColor(1.f, 0.f, 0.f));
	VertexColors.Add(FLinearColor(1.f, 0.f, 0.f));
	VertexColors.Add(FLinearColor(0.f, 1.f, 0.f));
	VertexColors.Add(FLinearColor(0.5f, 1.f, 0.5f));
	VertexColors.Add(FLinearColor(0.f, 1.f, 0.f));
	VertexColors.Add(FLinearColor(1.f, 1.f, 0.f));
	VertexColors.Add(FLinearColor(0.f, 1.f, 1.f));
	///*

	//VertexColors.Add(FLinearColor(0.f, 0.f, 1.f));
	//VertexColors.Add(FLinearColor(1.f, 0.f, 0.f));
	//VertexColors.Add(FLinearColor(1.f, 0.f, 0.f));
	//VertexColors.Add(FLinearColor(0.f, 1.f, 0.f));
	//VertexColors.Add(FLinearColor(0.5f, 1.f, 0.5f));
	//VertexColors.Add(FLinearColor(0.f, 1.f, 0.f));
	//VertexColors.Add(FLinearColor(1.f, 1.f, 0.f));
	//VertexColors.Add(FLinearColor(0.f, 1.f, 1.f));*/

	GeneratedWall->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), VertexColors, TArray<FProcMeshTangent>(), true);
}

void APMWall::DeleteWall()
{
	GeneratedWall->DestroyComponent();
}


