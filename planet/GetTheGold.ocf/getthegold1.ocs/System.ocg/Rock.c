#appendto Rock

public func Hit()
{
	ScheduleCall(this, Global.Explode, RandomX(1, 6) * 36, nil, 20);
}