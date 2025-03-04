// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	DebugLineTracing();
}

void UGrabber::DebugLineTracing()
{
	UWorld *World = GetWorld();
	if (!World)
		return;

	AActor *Actor = GetOwner();

	// 시작 및 끝 지점 정의
	FVector Start = Actor->GetActorLocation();
	FVector End = Start + Actor->GetActorForwardVector() * CheckDistance;

	// F콜리전쿼리파람으로 충돌체 설정
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Actor); // 부착된 오브젝트는 트레이싱 무시

	// 부딪힌 결과 저장
	TArray<FHitResult> HitResult;

	bool HasHit = World->SweepMultiByChannel(
		HitResult,
		Start, End,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeSphere(CheckRadius), QueryParams);

	if (HasHit)
	{
		for (FHitResult &Hit : HitResult)
		{
			AActor *HitActor = Hit.GetActor();

			// tag가 존재 한다면
			if (HitActor && HitActor->ActorHasTag("Weapon1"))
			{
				UStaticMeshComponent *Mesh = HitActor->FindComponentByClass<UStaticMeshComponent>();
				if (Mesh)
				{
					Mesh->SetRelativeLocation(FVector(6.f, -7.f, 12.f)); // 원하는 오프셋 적용
					Mesh->SetMobility(EComponentMobility::Movable);

					AttachMeshToPlayerHand(Mesh);
				}
			}
		}
	}

	// DrawDebugSphere(World, End, CheckRadius, 10, FColor::Red, false, 2);
}

void UGrabber::AttachMeshToPlayerHand(UStaticMeshComponent *Mesh)
{
	AActor *Player = GetOwner();
	if (!Player)
		return;

	FName HandSocketName = TEXT("rightSword");

	USkeletalMeshComponent *PlayerMesh = Player->FindComponentByClass<USkeletalMeshComponent>();

	if (PlayerMesh)
	{
		Mesh->AttachToComponent(PlayerMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, HandSocketName);
		Mesh->SetSimulatePhysics(false);

		// 부착 확인
		if (Mesh->GetAttachParent() == PlayerMesh)
		{
			UE_LOG(LogTemp, Display, TEXT("Mesh successfully attached to: %s"), *PlayerMesh->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Mesh attachment failed!"));
		}

		// 소켓 이름 확인
		FName AttachedSocket = Mesh->GetAttachSocketName();
		if (AttachedSocket == HandSocketName)
		{
			UE_LOG(LogTemp, Display, TEXT("Mesh successfully attached to socket: %s"), *AttachedSocket.ToString());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Mesh attached to the wrong socket: %s"), *AttachedSocket.ToString());
		}

		// 상대 위치 및 회전 확인
		FTransform RelativeTransform = Mesh->GetRelativeTransform();
		UE_LOG(LogTemp, Display, TEXT("Relative Location: %s, Rotation: %s"),
			   *RelativeTransform.GetLocation().ToString(),
			   *RelativeTransform.GetRotation().Rotator().ToString());
	}
}