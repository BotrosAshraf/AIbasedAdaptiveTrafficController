#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrafficDeadlockSolver.generated.h"

UCLASS()
class NPCVEHICLESYSTEM4_API ATrafficDeadlockSolver : public AActor
{
	GENERATED_BODY()

public:
	ATrafficDeadlockSolver();

protected:
	virtual void BeginPlay() override;

public:
	// الدالة الرئيسية التي ستفحص الطريق كل ثانية
	void CheckForDeadlocks();

	// دالة لإعادة السيارة لطبيعتها (إلغاء الشبح)
	UFUNCTION()
	void RestoreCarCollision(UPrimitiveComponent* CarMesh, UPrimitiveComponent* CarTrigger);

	// مؤقت لتكرار الفحص
	FTimerHandle TimerHandle_DeadlockLoop;

	// إعدادات قابلة للتعديل من المحرر
	UPROPERTY(EditAnywhere, Category = "Traffic AI")
	float CheckInterval = 1.0f; // كل كم ثانية نفحص؟

	UPROPERTY(EditAnywhere, Category = "Traffic AI")
	float StopSpeedThreshold = 10.0f; // السرعة التي نعتبر السيارة عندها "واقفة"

	UPROPERTY(EditAnywhere, Category = "Traffic AI")
	float GhostDuration = 4.0f; // مدة وضع الشبح
};