// Copyright Joni Käki-Mäkelä

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CelestialSphereRotationComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CELESTIALSPHERE_API UCelestialSphereRotationComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCelestialSphereRotationComponent();

protected:
	virtual void BeginPlay() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	UPROPERTY()
	USceneComponent* OwnerRoot;

	UPROPERTY()
	FTransform StartTransform;

	UPROPERTY()
	FDateTime GameStartTime;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Coordinates")
	float Latitude;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Coordinates")
	float Longitude;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Time")
	float CurrentLocalMeanSiderealAngle;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Time")
	float CurrentGreenwichMeanSiderealAngle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Time")
	FDateTime CurrentTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Time")
	float TimeScale;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	FDateTime GetScaledElapsedTime() const;

	UFUNCTION(BlueprintCallable)
	void RotateSky();
};
