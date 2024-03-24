#include "DefaultGameMode.h"

ADefaultGameMode::ADefaultGameMode() : Super() 
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_PlayerCharacterV2"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
}