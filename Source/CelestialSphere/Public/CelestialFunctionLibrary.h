// Copyright Joni Käki-Mäkelä

#pragma once

#include "CoreMinimal.h"
#include "Star.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CelestialFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class CELESTIALSPHERE_API UCelestialFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	static FDateTime J2000Epoch;
	
public:	
	UFUNCTION(BlueprintCallable)
	static float GetGreenwichMeanSiderealAngle(const FDateTime& GregorianDateTime);

	UFUNCTION(BlueprintCallable)
    static float GetLocalMeanSiderealAngle(float InGreenwichMeanSiderealAngle, float Longitude);

	// Converts the star's B-V colour index into Kelvins.
	UFUNCTION(BlueprintCallable)
	static float GetStarKelvins(const FStar& StarDataRow);

	static double GetJulianDay(const FDateTime& dateTime);

	UFUNCTION(BlueprintCallable)
	static FDateTime GetJ2000Epoch();
};
