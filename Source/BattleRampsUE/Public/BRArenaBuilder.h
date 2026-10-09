#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BRArenaBuilder.generated.h"
class UMaterialInterface;
UCLASS()
class BATTLERAMPSUE_API ABRArenaBuilder : public AActor
{
    GENERATED_BODY()
public:
    ABRArenaBuilder();
    virtual void BeginPlay() override;
    UPROPERTY(EditDefaultsOnly, Category="Art") TObjectPtr<UMaterialInterface> GrassMaterial;
    UPROPERTY(EditDefaultsOnly, Category="Art") TObjectPtr<UMaterialInterface> StadiumMaterial;
private:
    void AddBox(const FVector& Location, const FVector& Scale, const FRotator& Rotation,
                const FLinearColor& Color, bool bCollision=true, UMaterialInterface* OverrideMaterial=nullptr);
};
