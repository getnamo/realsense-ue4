#include "RealSensePluginPrivatePCH.h"
#include "RealSenseComponent.h"
#include "SlateBasics.h"
#include "IRealSensePlugin.h"
#include "RealSenseInterface.h"
#include <vector>

#include <sstream> //for std::stringstream 
#include <string>  //for std::string



class RealSenseComponentPrivate
{
public:
	~RealSenseComponentPrivate()
	{
	}

	//delegate pointer
	UObject* interfaceDelegate;

};

//ULeapController
URealSenseComponent::URealSenseComponent(const FObjectInitializer &init) : UActorComponent(init), _private(new RealSenseComponentPrivate())
{
	bWantsInitializeComponent = true;
	bAutoActivate = true;
	PrimaryComponentTick.bCanEverTick = true;	//the component automatically ticks so we don't have to
}

URealSenseComponent::~URealSenseComponent()
{
	delete _private;
}

void URealSenseComponent::OnRegister()
{
	UE_LOG(LogClass, Log, TEXT("Registered RealSense Component"));
	Super::OnRegister();

	//Attach the delegate pointer automatically
	SetInterfaceDelegate(GetOwner());

	//Track and warn users about multiple components.
	//controllerCount++;
	//if (controllerCount>1)
	//	UE_LOG(LeapPluginLog, Warning, TEXT("Warning! More than one (%d) RealSense Component detected! Duplication of work and output may result (inefficiency warning)."), controllerCount);
}

void URealSenseComponent::OnUnregister()
{
	UE_LOG(LogClass, Log, TEXT("Unregistered RealSense Component"));
	Super::OnUnregister();

	//Allow GC of private UObject data
	/*_private->CleanupEventData();	//UObjects attached to root need to be unrooted before we unregister so we do not crash between pie sessions (or without attaching to this object, leak them)
	controllerCount--;

	
	//*/
}

void URealSenseComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
									FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Forward the component tick
	InterfaceEventTick(DeltaTime);

}

void URealSenseComponent::SetInterfaceDelegate(UObject* newDelegate)
{
	UE_LOG(LogClass, Log, TEXT("InterfaceObject: %s"), *newDelegate->GetName());

	//Use this format to support both blueprint and C++ form
	if (newDelegate->GetClass()->ImplementsInterface(URealSenseInterface::StaticClass()))
	{
		_private->interfaceDelegate = newDelegate;

		//set module delegate
		if (IRealSensePlugin::IsAvailable())
		{
			IRealSensePlugin::Get().SetInterfaceDelegate(newDelegate);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("URealSenseComponent Warning: Delegate is NOT set, did you implement RealSenseInterface?"));
	}
}

void URealSenseComponent::EnableGestures(bool enableAll)
{
	if (IRealSensePlugin::IsAvailable())
	{
		IRealSensePlugin::Get().EnableGestureDetection(enableAll);
	}
}

//Main Event driven tick
void URealSenseComponent::InterfaceEventTick(float DeltaTime)
{
	//This is our tick event that is forwarded from the delegate, check validity
	//if (!_private->interfaceDelegate) return;
	if (IRealSensePlugin::IsAvailable())
	{
		IRealSensePlugin::Get().RealSenseTick(DeltaTime);
	}

	//Check all the data we collected

	//Call joint moved for each joint we've collected
	//IRealSenseInterface::Execute_JointMoved(_private->interfaceDelegate, blah);
	
}