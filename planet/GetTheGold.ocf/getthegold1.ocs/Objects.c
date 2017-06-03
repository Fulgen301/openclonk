/* Automatically created objects file */

func InitializeObjects()
{
	var Sequence001 = CreateObject(Sequence, 540, 162);
	Sequence001->SetPosition(540, 162);
	Sequence001->SetTrigger({Rect=[-122,-20,170,49],Trigger="player_enter_region_rect"}, 3);
	Sequence001->SetAction({Actions=[{AfterMessage=10,Function="message",Options=[],Speaker={Function="triggering_clonk"},TargetPlayers={Function="triggering_player_list"},Text={Function="string_constant",Value={DE="Verkaufe den verfluchten Goldnugget!",Function="Translate",US="Sell the cursed nugget!"}}},{Context={Function="object_constant",Value=nil},Function="Action_script",Script={Function="string_constant",Value="Explode(20)"}}],Function="sequence"}, nil);
	var Flagpole001 = CreateObject(Flagpole, 436, 151, 0);
	Flagpole001->SetPosition(436, 151);
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
