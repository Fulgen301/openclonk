//Some cool effekts when the nugget is collected


#appendto Nugget

public func Entrance(object container)
{
	if (container && container->~IsClonk())
	{
		Schedule(CreateObject(Rock,Random(1100)+200), 2, 500);
		Schedule(FindObject(Find_ID(Rock)), "Explode(100)", 216, 500);
	}	
}