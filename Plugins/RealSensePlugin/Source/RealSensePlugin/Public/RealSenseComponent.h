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
	virtual void InitializeComponent() override;

	UFUNCTION(BlueprintCallable, Category = "RealSense Component")
	void SetInterfaceDelegate(UObject* newDelegate);

	//Simple toggle for now
	UFUNCTION(BlueprintCallable, Category = "RealSense Component")
	void EnableGesture(bool enable, FString gesture);

	//UFUNCTION(BlueprintCallable, Category = "RealSense Component")
	void EnableAllGestures(bool enableAll);

	UFUNCTION(BlueprintCallable, Category = "RealSense Component")
	void EnableHandTracking(bool enable);

private:
	class RealSenseComponentPrivate* _private;
	void InterfaceEventTick(float deltaTime);
};