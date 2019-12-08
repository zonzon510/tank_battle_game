// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"


void ATankPlayerController::BeginPlay()
{
  Super::BeginPlay();

        auto ControlledTank = GetControlledTank();
        if(!ControlledTank){
          UE_LOG(LogTemp, Warning, TEXT( "PlayerController not possessing a tank" ));
        }
        else{
          UE_LOG(LogTemp, Warning, TEXT( "PlayerController possessing %s" ), *ControlledTank->GetName());
        }
}
void ATankPlayerController::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
  AimTowardsCrossHair();
}
ATank* ATankPlayerController::GetControlledTank() const
{
  return Cast<ATank>(GetPawn());
}
void ATankPlayerController::AimTowardsCrossHair()
{
  if(!GetControlledTank()){ return; }

  FVector OutHitLocation; // out parameter
  // get world location through crosshair
  if(GetSightRayHitLocation(OutHitLocation))
  {
    GetControlledTank()->AimAt(OutHitLocation);
  }
  // if it hits something
    // tell the control tank to aim at this point
}
bool ATankPlayerController::GetSightRayHitLocation(FVector& HitLocationIn) const
{
  // find the crosshair position
  int32 ViewportSizeX, ViewportSizeY;
  GetViewportSize(ViewportSizeX, ViewportSizeY);
  auto ScreenLocation = FVector2D(ViewportSizeX*CrossHairXLocation, ViewportSizeY*CrossHairYLocation);
  // de - projection screen position of the crosshair to a world direction

  FVector LookDirection;
  if(GetLookDirection(ScreenLocation, LookDirection))
  {
    // line trace through that direction, and see what we hit (up to max range)
    // UE_LOG(LogTemp, Warning, TEXT( "LookDirection: %s" ), *LookDirection.ToString());
    GetLookVectorHitLocation(LookDirection, HitLocationIn);
  }
  return true;
}
bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const
{
  FVector CameraWorldLocation; // for discarding
  return DeprojectScreenPositionToWorld(
      ScreenLocation.X,
      ScreenLocation.Y,
      CameraWorldLocation,
      LookDirection
      );
}
bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const
{
  FHitResult HitResult;
  auto StartLocation = PlayerCameraManager->GetCameraLocation();
  auto EndLocation = StartLocation + (LookDirection*LineTraceRange);
  if(GetWorld()->LineTraceSingleByChannel(
      HitResult, // Out hit
      StartLocation, // start
      EndLocation, // end
      ECollisionChannel::ECC_Visibility)
      )
  {
    HitLocation = HitResult.Location;
    return true;
  }
  HitLocation = FVector(0);
  return false;
}
