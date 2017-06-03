/*
	Scenario: GetTheGold1
	Author: DasWipf
	
	Get that Gold! 
*/


func Initialize()
{
	var Flagpole = CreateObject(Flagpole);
	Flagpole->SetPosition(14, 284);

	var Basement = CreateObject(Basement);
	Basement->SetParent(Flagpole);
	Basement->SetPosition(14, 323);

	var PlayerStart = CreateObject(PlayerStart);
	PlayerStart->SetPosition(26, 295);
	PlayerStart->SetStartingKnowledge();
	PlayerStart->SetStartingMaterial([]);
	PlayerStart->SetClonkMaxContentsCount(1);
	PlayerStart->SetClonkMaxEnergy(2000);
	PlayerStart->SetViewLock(false);
	var Nugget = CreateObjectAbove(Nugget, 1391, 319);
	Nugget->Unstick(7);
	return ;
}

