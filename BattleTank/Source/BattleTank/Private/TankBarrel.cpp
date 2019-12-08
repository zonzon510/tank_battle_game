// Fill out your copyright notice in the Description page of Project Settings.


#include "TankBarrel.h"

void UTankBarrel::Elevate(float RelativeSpeed)
{
  // clamp relative speed
  RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1, +1);

  auto ElevationChange = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
  auto RawNewElevation = RelativeRotation.Pitch + ElevationChange;


  auto Elevation = FMath::Clamp<float>(RawNewElevation, MinElevationDeg, MaxElevationDeg);
  SetRelativeRotation(FRotator(Elevation,0,0));

}



