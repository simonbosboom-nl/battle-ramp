#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BRRamp.generated.h"
class UStaticMeshComponent;
UCLASS()
class BATTLERAMPSUE_API ABRRamp : public AActor
{
    GENERATED_BODY()
public:
    ABRRamp();
    UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> Mesh;
};
