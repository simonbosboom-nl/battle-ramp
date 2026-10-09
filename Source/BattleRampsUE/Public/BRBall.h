#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BRBall.generated.h"
class UStaticMeshComponent;
UCLASS()
class BATTLERAMPSUE_API ABRBall : public AActor
{
    GENERATED_BODY()
public:
    ABRBall();
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TObjectPtr<UStaticMeshComponent> Mesh;
    void ResetBall(const FVector& Location);
};
