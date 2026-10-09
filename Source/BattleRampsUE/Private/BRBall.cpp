#include "BRBall.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

ABRBall::ABRBall()
{
    PrimaryActorTick.bCanEverTick = false;
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
    SetRootComponent(Mesh);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    if (SphereMesh.Succeeded()) Mesh->SetStaticMesh(SphereMesh.Object);
    Mesh->SetRelativeScale3D(FVector(0.68f));
    Mesh->SetCollisionProfileName(TEXT("PhysicsActor"));
    Mesh->SetSimulatePhysics(true);
    Mesh->SetEnableGravity(true);
    Mesh->SetNotifyRigidBodyCollision(true);
    Mesh->SetMassOverrideInKg(NAME_None, 18.0f, true);
    Mesh->SetLinearDamping(0.25f);
    Mesh->SetAngularDamping(0.35f);
    SetActorEnableCollision(true);
}
void ABRBall::ResetBall(const FVector& Location)
{
    Mesh->SetSimulatePhysics(false);
    SetActorLocation(Location, false, nullptr, ETeleportType::TeleportPhysics);
    SetActorRotation(FRotator::ZeroRotator);
    Mesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
    Mesh->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
    Mesh->SetSimulatePhysics(true);
}
