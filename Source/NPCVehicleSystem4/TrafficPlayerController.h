#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TrafficPlayerController.generated.h"

UCLASS()
class NPCVEHICLESYSTEM4_API ATrafficPlayerController : public APlayerController
{
    GENERATED_BODY()
    // ... (الكود السابق)

public:
    virtual void Tick(float DeltaTime) override;

    // (التغيير هنا): غيّرنا اسم المدخل من CurrentSpeed إلى NewSpeed
    UFUNCTION(BlueprintImplementableEvent, Category = "Vehicle HUD")
    void UpdateHUD(float NewSpeed, int32 CurrentGear);

protected:
    // هذا المتغير يظل كما هو
    float CurrentSpeed = 0.0f;
};