#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BRCarPawn.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UMaterialInstanceDynamic;
class USpringArmComponent;
class UCameraComponent;
class UStaticMesh;
class ABRBall;
UENUM(BlueprintType)
enum class EBRVehicleType : uint8 { RallyGT, NeonSpeedster, BattleTank, DuneBuggy, HyperX };

UCLASS()
class BATTLERAMPSUE_API ABRCarPawn : public APawn
{
    GENERATED_BODY()
public:
    ABRCarPawn();
    virtual void Tick(float DeltaSeconds) override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    void SetAIControlled(bool bValue) { bAIControlled = bValue; }
    void SetBallTarget(ABRBall* InBall) { BallTarget = InBall; }
    void SetTeamIndex(int32 InTeam) { TeamIndex = InTeam; UpdatePaint(); }
    void SetVehicleType(EBRVehicleType NewType);
    void ApplyHit(float Damage, const FVector& Impulse);
    void AddBoost(float Amount);
    void AddPowerKicks(int32 Amount);
    void AddRamps(int32 Amount);
    void Repair(float Amount);
    int32 GetPowerKicks() const { return PowerKicks; }
    float GetBoostAmount() const { return BoostAmount; }
    int32 GetRampCount() const { return RampCharges; }
    float GetHealth() const { return Health; }
    float GetMaxHealth() const { return MaxHealth; }
    int32 GetTeamIndex() const { return TeamIndex; }
    void ApplyCosmetic(int32 StyleIndex);
    void RespawnAt(const FTransform& Transform);
protected:
    virtual void BeginPlay() override;
private:
    UPROPERTY(VisibleAnywhere) TObjectPtr<UBoxComponent> CollisionRoot;
    UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> Body;
    UPROPERTY(VisibleAnywhere) TArray<TObjectPtr<UStaticMeshComponent>> Wheels;
    UPROPERTY(VisibleAnywhere) TArray<TObjectPtr<UStaticMeshComponent>> DetailMeshes;
    UPROPERTY(VisibleAnywhere) TObjectPtr<USpringArmComponent> SpringArm;
    UPROPERTY(VisibleAnywhere) TObjectPtr<UCameraComponent> Camera;
    UPROPERTY() TObjectPtr<ABRBall> BallTarget;
    UPROPERTY(EditAnywhere, Category="Battle Ramps") EBRVehicleType VehicleType = EBRVehicleType::RallyGT;
    UPROPERTY(EditDefaultsOnly, Category="Art") TObjectPtr<UStaticMesh> ArtBodyMesh;
    UPROPERTY(EditDefaultsOnly, Category="Art") TArray<TObjectPtr<UStaticMesh>> ArtBodyMeshesByType;
    UPROPERTY(EditDefaultsOnly, Category="Art") TObjectPtr<UStaticMesh> ArtWheelMesh;
    UPROPERTY(EditDefaultsOnly, Category="Art") FVector ArtBodyScale = FVector(1.0f);
    UPROPERTY(EditDefaultsOnly, Category="Art") bool bArtModelIncludesWheels = true;
    UPROPERTY(EditDefaultsOnly, Category="Art") bool bTintArtWithTeamColor = false;
    UPROPERTY(EditAnywhere, Category="Battle Ramps") bool bAIControlled = false;
    UPROPERTY(EditAnywhere, Category="Battle Ramps") int32 TeamIndex = 0;
    UPROPERTY(EditAnywhere, Category="Battle Ramps") float MaxHealth = 150.0f;
    UPROPERTY(EditAnywhere, Category="Battle Ramps") float MaxSpeed = 2100.0f;
    UPROPERTY(EditAnywhere, Category="Battle Ramps") float Acceleration = 1600.0f;
    UPROPERTY(EditAnywhere, Category="Battle Ramps") float TurnRate = 95.0f;
    float Health = 150.0f;
    float ForwardSpeed = 0.0f, VerticalSpeed = 0.0f, BoostAmount = 100.0f;
    float PowerKickCooldown = 0.0f, ImpactCooldown = 0.0f, RespawnCooldown = 0.0f;
    float ThrottleInput = 0.0f, SteeringInput = 0.0f;
    int32 PowerKicks = 3, RampCharges = 2, CosmeticIndex = 0;
    bool bBoostHeld = false, bInAir = false, bDestroyed = false, bFirstPerson = false;
    FTransform SpawnTransform;
    UPROPERTY() TObjectPtr<UMaterialInstanceDynamic> BodyMaterial;
    void MoveForward(float Value);
    void MoveRight(float Value);
    void BoostPressed(); void BoostReleased();
    void RestartMatch(); void BallPulse(); void BuildRamp(); void Jump(); void ToggleCamera();
    void IncreaseTeamSize(); void DecreaseTeamSize(); void BuyCosmetic();
    void SelectVehicle1(); void SelectVehicle2(); void SelectVehicle3(); void SelectVehicle4(); void SelectVehicle5();
    void UpdatePaint(); void UpdateAI(float DeltaSeconds); void UpdateMovement(float DeltaSeconds);
};
