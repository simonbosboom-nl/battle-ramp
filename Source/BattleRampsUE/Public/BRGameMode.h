#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BRGameMode.generated.h"
class ABRCarPawn; class ABRBall; class ABRArenaBuilder;
UCLASS()
class BATTLERAMPSUE_API ABRGameMode : public AGameModeBase
{
    GENERATED_BODY()
public:
    ABRGameMode();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;
    UPROPERTY(BlueprintReadOnly) int32 BlueScore = 0;
    UPROPERTY(BlueprintReadOnly) int32 OrangeScore = 0;
    UPROPERTY(BlueprintReadOnly) int32 TeamSize = 3;
    UPROPERTY(BlueprintReadOnly) int32 Wins = 0;
    UPROPERTY(BlueprintReadOnly) int32 Coins = 0;
    UPROPERTY(BlueprintReadOnly) int32 CosmeticStyle = 0;
    UPROPERTY(BlueprintReadOnly) float TimeRemaining = 300.0f;
    UPROPERTY(EditDefaultsOnly, Category="Battle Ramps|Art") TSubclassOf<ABRCarPawn> CarPawnClass;
    UPROPERTY(EditDefaultsOnly, Category="Battle Ramps|Art") TSubclassOf<ABRArenaBuilder> ArenaBuilderClass;
    UPROPERTY(BlueprintReadOnly) bool bMatchEnded = false;
    UPROPERTY(BlueprintReadOnly) bool bLastMinuteWarning = false;
    void AdjustTeamSize(int32 Delta);
    void PurchaseCosmetic();
    void RestartMatch();
    void RegisterGoal(int32 ScoringTeam);
private:
    UPROPERTY() TObjectPtr<ABRBall> Ball;
    UPROPERTY() TObjectPtr<ABRCarPawn> PlayerCar;
    UPROPERTY() TArray<TObjectPtr<ABRCarPawn>> BlueBots;
    UPROPERTY() TArray<TObjectPtr<ABRCarPawn>> OrangeBots;
    FTimerHandle DropTimer;
    bool bWorldBuilt = false;
    void SetupWorld(); void RebuildTeams(); void SpawnSupplyDrop();
    void EndMatch(); void SaveProgress(); void LoadProgress();
};
