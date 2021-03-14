// Copyright Joni Käki-Mäkelä


#include "CelestialSphereRotationComponent.h"
#include "CelestialFunctionLibrary.h"

// Sets default values for this component's properties
UCelestialSphereRotationComponent::UCelestialSphereRotationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	FDateTime::ParseIso8601(TEXT("2000-03-20T12:00:00.000Z"), StartTime);
	TimeScale = 1.f;
	Latitude = 60.1733244;
	Longitude = 24.9410248;
}


// Called when the game starts
void UCelestialSphereRotationComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerRoot = GetOwner()->GetRootComponent();
	GameStartTime = FDateTime::UtcNow();
	StartTransform = GetOwner()->GetActorTransform();	
}

void UCelestialSphereRotationComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);	
	if(GetOwner() && GetWorld() && !GetWorld()->IsPreviewWorld())
	{
		CurrentGreenwichMeanSiderealAngle = UCelestialFunctionLibrary::GetGreenwichMeanSiderealAngle(CurrentTime);
		CurrentLocalMeanSiderealAngle = UCelestialFunctionLibrary::GetLocalMeanSiderealAngle(CurrentGreenwichMeanSiderealAngle, Longitude);
		RotateSky();
	}
}

void UCelestialSphereRotationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CurrentTime = GetScaledElapsedTime();
	CurrentGreenwichMeanSiderealAngle = UCelestialFunctionLibrary::GetGreenwichMeanSiderealAngle(CurrentTime);
	CurrentLocalMeanSiderealAngle = UCelestialFunctionLibrary::GetLocalMeanSiderealAngle(CurrentGreenwichMeanSiderealAngle, Longitude);
	RotateSky();
}

FDateTime UCelestialSphereRotationComponent::GetScaledElapsedTime() const
{
	auto now = FDateTime::UtcNow();
	auto timeSinceStart = now - GameStartTime;
	timeSinceStart = timeSinceStart * TimeScale;
	const auto time = StartTime + timeSinceStart;
	return time;
}

void UCelestialSphereRotationComponent::RotateSky()
{
	const float flippedAngle = CurrentLocalMeanSiderealAngle * -1.f;
	const FRotator rot(Latitude, 0.f, flippedAngle);
	GetOwner()->SetActorRelativeRotation(rot);
}
