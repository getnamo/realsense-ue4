#pragma once

#include "RealSensePluginPublicPCH.h"
#include "RealSenseComponent.generated.h"

UCLASS(ClassGroup = Input, meta = (BlueprintSpawnableComponent))
class URealSenseComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

public:
	~URealSenseComponent();
	virtual void OnRegister() override;
	virtual void OnUnregister() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "RealSense Component")
	void SetInterfaceDelegate(UObject* newDelegate);

	//Simple toggle for now
	UFUNCTION(BlueprintCallable, Category = "RealSense Component")
	void EnableGestures(bool enableAll);

private:
	class RealSenseComponentPrivate* _private;
	void InterfaceEventTick(float deltaTime);
};