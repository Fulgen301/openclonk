/**
	Crystal energy extractor
	
	@author Fulgen, Maikel
*/

#include Library_Structure
#include Library_Ownable
#include Library_PowerProducer
#include Library_Flag
#include Library_CrystalTechnology

local DefaultFlagRadius = 200;

protected func Initialize()
{
	_inherited(...);
	SetAction("Idle");
	AddTimer(this.Check, 20);
}

private func Check()
{
	if (IsWorking() && GetCrystals()[0])
	{
		if (GetCrystals()[0]) SetSign(GetCrystals()[0]->GetID());
	}
	else SetSign();
	
	if (ReadyToAct())
		RegisterPowerProduction(100);
}

public func IsSteadyPowerProducer()	{ return false; }
public func GetProducerPriority()	{ return 0; }
public func NeededCrystalEnergy()	{ return 1; }
public func IsWorking()				{ return GetAction() == "Work"; }
public func IsContainer()			{ return true; }

public func OnPowerProductionStart(int amount)
{
	if (!IsWorking())
		SetAction("Work");
	return true;
}

public func OnPowerProductionStop(int amount)
{
	if (IsWorking())
		SetAction("Idle");
	return true;
}

private func Working()
{
	DoCrystalEnergy(-1);
	if (!ReadyToAct())
	{
		SetAction("Idle");
		UnregisterPowerProduction();
	}
}

private func WorkStart()
{
	SetGraphics("Working");
	if (GetCrystals()[0]) SetSign(GetCrystals()[0]->GetID());
}

private func WorkAbort()
{
	SetGraphics("");
	SetSign();
}

public func SetSign(proplist def)
{
	if (!def) return SetGraphics("", nil, GFX_Overlay, 4);;
	SetGraphics("", def, GFX_Overlay, 4);
	SetObjDrawTransform(300, 0, 0, 0, 300, -15500, GFX_Overlay);
}

/*-- Properties --*/

local Name = "$Name$";
local Description = "$Description$";
local ActMap = {
	Idle = {
		Prototype = Action,
		Name = "Idle",
		Procedure = DFA_NONE,
		Directions = 2,
		FlipDir = 1,
		Length = 1,
		Delay = 0,
		FacetBase = 1,
		NextAction = "Idle",
	},
	Work = {
		Prototype = Action,
		Name = "Work",
		Procedure = DFA_NONE,
		Directions = 2,
		FlipDir = 1,
		Length = 20,
		Delay = 8,
		FacetBase = 1,
		NextAction = "Work",
		StartCall = "WorkStart",
		PhaseCall = "Working",
		AbortCall = "WorkAbort"
	}
};
