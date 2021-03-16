// Copyright Joni Käki-Mäkelä

#pragma once

#include "CoreMinimal.h"
#include "Star.h"
#include "GameFramework/Actor.h"
#include "ProceduralCelestialSphere.generated.h"

class UCelestialSphereRotationComponent;
class UArrowComponent;
class UTextRenderComponent;


UENUM(BlueprintType)
enum class EStarGeometry : uint8
{
	Triangle,
	Quad
};

UCLASS()
class CELESTIALSPHERE_API AProceduralCelestialSphere : public AActor
{
	GENERATED_BODY()

public:

	AProceduralCelestialSphere();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
	class UCelestialSphereRotationComponent* CelestialSphereRotationComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
	class USceneComponent* SceneRoot;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	UArrowComponent* CelestialNorth;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	UTextRenderComponent* CelestialNorthText;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	UArrowComponent* XPositive;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	UTextRenderComponent* XPositiveText;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	UArrowComponent* YPositive;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	UTextRenderComponent* YPositiveText;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
	class UProceduralMeshComponent* CelestialMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Celestial Sphere|Labels")
	TArray<UTextRenderComponent*> TextRenderComponents;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Celestial Sphere|Labels")
	bool AddTextComponents;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Celestial Sphere|Labels")
	float TextScale;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Celestial Sphere")
	UMaterialInterface* TextMaterial;

	// Size of the generated quad/triangle geometry of the star in unreal units.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Celestial Sphere")
	float StarSize = 1.f;

	// Distance of the star geometry from the actor origin.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Celestial Sphere")
	float StarDistance = 1.f;

	// Sorts the data by star magnitude before creating geometry.
	// In practise this means that the most visible stars are generated first.
	// This allows you to create, for example, the first 100 most visible stars on the sky if you want to.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Celestial Sphere")
	bool SortByApparentMagnitude;

	// Maximum number of stars to create. Only applies when value is above zero. 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Celestial Sphere")
	int32 MaxNumberStars = 0;

	// Minimum magnitude of the generated stars. This means the brightest one.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Celestial Sphere|Stats")
	float MinMagnitude;

	// Maximum magnitude of the generated stars. This means the dimmest one.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Celestial Sphere|Stats")
	float MaxMagnitude;

	// Type of geometry to generate: quads or triangles.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Celestial Sphere")
	EStarGeometry Geometry;

	// Add text component for the star, only adds one of the given Star data has a proper name.
	UFUNCTION(BlueprintCallable)
	void AddTextComponentForStar(const FStar& StarData);

	// Builds the procedural mesh
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void BuildMesh(const TArray<FStar>& InData);

	// Generates an array of vertices that form a quad the size of the given scale.
	// The vertices are rotated around the actor origin so that the position corresponds to given star data.
	// Finally the quad is also rotated to face the actor origin.
	UFUNCTION(BlueprintCallable)
	static TArray<FVector> GetQuadVerts(const FStar& StarData, float Scale = 1.f, float Distance = 1.f);

	// Creates an index buffer for a quad.
	UFUNCTION(BlueprintCallable)
	static TArray<int32> GetQuadTriangleBuffer(int32 index);

	// Generate UVs for a quad
	UFUNCTION(BlueprintCallable)
	static TArray<FVector2D> GetQuadUVs();

	// Generate UVs for a triangle. The UV forms an equilateral triangle with the center at [0.5, 0.5] in UV space
	// with the triangle rotated so that the bottom edge is horizontal.
	UFUNCTION(BlueprintCallable)
	static TArray<FVector2D> GetTriangleUVs();

	// Creates vertices that form an equilateral triangle at the given star coordinates. 
	// The triangle is rotated to face the actor origin.
	UFUNCTION(BlueprintCallable)
	static TArray<FVector> GetTriangleVerts(const FStar& StarData, float Scale = 1.f, float Distance = 1.f);

	// Generates a triangle index buffer.
	UFUNCTION(BlueprintCallable)
	static TArray<int32> GetTriangleBuffer(int32 index);

	// Reads star data from the given data table.
	UFUNCTION(BlueprintCallable)
	static TArray<FStar> GetRowData(UDataTable* Table);

	// Converts J2000 XYZ coordinates to UE coordinate system.
	// Essentially it just switches X and Z coordinates.
	UFUNCTION(BlueprintCallable)
	void ConvertCoordinates(UPARAM(ref) TArray<FStar>& data);
};
