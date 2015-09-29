//
// CPlusPlusJigs
//
// This program collects data from the testing jigs and saves them to a file.
// NOTE: this program will only work on Windows.
//
#include <iostream>
#include <string>
#include "stdafx.h"
#include "NodPlugin.h"
#include "windows.h"
#using <System.dll>
#pragma comment(lib, "OpenSpatialDll.lib")

using namespace std;
using namespace System;
using namespace System::IO::Ports;
using namespace System::ComponentModel;

/**
* COM ports.
*/
string mEncoderCOMPort;
string mFabricSensorsCOMPort;

/**
* Called whenever an IMU event is fired.
*
*
*/
void IMUEventFired(NodEvent vEvent)
{
	if (vEvent.type == EventType::ServiceReady)
	{
		if (NodNumRings() > 0)
		{
			NodSubscribe(Modality::EulerMode, NodGetRingName(0));
		}
	}

	else if (vEvent.type == EventType::EulerAngles)
	{
		printf("\nEuler Angle Fired, roll: %f, pitch: %f, yaw: %f", vEvent.roll, vEvent.pitch, vEvent.yaw);
	}
}

int main()
{
	// Select the COM ports.
	cout << "\nAvailable COM ports:\n";
	for each(String^ vPortName in SerialPort::GetPortNames())
	{
		Console::WriteLine("- " + vPortName);
	}

	Console::Write("\nType in the COM port for the encoder (leave blank if not in use): ");
	getline(cin, mEncoderCOMPort);

	Console::Write("\nType in the COM port for the fabric sensors (leave blank if not in use): ");
	getline(cin, mFabricSensorsCOMPort);

	// Open the COM ports.

	// ...

	// Connect the IMUs.
	NodInitialize(IMUEventFired);

	// ...

	// Open the file to be written to.

	// ...

	return 0;
}
