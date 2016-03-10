/*--
	Stackable
	Author: Newton
	
	Including this object means, the object is stackable. Other objects of
	the same type will be added automatically to the object. This functionality
	is similar to the Pack-functionality of the arrows in old clonk titles only
	more general.
	The count of how many objects are stacked together (into a single one, this
	one) is shown in the picture of the object and can be queried and set via
	GetStackCount()/SetStackCount().
	To take one object of the stack, call TakeObject(). As long
	as the object exists, one can always take an object, even if it is the last
	one (self). This object is always outside.
	Infinite stackable count can by achieved using SetInfiniteStackCount.
	
	On entrance (or to be more precise: on RejectEntrance), it will be checked
	if the entering stackable object can be distributed over the other objects
	of the same ID. If yes, this object is deleted and the other object(s) will
	have a higher stack-count.
	
	Example 1:
	'15x Arrow' is about to enter a clonk which has '5x Arrow'. 15 will be added
	to the stack-count of the clonks '5x Arrow'-object (making it '20x Arrow'),
	the entering object will be deleted.
	
	Example 2:
	'17x Arrow' is about to enter a clonk which has '15x Arrow' and a bow with
	'10x Arrow' in it's ammunition slot. 10 will be added to the stack-count
	of the arrows in the bow, 5 to the stack-count of the arrows in the clonk
	(assuming MaxStackCount() is 20) and the original arrows-object will have
	2 arrows left. If there is an inventory slot left, the '2x Arrow" object
	will enter the clonk.
	
	Most objects which can be stacked might want to set different pictures
	and ingame graphics for different counts of objects. This can be done
	by overloading UpdatePicture(), but remember to write _inherited() then.
--*/


local count, count_is_infinite;

// Max size of stack
static const Stackable_Max_Count = 2147483647;
static const Stackable_Max_Display_Count = 999;

// What GetStackCount should return if the count is set to infinite
// Set this to a fairly large number and not e.g. -1, so naive
// implementations that update their graphics by GetStackCount() show a
// bunch of items. However, the number shouldn't be too large so the
// object doesn't get overly heavy.
// Note that count_is_infinite is a separate variable, so we can support
// stacks >999 but <Inf in the future.
static const Stackable_Infinite_Count = 50;

public func IsStackable() { return true; }
public func GetStackCount() { return count; }
public func MaxStackCount() { return 20; }
public func InitialStackCount() { return MaxStackCount(); }
public func IsFullStack() { return this->IsInfiniteStackCount() || (GetStackCount() >= MaxStackCount()); }
public func IsInfiniteStackCount() { return !!count_is_infinite; }

protected func Construction()
{
	count = InitialStackCount();
	return _inherited(...);
}

func Destruction()
{
	var container = Contained();
	if (container)
	{
		// has an extra slot
		if (container->~HasExtraSlot())
			container->~NotifyHUD();
	}
	return _inherited(...);
}


/**
 * Puts the stack count of another object on top of this stack.
 * The stack count of the other object is not modified.
 * @par other the other object. Must be of the same ID as the stack.
 * @return the amount of objects that could be stacked.
 */
public func Stack(object other)
{
	if (other->GetID() != GetID()) return 0;
	if (other == this) return 0; 

	// Infinite stacks can always take everything
	if (this->IsInfiniteStackCount()) return other->GetStackCount();
	if (other->~IsInfiniteStackCount())
	{
		SetInfiniteStackCount();
		return other->GetStackCount();
	}

	var howmany = Min(other->GetStackCount(), MaxStackCount() - GetStackCount());
	var future_count = GetStackCount() + howmany;
	//Log("*** Added %d objects to stack", howmany);
	
	if (howmany <= 0 || future_count > Stackable_Max_Count)
		return 0;

	SetStackCount(future_count);
	return howmany;
}

public func SetStackCount(int amount)
{
	count = BoundBy(amount, 0, Stackable_Max_Count); // allow 0, so that the object can be removed in UpdateStackDisplay
	count_is_infinite = false;
	UpdateStackDisplay();
	return true;
}

public func DoStackCount(int change)
{
	if (!(this->IsInfiniteStackCount()))
	{
		count += change;
		UpdateStackDisplay();
	}
}

public func SetInfiniteStackCount()
{
	count = Stackable_Infinite_Count;
	count_is_infinite = true;
	UpdateStackDisplay();
	return true;
}

public func TakeObject()
{
	if (GetStackCount() == 1)
	{
		Exit();
		return this;
	}
	else
	{
		DoStackCount(-1);
		var take = CreateObject(GetID(), 0, 0, GetOwner());
		take->SetStackCount(1);
		return take;
	}
}

public func UpdateStackDisplay()
{
	// empty stacks have to be removed
	if (GetStackCount() <= 0)
	{
		RemoveObject();
		return;
	}
	// otherwise update the object
	UpdatePicture();
	UpdateMass();
	UpdateName();
	// notify hud
	var container = Contained();
	if (container)
	{
		// has an extra slot
		if (container->~HasExtraSlot())
		{
			container->~NotifyHUD();
		}
		// is a clonk with new inventory system
		else
		{
			container->~OnInventoryChange();
		}
	}
}

private func UpdatePicture()
{
	// Allow other objects to adjust their picture.
	return _inherited(...);
}

private func UpdateName()
{
	if (this->IsInfiniteStackCount())
		SetName(Format("$Infinite$ %s", GetID()->GetName()));
	else
		SetName(Format("%dx %s", GetStackCount(), GetID()->GetName()));
}

private func UpdateMass()
{
	SetMass(GetID()->GetMass() * Max(GetStackCount(), 1) / InitialStackCount());
}

/*
	Try to merge packs BEFORE entering the container.
	That means that a container can not prevent objects stacking into it.
	However, the other way round (after the object has entered) presents more issues.
*/
protected func RejectEntrance(object into)
{
	var try_put = TryPutInto(into);
	//Log("***** TryPutInto did in fact return %v", try_put);
	if (try_put)
	{
		//Log("****** Rejected entrance into %s!!", into->GetName());
		return true;
	}
	//Log("***** Entered %v %s!!", this, into->GetName());
	return _inherited(into, ...);
}

/* Value */

public func CalcValue(object in_base, int for_plr)
{
	return GetID()->GetValue() * Max(GetStackCount(), 1) / InitialStackCount();
}

/* Tries to add this object to another stack. Returns true if successful.
	This call might remove this item. */
public func TryAddToStack(object other)
{
	if (other == this) return false;
	
	// Is a stack possible in theory?
	if (other->~IsStackable() && other->GetID() == GetID())
	{
		var howmany = other->Stack(this);
		if (howmany > 0)
		{
			var stack = this;
			DoStackCount(-howmany);
			if (stack && stack->IsInfiniteStackCount()) stack->RemoveObject(); 
			// Stack succesful! No matter how many items were transfered.
			return true;
		}
	}
	return false;
}

/* Attempts to add this stack either to existing stacks in an object or
	if only_add_to_existing_stacks is not set, also recursively into HasExtraSlot containers in that object.*/
public func TryPutInto(object into, bool only_add_to_existing_stacks)
{
	only_add_to_existing_stacks = only_add_to_existing_stacks ?? false;
	var before = GetStackCount();
	var contents = FindObjects(Find_Container(into));

	if (!only_add_to_existing_stacks)
	{
		// first check if stackable can be put into object with extra slot
		for (var content in contents)
		{
			if (!content)
				continue;
			if (content->~HasExtraSlot())
				if (TryPutInto(content))
					return true;
		}
	}
	
	var added_to_stack = false;
	
	// then check this object
	for (var content in contents)
	{
		if (!content)
			continue;
		added_to_stack = TryAddToStack(content) || added_to_stack;
		if (!this) return true;
	}
	
	//Log("***** Stack can enter the object %s? TryPutInto will return %v", into->GetName(), added_to_stack);
	
	// IFF anything changed, we need to update the display.
	if (before != GetStackCount())
	{
		UpdateStackDisplay();
	}
	return added_to_stack;
}

// Infinite stacks can only be stacked on top of others.
public func CanBeStackedWith(object other)
{
	if (other->~IsInfiniteStackCount() != this->IsInfiniteStackCount()) return false;
	return _inherited(other, ...);
}

// Infinite stacks show a little symbol in their corner.
public func GetInventoryIconOverlay()
{
	if (!(this->IsInfiniteStackCount())) return nil;
	return {Left = "50%", Bottom="50%", Symbol=Icon_Number, GraphicsName="Inf"};
}

// Save stack counts in saved scenarios
public func SaveScenarioObject(props)
{
	if (!inherited(props, ...)) return false;
	props->Remove("Name");
	if (this->IsInfiniteStackCount())
		props->AddCall("Stack", this, "SetInfiniteStackCount");
	else if (GetStackCount() != InitialStackCount())
		props->AddCall("Stack", this, "SetStackCount", GetStackCount());
	return true;
}

// Tell the interaction menu as how many objects this object should be displayed
func GetInteractionMenuAmount()
{
	var object_amount = this->GetStackCount() ?? 1;
	// Infinite stacks work differently - showing an arbitrary amount would not make sense.
	if (object_amount > 1 && this->IsInfiniteStackCount())
		object_amount = 1;
	return Min(object_amount, Stackable_Max_Display_Count);
}
