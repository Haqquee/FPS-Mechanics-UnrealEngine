#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Rifle.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERMECHANICS_API ARifle : public AWeapon
{
	GENERATED_BODY()

public:
	ARifle();

	virtual void SetAttachmentTransform() override;
	
};
