// Copyright Joni Käki-Mäkelä

#include "CelestialFunctionLibrary.h"
BEGIN_DEFINE_SPEC(CelestialFunctionLibrarySpec, "CelestialFunctionLibrary",
                  EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
END_DEFINE_SPEC(CelestialFunctionLibrarySpec)

void CelestialFunctionLibrarySpec::Define()
{
	Describe("2000-01-01T12:00", [this]()
	{
		// noon jan 1 2000
		FDateTime time = FDateTime::FromUnixTimestamp(946728000);
		It("should calculate julian day correctly", [this, time]()
		{
			double julianday = UCelestialFunctionLibrary::GetJulianDay(time);
			double expected = 2451545.0;
			TestEqual("Julian day", julianday, expected);
		});

		It("should calculate greenwich mean sidereal time correctly", [this, time]()
		{
			float gmst = UCelestialFunctionLibrary::GetGreenwichMeanSiderealAngle(time);
			TestEqual("Greenwich mean sidereal time degrees", gmst, 280.4606183700f);
		});

		It("should calculate local mean sidereal time correctly at 0 longitude", [this, time]()
		{
			float gms = UCelestialFunctionLibrary::GetGreenwichMeanSiderealAngle(time);
			float localMeanSidereal = UCelestialFunctionLibrary::GetLocalMeanSiderealAngle(gms, 0.f);
			TestEqual("local mean sidereal time degrees", localMeanSidereal, 280.4606183700f);
		});

		It("should calculate local mean sidereal time correctly at 90 degrees E longitude", [this, time]()
		{
			float gms = UCelestialFunctionLibrary::GetGreenwichMeanSiderealAngle(time);
			float localMeanSidereal = UCelestialFunctionLibrary::GetLocalMeanSiderealAngle(gms, 90.f);
			TestEqual("local mean sidereal time degrees", localMeanSidereal, 10.4606183700f);
		});

		It("should calculate local mean sidereal time correctly at -90 degrees E longitude", [this, time]()
		{
			float gms = UCelestialFunctionLibrary::GetGreenwichMeanSiderealAngle(time);
			float localMeanSidereal = UCelestialFunctionLibrary::GetLocalMeanSiderealAngle(gms, -90.f);
			TestEqual("local mean sidereal time degrees", localMeanSidereal, 190.4606183700f);
		});
	});
}
