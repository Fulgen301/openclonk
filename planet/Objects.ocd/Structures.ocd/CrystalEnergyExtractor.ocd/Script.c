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
	SetAction("Idle");
	AddTimer(this.Check, 20);
}

private func Check()
{
	if (GetAction() == "Work")
		return;
	
	if (ReadyToAct())
		RegisterPowerProduction(100);
}

public func IsSteadyPowerProducer()	{ return false; }
public func GetProducerPriority()	{ return 0; }
public func NeededCrystalEnergy()	{ return 1; }
public func IsWorking()				{ return GetAction() == "Work"; }

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
}

private func WorkAbort()
{
	SetGraphics("");
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
		Delay = 5,
		FacetBase = 1,
		NextAction = "Work",
		StartCall = "WorkStart",
		PhaseCall = "Working",
		AbortCall = "WorkAbort"
	}
};
