/* Automatically created objects file */

func InitializeObjects()
{
	var Clonk001 = CreateObjectAbove(Clonk, 502, 182);
	Clonk001->Unstick(7);
	Clonk001->SetName("Fulgen");
	Clonk001->DoEnergy(-38);
	Clonk001->SetSkin(2);
	Clonk001->SetDir(DIR_Left);

	var Sequence001 = CreateObject(Sequence, 500, -8);
	Sequence001->SetPosition(500, -8);
	Sequence001->SetTrigger({Rect=[-122,144,170,44],Trigger="player_enter_region_rect"}, 9);
	Sequence001->SetAction({Actions=[{AfterMessage="next",Function="message",Options=[],Speaker={Function="object_constant",Value=Clonk001},TargetPlayers={Function="triggering_player_list"},Text={Function="string_constant",Value={DE="Siehst du den Goldklumpen dort hinten? Er ist von enormen Wert! Man sagt zwar er sei verflucht, aber was soll mir schon passieren?",Function="Translate",US="Look at this nugget! It is very precious! Someone told me it's cursed but nevermind.. what could happen to me?"}}},{Function="do_energy",Object={Function="object_constant",Value=Clonk001},Value={Function="int_constant",Value=1}},{Function="wait",Time=36},{Function="create_object",ID={Function="def_constant",Value=Firestone},Rotation={Function="int_constant",Value=0},SpeedR={Function="int_constant",Value=0},SpeedX={Function="int_constant",Value=0},SpeedY={Function="int_constant",Value=10}},{Function="stop_sequence"}],Function="sequence"}, nil);
	Sequence001->SetDeactivateAfterAction(true);
	var Flagpole001 = CreateObject(Flagpole, 436, 151, 0);
	Flagpole001->SetPosition(436, 151);
	Flagpole001->SetOwner(0);
	Flagpole001->MakeInvincible();

	var PlayerStart001 = CreateObject(PlayerStart, 483, 171);
	PlayerStart001->SetPosition(483, 171);
	PlayerStart001->SetStartingKnowledge();
	PlayerStart001->SetStartingMaterial([]);
	PlayerStart001->SetStartingBaseMaterial([]);
	PlayerStart001->SetClonkMaxContentsCount(1);
	PlayerStart001->SetClonkMaxEnergy(2500);
	PlayerStart001->SetViewLock(false);
	CreateObjectAbove(Nugget, 1120, 184);
	return true;
}
