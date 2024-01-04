#include "PlayerCharacter.h"

#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"

#include "Treasure.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 20.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = true;

	IsPaused = false;
	HasWon = false;
	IsDead = false;
	IsPlayerDead = false;
	IsPlayerUnderAttack = false;
	Life = 100.0f;
	RunningPower = 30.0f;
	IsRunningBlocked = false;
	IsRunning = false;
	CatchedTargets = 0;
	Seconds = 0;
	Minutes = 0;
	TemporaryTime = 0.0f;
	IsAttackSoundPlaying = false;

	WalkingSpeed = GetCharacterMovement()->MaxWalkSpeed;
}

void APlayerCharacter::OnBeginOverlap(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &Sweep)
{
	if (OtherActor->ActorHasTag("Treasure")) {
		CatchedTargets += 1;
		Cast<ATreasure>(OtherActor)->Catch();
	};
	
	if (OtherActor->ActorHasTag("Enemy")) {
		IsPlayerUnderAttack = true;
	};
}

void APlayerCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Enemy")) {
		IsPlayerUnderAttack = false;
	};
}

void APlayerCharacter::MoveForward(float Axis)
{
	if (!IsPlayerDead) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, Axis);
	};
}

void APlayerCharacter::MoveRight(float Axis)
{
	if (!IsPlayerDead) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Axis);
	};
}

void APlayerCharacter::PlayerRunning()
{
	if (!IsRunningBlocked) {
		IsRunning = true;
		GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed * 2;
	};
}

void APlayerCharacter::PlayerNotRunning()
{
	IsRunning = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;
}

void APlayerCharacter::PauseGame()
{
	IsPaused = !IsPaused;

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->Pause();
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = IsPaused;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bEnableClickEvents = IsPaused;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bEnableMouseOverEvents = IsPaused;
}

void APlayerCharacter::RestartGame()
{
	if (IsPaused || HasWon || IsDead) {
		UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
	};
}

void APlayerCharacter::QuitGame()
{
	if (IsPaused) {
		FGenericPlatformMisc::RequestExit(false);
	};
}

void APlayerCharacter::Victory()
{
	HasWon = true;

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &APlayerCharacter::RestartGame, 5.0f, false);
}

void APlayerCharacter::Death()
{
	IsDead = true;

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &APlayerCharacter::RestartGame, 5.0f, false);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapEnd);
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsRunning && RunningPower > 0.0f && !IsRunningBlocked) {
		RunningPower -= DeltaTime * 2;
	} else if (RunningPower < 30.0f) {
		RunningPower += DeltaTime * 0.5f;
	};

	if (RunningPower <= 0) {
		IsRunningBlocked = true;
	};

	if (IsRunningBlocked && RunningPower >= 15.0f) {
		IsRunningBlocked = false;
	};

	TemporaryTime += DeltaTime;
	if (TemporaryTime >= 1.0f) {
		TemporaryTime -= 1;
		Seconds += 1;
	};

	if (Seconds >= 60.0f) {
		Minutes += 1;
		Seconds = 0.0f;
	};

	if (CatchedTargets == 3) {
		Victory();
	};

	if (Life <= 0) {
		IsPlayerDead = true;
		Death();
	};

	if (IsPlayerUnderAttack && !IsPlayerDead) {
		Life -= 20.0f * DeltaTime;
		if (!IsAttackSoundPlaying) {
			AttackSoundComponent = UGameplayStatics::SpawnSound2D(this, AttackSound, 0.5f);
			IsAttackSoundPlaying = true;
		};
	} else if (Life <= 100.0f && !IsPlayerDead) {
		Life += 2.0f * DeltaTime;
		if (IsAttackSoundPlaying) {
			IsAttackSoundPlaying = false;
			AttackSoundComponent->Stop();
		};
	};
	
	if (IsPlayerDead) {
		AttackSoundComponent->Stop();
	};
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Running", IE_Pressed, this, &APlayerCharacter::PlayerRunning);
	PlayerInputComponent->BindAction("Running", IE_Released, this, &APlayerCharacter::PlayerNotRunning);
	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &APlayerCharacter::PauseGame).bExecuteWhenPaused = true;
	PlayerInputComponent->BindAction("RestartGame", IE_Pressed, this, &APlayerCharacter::RestartGame).bExecuteWhenPaused = true;
	PlayerInputComponent->BindAction("QuitGame", IE_Pressed, this, &APlayerCharacter::QuitGame).bExecuteWhenPaused = true;
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
}