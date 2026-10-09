#include "BRGameMode.h"
#include "BRCarPawn.h"
#include "BRBall.h"
#include "BRArenaBuilder.h"
#include "BRSupplyDrop.h"
#include "BRHUD.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Engine/GameEngine.h"
#include "Misc/ConfigCacheIni.h"
#include "TimerManager.h"

ABRGameMode::ABRGameMode()
{
    PrimaryActorTick.bCanEverTick = true;
    CarPawnClass = ABRCarPawn::StaticClass();
    ArenaBuilderClass = ABRArenaBuilder::StaticClass();
    DefaultPawnClass = ABRCarPawn::StaticClass();
    HUDClass = ABRHUD::StaticClass();
    TeamSize = 3;
}
void ABRGameMode::BeginPlay()
{
    Super::BeginPlay();
    LoadProgress();
    GetWorldTimerManager().SetTimerForNextTick(this, &ABRGameMode::SetupWorld);
    GetWorldTimerManager().SetTimer(DropTimer, this, &ABRGameMode::SpawnSupplyDrop, 24.0f, true);
}
void ABRGameMode::SetupWorld()
{
    if(bWorldBuilt || !GetWorld()) return;
    bWorldBuilt = true;
    if(ArenaBuilderClass) GetWorld()->SpawnActor<ABRArenaBuilder>(ArenaBuilderClass.Get(),FVector::ZeroVector,FRotator::ZeroRotator);
    Ball = GetWorld()->SpawnActor<ABRBall>(FVector(0,0,130),FRotator::ZeroRotator);
    APlayerController* PC = UGameplayStatics::GetPlayerController(this,0);
    if(PC)
    {
        APawn* ExistingPawn=PC->GetPawn();
        PlayerCar=Cast<ABRCarPawn>(ExistingPawn);
        const bool bWrongCarClass=PlayerCar && CarPawnClass && !PlayerCar->IsA(CarPawnClass.Get());
        if(!PlayerCar || bWrongCarClass)
        {
            if(ExistingPawn) ExistingPawn->Destroy();
            PlayerCar=CarPawnClass?GetWorld()->SpawnActor<ABRCarPawn>(CarPawnClass.Get(),FVector(-3650,0,45),FRotator::ZeroRotator):nullptr;
            if(PlayerCar) PC->Possess(PlayerCar);
        }
        if(PlayerCar)
        {
            PlayerCar->SetTeamIndex(0); PlayerCar->SetAIControlled(false); PlayerCar->SetBallTarget(Ball);
            PlayerCar->RespawnAt(FTransform(FRotator::ZeroRotator,FVector(-3650,0,45)));
            if(CosmeticStyle>0) PlayerCar->ApplyCosmetic(CosmeticStyle);
        }
    }
    RebuildTeams();
}
void ABRGameMode::RebuildTeams()
{
    for(TObjectPtr<ABRCarPawn>& B:BlueBots) if(IsValid(B.Get())) B->Destroy();
    for(TObjectPtr<ABRCarPawn>& B:OrangeBots) if(IsValid(B.Get())) B->Destroy();
    BlueBots.Reset(); OrangeBots.Reset();
    if(!GetWorld()||!Ball) return;
    for(int32 I=1;I<TeamSize;++I)
    {
        const float Y=(I%2==0?1.0f:-1.0f)*600.0f*((I+1)/2);
        ABRCarPawn* Bot=CarPawnClass?GetWorld()->SpawnActor<ABRCarPawn>(CarPawnClass.Get(),FVector(-2700.0f-I*230.0f,Y,45),FRotator(0,0,0)):nullptr;
        if(Bot){Bot->SetTeamIndex(0);Bot->SetAIControlled(true);Bot->SetBallTarget(Ball);Bot->SetVehicleType((EBRVehicleType)(I%5));BlueBots.Add(Bot);}
    }
    for(int32 I=0;I<TeamSize;++I)
    {
        const float Y=(I%2==0?1.0f:-1.0f)*650.0f*((I/2)+1);
        ABRCarPawn* Bot=CarPawnClass?GetWorld()->SpawnActor<ABRCarPawn>(CarPawnClass.Get(),FVector(2800.0f+I*180.0f,Y,45),FRotator(0,180,0)):nullptr;
        if(Bot){Bot->SetTeamIndex(1);Bot->SetAIControlled(true);Bot->SetBallTarget(Ball);Bot->SetVehicleType((EBRVehicleType)((I+2)%5));OrangeBots.Add(Bot);}
    }
}
void ABRGameMode::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if(!bWorldBuilt||bMatchEnded) return;
    TimeRemaining=FMath::Max(0.0f,TimeRemaining-DeltaSeconds);
    bLastMinuteWarning=TimeRemaining<=60.0f;
    if(Ball)
    {
        const FVector P=Ball->GetActorLocation();
        if(FMath::Abs(P.Y)<800.0f && P.Z<520.0f && P.X>5700.0f) RegisterGoal(0);
        else if(FMath::Abs(P.Y)<800.0f && P.Z<520.0f && P.X< -5700.0f) RegisterGoal(1);
    }
    if(TimeRemaining<=0.0f) EndMatch();
}
void ABRGameMode::RegisterGoal(int32 ScoringTeam)
{
    if(bMatchEnded) return;
    if(ScoringTeam==0) ++BlueScore; else ++OrangeScore;
    if(Ball) Ball->ResetBall(FVector(0,0,130));
    if(PlayerCar) PlayerCar->RespawnAt(FTransform(FRotator::ZeroRotator,FVector(-3650,0,45)));
    for(int32 I=0;I<BlueBots.Num();++I) if(IsValid(BlueBots[I])) BlueBots[I]->SetActorLocation(FVector(-2600-I*230,(I%2?1:-1)*650,45));
    for(int32 I=0;I<OrangeBots.Num();++I) if(IsValid(OrangeBots[I])) OrangeBots[I]->SetActorLocation(FVector(2700+I*180,(I%2?1:-1)*650,45));
}
void ABRGameMode::AdjustTeamSize(int32 Delta)
{
    TeamSize=FMath::Clamp(TeamSize+Delta,1,5);
    RebuildTeams();
}
void ABRGameMode::SpawnSupplyDrop()
{
    if(bMatchEnded||!GetWorld()) return;
    if(GEngine) GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Yellow,TEXT("DROPPING!!! SUPPLY CRATE INCOMING"));
    const FVector DropLocation(FMath::FRandRange(-4300,4300),FMath::FRandRange(-2600,2600),1500.0f);
    GetWorld()->SpawnActor<ABRSupplyDrop>(DropLocation,FRotator::ZeroRotator);
}
void ABRGameMode::EndMatch()
{
    if(bMatchEnded) return;
    bMatchEnded=true;
    if(BlueScore>OrangeScore){++Wins;Coins+=3;SaveProgress();}
    if(GEngine)
    {
        const FString Result=BlueScore>OrangeScore?TEXT("BLUE WIN! +3 SHOP COINS"):BlueScore<OrangeScore?TEXT("ORANGE WIN!"):TEXT("DRAW!");
        GEngine->AddOnScreenDebugMessage(-1,7.0f,FColor::Cyan,Result);
    }
}
void ABRGameMode::RestartMatch()
{
    BlueScore=0; OrangeScore=0; TimeRemaining=300.0f; bMatchEnded=false; bLastMinuteWarning=false;
    if(Ball) Ball->ResetBall(FVector(0,0,130));
    if(PlayerCar) PlayerCar->RespawnAt(FTransform(FRotator::ZeroRotator,FVector(-3650,0,45)));
}
void ABRGameMode::PurchaseCosmetic()
{
    if(Coins<3){if(GEngine)GEngine->AddOnScreenDebugMessage(-1,2.5f,FColor::Yellow,TEXT("SHOP: 3 coins needed for a cosmetic"));return;}
    Coins-=3; CosmeticStyle=FMath::RandRange(1,5);
    if(PlayerCar) PlayerCar->ApplyCosmetic(CosmeticStyle);
    SaveProgress();
    if(GEngine) GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Green,TEXT("COSMETIC UNLOCKED! -3 coins"));
}
void ABRGameMode::LoadProgress()
{
    if(GConfig)
    {
        GConfig->GetInt(TEXT("BattleRamps"),TEXT("Wins"),Wins,GGameIni);
        GConfig->GetInt(TEXT("BattleRamps"),TEXT("Coins"),Coins,GGameIni);
        GConfig->GetInt(TEXT("BattleRamps"),TEXT("CosmeticStyle"),CosmeticStyle,GGameIni);
    }
}
void ABRGameMode::SaveProgress()
{
    if(GConfig)
    {
        GConfig->SetInt(TEXT("BattleRamps"),TEXT("Wins"),Wins,GGameIni);
        GConfig->SetInt(TEXT("BattleRamps"),TEXT("Coins"),Coins,GGameIni);
        GConfig->SetInt(TEXT("BattleRamps"),TEXT("CosmeticStyle"),CosmeticStyle,GGameIni);
        GConfig->Flush(false,GGameIni);
    }
}
