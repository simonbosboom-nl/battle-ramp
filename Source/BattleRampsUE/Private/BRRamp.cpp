#include "BRRamp.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
ABRRamp::ABRRamp()
{
    PrimaryActorTick.bCanEverTick = false;
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RampMesh"));
    SetRootComponent(Mesh);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeMesh.Succeeded()) Mesh->SetStaticMesh(CubeMesh.Object);
    Mesh->SetRelativeScale3D(FVector(3.0f, 2.0f, 0.16f));
    Mesh->SetCollisionProfileName(TEXT("BlockAll"));
    SetLifeSpan(45.0f);
}
