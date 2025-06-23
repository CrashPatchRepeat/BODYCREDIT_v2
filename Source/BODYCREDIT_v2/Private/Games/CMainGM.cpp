#include "Games/CMainGM.h"
#include "Global.h"

ACMainGM::ACMainGM()
{
	// DefaultPawnClass
	CHelpers::GetClass(&DefaultPawnClass, TEXT("/Script/Engine.Blueprint'/Game/Characters/Runner/BP_CNox_Runner.BP_CNox_Runner_C'"));

	// PlayerController
	CHelpers::GetClass(&PlayerControllerClass, TEXT("/Script/Engine.Blueprint'/Game/Games/BP_CNoxController.BP_CNoxController_C'"));
}
