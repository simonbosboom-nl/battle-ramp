#include "BRHUD.h"
#include "BRGameMode.h"
#include "BRCarPawn.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
void ABRHUD::DrawHUD()
{
    Super::DrawHUD(); if(!Canvas)return;
    UWorld* World=GetWorld(); if(!World)return;
    ABRGameMode* GM=World->GetAuthGameMode<ABRGameMode>(); if(!GM)return;
    const float Scale=FMath::Clamp(Canvas->ClipX/1500.0f,0.65f,1.25f);
    auto Text=[&](const FString& S,float X,float Y,FLinearColor Color,float Size=1.0f){FCanvasTextItem Item(FVector2D(X,Y),FText::FromString(S),GEngine->GetSmallFont(),Color);Item.Scale=FVector2D(Size*Scale,Size*Scale);Item.EnableShadow(FLinearColor::Black);Canvas->DrawItem(Item);};
    Text(FString::Printf(TEXT("BLUE  %d     |     %d  ORANGE"),GM->BlueScore,GM->OrangeScore),Canvas->ClipX*0.5f-160*Scale,24,FLinearColor::White,1.5f);
    const int32 Minutes=FMath::Max(0,(int32)GM->TimeRemaining)/60, Seconds=FMath::Max(0,(int32)GM->TimeRemaining)%60;
    Text(FString::Printf(TEXT("%d:%02d"),Minutes,Seconds),Canvas->ClipX*0.5f-28*Scale,60,GM->bLastMinuteWarning?FLinearColor(1,0.18f,0.08f):FLinearColor::White,1.5f);
    if(GM->bLastMinuteWarning) Text(TEXT("LAST MINUTE!"),Canvas->ClipX*0.5f-70*Scale,92,FLinearColor(1,0.17f,0.05f),1.35f);
    if(ABRCarPawn* Car=GetOwningPlayerController()?Cast<ABRCarPawn>(GetOwningPlayerController()->GetPawn()):nullptr)
    {
        Text(FString::Printf(TEXT("HP %.0f / %.0f    BALL PULSES %d    RAMPS %d"),Car->GetHealth(),Car->GetMaxHealth(),Car->GetPowerKicks(),Car->GetRampCount()),20,Canvas->ClipY-60,FLinearColor::White,1.0f);
        Text(FString::Printf(TEXT("WINS %d    COINS %d    TEAM SIZE %dv%d"),GM->Wins,GM->Coins,GM->TeamSize,GM->TeamSize),20,Canvas->ClipY-36,FLinearColor(0.45f,0.9f,1.0f),1.0f);
    }
    Text(TEXT("PRODUCED BY HOMEWORK NINJA STUDIOS"),Canvas->ClipX*0.5f-154*Scale,Canvas->ClipY-27,FLinearColor(0.75f,0.82f,0.9f),0.75f);
    Text(TEXT("WASD drive | SHIFT boost | SPACE ball pulse | J jump | B ramp | O/P teams | K shop | V camera"),Canvas->ClipX*0.5f-250*Scale,Canvas->ClipY-48,FLinearColor(0.8f,0.9f,0.95f),0.72f);
    if(GM->bMatchEnded) Text(TEXT("MATCH OVER - Press ENTER to restart"),Canvas->ClipX*0.5f-170*Scale,Canvas->ClipY*0.35f,FLinearColor::Yellow,1.6f);
}
