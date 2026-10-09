#pragma once
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BRHUD.generated.h"
UCLASS()
class BATTLERAMPSUE_API ABRHUD : public AHUD
{
    GENERATED_BODY()
public:
    virtual void DrawHUD() override;
};
