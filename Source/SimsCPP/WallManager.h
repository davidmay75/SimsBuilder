#pragma once

#include "ProceduralMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "KismetProceduralMeshLibrary.h"
#include "PMWall.h"
#include "PMFloor.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WallManager.generated.h"

UCLASS()
class SIMSCPP_API AWallManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWallManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APMWall* APMWallPreview;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	APMFloor* APMFloorPreview;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class APMWall> MyBP_PMWall;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* MyGeneratedBoxMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* WallPreview;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* PreviewWallMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* FloorPreview;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* PreviewFloorMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* DefaultWallMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* DefaultFloorMaterial;


	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//bool previewOnly;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float wallHeight = 300;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float wallWidth = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float floorHeight = 1;




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	/* The vertices of the mesh */
	TArray<FVector> Vertices;

	/* The triangles of the mesh */
	TArray<int32> Triangles;

	/* Creates a triangle that connects the given vertices */
	void AddTriangle(int32 V1, int32 V2, int32 V3);
	void AddVerticiesAndTriangles(FVector wallStartPoint, FVector wallEndPoint, bool preview, float halfWidth, float lengthX, float lengthY, float height);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void GenerateWall(float width, float height, FVector wallStartPoint, FVector wallEndPoint, bool preview);
	
	UFUNCTION(BlueprintCallable)
	void SetWallPreview(FTransform SpawnLocAndRotation);

	UFUNCTION(BlueprintCallable)
	void DestroyWallPreview();

	UFUNCTION(BlueprintCallable)
	void DestroyFloorPreview();

	UFUNCTION(BlueprintCallable)
	void GenerateFloor(float width, float height, FVector wallStartPoint, FVector wallEndPoint, bool preview);
	
	UFUNCTION(BlueprintCallable)
	void MyGenerateBoxMesh(FVector BoxRadius, TArray<FVector>& Vertices1, TArray<int32>& Triangles1, TArray<FVector>& Normals, TArray<FVector2D>& UVs, TArray<FProcMeshTangent>& Tangents);
	//FVector DragStart, FVector DragEnd,

	/*UFUNCTION(BlueprintCallable)
	void GeneratePreview(float width, float height, FVector wallStartPoint, FVector wallEndPoint, bool preview, bool objectType);*/
};
