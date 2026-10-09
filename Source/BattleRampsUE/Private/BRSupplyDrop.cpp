#include "BRSupplyDrop.h"
#include "BRCarPawn.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
ABRSupplyDrop::ABRSupplyDrop()
{
    PrimaryActorTick.bCanEverTick = true;
    Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("PickupTrigger")); SetRootComponent(Trigger);
    Trigger->SetBoxExtent(FVector(100,100,100)); Trigger->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    Trigger->SetGenerateOverlapEvents(true);
    CrateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SupplyCrate")); CrateMesh->SetupAttachment(Trigger);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (Cube.Succeeded()) CrateMesh->SetStaticMesh(Cube.Object);
    CrateMesh->SetRelativeScale3D(FVector(0.75f)); CrateMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
void ABRSupplyDrop::BeginPlay()
{
    Super::BeginPlay(); Trigger->OnComponentBeginOverlap.AddDynamic(this, &ABRSupplyDrop::OnDropOverlap);
    GroundZ = 85.0f;
}
void ABRSupplyDrop::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if (GetActorLocation().Z > GroundZ) SetActorLocation(GetActorLocation() - FVector(0,0,420.0f*DeltaSeconds));
    else SetActorLocation(FVector(GetActorLocation().X,GetActorLocation().Y,GroundZ));
    AddActorLocalRotation(FRotator(0,32.0f*DeltaSeconds,0));
}
void ABRSupplyDrop::OnDropOverlap(UPrimitiveComponent*, AActor* OtherActor, UPrimitiveComponent*, int32, bool, const FHitResult&)
{
    if (ABRCarPawn* Car = Cast<ABRCarPawn>(OtherActor))
    {
        switch (FMath::RandRange(0,2))
        {
        case 0: Car->AddPowerKicks(2); break;
        case 1: Car->AddRamps(2); break;
        default: Car->Repair(45.0f); Car->AddBoost(20.0f); break;
        }
        Destroy();
    }
}
