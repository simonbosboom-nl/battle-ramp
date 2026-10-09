#include "BRCarPawn.h"
#include "BRBall.h"
#include "BRRamp.h"
#include "BRGameMode.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/PlayerController.h"
#include "InputCoreTypes.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"

ABRCarPawn::ABRCarPawn()
{
    PrimaryActorTick.bCanEverTick = true;
    CollisionRoot = CreateDefaultSubobject<UBoxComponent>(TEXT("VehicleCollision"));
    SetRootComponent(CollisionRoot);
    CollisionRoot->SetBoxExtent(FVector(175.0f,86.0f,55.0f));
    CollisionRoot->SetCollisionProfileName(TEXT("Pawn"));
    CollisionRoot->SetCollisionResponseToAllChannels(ECR_Block);
    CollisionRoot->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Overlap);
    CollisionRoot->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);

    Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CarBody"));
    Body->SetupAttachment(CollisionRoot);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("/Engine/BasicShapes/Cube.Cube"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Cylinder(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
    if (Cube.Succeeded()) Body->SetStaticMesh(Cube.Object);
    Body->SetRelativeScale3D(FVector(1.95f,1.0f,0.36f));
    Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Body->SetSimulatePhysics(false);
    for (int32 I=0; I<4; ++I)
    {
        UStaticMeshComponent* Wheel=CreateDefaultSubobject<UStaticMeshComponent>(*FString::Printf(TEXT("Wheel_%d"),I));
        Wheel->SetupAttachment(CollisionRoot);
        if(Cylinder.Succeeded()) Wheel->SetStaticMesh(Cylinder.Object);
        const float X=(I<2)?118.0f:-118.0f; const float Y=(I%2==0)?-78.0f:78.0f;
        Wheel->SetRelativeLocation(FVector(X,Y,-20.0f));
        Wheel->SetRelativeRotation(FRotator(90,0,0));
        Wheel->SetRelativeScale3D(FVector(0.66f,0.28f,0.66f));
        Wheel->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        Wheels.Add(Wheel);
        UMaterialInterface* WheelBase=LoadObject<UMaterialInterface>(nullptr,TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
        if(WheelBase){UMaterialInstanceDynamic* WheelMat=UMaterialInstanceDynamic::Create(WheelBase,Wheel);if(WheelMat){WheelMat->SetVectorParameterValue(TEXT("Color"),FLinearColor(0.018f,0.022f,0.028f));WheelMat->SetVectorParameterValue(TEXT("BaseColor"),FLinearColor(0.018f,0.022f,0.028f));Wheel->SetMaterial(0,WheelMat);}}
    }
    auto MakeDetail = [this](const FName Name, const FVector Loc, const FVector Scale, const FLinearColor Color)
    {
        UStaticMeshComponent* Part=CreateDefaultSubobject<UStaticMeshComponent>(Name);
        Part->SetupAttachment(CollisionRoot);
        if(Cube.Succeeded())Part->SetStaticMesh(Cube.Object);
        Part->SetRelativeLocation(Loc);Part->SetRelativeScale3D(Scale);Part->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        UMaterialInterface* Base=LoadObject<UMaterialInterface>(nullptr,TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
        if(Base){UMaterialInstanceDynamic* M=UMaterialInstanceDynamic::Create(Base,Part);if(M){M->SetVectorParameterValue(TEXT("Color"),Color);M->SetVectorParameterValue(TEXT("BaseColor"),Color);Part->SetMaterial(0,M);}}
        DetailMeshes.Add(Part);
    };
    MakeDetail(TEXT("Cabin"),FVector(-10,0,25),FVector(0.72f,0.68f,0.24f),FLinearColor(0.025f,0.055f,0.075f));
    MakeDetail(TEXT("Windshield"),FVector(45,0,23),FVector(0.18f,0.62f,0.17f),FLinearColor(0.15f,0.27f,0.32f));
    MakeDetail(TEXT("FrontBumper"),FVector(191,0,-9),FVector(0.12f,0.94f,0.12f),FLinearColor(0.045f,0.05f,0.06f));
    MakeDetail(TEXT("RearBumper"),FVector(-191,0,-9),FVector(0.10f,0.92f,0.11f),FLinearColor(0.045f,0.05f,0.06f));
    MakeDetail(TEXT("HeadlampLeft"),FVector(162,-61,4),FVector(0.07f,0.12f,0.075f),FLinearColor(0.82f,0.94f,1.0f));
    MakeDetail(TEXT("HeadlampRight"),FVector(162,61,4),FVector(0.07f,0.12f,0.075f),FLinearColor(0.82f,0.94f,1.0f));
    MakeDetail(TEXT("TailLampLeft"),FVector(-162,-61,4),FVector(0.06f,0.12f,0.075f),FLinearColor(0.9f,0.025f,0.01f));
    MakeDetail(TEXT("TailLampRight"),FVector(-162,61,4),FVector(0.06f,0.12f,0.075f),FLinearColor(0.9f,0.025f,0.01f));
    SpringArm=CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    SpringArm->SetupAttachment(CollisionRoot); SpringArm->SetRelativeLocation(FVector(0,0,90));
    SpringArm->TargetArmLength=680.0f; SpringArm->SetRelativeRotation(FRotator(-10,0,0));
    SpringArm->bEnableCameraLag=true; SpringArm->CameraLagSpeed=6.5f; SpringArm->bUsePawnControlRotation=false;
    Camera=CreateDefaultSubobject<UCameraComponent>(TEXT("DriverCamera")); Camera->SetupAttachment(SpringArm,USpringArmComponent::SocketName);
    AutoPossessPlayer=EAutoReceiveInput::Disabled;
}
void ABRCarPawn::BeginPlay()
{
    Super::BeginPlay();
    if(ArtWheelMesh)for(TObjectPtr<UStaticMeshComponent>& Wheel:Wheels)if(Wheel)Wheel->SetStaticMesh(ArtWheelMesh);
    SetVehicleType(VehicleType);
    SpawnTransform=GetActorTransform();
    Health=MaxHealth;
}
void ABRCarPawn::SetVehicleType(EBRVehicleType NewType)
{
    VehicleType=NewType;
    switch(VehicleType)
    {
        case EBRVehicleType::RallyGT: MaxHealth=150;MaxSpeed=2100;Acceleration=1600;TurnRate=98;break;
        case EBRVehicleType::NeonSpeedster: MaxHealth=115;MaxSpeed=2650;Acceleration=2050;TurnRate=88;break;
        case EBRVehicleType::BattleTank: MaxHealth=260;MaxSpeed=1500;Acceleration=1150;TurnRate=72;break;
        case EBRVehicleType::DuneBuggy: MaxHealth=135;MaxSpeed=2200;Acceleration=1750;TurnRate=112;break;
        case EBRVehicleType::HyperX: MaxHealth=130;MaxSpeed=2500;Acceleration=1950;TurnRate=92;break;
        default: break;
    }
    Health=MaxHealth;
    if(Body)
    {
        UStaticMesh* SelectedArt=ArtBodyMesh.Get();
        if(ArtBodyMeshesByType.IsValidIndex(static_cast<int32>(VehicleType)) && ArtBodyMeshesByType[static_cast<int32>(VehicleType)])SelectedArt=ArtBodyMeshesByType[static_cast<int32>(VehicleType)].Get();
        if(SelectedArt) Body->SetStaticMesh(SelectedArt);
        Body->SetRelativeScale3D(SelectedArt?ArtBodyScale:(VehicleType==EBRVehicleType::BattleTank?FVector(2.1f,1.27f,0.55f):VehicleType==EBRVehicleType::DuneBuggy?FVector(1.65f,1.05f,0.34f):FVector(1.95f,1.0f,0.36f)));
        CollisionRoot->SetBoxExtent(VehicleType==EBRVehicleType::BattleTank?FVector(205,112,68):FVector(175,86,55));
        for(TObjectPtr<UStaticMeshComponent>& Part:DetailMeshes)if(Part)Part->SetVisibility(SelectedArt==nullptr,true);
        for(TObjectPtr<UStaticMeshComponent>& Wheel:Wheels)if(Wheel)Wheel->SetVisibility(!(SelectedArt && bArtModelIncludesWheels),true);
        UpdatePaint();
    }
}
void ABRCarPawn::SetupPlayerInputComponent(UInputComponent* Input)
{
    Super::SetupPlayerInputComponent(Input); if(!Input || bAIControlled)return;
    Input->BindAxis(TEXT("MoveForward"),this,&ABRCarPawn::MoveForward);
    Input->BindAxis(TEXT("Turn"),this,&ABRCarPawn::MoveRight);
    Input->BindAction(TEXT("Boost"),IE_Pressed,this,&ABRCarPawn::BoostPressed);
    Input->BindAction(TEXT("Boost"),IE_Released,this,&ABRCarPawn::BoostReleased);
    Input->BindAction(TEXT("BallPulse"),IE_Pressed,this,&ABRCarPawn::BallPulse);
    Input->BindAction(TEXT("BuildRamp"),IE_Pressed,this,&ABRCarPawn::BuildRamp);
    Input->BindAction(TEXT("Jump"),IE_Pressed,this,&ABRCarPawn::Jump);
    Input->BindAction(TEXT("CameraToggle"),IE_Pressed,this,&ABRCarPawn::ToggleCamera);
    Input->BindAction(TEXT("TeamsUp"),IE_Pressed,this,&ABRCarPawn::IncreaseTeamSize);
    Input->BindAction(TEXT("TeamsDown"),IE_Pressed,this,&ABRCarPawn::DecreaseTeamSize);
    Input->BindAction(TEXT("Shop"),IE_Pressed,this,&ABRCarPawn::BuyCosmetic);
    Input->BindAction(TEXT("RestartMatch"),IE_Pressed,this,&ABRCarPawn::RestartMatch);
    Input->BindKey(EKeys::One,EInputEvent::IE_Pressed,this,&ABRCarPawn::SelectVehicle1);
    Input->BindKey(EKeys::Two,EInputEvent::IE_Pressed,this,&ABRCarPawn::SelectVehicle2);
    Input->BindKey(EKeys::Three,EInputEvent::IE_Pressed,this,&ABRCarPawn::SelectVehicle3);
    Input->BindKey(EKeys::Four,EInputEvent::IE_Pressed,this,&ABRCarPawn::SelectVehicle4);
    Input->BindKey(EKeys::Five,EInputEvent::IE_Pressed,this,&ABRCarPawn::SelectVehicle5);
}
void ABRCarPawn::MoveForward(float Value){if(!bDestroyed)ThrottleInput=FMath::Clamp(Value,-1.0f,1.0f);}
void ABRCarPawn::MoveRight(float Value){if(!bDestroyed)SteeringInput=FMath::Clamp(Value,-1.0f,1.0f);}
void ABRCarPawn::BoostPressed(){bBoostHeld=true;}
void ABRCarPawn::BoostReleased(){bBoostHeld=false;}
void ABRCarPawn::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if(bDestroyed){RespawnCooldown-=DeltaSeconds;if(RespawnCooldown<=0)RespawnAt(SpawnTransform);return;}
    PowerKickCooldown=FMath::Max(0.0f,PowerKickCooldown-DeltaSeconds);ImpactCooldown=FMath::Max(0.0f,ImpactCooldown-DeltaSeconds);
    if(bAIControlled)UpdateAI(DeltaSeconds);
    UpdateMovement(DeltaSeconds);
    for(UStaticMeshComponent* Wheel:Wheels)if(Wheel)Wheel->AddLocalRotation(FRotator(ForwardSpeed*DeltaSeconds*0.045f,0,0));
}
void ABRCarPawn::UpdateAI(float DeltaSeconds)
{
    if(!BallTarget)return;
    FVector Target=BallTarget->GetActorLocation();
    const int32 Slot=static_cast<int32>(GetUniqueID()%3)-1;
    Target.Y+=((TeamIndex==0)?1.0f:-1.0f)*Slot*270.0f;
    FVector ToTarget=Target-GetActorLocation();ToTarget.Z=0;const float Distance=ToTarget.Size();if(Distance<1)return;
    const float DesiredYaw=FMath::RadiansToDegrees(FMath::Atan2(ToTarget.Y,ToTarget.X));
    const float DeltaYaw=FMath::FindDeltaAngleDegrees(GetActorRotation().Yaw,DesiredYaw);
    SteeringInput=FMath::Clamp(DeltaYaw/42.0f,-1.0f,1.0f);
    if(FMath::Abs(DeltaYaw)>108.0f&&ForwardSpeed>260.0f)ThrottleInput=-0.32f;
    else if(Distance<330.0f)ThrottleInput=0.55f;else ThrottleInput=1.0f;
    bBoostHeld=Distance>1800.0f&&FMath::Abs(DeltaYaw)<18.0f&&BoostAmount>12.0f;
}
void ABRCarPawn::UpdateMovement(float DeltaSeconds)
{
    const float SpeedFraction=FMath::Clamp(FMath::Abs(ForwardSpeed)/MaxSpeed,0.18f,1.0f);
    const float ReverseFactor=ForwardSpeed< -35.0f?-1.0f:1.0f;
    AddActorWorldRotation(FRotator(0,SteeringInput*TurnRate*SpeedFraction*ReverseFactor*DeltaSeconds,0));
    if(FMath::Abs(ThrottleInput)>0.04f)ForwardSpeed+=ThrottleInput*Acceleration*DeltaSeconds;
    else ForwardSpeed=FMath::FInterpTo(ForwardSpeed,0.0f,DeltaSeconds,0.62f);
    if(bBoostHeld&&BoostAmount>0.0f&&ThrottleInput>0.0f){ForwardSpeed+=Acceleration*1.15f*DeltaSeconds;BoostAmount=FMath::Max(0.0f,BoostAmount-30.0f*DeltaSeconds);}
    else BoostAmount=FMath::Min(100.0f,BoostAmount+6.0f*DeltaSeconds);
    ForwardSpeed=FMath::Clamp(ForwardSpeed,-MaxSpeed*0.36f,MaxSpeed*(bBoostHeld?1.35f:1.0f));
    if(bInAir){VerticalSpeed-=1900.0f*DeltaSeconds;FVector Pos=GetActorLocation();Pos.Z+=VerticalSpeed*DeltaSeconds;if(Pos.Z<=SpawnTransform.GetLocation().Z){Pos.Z=SpawnTransform.GetLocation().Z;bInAir=false;VerticalSpeed=0;}SetActorLocation(Pos,false);}
    FHitResult Hit;AddActorWorldOffset(GetActorForwardVector()*ForwardSpeed*DeltaSeconds,true,&Hit);
    if(Hit.IsValidBlockingHit())
    {
        if(ABRCarPawn* Other=Cast<ABRCarPawn>(Hit.GetActor()))
        {
            if(ImpactCooldown<=0.0f&&FMath::Abs(ForwardSpeed)>1000.0f){Other->ApplyHit(FMath::Abs(ForwardSpeed)*0.008f,GetActorForwardVector()*ForwardSpeed);ImpactCooldown=0.7f;}
        }
        ForwardSpeed*=0.62f;
    }
    FVector P=GetActorLocation();P.X=FMath::Clamp(P.X,-5700.0f,5700.0f);P.Y=FMath::Clamp(P.Y,-3650.0f,3650.0f);if(P.Z<SpawnTransform.GetLocation().Z)P.Z=SpawnTransform.GetLocation().Z;SetActorLocation(P,false);
}
void ABRCarPawn::BallPulse()
{
    if(bAIControlled||bDestroyed||PowerKicks<=0||PowerKickCooldown>0.0f||!BallTarget)return;
    const FVector Delta=BallTarget->GetActorLocation()-GetActorLocation();if(Delta.Size()>1250.0f){if(GEngine)GEngine->AddOnScreenDebugMessage(-1,1.5f,FColor::Yellow,TEXT("Get closer to the ball for a power kick"));return;}
    if(BallTarget->Mesh)BallTarget->Mesh->AddImpulse((GetActorForwardVector()+FVector(0,0,0.12f)).GetSafeNormal()*36000.0f,NAME_None,true);
    --PowerKicks;PowerKickCooldown=1.0f;
}
void ABRCarPawn::BuildRamp()
{
    if(bAIControlled||bDestroyed)return;if(RampCharges<=0){if(GEngine)GEngine->AddOnScreenDebugMessage(-1,1.7f,FColor::Yellow,TEXT("No ramps left - find a supply drop!"));return;}
    const FVector Location=GetActorLocation()+GetActorForwardVector()*430.0f+FVector(0,0,-42);FRotator Rotation=GetActorRotation();Rotation.Pitch=-7.0f;
    if(GetWorld()->SpawnActor<ABRRamp>(Location,Rotation))--RampCharges;
}
void ABRCarPawn::Jump(){if(!bDestroyed&&!bInAir){bInAir=true;VerticalSpeed=750.0f;}}
void ABRCarPawn::ToggleCamera(){bFirstPerson=!bFirstPerson;if(bFirstPerson){SpringArm->TargetArmLength=0;SpringArm->SetRelativeLocation(FVector(80,0,105));SpringArm->SetRelativeRotation(FRotator(0,0,0));}else{SpringArm->TargetArmLength=680;SpringArm->SetRelativeLocation(FVector(0,0,90));SpringArm->SetRelativeRotation(FRotator(-10,0,0));}}
void ABRCarPawn::IncreaseTeamSize(){if(ABRGameMode* GM=GetWorld()->GetAuthGameMode<ABRGameMode>())GM->AdjustTeamSize(1);}
void ABRCarPawn::DecreaseTeamSize(){if(ABRGameMode* GM=GetWorld()->GetAuthGameMode<ABRGameMode>())GM->AdjustTeamSize(-1);}
void ABRCarPawn::BuyCosmetic(){if(ABRGameMode* GM=GetWorld()->GetAuthGameMode<ABRGameMode>())GM->PurchaseCosmetic();}
void ABRCarPawn::RestartMatch(){if(ABRGameMode* GM=GetWorld()->GetAuthGameMode<ABRGameMode>())GM->RestartMatch();}
void ABRCarPawn::SelectVehicle1(){SetVehicleType(EBRVehicleType::RallyGT);}
void ABRCarPawn::SelectVehicle2(){SetVehicleType(EBRVehicleType::NeonSpeedster);}
void ABRCarPawn::SelectVehicle3(){SetVehicleType(EBRVehicleType::BattleTank);}
void ABRCarPawn::SelectVehicle4(){SetVehicleType(EBRVehicleType::DuneBuggy);}
void ABRCarPawn::SelectVehicle5(){SetVehicleType(EBRVehicleType::HyperX);}
void ABRCarPawn::UpdatePaint()
{
    if(!Body)return;
    const int32 VehicleIndex=static_cast<int32>(VehicleType);
    const bool bHasArtModel=ArtBodyMesh || (ArtBodyMeshesByType.IsValidIndex(VehicleIndex) && ArtBodyMeshesByType[VehicleIndex].Get()!=nullptr);
    if(bHasArtModel && !bTintArtWithTeamColor)
    {
        UStaticMesh* ActiveArt=(ArtBodyMeshesByType.IsValidIndex(VehicleIndex) && ArtBodyMeshesByType[VehicleIndex])?ArtBodyMeshesByType[VehicleIndex].Get():ArtBodyMesh.Get();
        BodyMaterial=nullptr;
        if(ActiveArt)for(int32 Slot=0;Slot<ActiveArt->GetStaticMaterials().Num();++Slot)Body->SetMaterial(Slot,ActiveArt->GetMaterial(Slot));
        return;
    }
    if(!BodyMaterial){static UMaterialInterface* Base=LoadObject<UMaterialInterface>(nullptr,TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));if(Base)BodyMaterial=UMaterialInstanceDynamic::Create(Base,this);if(BodyMaterial)Body->SetMaterial(0,BodyMaterial);}
    FLinearColor Color=TeamIndex==0?FLinearColor(0.025f,0.32f,0.95f):FLinearColor(0.95f,0.16f,0.035f);
    if(VehicleType==EBRVehicleType::BattleTank)Color=TeamIndex==0?FLinearColor(0.07f,0.22f,0.25f):FLinearColor(0.32f,0.12f,0.08f);
    if(CosmeticIndex>0){const FLinearColor Colors[]={FLinearColor(0.02f,0.9f,0.9f),FLinearColor(0.7f,0.05f,0.95f),FLinearColor(0.98f,0.6f,0.02f),FLinearColor(0.95f,0.95f,0.85f),FLinearColor(0.12f,0.95f,0.24f)};Color=Colors[(CosmeticIndex-1)%5];}
    if(BodyMaterial){BodyMaterial->SetVectorParameterValue(TEXT("Color"),Color);BodyMaterial->SetVectorParameterValue(TEXT("BaseColor"),Color);}
}
void ABRCarPawn::ApplyCosmetic(int32 StyleIndex){CosmeticIndex=FMath::Max(1,StyleIndex);UpdatePaint();}
void ABRCarPawn::ApplyHit(float Damage,const FVector& Impulse)
{
    if(bDestroyed)return;Health=FMath::Max(0.0f,Health-Damage);ForwardSpeed-=FMath::Min(350.0f,Damage*1.7f);if(!Impulse.IsNearlyZero())SteeringInput=FMath::Clamp(SteeringInput+FMath::Sign(Impulse.Y)*0.08f,-1.0f,1.0f);
    if(Health<=0.0f){bDestroyed=true;RespawnCooldown=4.0f;ForwardSpeed=0;CollisionRoot->SetCollisionEnabled(ECollisionEnabled::NoCollision);Body->SetVisibility(false,true);for(TObjectPtr<UStaticMeshComponent>& W:Wheels)if(W)W->SetVisibility(false,true);for(TObjectPtr<UStaticMeshComponent>& Part:DetailMeshes)if(Part)Part->SetVisibility(false,true);}
}
void ABRCarPawn::AddBoost(float Amount){BoostAmount=FMath::Clamp(BoostAmount+Amount,0.0f,100.0f);}
void ABRCarPawn::AddPowerKicks(int32 Amount){PowerKicks=FMath::Clamp(PowerKicks+Amount,0,8);}
void ABRCarPawn::AddRamps(int32 Amount){RampCharges=FMath::Clamp(RampCharges+Amount,0,6);}
void ABRCarPawn::Repair(float Amount){Health=FMath::Min(MaxHealth,Health+Amount);}
void ABRCarPawn::RespawnAt(const FTransform& Transform)
{
    SpawnTransform=Transform;SetActorTransform(Transform,false,nullptr,ETeleportType::TeleportPhysics);bDestroyed=false;Health=MaxHealth;ForwardSpeed=VerticalSpeed=0;ThrottleInput=SteeringInput=0;CollisionRoot->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);Body->SetVisibility(true,true);const int32 VehicleIndex=static_cast<int32>(VehicleType);const bool bHasArtModel=ArtBodyMesh || (ArtBodyMeshesByType.IsValidIndex(VehicleIndex) && ArtBodyMeshesByType[VehicleIndex].Get()!=nullptr);for(TObjectPtr<UStaticMeshComponent>& W:Wheels)if(W)W->SetVisibility(!(bHasArtModel && bArtModelIncludesWheels),true);for(TObjectPtr<UStaticMeshComponent>& Part:DetailMeshes)if(Part)Part->SetVisibility(!bHasArtModel,true);
}
