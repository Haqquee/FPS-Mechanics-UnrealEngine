#include "Rifle.h"



ARifle::ARifle()
{
	
}

void ARifle::SetAttachmentTransform()
{
	FVector AttachmentLocation = FVector(0.f, 7.5f, 2.5f);
	FRotator AttachmentRotation = FRotator(15.f, -5.f, 0.f);

	Mesh->SetRelativeLocation(AttachmentLocation);
	Mesh->SetRelativeRotation(AttachmentRotation);
}
