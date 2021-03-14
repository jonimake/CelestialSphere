// Copyright Joni Käki-Mäkelä

#include "ProceduralCelestialSphere.h"
#include "CelestialFunctionLibrary.h"
#include "CelestialSphereRotationComponent.h"
#include "DrawDebugHelpers.h"
#include "ProceduralMeshComponent.h"
#include "Star.h"
#include "Components/ArrowComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/DataTable.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AProceduralCelestialSphere::AProceduralCelestialSphere()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetActorEnableCollision(false);

	ConstructorHelpers::FObjectFinder<UMaterial> textMaterialAsset(TEXT("Material'/CelestialSphere/Materials/AlignedTextMaterialOpaque.AlignedTextMaterialOpaque'"));
	if(textMaterialAsset.Succeeded())
	{
		TextMaterial = textMaterialAsset.Object;
	}

	CelestialSphereRotationComponent = CreateDefaultSubobject<UCelestialSphereRotationComponent>(TEXT("CelestialSphereRotationComponent"));
	
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	CelestialMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Celestial Mesh"));
	CelestialMesh->SetCastShadow(false);
	CelestialMesh->bUseAsyncCooking = true;
	CelestialMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CelestialMesh->SetCanEverAffectNavigation(false);
	CelestialMesh->SetupAttachment(GetRootComponent());
	CelestialMesh->SetGenerateOverlapEvents(false);
	CelestialMesh->CanCharacterStepUpOn = ECB_No;
	
	CelestialNorth = CreateDefaultSubobject<UArrowComponent>(TEXT("Celestial North Arrow"));
	CelestialNorth->SetupAttachment(CelestialMesh);
	CelestialNorth->SetRelativeRotation(FRotator::ZeroRotator);
	CelestialNorth->SetArrowColor(FColor::Blue);
	CelestialNorth->ArrowLength = 200.f;

	CelestialNorthText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Celestial North Label"));
	CelestialNorthText->SetText(FText::FromString("Celestial North Pole"));
	CelestialNorthText->SetupAttachment(CelestialNorth);
	CelestialNorthText->SetRelativeLocation(FVector(CelestialNorth->ArrowLength, 0,0 ));
	CelestialNorthText->SetMaterial(0, TextMaterial);
	CelestialNorthText->SetHiddenInGame(true);
	
	XPositive = CreateDefaultSubobject<UArrowComponent>(TEXT("X Positive Arrow"));
	XPositive->SetupAttachment(CelestialMesh);
	XPositive->SetRelativeRotation(FRotator(90,0,0));
	XPositive->SetArrowColor(FColor::Red);
	XPositive->ArrowLength = 200.f;
	
	XPositiveText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("X Positive Label"));
	XPositiveText->SetText(FText::FromString("Vernal Equinox"));
	XPositiveText->SetupAttachment(XPositive);
	XPositiveText->SetRelativeLocation(FVector(XPositive->ArrowLength, 0,0 ));
	XPositiveText->SetMaterial(0, TextMaterial);
	XPositiveText->SetHiddenInGame(true);
	
	YPositive = CreateDefaultSubobject<UArrowComponent>(TEXT("Y Positive Arrow"));
	YPositive->SetupAttachment(CelestialMesh);
	YPositive->SetRelativeRotation(FRotator(0,90,0));
	YPositive->SetArrowColor(FColor::Green);
	YPositive->ArrowLength = 200.f;
	
	YPositiveText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Y Positive Label"));
	YPositiveText->SetText(FText::FromString("Y Positive"));
	YPositiveText->SetupAttachment(YPositive);
	YPositiveText->SetRelativeLocation(FVector(YPositive->ArrowLength, 0,0 ));
	YPositiveText->SetMaterial(0, TextMaterial);
	YPositiveText->SetHiddenInGame(true);
	
	SetActorRotation(FRotator::ZeroRotator);
	AddTextComponents = false;
	
	ConstructorHelpers::FObjectFinder<UMaterialInstance> starMaterialAsset(TEXT("MaterialInstanceConstant'/CelestialSphere/Materials/M_Star_Inst.M_Star_Inst'"));
	if(starMaterialAsset.Succeeded())
	{
		CelestialMesh->SetMaterial(0, starMaterialAsset.Object);
	}
}

void AProceduralCelestialSphere::AddTextComponentForStar(const FStar& row)
{
	if(row.Proper.IsEmpty()) return;
	
	const FVector position(row.X, row.Y, row.Z);
	FVector dirNormal(position);
	dirNormal.Normalize();
	const FRotator rot = dirNormal.ToOrientationRotator();
	 
	FVector pos(StarDistance,0,0);
	pos = rot.RotateVector(pos);
	
	auto* cmpnt = NewObject<UTextRenderComponent>(this);
	cmpnt->RegisterComponent();
	cmpnt->AttachToComponent(CelestialMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	cmpnt->SetCastShadow(false);
	cmpnt->SetRelativeLocation(pos);
	auto rotation = UKismetMathLibrary::FindLookAtRotation(pos, GetActorLocation());
	cmpnt->SetWorldRotation(FRotator::ZeroRotator);
	auto txt = FText::FromString(row.Proper);
	cmpnt->SetText(txt);
	cmpnt->SetXScale(TextScale);
	cmpnt->SetYScale(TextScale);
	cmpnt->SetTextMaterial(TextMaterial);
	cmpnt->SetBoundsScale(2);
	TextRenderComponents.Add(cmpnt);
}

TArray<FVector> AProceduralCelestialSphere::GetQuadVerts(const FStar& StarData, float Scale, float Distance)
{
	const FVector position(StarData.X, StarData.Y, StarData.Z);
	FVector dirNormal(position);
	dirNormal.Normalize();
	const FRotator rot = dirNormal.ToOrientationRotator();

	FVector NW(Distance,-0.5,0.5);
	FVector NE(Distance,0.5,0.5);
	FVector SE(Distance,0.5,-0.5);
	FVector SW(Distance,-0.5,-0.5);

	const float scaleCoeff = Scale;
	NW = NW * FVector(1, scaleCoeff, scaleCoeff);
	NE = NE * FVector(1, scaleCoeff, scaleCoeff);
	SE = SE * FVector(1, scaleCoeff, scaleCoeff);
	SW = SW * FVector(1, scaleCoeff, scaleCoeff);
	
	NW = rot.RotateVector(NW);
	NE = rot.RotateVector(NE);
	SE = rot.RotateVector(SE);
	SW = rot.RotateVector(SW);
		
	TArray<FVector> verts {NW, NE, SE, SW};
	return verts;
}

TArray<int32> AProceduralCelestialSphere::GetQuadTriangleBuffer(int32 index)
{
	TArray<int32> triangles;
	const int32 offset = 4 * index;
	// triangle from SW, NE, NW
	triangles.Add(offset+3);
	triangles.Add(offset+1);
	triangles.Add(offset);
	// triangle from SW, SE, NE
	triangles.Add(offset+3);
	triangles.Add(offset+2);
	triangles.Add(offset+1);
	return triangles;
}

TArray<FVector2D> AProceduralCelestialSphere::GetQuadUVs()
{
	TArray<FVector2D> uvs;
	uvs.Reserve(4);
	uvs.Add(FVector2D(0,0)); // NW
	uvs.Add(FVector2D(1,0)); // NE
	uvs.Add(FVector2D(1,1)); // SE
	uvs.Add(FVector2D(0,1)); // SW
	return uvs;
}

TArray<FVector2D> AProceduralCelestialSphere::GetTriangleUVs()
{
	const float angle = 90.f;
	const float R = 0.5f;
	const FVector2D origin(0.5, 0.5); // centered in 0..1 UV space
	
	FVector2D a = FVector2D(R * FMath::Cos(angle), R * FMath::Sin(angle)) + origin;
	FVector2D b = FVector2D(R * FMath::Cos(angle + 4.f * PI / 3.f), R * FMath::Sin(angle + 4.f * PI / 3)) + origin;
	FVector2D c = FVector2D(R * FMath::Cos(angle + 2.f * PI / 3.f), R * FMath::Sin(angle + 2.f * PI / 3)) + origin;

	return TArray<FVector2D>{a,b,c};
}

TArray<FVector> AProceduralCelestialSphere::GetTriangleVerts(const FStar& StarData, float Scale, float Distance)
{
	const FVector position(StarData.X, StarData.Y, StarData.Z);
	FVector dirNormal(position);
	dirNormal.Normalize();
	const FRotator rot = dirNormal.ToOrientationRotator();

	const float angle = 90.f;
	const float L = 1;
	const float R = Scale / 2.f;
	// form a triangle centered at X axis
	FVector a(Distance, R * FMath::Cos(angle), R * FMath::Sin(angle));
	FVector b(Distance, R * FMath::Cos(angle + 4.f * PI / 3.f), R * FMath::Sin(angle + 4.f * PI / 3));
	FVector c(Distance, R * FMath::Cos(angle + 2.f * PI / 3.f), R * FMath::Sin(angle + 2.f * PI / 3));
	
	a = rot.RotateVector(a);
	b = rot.RotateVector(b);
	c = rot.RotateVector(c);
	
	return TArray<FVector>{a,b,c};	
}

TArray<int32> AProceduralCelestialSphere::GetTriangleBuffer(int32 index)
{
	TArray<int32> triangles;
	const int32 offset = 3 * index;
	// a simple triangle with contiguous indices
	triangles.Add(offset+2);
	triangles.Add(offset+1);
	triangles.Add(offset);
	return triangles;
}

TArray<FStar> AProceduralCelestialSphere::GetRowData(UDataTable* Table)
{
	TArray<FStar*> rows;
	Table->GetAllRows(TEXT(""), rows);

	TArray<FStar> rowValues;
	rowValues.Reserve(rows.Num());
	for(auto&& row : rows)
	{
		rowValues.Add(*row);
	}
	return rowValues;
}

void AProceduralCelestialSphere::ConvertCoordinates(TArray<FStar>& data)
{
	for (FStar& starData : data)
	{
		auto x = starData.X;
		auto vx = starData.Vx;
		starData.X = starData.Z;
		starData.Vx = starData.Vz;
		starData.Z = x;
		starData.Vz = vx;
	}
}

void AProceduralCelestialSphere::BuildMesh_Implementation(const TArray<FStar>& InData)
{
	FTransform saved = GetActorTransform();
	SetActorTransform(FTransform::Identity);
	CelestialMesh->ClearAllMeshSections();
	for(auto* cmnt : TextRenderComponents)
	{
		if(cmnt)
		{
			cmnt->DestroyComponent();	
		}
	}
	TextRenderComponents.Empty();
	MinMagnitude = TNumericLimits<float>::Max();
	MaxMagnitude = TNumericLimits<float>::Min();
	
	TArray<FVector> vertices;
	TArray<int32> triangles;
	TArray<FVector> normals;
	TArray<FVector2D> uvs;
	TArray<FVector2D> uvs2;
	TArray<FColor> vertexColours;
	TArray<FProcMeshTangent> tangents;

	vertices.Reserve(InData.Num() * 4);
	triangles.Reserve(InData.Num() * 6);
	normals.Reserve(InData.Num() * 4);
	uvs.Reserve(InData.Num() * 4);
	uvs2.Reserve(InData.Num() * 4);
	vertexColours.Reserve(InData.Num() * 4);
	TArray<FStar> data(InData);
	data.Sort([&](const FStar& a, const FStar& b)
	{
		return a.Mag < b.Mag;
	});
;
	int32 limit = MaxNumberStars > 0 ? MaxNumberStars : data.Num();
	limit = FMath::Min(limit, data.Num());
	for(int32 index = 0; index < limit; index++)
	{
		const FStar row = data[index];
		const float kelvins = UCelestialFunctionLibrary::GetStarKelvins(row);
		const FVector2D kelvinsAndMagnitude(kelvins, row.Mag);
		
		FVector starPos(row.X, row.Y, row.Z);
		FVector dirNormal(starPos);
		dirNormal.Normalize();

		TArray<FVector> verts;
		TArray<int32> indices;
		TArray<FVector2D> starUvs;
		switch (Geometry)
		{
		case EStarGeometry::Triangle:
			verts = GetTriangleVerts(row, StarSize, StarDistance);
			indices = GetTriangleBuffer(index);
			starUvs = GetTriangleUVs();
			break;
		case EStarGeometry::Quad:
			verts = GetQuadVerts(row, StarSize, StarDistance);
			indices = GetQuadTriangleBuffer(index);
			starUvs = GetQuadUVs();
			break;
		default: checkNoEntry();
		}

		vertices.Append(verts);
		triangles.Append(indices);
		uvs.Append(starUvs);
		
		for(int32 i = 0; i < verts.Num(); i++)
		{
			normals.Add(dirNormal * -1);
			uvs2.Add(kelvinsAndMagnitude);
		}		

		MinMagnitude = FMath::Min(MinMagnitude, row.Mag);
		MaxMagnitude = FMath::Max(MaxMagnitude, row.Mag);

		if(AddTextComponents)
		{
			AddTextComponentForStar(row);
		}
	}
	
	bool createCollision = false;
	CelestialMesh->CreateMeshSection(0, vertices, triangles, normals, uvs, uvs2,TArray<FVector2D>(),TArray<FVector2D>(), vertexColours, tangents, createCollision);
	SetActorTransform(saved);
}
