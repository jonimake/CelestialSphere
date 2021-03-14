// Copyright Joni Käki-Mäkelä

#pragma once

#include "Engine/DataTable.h"
#include "Star.generated.h"

USTRUCT()
struct CELESTIALSPHERE_API FStar : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Proper;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Dist;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Mag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float AbsMag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Ci;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float X;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Y;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Z;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Vx;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Vy;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Vz;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Lum;

	FString ToString() const
	{
		return FString::Printf(TEXT("Proper = %s, (x=%f, y=%f, z=%f)"), *Proper, X, Y, Z);
	}
};
