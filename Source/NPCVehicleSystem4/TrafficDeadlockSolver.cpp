#include "TrafficDeadlockSolver.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "Components/BoxComponent.h"
#include "Components/PrimitiveComponent.h"
#include "TimerManager.h"

// الكونستركتور
ATrafficDeadlockSolver::ATrafficDeadlockSolver()
{
	PrimaryActorTick.bCanEverTick = false; // لا نحتاج Tick كل فريم توفيراً للأداء
}

// عند بدء اللعب
void ATrafficDeadlockSolver::BeginPlay()
{
	Super::BeginPlay();

	// تشغيل الفحص الدوري (كل 1 ثانية)
	GetWorldTimerManager().SetTimer(TimerHandle_DeadlockLoop, this, &ATrafficDeadlockSolver::CheckForDeadlocks, CheckInterval, true);
}

// دالة الفحص والحل
void ATrafficDeadlockSolver::CheckForDeadlocks()
{
	// 1. العثور على كل السيارات (Pawns) في العالم
	TArray<AActor*> AllVehicles;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawn::StaticClass(), AllVehicles);

	for (AActor* ActorA : AllVehicles)
	{
		APawn* CarA = Cast<APawn>(ActorA);
		if (!CarA) continue;

		// 2. هل السيارة A واقفة؟ (سرعتها قريبة من الصفر)
		if (CarA->GetVelocity().Size() <= StopSpeedThreshold)
		{
			// البحث عن الحساس (Trigger Box) في السيارة A
			// ملحوظة: هذا يبحث عن أول Box Component، تأكد أنه الحساس الأمامي
			UBoxComponent* TriggerA = CarA->FindComponentByClass<UBoxComponent>();

			if (TriggerA)
			{
				// 3. من يقف أمامها؟
				TArray<AActor*> OverlappingActors;
				TriggerA->GetOverlappingActors(OverlappingActors, APawn::StaticClass());

				for (AActor* ActorB : OverlappingActors)
				{
					APawn* CarB = Cast<APawn>(ActorB);
					// تأكد أنها سيارة أخرى وليست نفس السيارة
					if (CarB && CarB != CarA)
					{
						// 4. هل السيارة B (التي أمامنا) واقفة أيضاً؟ --> Deadlock!
						if (CarB->GetVelocity().Size() <= StopSpeedThreshold)
						{
							// 5. الحكم: كسر التعادل باستخدام المكان (X Location)
							// السيارة التي لها X أقل ستصبح "شبح" وتمر
							if (CarA->GetActorLocation().X < CarB->GetActorLocation().X)
							{
								// --- تنفيذ الحل (Make CarA a Ghost) ---

								// الحصول على جسم السيارة (Mesh)
								UPrimitiveComponent* MeshA = Cast<UPrimitiveComponent>(CarA->GetRootComponent());

								if (MeshA)
								{
									// أ. الجسم يتجاهل السيارات (عشان ميتخبطش)
									MeshA->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
									// ب. الحساس يتجاهل السيارات (عشان ميديهاش أمر توقف)
									TriggerA->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

									// طباعة رسالة للتوضيح
									if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Solving: %s is passing through!"), *CarA->GetName()));

									// 6. ضبط مؤقت لإعادتها لطبيعتها
									FTimerDelegate TimerDel;
									// نربط الدالة ونمرر لها الـ Mesh والـ Trigger عشان ترجعهم
									TimerDel.BindUFunction(this, FName("RestoreCarCollision"), MeshA, TriggerA);

									// تشغيل المؤقت مرة واحدة
									GetWorld()->GetTimerManager().SetTimerForNextTick(TimerDel); // حيلة صغيرة لتشغيل المؤقت، لكن الأصح استخدام SetTimer بالأسفل

									// تصحيح: نستخدم مؤقت حقيقي
									FTimerHandle TempHandle;
									GetWorldTimerManager().SetTimer(TempHandle, TimerDel, GhostDuration, false);

									// اخرج من اللوب الداخلي عشان منحلش المشكلة مرتين لنفس العربية
									goto EndLoopForCarA;
								}
							}
						}
					}
				}
			}
		}
	EndLoopForCarA:;
	}
}

// دالة استعادة الكوليجن
void ATrafficDeadlockSolver::RestoreCarCollision(UPrimitiveComponent* CarMesh, UPrimitiveComponent* CarTrigger)
{
	// إرجاع الجسم ليصطدم بالسيارات (Block)
	if (CarMesh && CarMesh->IsValidLowLevel())
	{
		CarMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	}

	// إرجاع الحساس ليرى السيارات (Overlap)
	if (CarTrigger && CarTrigger->IsValidLowLevel())
	{
		CarTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	}

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Collision Restored."));

}