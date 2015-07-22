////
////
//// This Code generates orientation of hips and knee joints and send it over pipe to another program for visualization. 
/// Therefore, first this should be excuted then x-IMU IMU and AHRS Algorithms code from 3D_AHRS folder should be executed to see the results


#include "stdafx.h"
#include "ExampleDelegate.h"
#include <iostream>
#include <windows.h>
#include <math.h>
#include <stdio.h>      /* printf */
#include <math.h>       /* acos */
#include <windows.h>
#include <cmath>
#include <sstream>
#include <string>
#include <stdexcept>
using namespace std;

# define PI           3.14159265358979323846  /* pi */


struct quaternion
{
	float x, y, z, w;
};

struct vec
{
	float x, y, z, l;
};



/*Functions Protos*/

vec cross(vec vector1, vec vector2);
void rb(float a[][3], float yaw, float pitch, float roll);
void ro(float a[][3], float yaw, float pitch, float roll);
float dot(vec vector1, vec vector2);
void rvector(float a[][3], vec u, float t);
void multi(float a[][3], float b[][3], float c[][3]);
void delay(int count);
void apply(float a[], float b[][3]);
void apply2(float a[], float b[][3]);
void apply3(float a[], float b[][3]);
void graphicalrotation(float a[][3], float c[][3]);

void HipOrientation(float CurrentHipOrientation[][3], float IntitialRotationLocalHip[][3], float IntitialRotationGloballHip[][3], float IntitialRotationLocalKnee[][3], float IntitialRotationGlobalKnee[][3], float NodHipYaw, float NodHipPitch, float NodHipRoll, float NodKneeYaw, float NodKneePitch, float NodKneeRoll, float StretchSensorKnee);
void KneeOrientation(float CurrentKneeOrientation[][3], float IntitialRotationLocalHip[][3], float IntitialRotationGloballHip[][3], float IntitialRotationLocalKnee[][3], float IntitialRotationGlobalKnee[][3], float NodHipYaw, float NodHipPitch, float NodHipRoll, float NodKneeYaw, float NodKneePitch, float NodKneeRoll, float StretchSensorKnee);

vec Ncross(vec vector1, vec vector2);
void RotationLocal(float a[][3], float yaw, float pitch, float roll);
void RotationGlobal(float a[][3], float yaw, float pitch, float roll);
void RotationVector(float a[][3], vec u, float t);














int __cdecl main(int argc, char **argv)
{
	OpenSpatialServiceController* controller;
	controller = new OpenSpatialServiceController;
	//ExampleDelegate* del = new ExampleDelegate;
	ExampleDelegate hi;
	controller->setDelegate(&hi);
	BOOL continuer = true;
	//controller->setBufferPackets(true);
	while (!controller->setup)
	{
		Sleep(100);
	}
	
		            printf("\nfirst controler name %s and second %s: ", controller->names.at(0).c_str(), controller->names.at(1).c_str());
					controller->controlService(SUBSCRIBE_TO_POSE6D, controller->names.at(0));
					controller->controlService(SUBSCRIBE_TO_POSE6D, controller->names.at(1));


					
					
				
					





					// Opening an pipe instannce

					wcout << "Creating an instance of a named pipe..." << endl;

					// Create a pipe to send data
					HANDLE pipe = CreateNamedPipe(
						L"\\\\.\\pipe\\testpipe", // name of the pipe
						PIPE_ACCESS_OUTBOUND, // 1-way pipe -- send only
						PIPE_TYPE_BYTE, // send data as a byte stream
						1, // only allow 1 instance of this pipe
						0, // no outbound buffer
						0, // no inbound buffer
						0, // use default wait time
						NULL // use default security attributes
						);

					if (pipe == NULL || pipe == INVALID_HANDLE_VALUE) {
						wcout << "Failed to create outbound pipe instance.";
						// look up error code here using GetLastError()
						system("pause");
						return 1;
					}

					wcout << "Waiting for a client to connect to the pipe..." << endl;

					// This call blocks until a client process connects to the pipe
					BOOL result = ConnectNamedPipe(pipe, NULL);
					if (!result) {
						wcout << "Failed to make connection on named pipe." << endl;
						// look up error code here using GetLastError()
						CloseHandle(pipe); // close the pipe
						system("pause");
						return 1;
					}

					wcout << "Sending data to pipe..." << endl;

					// This call blocks until a client process reads all the data
					const wchar_t *data = L"*** Hello Pipe World ***";
					DWORD numBytesWritten = 0;
					


					
					float HipOrientationLocal[3][3] = {};
					float HipOrientationLocali[3][3] = {};
					float KneeOrientationLocal[3][3] = {};
					float KneeOrientationLocali[3][3] = {};

					float HipOrientationGlobal[3][3] = {};
					float KneeOrientationGlobal[3][3] = {};
					float HipOrientationGlobali[3][3] = {};
					float KneeOrientationGlobali[3][3] = {};

					// for sending data over pipe
					float rx[9] = {}, rx2[9] = {};

					std::stringstream strm;
					
					rb(HipOrientationLocali, hi.nod1.yaw, hi.nod1.pitch, hi.nod1.roll);
					rb(KneeOrientationLocali, hi.nod2.yaw, hi.nod2.pitch, hi.nod2.roll);
					ro(HipOrientationGlobali, hi.nod1.yaw, hi.nod1.pitch, hi.nod1.roll);
					ro(KneeOrientationGlobali, hi.nod2.yaw, hi.nod2.pitch, hi.nod2.roll);

					while (true)
					{


						// calculate the new orientation
						HipOrientation(HipOrientationGlobal, HipOrientationLocali, HipOrientationGlobali, KneeOrientationLocali, KneeOrientationGlobali, hi.nod1.yaw, hi.nod1.pitch, hi.nod1.roll, hi.nod2.yaw, hi.nod2.pitch, hi.nod2.roll, 1);
						KneeOrientation(KneeOrientationGlobal, HipOrientationLocali, HipOrientationGlobali, KneeOrientationLocali, KneeOrientationGlobali, hi.nod1.yaw, hi.nod1.pitch, hi.nod1.roll, hi.nod2.yaw, hi.nod2.pitch, hi.nod2.roll, 1);
						

						// applying changes and adjustion for visualization
						apply2(rx, HipOrientationGlobal);
						apply2(rx2, KneeOrientationGlobal);




                    /// sending data to the pipe
						for (int i = 0; i < 9; i++)
						{

							strm << rx[i] << "\n";
							result = WriteFile(
								pipe, // handle to our outbound pipe
								strm.str().c_str(), // data to send
								strm.str().size(), // length of data to send (bytes)
								&numBytesWritten, // will store actual amount of data sent
								NULL // not using overlapped IO
								);
							strm.str("");


						}


						for (int i = 0; i < 9; i++)
						{

							strm << rx2[i] << "\n";
							result = WriteFile(
								pipe, // handle to our outbound pipe
								strm.str().c_str(), // data to send
								strm.str().size(), // length of data to send (bytes)
								&numBytesWritten, // will store actual amount of data sent
								NULL // not using overlapped IO
								);
							strm.str("");


						}



					}


					if (result) {
						wcout << "Number of bytes sent: " << numBytesWritten << endl;
					}
					else {
						wcout << "Failed to send data." << endl;
						// look up error code here using GetLastError()
					}

					// Close the pipe (automatically disconnects client too)
					CloseHandle(pipe);

					wcout << "Done." << endl;

					system("pause");

					return 0;


}





/**
* Apply2()
*	@This Fuction adjustes the orientation for visualizing software
*	@param float a[] : adjusted orientation ready to submit over pipe
*	@param float b[] : original 3*3 orientation
*	@return void
*/

void apply2(float a[], float b[][3])
{
	int i, j, k;

	a[0] = -b[0][1];
	a[1] = -b[1][1];
	a[2] = -b[2][1];
	a[3] = b[0][0];
	a[4] = b[1][0];
	a[5] = b[2][0];
	a[6] = b[0][2];
	a[7] = b[1][2];
	a[8] = b[2][2];
}




/**
* HipOrientation()
*	@This Fuction Provides The Final compensated Update for the Hip Orientation
*	@param CurrentHipOrientation[][3]: The final hip orientation
*	@param float IntitialRotationLocalHip[][3], this is the information of the initial frame for Hip joint
*	@param float IntitialRotationGloballHip[][3],  this is the information of the initial frame for Hip joint
*	@param float IntitialRotationLocalKnee[][3], this is the information of the initial frame for Knee joint
*	@param float IntitialRotationGlobalKnee[][3], this is the information of the initial frame for Knee joint
*	@param float NodHipYaw , .......... Hip and Knee Nods Inputs
*  @param float StretchSensorKnee    Stretch Sensor data for Knee
*	@return void
*/
void HipOrientation(float CurrentHipOrientation[][3], float IntitialRotationLocalHip[][3], float IntitialRotationGloballHip[][3], float IntitialRotationLocalKnee[][3], float IntitialRotationGlobalKnee[][3], float NodHipYaw, float NodHipPitch, float NodHipRoll, float NodKneeYaw, float NodKneePitch, float NodKneeRoll, float StretchSensorKnee)

{
	//Intermediate arrays until achive final orienation for hip and knee, they are Taged with F (forward rotation) and B (Backward rotation) and are numbered consecutively

	float HipF1[3][3] = {};
	float HipF2[3][3] = {};
	float HipF3[3][3] = {};
	float HipF4[3][3] = {};
	float HipF5[3][3] = {};
	float HipF6[3][3] = {};
	float HipF7[3][3] = {};

	float HipB1[3][3] = {};
	float HipB2[3][3] = {};
	float HipB3[3][3] = {};
	float HipB4[3][3] = {};


	float KneeF1[3][3] = {};
	float KneeF2[3][3] = {};
	float KneeF3[3][3] = {};
	float KneeF4[3][3] = {};

	float KneeB1[3][3] = {};
	float KneeB2[3][3] = {};
	float KneeB3[3][3] = {};
	float KneeB4[3][3] = {};



	float OrientationError = 0;
	float CompensationAngle = 0;



	float CompensationRotationKnee[3][3] = {};
	float CompensationRotationHip[3][3] = {};
	float CurrentKneeOrientation[3][3] = {};

	vec pitchHip, pitchKnee, NcrossHipKnee, RollHip, RollKnee, NcrossHipKneeRoll, fr, YawHip, YawKnee;






	/////////// Initial Frame Adjustments ///////////////////
	RotationGlobal(HipF1, NodHipYaw, NodHipPitch, NodHipRoll);
	RotationGlobal(KneeF1, NodKneeYaw, NodKneePitch, NodKneeRoll);

	RotationLocal(HipB1, NodHipYaw, NodHipPitch, NodHipRoll);
	RotationLocal(KneeB1, NodKneeYaw, NodKneePitch, NodKneeRoll);

	multi(HipF1, IntitialRotationLocalHip, HipF2);
	multi(KneeF1, IntitialRotationLocalKnee, KneeF2);

	multi(IntitialRotationGloballHip, HipB1, HipB2);
	multi(IntitialRotationGlobalKnee, KneeB1, KneeB2);



	///////////// Pitch Compensation Step ///////////////////
	pitchHip.x = HipB2[0][2];
	pitchHip.y = HipB2[1][2];
	pitchHip.z = HipB2[2][2];

	pitchKnee.x = KneeB2[0][2];
	pitchKnee.y = KneeB2[1][2];
	pitchKnee.z = KneeB2[2][2];

	// rotation axis for pitch compensation
	NcrossHipKnee = Ncross(pitchHip, pitchKnee);
	OrientationError = HipB2[0][2] * KneeB2[0][2] + HipB2[1][2] * KneeB2[1][2] + HipB2[2][2] * KneeB2[2][2];

	// Finding Pitch compensation Angle
	CompensationAngle = acos(OrientationError> 1.00 ? 1 : OrientationError);

	// Building Pitch compensation rotation matrices
	RotationVector(CompensationRotationHip, NcrossHipKnee, -0.5* CompensationAngle);
	RotationVector(CompensationRotationKnee, NcrossHipKnee, +0.5* CompensationAngle);

	// Applying Pitch Compensation 
	multi(KneeF2, CompensationRotationKnee, KneeF3);
	multi(HipF2, CompensationRotationHip, HipF3);
	multi(CompensationRotationHip, KneeB2, KneeB3);
	multi(CompensationRotationKnee, HipB2, HipB3);






	

	
	///////////// Knee 180 degree Constriant ///////////////////




	RollHip.x = HipB3[0][0];
	RollHip.y = HipB3[1][0];
	RollHip.z = HipB3[2][0];

	YawKnee.x = KneeB3[0][1];
	YawKnee.y = KneeB3[1][1];
	YawKnee.z = KneeB3[2][1];

	YawHip.x = HipB3[0][1];
	YawHip.y = HipB3[1][1];
	YawHip.z = HipB3[2][1];

	NcrossHipKneeRoll = Ncross(YawHip, YawKnee);
	if (dot(RollHip, YawKnee) < 0) /// this case when not obey 180 degree constraint
	{

		OrientationError = HipB3[0][1] * KneeB3[0][1] + HipB3[1][1] * KneeB3[1][1] + HipB3[2][1] * KneeB3[2][1];

		
		
		
		// Finding yaw compensation Angle
		if (acos(OrientationError > 1.00 ? 1 : OrientationError) > (PI / 2))
		{
			CompensationAngle = acos(OrientationError> 1.00 ? 1 : OrientationError) - PI;
		}
		else {
			CompensationAngle = acos(OrientationError > 1.00 ? 1 : OrientationError);
		}

		// Building yaw compensation rotation matrices
		RotationVector(CompensationRotationHip, NcrossHipKneeRoll, +0.5* CompensationAngle);
		RotationVector(CompensationRotationKnee, NcrossHipKneeRoll, -0.5* CompensationAngle);

		// Applying yaw Compensation 
		multi(KneeF3, CompensationRotationHip, KneeF4);
		multi(HipF3, CompensationRotationKnee, HipF4);
		multi(CompensationRotationKnee, KneeB3, KneeB4);
		multi(CompensationRotationHip, HipB3, HipB4);

	}
	else  /// this case when obey 180 degree constraint just to improve knee angle estimation
	{

		OrientationError = HipB3[0][1] * KneeB3[0][1] + HipB3[1][1] * KneeB3[1][1] + HipB3[2][1] * KneeB3[2][1];

		// Finding Pitch compensation Angle
		CompensationAngle = 0;

		// Building Pitch compensation rotation matrices
		RotationVector(CompensationRotationHip, NcrossHipKneeRoll, +0.5* CompensationAngle);
		RotationVector(CompensationRotationKnee, NcrossHipKneeRoll, -0.5* CompensationAngle);

		// Applying Pitch Compensation 
		multi(KneeF3, CompensationRotationHip, KneeF4);
		multi(HipF3, CompensationRotationKnee, HipF4);
		multi(CompensationRotationKnee, KneeB3, KneeB4);
		multi(CompensationRotationHip, HipB3, HipB4);

	}


	////////////////// setting to Final Body orientation ///////////////////////////////

	fr.x = HipB4[0][0];
	fr.y = HipB4[1][0];
	fr.z = HipB4[2][0];

	rvector(CompensationRotationHip, fr, 3.1415 / 2);

	multi(HipF4, CompensationRotationHip, HipF5);

	fr.x = HipB4[0][2];
	fr.y = HipB4[1][2];
	fr.z = HipB4[2][2];

	rvector(CompensationRotationHip, fr, 3.1415 / 2);

	multi(HipF5, CompensationRotationHip, HipF6);

	fr.x = HipB4[0][1];
	fr.y = HipB4[1][1];
	fr.z = HipB4[2][1];

	rvector(CompensationRotationHip, fr, 3.1415);

	multi(HipF6, CompensationRotationHip, HipF7);

	fr.x = 1;
	fr.y = 0;
	fr.z = 0;

	rvector(CompensationRotationHip, fr, -3.1415 / 2);

	multi(HipF7, CompensationRotationHip, CurrentHipOrientation);



}



/**
* KneeOrientation()
*	@This Fuction Provides The Final Compensated Update for the Hip Orientation
*	@param CurrentKneeOrientation[][3]: The final Knee orientation
*	@param float IntitialRotationLocalHip[][3], this is the information of the initial frame for Hip joint
*	@param float IntitialRotationGloballHip[][3],  this is the information of the initial frame for Hip joint
*	@param float IntitialRotationLocalKnee[][3], this is the information of the initial frame for Knee joint
*	@param float IntitialRotationGlobalKnee[][3], this is the information of the initial frame for Knee joint
*	@param float NodHipYaw , .......... Hip and Knee Nods Inputs
*   @param float StretchSensorKnee   Stretch Sensor data for Knee
*	@return void
*/
void KneeOrientation(float CurrentKneeOrientation[][3], float IntitialRotationLocalHip[][3], float IntitialRotationGloballHip[][3], float IntitialRotationLocalKnee[][3], float IntitialRotationGlobalKnee[][3], float NodHipYaw, float NodHipPitch, float NodHipRoll, float NodKneeYaw, float NodKneePitch, float NodKneeRoll, float StretchSensorKnee)

{

	//Intermediate arrays until achive final orienation for hip and knee, they are Taged with F (forward rotation) and B (Backward rotation) and are numbered consecutively


	float HipF1[3][3] = {};
	float HipF2[3][3] = {};
	float HipF3[3][3] = {};
	float HipF4[3][3] = {};

	float HipB1[3][3] = {};
	float HipB2[3][3] = {};
	float HipB3[3][3] = {};
	float HipB4[3][3] = {};


	float KneeF1[3][3] = {};
	float KneeF2[3][3] = {};
	float KneeF3[3][3] = {};
	float KneeF4[3][3] = {};
	float KneeF5[3][3] = {};
	float KneeF6[3][3] = {};
	float KneeF7[3][3] = {};

	float KneeB1[3][3] = {};
	float KneeB2[3][3] = {};
	float KneeB3[3][3] = {};
	float KneeB4[3][3] = {};



	float OrientationError = 0;
	float CompensationAngle = 0;



	float CompensationRotationKnee[3][3] = {};
	float CompensationRotationHip[3][3] = {};
	float CurrentHipOrientation[3][3] = {};

	vec pitchHip, pitchKnee, NcrossHipKnee, RollHip, RollKnee, NcrossHipKneeRoll, fr , YawKnee , YawHip;



	/////////// Initial Frame Adjustments ///////////////////
	RotationGlobal(HipF1, NodHipYaw, NodHipPitch, NodHipRoll);
	RotationGlobal(KneeF1, NodKneeYaw, NodKneePitch, NodKneeRoll);

	RotationLocal(HipB1, NodHipYaw, NodHipPitch, NodHipRoll);
	RotationLocal(KneeB1, NodKneeYaw, NodKneePitch, NodKneeRoll);

	multi(HipF1, IntitialRotationLocalHip, HipF2);
	multi(KneeF1, IntitialRotationLocalKnee, KneeF2);

	multi(IntitialRotationGloballHip, HipB1, HipB2);
	multi(IntitialRotationGlobalKnee, KneeB1, KneeB2);



	///////////// Pitch Compensation Step ///////////////////
	pitchHip.x = HipB2[0][2];
	pitchHip.y = HipB2[1][2];
	pitchHip.z = HipB2[2][2];

	pitchKnee.x = KneeB2[0][2];
	pitchKnee.y = KneeB2[1][2];
	pitchKnee.z = KneeB2[2][2];

	// rotation axix for pitch compensation
	NcrossHipKnee = Ncross(pitchHip, pitchKnee);
	OrientationError = HipB2[0][2] * KneeB2[0][2] + HipB2[1][2] * KneeB2[1][2] + HipB2[2][2] * KneeB2[2][2];

	// Finding Pitch compensation Angle
	CompensationAngle = acos(OrientationError> 1.00 ? 1 : OrientationError);

	// Building Pitch compensation rotation matrices
	RotationVector(CompensationRotationHip, NcrossHipKnee, -0.5* CompensationAngle);
	RotationVector(CompensationRotationKnee, NcrossHipKnee, +0.5* CompensationAngle);

	// Applying Pitch Compensation 
	multi(KneeF2, CompensationRotationKnee, KneeF3);
	multi(HipF2, CompensationRotationHip, HipF3);
	multi(CompensationRotationHip, KneeB2, KneeB3);
	multi(CompensationRotationKnee, HipB2, HipB3);




	///////////// Knee 180 degree Constriant ///////////////////

	
	RollHip.x = HipB3[0][0];
	RollHip.y = HipB3[1][0];
	RollHip.z = HipB3[2][0];

	YawKnee.x = KneeB3[0][1];
	YawKnee.y = KneeB3[1][1];
	YawKnee.z = KneeB3[2][1];

	YawHip.x = HipB3[0][1];
	YawHip.y = HipB3[1][1];
	YawHip.z = HipB3[2][1];

	NcrossHipKneeRoll = Ncross(YawHip, YawKnee);
	if (dot(RollHip, YawKnee) < 0 ) /// this case when not obey 180 degree constraint
	{

		OrientationError = HipB3[0][1] * KneeB3[0][1] + HipB3[1][1] * KneeB3[1][1] + HipB3[2][1] * KneeB3[2][1];




		// Finding yaw compensation Angle
		if (acos(OrientationError > 1.00 ? 1 : OrientationError) > (PI / 2))
		{
			CompensationAngle = acos(OrientationError> 1.00 ? 1 : OrientationError) - PI;
		}
		else {
			CompensationAngle = acos(OrientationError > 1.00 ? 1 : OrientationError);
		}

		// Building yaw compensation rotation matrices
		RotationVector(CompensationRotationHip, NcrossHipKneeRoll, +0.5* CompensationAngle);
		RotationVector(CompensationRotationKnee, NcrossHipKneeRoll, -0.5* CompensationAngle);

		// Applying yaw Compensation 
		multi(KneeF3, CompensationRotationHip, KneeF4);
		multi(HipF3, CompensationRotationKnee, HipF4);
		multi(CompensationRotationKnee, KneeB3, KneeB4);
		multi(CompensationRotationHip, HipB3, HipB4);

	}
	else  /// this case when obey 180 degree constraint just to improve knee angle estimation
	{

		OrientationError = HipB3[0][1] * KneeB3[0][1] + HipB3[1][1] * KneeB3[1][1] + HipB3[2][1] * KneeB3[2][1];
		printf(" angle3:%f\n ", acos(OrientationError > 1.00 ? 1 : OrientationError) * 180 / PI);

		// Finding Pitch compensation Angle
		CompensationAngle = 0;

		// Building Pitch compensation rotation matrices
		RotationVector(CompensationRotationHip, NcrossHipKneeRoll, +0.5* CompensationAngle);
		RotationVector(CompensationRotationKnee, NcrossHipKneeRoll, -0.5* CompensationAngle);

		// Applying Pitch Compensation 
		multi(KneeF3, CompensationRotationHip, KneeF4);
		multi(HipF3, CompensationRotationKnee, HipF4);
		multi(CompensationRotationKnee, KneeB3, KneeB4);
		multi(CompensationRotationHip, HipB3, HipB4);

	}






	////////////////// setting to Final Body orientation ///////////////////////////////

	fr.x = KneeB4[0][0];
	fr.y = KneeB4[1][0];
	fr.z = KneeB4[2][0];

	rvector(CompensationRotationKnee, fr, 3.1415 / 2);

	multi(KneeF4, CompensationRotationKnee, KneeF5);

	fr.x = KneeB4[0][2];
	fr.y = KneeB4[1][2];
	fr.z = KneeB4[2][2];

	rvector(CompensationRotationKnee, fr, 3.1415 / 2);

	multi(KneeF5, CompensationRotationKnee, KneeF6);

	fr.x = KneeB4[0][1];
	fr.y = KneeB4[1][1];
	fr.z = KneeB4[2][1];

	rvector(CompensationRotationKnee, fr, 3.1415);

	multi(KneeF6, CompensationRotationKnee, KneeF7);

	fr.x = 1;
	fr.y = 0;
	fr.z = 0;

	rvector(CompensationRotationKnee, fr, - 3.1415 / 2);

	multi(KneeF7, CompensationRotationKnee, CurrentKneeOrientation);

}














/**
* Ncross()
* @This function Provides a Normlized cross product of two vectors
* @return void
*/

vec Ncross(vec vector1, vec vector2)
{

	vec result;
	result.x = vector1.y *vector2.z - vector2.y * vector1.z;
	result.y = vector1.z *vector2.x - vector2.z * vector1.x;
	result.z = vector1.x *vector2.y - vector2.x *vector1.y;
	result.l = sqrt(result.x*result.x + result.y * result.y + result.z * result.z);

	//making it a unit vector
	result.x /= result.l;
	result.y /= result.l;
	result.z /= result.l;
	return result;
}




/**
* RotationLocal()
* @ This Performs Rotation From Local Coordinate System To Global
* @param float a[][3], Outputted Rotation Matrix
* @param float yaw, float pitch, float roll, Euler Angles
* @return void
*/// 
void RotationLocal(float a[][3], float yaw, float pitch, float roll)
{
	a[0][0] = cos(pitch)*cos(yaw);
	a[1][0] = cos(pitch)*sin(yaw);
	a[2][0] = -sin(pitch);
	a[0][1] = -cos(roll) *sin(yaw) + cos(yaw) *sin(pitch)*sin(roll);
	a[1][1] = sin(roll)*sin(yaw)*sin(pitch) + cos(yaw) * cos(roll);
	a[2][1] = cos(pitch)*sin(roll);
	a[0][2] = (sin(roll) *sin(yaw) + cos(yaw) *sin(pitch)*cos(roll));
	a[1][2] = (cos(roll)*sin(yaw)*sin(pitch) - cos(yaw) * sin(roll));
	a[2][2] = (cos(pitch)*cos(roll));

}

/**
* RotationGlobal()
* @ This Performs Rotation From Global Coordinate System To local
* @param float a[][3], Outputted Rotation Matrix
* @param float yaw, float pitch, float roll, Euler Angles
* @return void
*/// 
void RotationGlobal(float a[][3], float yaw, float pitch, float roll)
{
	a[0][0] = cos(pitch)*cos(yaw);
	a[1][0] = (sin(roll)*cos(yaw)*sin(pitch) - sin(yaw) * cos(roll));
	a[2][0] = (sin(roll) *sin(yaw) + cos(yaw) *sin(pitch)*cos(roll));
	a[0][1] = cos(pitch)*sin(yaw);
	a[1][1] = sin(roll)*sin(yaw)*sin(pitch) + cos(yaw) * cos(roll);
	a[2][1] = (cos(roll)*sin(yaw)*sin(pitch) - cos(yaw) * sin(roll));
	a[0][2] = -sin(pitch);
	a[1][2] = cos(pitch)*sin(roll);
	a[2][2] = (cos(pitch)*cos(roll));

}


/**
* RotationVector()
* @It provides a rotation matrix around an arbitary vector with desired angles
* @param float a[][3], The output rotation matrix
* @param vec u, arbitary unit vector
* @param flaot t, desired angle of rotation
* @return void
*/
void RotationVector(float a[][3], vec u, float t)
{
	a[0][0] = cos(t) + u.x*u.x* (1 - cos(t));
	a[1][0] = u.x*u.y* (1 - cos(t)) + u.z * sin(t);
	a[2][0] = u.x*u.z* (1 - cos(t)) - u.y * sin(t);
	a[0][1] = u.x*u.y* (1 - cos(t)) - u.z * sin(t);
	a[1][1] = cos(t) + u.y*u.y* (1 - cos(t));;
	a[2][1] = u.z*u.y* (1 - cos(t)) + u.x * sin(t);
	a[0][2] = u.x*u.z* (1 - cos(t)) + u.y * sin(t);
	a[1][2] = u.z*u.y* (1 - cos(t)) - u.x * sin(t);
	a[2][2] = cos(t) + u.z*u.z* (1 - cos(t));

}







// dot() 
// returns dot product of two vectors
// params vector1 and vector2
// return float vector1.vector2
float dot(vec vector1, vec vector2)
{

	return vector1.x *vector2.x + vector1.y *vector2.y + vector1.z *vector2.z;
}




/**
* rb()  
* @ This Performs Rotation From Local Coordinate System To Global
* @param float a[][3], Outputted Rotation Matrix
* @param float yaw, float pitch, float roll, Euler Angles
* @return void
*/// 

void rb(float a[][3], float yaw, float pitch, float roll)
{
	a[0][0] = cos(pitch)*cos(yaw);
	a[1][0] = cos(pitch)*sin(yaw);
	a[2][0] = -sin(pitch);
	a[0][1] = -cos(roll) *sin(yaw) + cos(yaw) *sin(pitch)*sin(roll);
	a[1][1] = sin(roll)*sin(yaw)*sin(pitch) + cos(yaw) * cos(roll);
	a[2][1] = cos(pitch)*sin(roll);
	a[0][2] = (sin(roll) *sin(yaw) + cos(yaw) *sin(pitch)*cos(roll));
	a[1][2] = (cos(roll)*sin(yaw)*sin(pitch) - cos(yaw) * sin(roll));
	a[2][2] = (cos(pitch)*cos(roll));

}





/**
* ro()
* @ This Performs Rotation From global Coordinate System To local
* @param float a[][3], Outputted Rotation Matrix
* @param float yaw, float pitch, float roll, Euler Angles
* @return void
*/// 

void ro(float a[][3], float yaw, float pitch, float roll)
{
	a[0][0] = cos(pitch)*cos(yaw);
	a[1][0] = (sin(roll)*cos(yaw)*sin(pitch) - sin(yaw) * cos(roll));
	a[2][0] = (sin(roll) *sin(yaw) + cos(yaw) *sin(pitch)*cos(roll));
	a[0][1] = cos(pitch)*sin(yaw);
	a[1][1] = sin(roll)*sin(yaw)*sin(pitch) + cos(yaw) * cos(roll);
	a[2][1] = (cos(roll)*sin(yaw)*sin(pitch) - cos(yaw) * sin(roll));
	a[0][2] = -sin(pitch);
	a[1][2] = cos(pitch)*sin(roll);
	a[2][2] = (cos(pitch)*cos(roll));

}





//		/**
//	* RotationVector()
//	* @It provides a rotation matrix around an arbitary vector with desired angles
//	* @param float a[][3], The output rotation matrix
//	* @param vec u, arbitary unit vector
//	* @param flaot t, desired angle of rotation
//	* @return void
//	*/
void rvector(float a[][3], vec u, float t)
{
	a[0][0] = cos(t) + u.x*u.x* (1 - cos(t));
	a[1][0] = u.x*u.y* (1 - cos(t)) + u.z * sin(t);
	a[2][0] = u.x*u.z* (1 - cos(t)) - u.y * sin(t);
	a[0][1] = u.x*u.y* (1 - cos(t)) - u.z * sin(t);
	a[1][1] = cos(t) + u.y*u.y* (1 - cos(t));;
	a[2][1] = u.z*u.y* (1 - cos(t)) + u.x * sin(t);
	a[0][2] = u.x*u.z* (1 - cos(t)) + u.y * sin(t);
	a[1][2] = u.z*u.y* (1 - cos(t)) - u.x * sin(t);
	a[2][2] = cos(t) + u.z*u.z* (1 - cos(t));

}


// multi()
// it performs 3*3 multiplication between 2 matrices
// Parameters: a , b , a *b  = c


void multi(float a[][3], float b[][3], float c[][3])
{
	int i, j, k;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{


			c[i][j] = a[i][0] * b[0][j] + a[i][1] * b[1][j] + a[i][2] * b[2][j];

		}
	}
}