//Some cool effects when the nugget is collected


#appendto Nugget

public func Entrance(object container)
{
	if (container && container->~IsClonk())
	{
		GameCallEx("OnNuggetCollected", this);
		AddTimer(this.Distasters, 10);
	}
}

public func Departure()
{
	RemoveTimer(this.Disasters);
}

private func Distasters()
{
	Global->CreateObject(Rock, Random(LandscapeWidth()), 120, NO_OWNER)->SetSpeed(RandomX(-30, 30), RandomX(100, 300));
	if (Contained() && Contained()->~IsClonk()) Contained()->DoEnergy(-20);
}

public func Sale(int player)
{
	Schedule(nil, "GameOver()", 20);
}