// Copyright Joni Käki-Mäkelä

#include "CelestialFunctionLibrary.h"
#include "Star.h"

struct FStar;
FDateTime UCelestialFunctionLibrary::J2000Epoch = FDateTime::FromUnixTimestamp(946728000);


float UCelestialFunctionLibrary::GetGreenwichMeanSiderealAngle(const FDateTime& GregorianDateTime)
{
	const double J2000 = GetJulianDay(J2000Epoch);
	const double julianDay = GetJulianDay(GregorianDateTime);

	double greenwichMeanSiderealTime = 280.46061837 + 360.98564736629 * (julianDay - J2000);
	const float sign = FMath::Sign(greenwichMeanSiderealTime);
	while(greenwichMeanSiderealTime < 0.f || greenwichMeanSiderealTime > 360.f)
	{
		greenwichMeanSiderealTime = greenwichMeanSiderealTime - sign * 360.f;
	}
	return greenwichMeanSiderealTime;
}

double UCelestialFunctionLibrary::GetJulianDay(const FDateTime& dateTime)
{
	return 1721425.5 + static_cast<double>(dateTime.GetTicks()) / static_cast<double>(ETimespan::TicksPerDay);	
}

FDateTime UCelestialFunctionLibrary::GetJ2000Epoch()
{
	return J2000Epoch;
}

float UCelestialFunctionLibrary::GetLocalMeanSiderealAngle(float InGreenwichMeanSiderealAngle, float Longitude)
{
	float lmst = InGreenwichMeanSiderealAngle + Longitude;
	// need to normalize to 0..360 again since longitude addition might cause the value to go out of 0..360 range
	const float sign = FMath::Sign(lmst);
	while(lmst < 0.f || lmst > 360.f)
	{
		lmst = lmst - sign * 360.f;
	}
	return lmst;
}


float UCelestialFunctionLibrary::GetStarKelvins(const FStar& StarDataRow)
{
	const float colourIndex = StarDataRow.Ci;
	const float x = 1 / (colourIndex * 0.92 + 1.7);
	const float y = 1 / (colourIndex * 0.92 + 0.62);
	return 4600.f * (x + y);
}