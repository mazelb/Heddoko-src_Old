#include "stdafx.h"
#include "ExampleDelegate.h"


ExampleDelegate::ExampleDelegate()
{
}


ExampleDelegate::~ExampleDelegate()
{
}

void ExampleDelegate::buttonEventFired(ButtonEvent event)
{
	printf("\nButton Event Fired. ButtonEventType: %d from id: %d", event.buttonEventType, event.sender);
}

void ExampleDelegate::pointerEventFired(PointerEvent event)
{
	printf("\nPointer Event Fired. Pointer X: %d, Pointer Y: %d from id: %d", event.x, event.y, event.sender);
}

void ExampleDelegate::gestureEventFired(GestureEvent event)
{
	printf("\nGesture Event Fired. Gesture Type: %d from id: %d", event.gestureType, event.sender);
}

void ExampleDelegate::pose6DEventFired(Pose6DEvent event)
{
	

	if (event.sender == 0)
	{
		nod1.yaw = event.yaw;
		nod1.pitch = event.pitch;
		nod1.roll = event.roll;
	}

	if (event.sender == 1)
	{
		nod2.yaw = event.yaw;
		nod2.pitch = event.pitch;
		nod2.roll = event.roll;
	}


	//printf("\nPose6D Event Fired. Yaw: %f, Pitch: %f, Roll %f from id: %d", yaw, pitch, roll, event.sender);
	//printf("hi");
	//OutputDebugString(L"pose6d");
	//printf("\nPose6D Event Fired. Yaw: %f, Pitch: %f, Roll %f from id: %d", event.yaw, event.pitch, event.roll, event.sender);
}

void ExampleDelegate::gameControlEventFired(GameControlEvent event)
{
	printf("\n Game Control (Backspin) Event Fired. X: %d, Y: %d, Trigger: %d from id: %d", event.x, event.y, event.trigger, event.sender);
}

void ExampleDelegate::motion6DEventFired(Motion6DEvent event)
{
	printf("\n Motion 6D Event Fired AX: %f, AY: %f, AZ: %f, GX: %f, GY: %f, GZ: %f from id: %d", event.accelX, event.accelY, event.accelZ, event.gyroX, event.gyroY, event.gyroZ, event.sender);
}