#include "TrafficPlayerController.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "GameFramework/Pawn.h"

void ATrafficPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 1. نحصل على السيارة التي نتحكم فيها
	APawn* ControlledPawn = GetPawn();

	// 2. (الحل السحري) هل السيارة موجودة؟ 
	// هذا الشرط (if) يمنع الكود من التشغيل لو السيارة ماتت (Destroyed)
	if (ControlledPawn)
	{
		// 3. نحاول نجيب مكون حركة السيارة
		UChaosWheeledVehicleMovementComponent* VehicleMovement = Cast<UChaosWheeledVehicleMovementComponent>(ControlledPawn->GetMovementComponent());

		// ... (داخل دالة Tick)

		if (VehicleMovement)
		{
			// ... حساب السرعة ...
			float Speed = VehicleMovement->GetForwardSpeed() * 0.036f;
			int32 Gear = VehicleMovement->GetCurrentGear();

			// (التغيير هنا): فقط تأكد أنك تمرر القيمة للدالة المعدلة
			UpdateHUD(FMath::Abs(Speed), Gear);
		}
	}
	else
	{
		// السيارة غير موجودة (ماتت)، لا تفعل شيئاً
		// هذا يمنع الإيرور الذي كان يظهر لك
	}
}