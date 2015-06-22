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
void QuatToMat(quaternion q, float m[][3]);
quaternion MatToQuat(float m[][3]);










int __cdecl main(int argc, char **argv)
{
	OpenSpatialServiceController* controller;
	controller = new OpenSpatialServiceController;
	//ExampleDelegate* del = new ExampleDelegate;
	ExampleDelegate hi;
	controller->setDelegate(&hi);
	BOOL continuer = true;
	
	
		std::string input;
		controller->subscribeToPose6D(controller->names.at(0));
		controller->subscribeToPose6D(controller->names.at(1));

			printf("\nfirst controler name %s and second %s: ", controller->names.at(0).c_str(), controller->names.at(1).c_str());
		
		
			float angle, angle2, angle3 = 0, angle4 = 0, angle6 = 0, angle5 = 0;
			float lnod1yaw = 0;
			float lnod2yaw = 0;
			float param, param2, param3 = 0, param4 = 0, param5, param6, paramx, paramy, paramz;
			float lnod1roll = 0;
			float lnod2roll = 0;
			float lnod1pitch = 0;
			float lnod2pitch = 0;
			float l1pitch = 0;
			float l2pitch = 0;
			float rp = 0;
			vec pitch2, pitch1, pitch3, crossp12, fr = {0};
			quaternion q1 = { 0 }, q2 = { 0 };
			float rb1[3][3] = {};
			float rb2[3][3] = {};
			float r[3][3] = {}, r2[3][3] = {}, r3[3][3] = {};
			float rb3[3][3] = {};
			float rb4[3][3] = {};
			float rb5[3][3] = {};
			float rb6[3][3] = {};
			float rbi[3][3] = {};
			float rbi2[3][3] = {};
			float ro1[3][3] = {};
			float ro2[3][3] = {};
			float ro3[3][3] = {};
			float ro4[3][3] = {};
			float ro5[3][3] = {};
			float ro6[3][3] = {};
			float ro7[3][3] = {};
			float ro8[3][3] = {};
			float ro9[3][3] = {};
			float ro10[3][3] = {};
			float roi[3][3] = {};
			float roi2[3][3] = {};


			
			

			
			
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
			float rx[9] = {}, rx2[9] = {}, rx3[9] = {}, rx4[9] = {};
			int hii = 44;
			int hiii = 232323;
			std::stringstream strm;


		    
			
			rb(rbi, hi.nod1.yaw, hi.nod1.pitch, hi.nod1.roll);
			rb(rbi2, hi.nod2.yaw, hi.nod2.pitch, hi.nod2.roll);
			ro(roi, hi.nod1.yaw, hi.nod1.pitch, hi.nod1.roll);
			ro(roi2, hi.nod2.yaw, hi.nod2.pitch, hi.nod2.roll);
			
					while  (true)
						{
							
							   
							HipOrientation(ro3, rbi, roi, rbi2, roi2, hi.nod1.yaw, hi.nod1.pitch, hi.nod1.roll, hi.nod2.yaw, hi.nod2.pitch, hi.nod2.roll,1);
							KneeOrientation(ro4, rbi, roi, rbi2, roi2, hi.nod1.yaw, hi.nod1.pitch, hi.nod1.roll, hi.nod2.yaw, hi.nod2.pitch, hi.nod2.roll,1);
								
							
							q1 = MatToQuat(ro3);
							q2 = MatToQuat(ro4);
							QuatToMat(q1, ro5);
							QuatToMat(q2, ro6);

							apply2(rx, ro5);
							apply2(rx2, ro6);


							apply2(rx3, ro3);
							apply2(rx4, ro4);


				
								//printf("%f, %f, %f, %f \n", rx3[4], rx4[5],rx3[6], rx4[7]);
				
				
				
				
				
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

								for (int i = 0; i < 9; i++)
								{

									strm << rx3[i] << "\n";
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

									strm << rx4[i] << "\n";
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




vec cross(vec vector1, vec vector2)
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


float dot(vec vector1, vec vector2)
{

	return vector1.x *vector2.x + vector1.y *vector2.y + vector1.z *vector2.z;
}


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

void ro(float a[][3], float yaw, float pitch, float roll)
{
	a[0][0] = cos(pitch)*cos(yaw);
	a[1][0] = (sin(roll)*cos(yaw)*sin(pitch) - sin(yaw) * cos(roll));
	a[2][0] =  (sin(roll) *sin(yaw) + cos(yaw) *sin(pitch)*cos(roll));
	a[0][1] = cos(pitch)*sin(yaw);
	a[1][1] = sin(roll)*sin(yaw)*sin(pitch) + cos(yaw) * cos(roll);
	a[2][1] = (cos(roll)*sin(yaw)*sin(pitch) - cos(yaw) * sin(roll));
	a[0][2] = -sin(pitch);
	a[1][2] = cos(pitch)*sin(roll);
	a[2][2] = (cos(pitch)*cos(roll));

}


void rvector(float a[][3], vec u, float t)
{
	a[0][0] = cos(t) + u.x*u.x* (1-cos(t));
	a[1][0] = u.x*u.y* (1 - cos(t)) + u.z * sin(t);
	a[2][0] = u.x*u.z* (1 - cos(t)) - u.y * sin(t);
	a[0][1] = u.x*u.y* (1 - cos(t)) - u.z * sin(t);
	a[1][1] = cos(t) + u.y*u.y* (1 - cos(t));;
	a[2][1] = u.z*u.y* (1 - cos(t)) + u.x * sin(t);
	a[0][2] = u.x*u.z* (1 - cos(t)) + u.y * sin(t);
	a[1][2] = u.z*u.y* (1 - cos(t)) - u.x * sin(t);
	a[2][2] = cos(t) + u.z*u.z* (1 - cos(t));

}

void multi (float a[][3], float b[][3], float c[][3])
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



void apply(float a[], float b[][3])
{
	int i, j, k;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{


			a[3*i+j] = b[i][j];

		}
	}
}



void apply2(float a[], float b[][3])
{
	int i, j, k;
	
	a[0] = -b[0][1];
	a[1] =- b[1][1];
	a[2] = -b[2][1];
	a[3] =  b[0][0];
	a[4] =  b[1][0];
	a[5] =  b[2][0];
	a[6] =  b[0][2];
	a[7] =  b[1][2];
	a[8] =  b[2][2];
}

void apply3(float a[], float b[][3])
{
	int i, j, k;

	a[0] = -b[0][1];
	a[1] =-b[1][1];
	a[2] = -b[2][1];
	a[3] = -b[0][0];
	a[4] = -b[1][0];
	a[5] = -b[2][0];
	a[6] = -b[0][2];
	a[7] = -b[1][2];
	a[8] = -b[2][2];
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

	vec pitchHip, pitchKnee, NcrossHipKnee, RollHip, RollKnee, NcrossHipKneeRoll , fr;


	



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

	RollKnee.x = KneeB3[0][0];
	RollKnee.y = KneeB3[1][0];
	RollKnee.z = KneeB3[2][0];

	pitchHip.x = HipB3[0][2];
	pitchHip.y = HipB3[1][2];
	pitchHip.z = HipB3[2][2];

	NcrossHipKneeRoll = Ncross(RollHip, RollKnee);
	if (dot(NcrossHipKneeRoll, pitchHip) > 0) /// this case when not obey 180 degree constraint
	{

		OrientationError = HipB2[0][0] * KneeB2[0][0] + HipB2[1][0] * KneeB2[1][0] + HipB2[2][0] * KneeB2[2][0];

		// Finding yaw compensation Angle
		CompensationAngle = acos(OrientationError> 1.00 ? 1 : OrientationError);
		
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

		OrientationError = HipB2[0][0] * KneeB2[0][0] + HipB2[1][0] * KneeB2[1][0] + HipB2[2][0] * KneeB2[2][0];

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

	fr.x = 0;
	fr.y = 1;
	fr.z = 0;

	rvector(CompensationRotationHip, fr, 3 * 3.1415 / 2);

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

	vec pitchHip, pitchKnee, NcrossHipKnee, RollHip, RollKnee, NcrossHipKneeRoll,fr;


	
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
	multi(KneeF2,CompensationRotationKnee, KneeF3);
    multi(HipF2, CompensationRotationHip, HipF3);
	multi(CompensationRotationHip, KneeB2, KneeB3);
	multi(CompensationRotationKnee, HipB2, HipB3);
	

	

	///////////// Knee 180 degree Constriant ///////////////////
	
	RollHip.x = HipB3[0][0];
	RollHip.y = HipB3[1][0];
	RollHip.z = HipB3[2][0];

	RollKnee.x = KneeB3[0][0];
	RollKnee.y = KneeB3[1][0];
	RollKnee.z = KneeB3[2][0];

	pitchHip.x = HipB3[0][2];
	pitchHip.y = HipB3[1][2];
	pitchHip.z = HipB3[2][2];

	NcrossHipKneeRoll = Ncross(RollHip, RollKnee);
	if (dot(NcrossHipKneeRoll, pitchHip) > 0) /// this case when not obey 180 degree constraint
	{

		OrientationError = HipB3[0][0] * KneeB3[0][0] + HipB3[1][0] * KneeB3[1][0] + HipB3[2][0] * KneeB3[2][0];

		// Finding yaw compensation Angle
		CompensationAngle = acos(OrientationError> 1.00 ? 1 : OrientationError);

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

		OrientationError = HipB3[0][0] * KneeB3[0][0] + HipB3[1][0] * KneeB3[1][0] + HipB3[2][0] * KneeB3[2][0];

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

	fr.x = 0;
	fr.y = 1;
	fr.z = 0;

	rvector(CompensationRotationKnee, fr, 3 * 3.1415 / 2);

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



/**
* SIGN()
* @It provides sign of an input
* @param float x is the input
* @return float sign of the x
*/
inline float SIGN(float x) { return (x >= 0.0f) ? +1.0f : -1.0f; }




/**
* MatToQuat
* @It converts a Matrix to a Quatrenion
* @param q is the transformed quatrenion
* @param float m[][3] is the original 3*3 matrix
* @return void
* @http://www.cg.info.hiroshima-cu.ac.jp/~miyazaki/knowledge/teche52.html
*/
quaternion MatToQuat(float m[][3])
{
	quaternion q;
	q.w = (m[0][0] + m[1][1] + m[2][2] + 1.0f) / 4.0f;
	q.x = (m[0][0] - m[1][1] - m[2][2] + 1.0f) / 4.0f;
	q.y = (-m[0][0] + m[1][1] - m[2][2] + 1.0f) / 4.0f;
	q.z = (-m[0][0] - m[1][1] + m[2][2] + 1.0f) / 4.0f;
	if (q.w < 0.0f) q.w = 0.0f;
	if (q.x < 0.0f) q.x = 0.0f;
	if (q.y < 0.0f) q.y = 0.0f;
	if (q.z < 0.0f) q.z = 0.0f;
	q.w = sqrt(q.w);
	q.x = sqrt(q.x);
	q.y = sqrt(q.y);
	q.z = sqrt(q.z);
	if (q.w >= q.x && q.w >= q.y && q.w >= q.z) {
		q.w *= +1.0f;
		q.x *= SIGN(m[2][1] - m[1][2]);
		q.y *= SIGN(m[0][2] - m[2][0]);
		q.z *= SIGN(m[1][0] - m[0][1]);
	}
	else if (q.x >= q.w && q.x >= q.y && q.x >= q.z) {
		q.w *= SIGN(m[2][1] - m[1][2]);
		q.x *= +1.0f;
		q.y *= SIGN(m[1][0] + m[0][1]);
		q.z *= SIGN(m[0][2] + m[2][0]);
	}
	else if (q.y >= q.w && q.y >= q.x && q.y >= q.z) {
		q.w *= SIGN(m[0][2] - m[2][0]);
		q.x *= SIGN(m[1][0] + m[0][1]);
		q.y *= +1.0f;
		q.z *= SIGN(m[2][1] + m[1][2]);
	}
	else if (q.z >= q.w && q.z >= q.x && q.z >= q.y) {
		q.w *= SIGN(m[1][0] - m[0][1]);
		q.x *= SIGN(m[2][0] + m[0][2]);
		q.y *= SIGN(m[2][1] + m[1][2]);
		q.z *= +1.0f;
	}
	else {
		printf("coding error\n");
	}
	float r = sqrt(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
	q.w /= r;
	q.x /= r;
	q.y /= r;
	q.z /= r;
	return q;
}






/**
* QuatToMat
* @It converts a Quatrenion to a Matrix
* @param q is the transformed quatrenion
* @param float m[][3] is the original 3*3 matrix
* @return void
*/
void QuatToMat(quaternion q, float m[][3])
{
	m[0][0] = 1 - 2 * (q.y*q.y) - 2 * (q.z*q.z);
	m[0][1] = 2 * (q.x*q.y) - 2 * (q.z*q.w);
	m[0][2] = 2 * (q.x*q.z) + 2 * (q.y*q.w);
	m[1][0] = 2 * (q.x*q.y) + 2 * (q.z*q.w);
	m[1][1] = 1 - 2 * (q.x*q.x) - 2 * (q.z*q.z);
	m[1][2] = 2 * (q.y*q.z) - 2 * (q.x*q.w);
	m[2][0] = 2 * (q.x*q.z) - 2 * (q.y*q.w);
	m[2][1] = 2 * (q.y*q.z) + 2 * (q.x*q.w);
	m[2][2] = 1 - 2 * (q.y*q.y) - 2 * (q.x*q.x);

}
