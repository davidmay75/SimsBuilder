// Fill out your copyright notice in the Description page of Project Settings.


#include "WallManager.h"

// Sets default values
AWallManager::AWallManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//GeneratedWall = CreateDefaultSubobject<UProceduralMeshComponent>("GeneratedWall");
	//SetRootComponent(GeneratedWall);
	//GeneratedWall->bUseAsyncCooking = true;

	WallPreview = CreateDefaultSubobject<UProceduralMeshComponent>("WallPreview");
	//SetRootComponent(WallPreview);
	WallPreview->bUseAsyncCooking = true;

	FloorPreview = CreateDefaultSubobject<UProceduralMeshComponent>("FloorPreview");
	//SetRootComponent(FloorPreview);
	FloorPreview->bUseAsyncCooking = true;


	//MyGeneratedBoxMesh = CreateDefaultSubobject<UProceduralMeshComponent>("MyGeneratedBoxMesh");
	////SetRootComponent(MyGeneratedBoxMesh);
	//MyGeneratedBoxMesh->bUseAsyncCooking = true;


	//APMWallPreview = APMWall();
}

// Called when the game starts or when spawned
void AWallManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWallManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWallManager::AddTriangle(int32 V1, int32 V2, int32 V3)
{
	Triangles.Add(V1);
	Triangles.Add(V2);
	Triangles.Add(V3);
}


void AWallManager::GenerateWall(float width, float height, FVector wallStartPoint, FVector wallEndPoint, bool preview) {

	////
	// I could actually always make the walls draw positive - just swap start and end once I know the direction
	//Determine which axis to draw wall on
	float lenY = wallEndPoint.Y - wallStartPoint.Y;
	float lenX = wallEndPoint.X - wallStartPoint.X;

	//if (lenY == 0 && lenX == 0) {
	//	//prevent drawing a post
	//	return;
	//} 

	if (!lenX * lenY == 0) {

		if (abs(lenX) < abs(lenY)) {
			//UE_LOG(LogTemp, Warning, TEXT("yDiff bigger - X: %f, Y: %f - drawingNegative: %d"), xDiffecernce, yDiffecernce, drawingNegative);
			lenX = 0;
		}
		//Dragged more in X axis
		else if (abs(lenX) > abs(lenY)) {
			//UE_LOG(LogTemp, Warning, TEXT("xDiff bigger - X: %f, Y: %f - drawingNegative: %d"), xDiffecernce, yDiffecernce, drawingNegative);
			lenY = 0;
		}
		//Dragged like a square
		else if (abs(lenX) == abs(lenY)) {
			lenY = 0;
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("ELSE - X: %f, Y: %f"), lenX, lenY);
		}
	}

	//SET VARIABLES
	//If I don't reverse the width, it renders them inside out cause they're pushed to opposite side
	bool drawingNegative = lenY < 0 || lenX < 0;
	float halfWidth = (drawingNegative) ? -1 * (width / 2.0f) : width / 2.0f;//set distance to negative so the math below works
	FTransform SpawnLocAndRotation;
	SpawnLocAndRotation.SetLocation(wallStartPoint);

	//Build Geometry
	AddVerticiesAndTriangles(wallStartPoint, wallEndPoint, preview, halfWidth, lenX, lenY, height);

	//APMWall class only
	//SpawnDeferred, add the triangles, then  let the begin play run
	//APMWall* SpawnedActorRef = GetWorld()->SpawnActorDeferred<APMWall>(APMWall::StaticClass(), SpawnLocAndRotation);
	//SpawnedActorRef->SetParameters(Vertices, Triangles);
	//SpawnedActorRef->SetActorEnableCollision(true);
	//SpawnedActorRef->FinishSpawning(SpawnLocAndRotation);

	//BP subclass of APMWall
	APMWall* SpawnedBP_PMWall = GetWorld()->SpawnActorDeferred<APMWall>(MyBP_PMWall, SpawnLocAndRotation);
	SpawnedBP_PMWall->SetParameters(Vertices, Triangles);
	SpawnedBP_PMWall->SetActorEnableCollision(true);
	SpawnedBP_PMWall->FinishSpawning(SpawnLocAndRotation);
	

	if (preview) {
		/*APMWallPreview = SpawnedActorRef;
		APMWallPreview->GeneratedWall->SetMaterial(0, PreviewWallMaterial);*/

		APMWallPreview = SpawnedBP_PMWall;
		APMWallPreview->GeneratedWall->SetMaterial(0, PreviewWallMaterial);

		UE_LOG(LogTemp, Warning, TEXT("Preview wall has been set"));

	}
	else {

		SpawnedBP_PMWall->GeneratedWall->SetMaterial(0, DefaultWallMaterial);



		//SpawnedActorRef->GeneratedWall->SetMaterial(0, DefaultWallMaterial);
		//UProceduralMeshComponent* outMesh;
		//UProceduralMeshComponent* outMesh2;

		//EProcMeshSliceCapOption capOption = EProcMeshSliceCapOption::UseLastSectionForCap;
		
		// normals
		//FVector(0,-1,0) is cutting vertically and leaving the -y axis side
		//UKismetProceduralMeshLibrary::SliceProceduralMesh(SpawnedActorRef->GeneratedWall,FVector(0,100-halfWidth,100),FVector(0,-1,0),true, outMesh, capOption, DefaultFloorMaterial);//this would need -halfwidth in y
		//UKismetProceduralMeshLibrary::SliceProceduralMesh(outMesh, FVector(0, 200+halfWidth, 100), FVector(0, 1, 0), false, outMesh2, capOption, DefaultFloorMaterial);//this would need +halfwidth in y

	}	

	//Reset to use on next wall
	Triangles.Reset();
	Vertices.Reset();
}


void AWallManager::SetWallPreview(FTransform SpawnLocAndRotation) {//not used
	TArray<FLinearColor> VertexColors;// IDK - used to make mesh - could pass from heres
	//UE_LOG(LogTemp, Warning, TEXT("ELSE - X: %f, Y: %f - drawingNegative: %d"), xDiffecernce, yDiffecernce, drawingNegative);

	WallPreview->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), VertexColors, TArray<FProcMeshTangent>(), true);

	//WallPreview->SetRelativeTransform(SpawnLocAndRotation);
}

void AWallManager::AddVerticiesAndTriangles(FVector wallStartPoint, FVector wallEndPoint, bool preview, float halfWidth, float legnthX, float lengthY, float height) {
	bool drawShadow = true;
	FVector drawPos(0.0f);
	FColor drawColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f).ToFColor(true);
	float drawDuration = 123456789.0f;





	//wallStartPoint

	//DrawDebugString(GetWorld(), wallStartPoint + FVector()    , *FString::Printf(TEXT("%s"), TEXT(" ")), NULL, drawColor, drawDuration, drawShadow);


	Vertices.Add(FVector(0 - halfWidth, 0 - halfWidth, floorHeight)); //lower left - 0
	//DrawDebugString(GetWorld(), wallStartPoint + FVector(0 - halfWidth, 0 - halfWidth, 0.f), *FString::Printf(TEXT("%s"), TEXT("lower left - 0")), NULL, drawColor, drawDuration, drawShadow);

	Vertices.Add(FVector(0 - halfWidth, 0 - halfWidth, height)); //upper left - 1 - Y to align corner
	//DrawDebugString(GetWorld(), wallStartPoint + FVector(0 - halfWidth, 0 - halfWidth, height), *FString::Printf(TEXT("%s"), TEXT("upper left - 1 ")), NULL, drawColor, drawDuration, drawShadow);

	Vertices.Add(FVector(0 - halfWidth, lengthY + halfWidth, floorHeight)); //lower right - 2 
	//DrawDebugString(GetWorld(), wallStartPoint + FVector(0 - halfWidth, lengthY + halfWidth, 0), *FString::Printf(TEXT("%s"), TEXT("lower right - 2 ")), NULL, drawColor, drawDuration, drawShadow);

	Vertices.Add(FVector(0 - halfWidth, lengthY + halfWidth, height)); //upper right - 3
	//DrawDebugString(GetWorld(), wallStartPoint + FVector(0 - halfWidth, lengthY + halfWidth, height), *FString::Printf(TEXT("%s"), TEXT("upper right - 3")), NULL, drawColor, drawDuration, drawShadow);

	Vertices.Add(FVector(legnthX + halfWidth, 0 - halfWidth, floorHeight)); //lower front left - 4
	//DrawDebugString(GetWorld(), wallStartPoint + FVector(legnthX + halfWidth, 0 - halfWidth, 0), *FString::Printf(TEXT("%s"), TEXT("lower front left - 4 ")), NULL, drawColor, drawDuration, drawShadow);

	Vertices.Add(FVector(legnthX + halfWidth, 0 - halfWidth, height)); //upper front left - 5
	//DrawDebugString(GetWorld(), wallStartPoint + FVector(legnthX + halfWidth, 0 - halfWidth, height), *FString::Printf(TEXT("%s"), TEXT("upper front left - 5 ")), NULL, drawColor, drawDuration, drawShadow);

	Vertices.Add(FVector(legnthX + halfWidth, lengthY + halfWidth, height)); //upper front right - 6
	//DrawDebugString(GetWorld(), wallStartPoint + FVector(legnthX + halfWidth, lengthY + halfWidth, height), *FString::Printf(TEXT("%s"), TEXT("upper front right - 6 ")), NULL, drawColor, drawDuration, drawShadow);

	Vertices.Add(FVector(legnthX + halfWidth, lengthY + halfWidth, floorHeight)); //lower front right - 7
	//DrawDebugString(GetWorld(), wallStartPoint + FVector(legnthX + halfWidth, lengthY + halfWidth, 0), *FString::Printf(TEXT("%s"), TEXT("lower front right - 7 ")), NULL, drawColor, drawDuration, drawShadow);

	//Back face of cube
	AddTriangle(0, 2, 3);
	AddTriangle(3, 1, 0);

	//Left face of cube
	AddTriangle(0, 1, 4);
	AddTriangle(4, 1, 5);

	//Front face of cube
	AddTriangle(4, 5, 7);
	AddTriangle(7, 5, 6);

	//Right face of cube
	AddTriangle(7, 6, 3);
	AddTriangle(3, 2, 7);

	//Top face
	AddTriangle(1, 3, 5);
	AddTriangle(6, 5, 3);

	//bottom face
	AddTriangle(2, 0, 4);
	AddTriangle(4, 7, 2);


}

void AWallManager::DestroyWallPreview() {
	//APMWallPreview->GeneratedWall->ClearAllMeshSections(); //doesn't do what I thought
	APMWallPreview->Destroy();
}
void AWallManager::DestroyFloorPreview() {
	APMFloorPreview->Destroy();
}


// FLOORS
void AWallManager::GenerateFloor(float width, float height, FVector wallStartPoint, FVector wallEndPoint, bool preview) {


	if (wallEndPoint.Y < wallStartPoint.Y) {//dragging left (-Y)
		float temp = wallEndPoint.Y;
		wallEndPoint.Y = wallStartPoint.Y;
		wallStartPoint.Y = temp;
	}

	if (wallEndPoint.X < wallStartPoint.X) {//dragging left (-Y)
		float temp = wallEndPoint.X;
		wallEndPoint.X = wallStartPoint.X;
		wallStartPoint.X = temp;
	}

	//Determine which axis to draw wall on
	float lenY = wallEndPoint.Y - wallStartPoint.Y;
	float lenX = wallEndPoint.X - wallStartPoint.X;


	UE_LOG(LogTemp, Warning, TEXT("DRAW FLOOR LEN X: %f, Y: %f"), lenX, lenY );


	//SET VARIABLES
	//If I don't reverse the width, it renders them inside out cause they're pushed to opposite side
	bool drawingNegative = lenY < 0 || lenX < 0;
	float halfWidth = (drawingNegative) ? -1 * (width / 2.0f) : width / 2.0f;//set distance to negative so the math below works
	FTransform SpawnLocAndRotation;
	SpawnLocAndRotation.SetLocation(wallStartPoint);
	//SpawnLocAndRotation.SetLocation(FVector(0));//This will spawn actor at origin and draw the mesh in world space


	//Build Geometry
	//AddVerticiesAndTriangles(wallStartPoint, wallEndPoint, preview, halfWidth, lenX, lenY, height);

	bool drawShadow = true;
	FVector drawPos(0.0f);
	FColor drawColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f).ToFColor(true);
	float drawDuration = 123456789.0f;
	//wallStartPoint

	//DrawDebugString(GetWorld(), wallStartPoint + FVector()    , *FString::Printf(TEXT("%s"), TEXT(" ")), NULL, drawColor, drawDuration, drawShadow);


	Vertices.Add(FVector(0 - halfWidth, 0 - halfWidth, 0)); //lower left - 0
	//DrawDebugString(GetWorld(), wallStartPoint + FVector(0 - halfWidth, 0 - halfWidth, 0.f), *FString::Printf(TEXT("%s"), TEXT("lower left - 0")), NULL, drawColor, drawDuration, drawShadow);

	Vertices.Add(FVector(0 - halfWidth, 0 - halfWidth, height)); //upper left - 1 - Y to align corner
	//DrawDebugString(GetWorld(), wallStartPoint + FVector(0 - halfWidth, 0 - halfWidth, height), *FString::Printf(TEXT("%s"), TEXT("upper left - 1 ")), NULL, drawColor, drawDuration, drawShadow);

	Vertices.Add(FVector(0 - halfWidth, lenY + halfWidth, 0)); //lower right - 2 
	//DrawDebugString(GetWorld(), wallStartPoint + FVector(0 - halfWidth, lenY + halfWidth, 0), *FString::Printf(TEXT("%s"), TEXT("lower right - 2 ")), NULL, drawColor, drawDuration, drawShadow);

	Vertices.Add(FVector(0 - halfWidth, lenY + halfWidth, height)); //upper right - 3
	//DrawDebugString(GetWorld(), wallStartPoint + FVector(0 - halfWidth, lenY + halfWidth, height), *FString::Printf(TEXT("%s"), TEXT("upper right - 3")), NULL, drawColor, drawDuration, drawShadow);

	Vertices.Add(FVector(lenX + halfWidth, 0 - halfWidth, 0)); //lower front left - 4
	//DrawDebugString(GetWorld(), wallStartPoint + FVector(lenX + halfWidth, 0 - halfWidth, 0), *FString::Printf(TEXT("%s"), TEXT("lower front left - 4 ")), NULL, drawColor, drawDuration, drawShadow);

	Vertices.Add(FVector(lenX + halfWidth, 0 - halfWidth, height)); //upper front left - 5
	//DrawDebugString(GetWorld(), wallStartPoint + FVector(lenX + halfWidth, 0 - halfWidth, height), *FString::Printf(TEXT("%s"), TEXT("upper front left - 5 ")), NULL, drawColor, drawDuration, drawShadow);

	Vertices.Add(FVector(lenX + halfWidth, lenY + halfWidth, height)); //upper front right - 6
	//DrawDebugString(GetWorld(), wallStartPoint + FVector(lenX + halfWidth, lenY + halfWidth, height), *FString::Printf(TEXT("%s"), TEXT("upper front right - 6 ")), NULL, drawColor, drawDuration, drawShadow);

	Vertices.Add(FVector(lenX + halfWidth, lenY + halfWidth, 0)); //lower front right - 7
	//DrawDebugString(GetWorld(), wallStartPoint + FVector(lenX + halfWidth, lenY + halfWidth, 0), *FString::Printf(TEXT("%s"), TEXT("lower front right - 7 ")), NULL, drawColor, drawDuration, drawShadow);

	//Back face of cube
	AddTriangle(0, 2, 3);
	AddTriangle(3, 1, 0);

	//Left face of cube
	AddTriangle(0, 1, 4);
	AddTriangle(4, 1, 5);

	//Front face of cube
	AddTriangle(4, 5, 7);
	AddTriangle(7, 5, 6);

	//Right face of cube
	AddTriangle(7, 6, 3);
	AddTriangle(3, 2, 7);

	//Top face
	AddTriangle(1, 3, 5);
	AddTriangle(6, 5, 3);

	//bottom face
	AddTriangle(2, 0, 4);
	AddTriangle(4, 7, 2);

	//SpawnDeferred, add the triangles, then  let the begin play run
	APMFloor* SpawnedFloorRef = GetWorld()->SpawnActorDeferred<APMFloor>(APMFloor::StaticClass(), SpawnLocAndRotation);
	SpawnedFloorRef->SetParameters(Vertices, Triangles);
	SpawnedFloorRef->SetActorEnableCollision(false);
	SpawnedFloorRef->FinishSpawning(SpawnLocAndRotation);
	//UE_LOG(LogTemp, Warning, TEXT("SpawnedFloorRef"));

	


	//SpawnedFloorRef->GeneratedFloor->SetMaterial(0, DefaultFloorMaterial);
	
	if (preview) {
		APMFloorPreview = SpawnedFloorRef;
		APMFloorPreview->GeneratedFloor->SetMaterial(0, PreviewFloorMaterial);
	}
	else {
		SpawnedFloorRef->GeneratedFloor->SetMaterial(0, DefaultFloorMaterial);
	}

	//Reset to use on next wall
	Triangles.Reset();
	Vertices.Reset();
}

void AWallManager::MyGenerateBoxMesh(FVector BoxRadius, TArray<FVector>& Vertices1, TArray<int32>& Triangles1, TArray<FVector>& Normals, TArray<FVector2D>& UVs, TArray<FProcMeshTangent>& Tangents)
{
	//I can make the wall always the same direction except swap x and y lengths
	FVector DragStart, DragEnd;

	//I basically need to make sure the drag is always positive X or Y
	// So swap when negative like on the floor
	// then if drag is going in the X direction I need to swap the width and length 

	float wallHeight1 = 300;
	float wallWidth1 = 10;

	// Generate verts
	FVector BoxVerts[8];
	//BoxVerts[0] = FVector(-BoxRadius.X, BoxRadius.Y, BoxRadius.Z);
	//BoxVerts[1] = FVector(BoxRadius.X, BoxRadius.Y, BoxRadius.Z);
	//BoxVerts[2] = FVector(BoxRadius.X, -BoxRadius.Y, BoxRadius.Z);
	//BoxVerts[3] = FVector(-BoxRadius.X, -BoxRadius.Y, BoxRadius.Z);

	//BoxVerts[4] = FVector(-BoxRadius.X, BoxRadius.Y, -BoxRadius.Z);
	//BoxVerts[5] = FVector(BoxRadius.X, BoxRadius.Y, -BoxRadius.Z);
	//BoxVerts[6] = FVector(BoxRadius.X, -BoxRadius.Y, -BoxRadius.Z);
	//BoxVerts[7] = FVector(-BoxRadius.X, -BoxRadius.Y, -BoxRadius.Z);

	bool drawShadow = true;
	FColor drawColor = FLinearColor(0.0f, 1.0f, 0.0f, 1.0f).ToFColor(true);
	float drawDuration = 123456789.0f;
	DrawDebugString(GetWorld(), FVector(0,0,0), *FString::Printf(TEXT("%s"), TEXT("FUCK!")), NULL, drawColor, drawDuration, drawShadow);


	//TOP FACE
	FVector V0 = FVector(-BoxRadius.X - 5, BoxRadius.Y, BoxRadius.Z);
	BoxVerts[0] = V0;
	DrawDebugString(GetWorld(), V0, *FString::Printf(TEXT("%s"), TEXT("0")), NULL, drawColor, drawDuration, drawShadow);

	FVector V1 = FVector(BoxRadius.X, BoxRadius.Y, BoxRadius.Z);
	BoxVerts[1] = V1;
	DrawDebugString(GetWorld(), V1, *FString::Printf(TEXT("%s"), TEXT("1")), NULL, drawColor, drawDuration, drawShadow);

	FVector V2 = FVector(BoxRadius.X, -BoxRadius.Y, BoxRadius.Z);
	BoxVerts[2] = V2;
	DrawDebugString(GetWorld(), V2, *FString::Printf(TEXT("%s"), TEXT("2")), NULL, drawColor, drawDuration, drawShadow);

	FVector V3 = FVector(-BoxRadius.X, -BoxRadius.Y, BoxRadius.Z);
	BoxVerts[3] = V3;
	DrawDebugString(GetWorld(), V3, *FString::Printf(TEXT("%s"), TEXT("3")), NULL, drawColor, drawDuration, drawShadow);



	//Bottom Face
	FVector V4 = FVector(-BoxRadius.X, BoxRadius.Y, -BoxRadius.Z);
	BoxVerts[4] = V4;
	DrawDebugString(GetWorld(), V4, *FString::Printf(TEXT("%s"), TEXT("4")), NULL, drawColor, drawDuration, drawShadow);

	FVector V5 = FVector(BoxRadius.X, BoxRadius.Y, -BoxRadius.Z);
	BoxVerts[5] = V5;
	DrawDebugString(GetWorld(), V5, *FString::Printf(TEXT("%s"), TEXT("5")), NULL, drawColor, drawDuration, drawShadow);

	FVector V6 = FVector(BoxRadius.X, -BoxRadius.Y, -BoxRadius.Z);
	BoxVerts[6] = V6;
	DrawDebugString(GetWorld(), V6, *FString::Printf(TEXT("%s"), TEXT("6")), NULL, drawColor, drawDuration, drawShadow);

	FVector V7 = FVector(-BoxRadius.X, -BoxRadius.Y, -BoxRadius.Z);
	BoxVerts[7] = V7;
	DrawDebugString(GetWorld(), V7, *FString::Printf(TEXT("%s"), TEXT("7")), NULL, drawColor, drawDuration, drawShadow);


	// Generate triangles (from quads)
	Triangles.Reset();
	const int32 NumVerts = 24; // 6 faces x 4 verts per face

	Vertices.Reset();
	Vertices.AddUninitialized(NumVerts);

	Normals.Reset();
	Normals.AddUninitialized(NumVerts);

	Tangents.Reset();
	Tangents.AddUninitialized(NumVerts);

	//Top Face
	Vertices[0] = BoxVerts[0];
	Vertices[1] = BoxVerts[1];
	Vertices[2] = BoxVerts[2];
	Vertices[3] = BoxVerts[3];
	UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 0, 1, 2, 3);
	Normals[0] = Normals[1] = Normals[2] = Normals[3] = FVector(0, 0, 1);
	Tangents[0] = Tangents[1] = Tangents[2] = Tangents[3] = FProcMeshTangent(0.f, -1.f, 0.f);

	//South Side - Facing -X
	Vertices[4] = BoxVerts[4];
	Vertices[5] = BoxVerts[0];
	Vertices[6] = BoxVerts[3];
	Vertices[7] = BoxVerts[7];
	UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 4, 5, 6, 7);
	Normals[4] = Normals[5] = Normals[6] = Normals[7] = FVector(-1, 0, 0);
	Tangents[4] = Tangents[5] = Tangents[6] = Tangents[7] = FProcMeshTangent(0.f, -1.f, 0.f);

	//East Side - Facing +Y
	Vertices[8] = BoxVerts[5];
	Vertices[9] = BoxVerts[1];
	Vertices[10] = BoxVerts[0];
	Vertices[11] = BoxVerts[4];
	UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 8, 9, 10, 11);
	Normals[8] = Normals[9] = Normals[10] = Normals[11] = FVector(0, 1, 0);
	Tangents[8] = Tangents[9] = Tangents[10] = Tangents[11] = FProcMeshTangent(-1.f, 0.f, 0.f);

	//North Side - Facing +X
	Vertices[12] = BoxVerts[6];
	Vertices[13] = BoxVerts[2];
	Vertices[14] = BoxVerts[1];
	Vertices[15] = BoxVerts[5];
	UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 12, 13, 14, 15);
	Normals[12] = Normals[13] = Normals[14] = Normals[15] = FVector(1, 0, 0);
	Tangents[12] = Tangents[13] = Tangents[14] = Tangents[15] = FProcMeshTangent(0.f, 1.f, 0.f);

	//West Side - Facing -Y
	Vertices[16] = BoxVerts[7];
	Vertices[17] = BoxVerts[3];
	Vertices[18] = BoxVerts[2];
	Vertices[19] = BoxVerts[6];
	UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 16, 17, 18, 19);
	Normals[16] = Normals[17] = Normals[18] = Normals[19] = FVector(0, -1, 0);
	Tangents[16] = Tangents[17] = Tangents[18] = Tangents[19] = FProcMeshTangent(1.f, 0.f, 0.f);

	//Bottom Face
	Vertices[20] = BoxVerts[7];
	Vertices[21] = BoxVerts[6];
	Vertices[22] = BoxVerts[5];
	Vertices[23] = BoxVerts[4];
	UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 20, 21, 22, 23);
	Normals[20] = Normals[21] = Normals[22] = Normals[23] = FVector(0, 0, -1);
	Tangents[20] = Tangents[21] = Tangents[22] = Tangents[23] = FProcMeshTangent(0.f, 1.f, 0.f);

	// UVs
	UVs.Reset();
	UVs.AddUninitialized(NumVerts);

	//There is 1 per face
	//Top --- East --- North --- West --- South --- Bottom? UVs[20] is vertex 7 which seems wrong like it's drawing up into the wall/sky
	UVs[0] = UVs[4] = UVs[8] = UVs[12] = UVs[16] = UVs[20] = FVector2D(0.f, 0.f);
	UVs[1] = UVs[5] = UVs[9] = UVs[13] = UVs[17] = UVs[21] = FVector2D(0.f, 1.f);
	UVs[2] = UVs[6] = UVs[10] = UVs[14] = UVs[18] = UVs[22] = FVector2D(1.f, 1.f);
	UVs[3] = UVs[7] = UVs[11] = UVs[15] = UVs[19] = UVs[23] = FVector2D(1.f, 0.f);

	TArray<FLinearColor> VertexColors;

	MyGeneratedBoxMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, VertexColors, Tangents, true);



}
