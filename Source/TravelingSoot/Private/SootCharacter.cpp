#include "SootCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"

// Sets default values
ASootCharacter::ASootCharacter()
{
  // Set this character to call Tick() every frame.  You can turn this off to
  // improve performance if you don't need it. PrimaryActorTick.bCanEverTick =
  // true;

  bReplicates = true;

  // Set size for collision capsule
  GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

  // set our turn rates for input
  BaseTurnRate = 45.f;
  BaseLookUpRate = 45.f;

  // Create a CameraComponent
  FirstPersonCameraComponent =
      CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
  FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
  // Position the camera
  FirstPersonCameraComponent->SetRelativeLocation(
      FVector(-39.56f, 1.75f, 64.f));
  FirstPersonCameraComponent->bUsePawnControlRotation = true;
}

// Called when the game starts or when spawned
void ASootCharacter::BeginPlay()
{
  Super::BeginPlay();
}

// Called every frame
// void ASootCharacter::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);

// }

// Called to bind functionality to input
void ASootCharacter::SetupPlayerInputComponent(
    class UInputComponent *PlayerInputComponent)
{
  // set up gameplay key bindings
  check(PlayerInputComponent);

  // Bind jump events
  PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
  PlayerInputComponent->BindAction("Jump",
                                   IE_Released,
                                   this,
                                   &ACharacter::StopJumping);

  // Bind movement events
  PlayerInputComponent->BindAxis("MoveForward",
                                 this,
                                 &ASootCharacter::MoveForward);
  PlayerInputComponent->BindAxis("MoveRight",
                                 this,
                                 &ASootCharacter::MoveRight);

  // We have 2 versions of the rotation bindings to handle different kinds of
  // devices differently
  //
  // "turn" handles devices that provide an absolute delta, such as a mouse.
  //
  // "turnrate" is for devices that we choose to treat as a rate of change,
  // such as an analog joystick
  //
  PlayerInputComponent->BindAxis("Turn",
                                 this,
                                 &APawn::AddControllerYawInput);
  PlayerInputComponent->BindAxis("TurnRate",
                                 this,
                                 &ASootCharacter::TurnAtRate);
  PlayerInputComponent->BindAxis("LookUp",
                                 this,
                                 &APawn::AddControllerPitchInput);
  PlayerInputComponent->BindAxis("LookUpRate",
                                 this,
                                 &ASootCharacter::LookUpAtRate);
}

// Movin'...

void ASootCharacter::MoveForward(float Value)
{
  if (Value != 0.0f)
  {
    // add movement in that direction
    AddMovementInput(GetActorForwardVector(), Value);
  }
}

void ASootCharacter::MoveRight(float Value)
{
  if (Value != 0.0f)
  {
    // add movement in that direction
    AddMovementInput(GetActorRightVector(), Value);
  }
}

void ASootCharacter::TurnAtRate(float Rate)
{
  // calculate delta for this frame from the rate information
  AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASootCharacter::LookUpAtRate(float Rate)
{
  // calculate delta for this frame from the rate information
  AddControllerPitchInput(
      Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
