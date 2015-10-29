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
}

void URealSenseComponent::OnUnregister()
{
	UE_LOG(LogClass, Log, TEXT("Unregistered RealSense Component"));
	Super::OnUnregister();

	SetInterfaceDelegate(NULL);
}

void URealSenseComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
									FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Forward the component tick
	InterfaceEventTick(DeltaTime);

}


void URealSenseComponent::InitializeComponent()
{
	Super::InitializeComponent();

	//by default disable gestures so each begin play is unique
	EnableGesture(false,FString());
}

void URealSenseComponent::SetInterfaceDelegate(UObject* newDelegate)
{

	if (newDelegate == NULL)
	{
		UE_LOG(LogClass, Log, TEXT("NULL delegate passed, clearing delegate"));
		//set module delegate
		if (IRealSensePlugin::IsAvailable())
		{
			IRealSensePlugin::Get().SetInterfaceDelegate(NULL);
		}
		return;
	}

	//Its not null so its probably a UObject.
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

void URealSenseComponent::EnableGesture(bool enable, FString gesture)
{
	if (IRealSensePlugin::IsAvailable())
	{
		IRealSensePlugin::Get().EnableGestureDetection(enable, gesture);
	}
}


void URealSenseComponent::EnableAllGestures(bool enableAll)
{
	if (IRealSensePlugin::IsAvailable())
	{
		IRealSensePlugin::Get().EnableAllGestureDetection(enableAll);
	}
}

void URealSenseComponent::EnableHandTracking(bool enable)
{
	if (IRealSensePlugin::IsAvailable())
	{
		IRealSensePlugin::Get().EnableHandDetection(enable);
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
}