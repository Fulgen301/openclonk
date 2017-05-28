/**
	CrystalTechnology
	
	@author Fulgen
	
	This library provides crystal technology.
*/

static g_crystal_tech_producable;

/* Callbacks */

public func AcceptCrystal(proplist crystal)
{
	return crystal && crystal->~IsCrystal();
}

public func IsCrystalTechnology()
{
	return true;
}

public func NeededCrystalEnergy()
{
	return 0;
}

public func ReadyToAct()
{
	return GetCrystalEnergy() >= NeededCrystalEnergy();
}

public func GetCrystalEnergy()
{
	var energy;
	for (var crystal in GetCrystals())
	{
		if (crystal)
			energy += crystal->~GetEnergy();
	}
	return energy;
}

public func DoCrystalEnergy(int energy)
{
	for (var crystal in GetCrystals())
	{
		if (crystal)
		{
			if (energy >= 0)
			{
				var delta = 100 - BoundBy(energy, 0, crystal->GetCon());
				if (energy >= delta)
				{
					energy -= delta;
				}
				
				else
				{
					crystal->~DoEnergy(energy);
					return this;
				}
				crystal->~DoEnergy(delta);
			}
			
			else if (energy < 0)
			{
				if (crystal->~GetEnergy() > energy)
				{
					crystal->~DoEnergy(energy);
					return this;
				}
				
				else
				{
					energy += 100;
					crystal->RemoveObject();
				}
			}
		}
	}
	return this;
}

public func GetCrystals()
{
	return FindObjects(Find_Container(this), Find_Func("IsCrystal"));
}
/* Engine callbacks */

public func RejectCollect(proplist to_collect)
{
	if (_inherited(to_collect, ...)) return true;
	
	return !AcceptCrystal(to_collect);
}

public func Collection2(object collect)
{
	_inherited(collect, ...);
	if (collect && collect->~IsCrystal())
		this->~OnCrystalCollected(collect);
	
	return true;
}

public func ContentsDestruction(object obj)
{
	_inherited(obj, ...);
	if (obj && obj->~IsCrystal())
		this->~OnCrystalLost(obj);
}

public func Departure(object obj)
{
	_inherited(obj, ...);
	if (obj && obj->~IsCrystal())
		this->~OnCrystalLost(obj);
}

/* Globals */

global func CrystalTechnologyAvailable()
{
	if (g_crystal_tech_producable == nil)
	{
		if (FrameCounter() < 500)
		{
			return nil;
		}
		
		if (GetScenarioVal("NoScan", "Landscape"))
		{
			return g_crystal_tech_producable = true;
		}
		
		for (var mat in Library_CrystalTechnology->CrystalMaterials())
		{
			if (GetMaterialCount(Material(mat)) != -1)
			{
				return g_crystal_tech_producable = true;
			}
		}
		return g_crystal_tech_producable = false;
	}
	else return g_crystal_tech_producable;
}

private func CrystalMaterials()
{
	return ["Ruby", "Amethyst"];
}

public func Definition(proplist def)
{
	ScheduleCall(nil, def.CrystalTechnologyAvailable, 550);
}