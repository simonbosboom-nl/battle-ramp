#include "BRArenaBuilder.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/DirectionalLight.h"
#include "Components/DirectionalLightComponent.h"
#include "Engine/SkyLight.h"
#include "Components/SkyLightComponent.h"
#include "Engine/SkyAtmosphere.h"
#include "Engine/ExponentialHeightFog.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Engine/SpotLight.h"
#include "Components/SpotLightComponent.h"

ABRArenaBuilder::ABRArenaBuilder()
{
    PrimaryActorTick.bCanEverTick=false;
    SetActorTickEnabled(false);
}
void ABRArenaBuilder::BeginPlay()
{
    Super::BeginPlay();
    UWorld* World=GetWorld();
    ASkyLight* ArenaSkyLight=nullptr;
    if(World)
    {
        ADirectionalLight* Sun=World->SpawnActor<ADirectionalLight>(FVector(0,0,4000),FRotator(-48,-28,0));
        if(Sun)
        {
            Sun->GetLightComponent()->SetMobility(EComponentMobility::Movable);
            Sun->GetLightComponent()->SetIntensity(85000.0f);
            Sun->GetLightComponent()->SetLightColor(FLinearColor(1.0f,0.94f,0.82f));
            Sun->GetLightComponent()->bAtmosphereSunLight=true;
        }
        ArenaSkyLight=World->SpawnActor<ASkyLight>(FVector(0,0,2000),FRotator::ZeroRotator);
        if(ArenaSkyLight)
        {
            ArenaSkyLight->GetLightComponent()->SetMobility(EComponentMobility::Movable);
            ArenaSkyLight->GetLightComponent()->SetIntensity(0.75f);
        }
        World->SpawnActor<ASkyAtmosphere>(FVector::ZeroVector,FRotator::ZeroRotator);
        AExponentialHeightFog* Fog=World->SpawnActor<AExponentialHeightFog>(FVector::ZeroVector,FRotator::ZeroRotator);
        if(Fog)
        {
            Fog->GetComponent()->SetFogDensity(0.002f);
            Fog->GetComponent()->SetFogInscatteringColor(FLinearColor(0.48f,0.58f,0.68f));
        }
        for(int32 XSide : {-1,1}) for(int32 YSide : {-1,1})
        {
            ASpotLight* Lamp=World->SpawnActor<ASpotLight>(FVector(XSide*5000.0f,YSide*3500.0f,2450.0f),FRotator(-32.0f,XSide<0?0.0f:180.0f,0));
            if(Lamp)
            {
                USpotLightComponent* L=Lamp->GetLightComponent();
                L->SetMobility(EComponentMobility::Movable);
                L->SetIntensity(1600000.0f);
                L->SetAttenuationRadius(12000.0f);
                L->SetInnerConeAngle(18.0f);
                L->SetOuterConeAngle(38.0f);
                L->SetLightColor(FLinearColor(0.78f,0.88f,1.0f));
            }
        }
    }
    // Large stadium field in Unreal centimeters. Replace primitive geometry with licensed art for final quality.
    AddBox(FVector(0,0,-70),FVector(120,80,1.2f),FRotator::ZeroRotator,FLinearColor(0.045f,0.22f,0.075f),true,GrassMaterial);
    for(int32 I=0;I<12;++I)
        AddBox(FVector(-5500.0f+I*1000.0f,0,-5),FVector(5,39,0.08f),FRotator::ZeroRotator,
            (I%2)?FLinearColor(0.05f,0.25f,0.08f):FLinearColor(0.035f,0.19f,0.055f),false);
    AddBox(FVector(0,0,2),FVector(0.08f,39,0.06f),FRotator::ZeroRotator,FLinearColor(0.82f,0.91f,0.83f),false);
    for(int32 I=0;I<64;++I)
    {
        const float A=2*PI*I/64.0f, B=2*PI*(I+1)/64.0f;
        const FVector Mid((FMath::Cos(A)+FMath::Cos(B))*700.0f,(FMath::Sin(A)+FMath::Sin(B))*700.0f,2);
        const float Len=FVector2D(FMath::Cos(B)-FMath::Cos(A),FMath::Sin(B)-FMath::Sin(A)).Size()*700.0f;
        AddBox(Mid,FVector(Len/100.0f,0.055f,0.05f),FRotator(0,FMath::RadiansToDegrees(FMath::Atan2(FMath::Sin(B)-FMath::Sin(A),FMath::Cos(B)-FMath::Cos(A))),0),FLinearColor(0.83f,0.9f,0.83f),false);
    }
    AddBox(FVector(0,4000,95),FVector(120,0.8f,1.9f),FRotator::ZeroRotator,FLinearColor(0.08f,0.13f,0.17f));
    AddBox(FVector(0,-4000,95),FVector(120,0.8f,1.9f),FRotator::ZeroRotator,FLinearColor(0.08f,0.13f,0.17f));
    AddBox(FVector(6000,0,95),FVector(0.8f,80,1.9f),FRotator::ZeroRotator,FLinearColor(0.08f,0.13f,0.17f));
    AddBox(FVector(-6000,0,95),FVector(0.8f,80,1.9f),FRotator::ZeroRotator,FLinearColor(0.08f,0.13f,0.17f));
    for(int32 Side : {-1,1})
    {
        const float GX=Side*5550.0f;
        const FLinearColor GoalColor=Side<0?FLinearColor(1.0f,0.23f,0.07f):FLinearColor(0.03f,0.63f,1.0f);
        AddBox(FVector(GX,0,220),FVector(1.6f,0.22f,4.4f),FRotator::ZeroRotator,GoalColor);
        AddBox(FVector(GX,-950,220),FVector(1.6f,0.22f,4.4f),FRotator::ZeroRotator,GoalColor);
        AddBox(FVector(GX,950,220),FVector(1.6f,0.22f,4.4f),FRotator::ZeroRotator,GoalColor);
        AddBox(FVector(GX,0,430),FVector(1.6f,19.0f,0.22f),FRotator::ZeroRotator,GoalColor);
        AddBox(FVector(Side*5900.0f,0,780),FVector(0.4f,80,0.35f),FRotator::ZeroRotator,FLinearColor(0.025f,0.04f,0.055f),false);
    }
    AddBox(FVector(0,4350,360),FVector(118,0.8f,5.2f),FRotator::ZeroRotator,FLinearColor(0.04f,0.065f,0.09f),false,StadiumMaterial);
    AddBox(FVector(0,-4350,360),FVector(118,0.8f,5.2f),FRotator::ZeroRotator,FLinearColor(0.04f,0.065f,0.09f),false,StadiumMaterial);
    AddBox(FVector(6350,0,360),FVector(0.8f,83,5.2f),FRotator::ZeroRotator,FLinearColor(0.04f,0.065f,0.09f),false,StadiumMaterial);
    AddBox(FVector(-6350,0,360),FVector(0.8f,83,5.2f),FRotator::ZeroRotator,FLinearColor(0.04f,0.065f,0.09f),false,StadiumMaterial);
    for(int32 Row=0;Row<5;++Row)
    {
        const float Z=450.0f+Row*165.0f;
        AddBox(FVector(0,4500+Row*210,Z),FVector(116,1.3f,1.1f),FRotator::ZeroRotator,FLinearColor(0.055f+Row*0.006f,0.075f,0.105f),false);
        AddBox(FVector(0,-4500-Row*210,Z),FVector(116,1.3f,1.1f),FRotator::ZeroRotator,FLinearColor(0.055f+Row*0.006f,0.075f,0.105f),false);
        AddBox(FVector(6500+Row*190,0,Z),FVector(1.2f,80,1.1f),FRotator::ZeroRotator,FLinearColor(0.055f,0.075f,0.105f),false);
        AddBox(FVector(-6500-Row*190,0,Z),FVector(1.2f,80,1.1f),FRotator::ZeroRotator,FLinearColor(0.055f,0.075f,0.105f),false);
    }
    for(int32 XSide : {-1,1}) for(int32 YSide : {-1,1})
    {
        const float X=XSide*5200.0f,Y=YSide*3650.0f;
        AddBox(FVector(X,Y,900),FVector(0.55f,0.55f,18),FRotator::ZeroRotator,FLinearColor(0.16f,0.19f,0.22f),false);
        AddBox(FVector(X,Y,1800),FVector(0.8f,0.8f,0.8f),FRotator::ZeroRotator,FLinearColor(0.95f,0.96f,0.89f),false);
    }
    if(ArenaSkyLight) ArenaSkyLight->GetLightComponent()->RecaptureSky();
}
void ABRArenaBuilder::AddBox(const FVector& Location,const FVector& Scale,const FRotator& Rotation,const FLinearColor& Color,bool bCollision,UMaterialInterface* OverrideMaterial)
{
    UWorld* World=GetWorld(); if(!World)return;
    AStaticMeshActor* A=World->SpawnActor<AStaticMeshActor>(Location,Rotation); if(!A)return;
    A->SetActorScale3D(Scale);
    UStaticMeshComponent* M=A->GetStaticMeshComponent();
    static UStaticMesh* Cube=nullptr; if(!Cube) Cube=LoadObject<UStaticMesh>(nullptr,TEXT("/Engine/BasicShapes/Cube.Cube"));
    if(Cube) M->SetStaticMesh(Cube);
    M->SetCollisionEnabled(bCollision?ECollisionEnabled::QueryAndPhysics:ECollisionEnabled::NoCollision);
    M->SetCollisionResponseToAllChannels(bCollision?ECR_Block:ECR_Ignore);
    if(OverrideMaterial) M->SetMaterial(0,OverrideMaterial);
    else
    {
        static UMaterialInterface* Base=nullptr; if(!Base) Base=LoadObject<UMaterialInterface>(nullptr,TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
        if(Base)
        {
            UMaterialInstanceDynamic* Dyn=UMaterialInstanceDynamic::Create(Base,M);
            if(Dyn){Dyn->SetVectorParameterValue(TEXT("Color"),Color);Dyn->SetVectorParameterValue(TEXT("BaseColor"),Color);M->SetMaterial(0,Dyn);}
        }
    }
    A->SetActorEnableCollision(bCollision);
}
