#include "SootGameMode.h"
#include "SootCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASootGameMode::ASootGameMode()
    : Super()
{
  // set default pawn class to our Blueprinted character
  static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(
      TEXT("/TravelingSoot/Blueprints/SootCharacter"));

  if (PlayerPawnClassFinder.Class != NULL)
  {
    DefaultPawnClass = PlayerPawnClassFinder.Class;
  }
}
