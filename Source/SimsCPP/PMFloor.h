// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ProceduralMeshComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PMFloor.generated.h"

UCLASS()
class SIMSCPP_API APMFloor : public AActor
{
	GENERATED_BODY()

		

	
public:	
	// Sets default values for this actor's properties
	APMFloor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* GeneratedFloor;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* The vertices of the mesh */
	TArray<FVector> Vertices;
	/* The triangles of the mesh */
	TArray<int32> Triangles;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void GenerateFloor();

	UFUNCTION(BlueprintCallable)
		void DeleteFloor();

	UFUNCTION(BlueprintCallable)
		void SetParameters(TArray<FVector> floorVerticies, TArray<int32> floorTriangles);

};
