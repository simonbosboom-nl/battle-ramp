#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BRSupplyDrop.generated.h"
class UBoxComponent;
class UStaticMeshComponent;
UCLASS()
class BATTLERAMPSUE_API ABRSupplyDrop : public AActor
{
    GENERATED_BODY()
public:
    ABRSupplyDrop();
    virtual void Tick(float DeltaSeconds) override;
protected:
    virtual void BeginPlay() override;
private:
    UPROPERTY(VisibleAnywhere) TObjectPtr<UBoxComponent> Trigger;
    UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> CrateMesh;
    float GroundZ = 85.0f;
    UFUNCTION() void OnDropOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
