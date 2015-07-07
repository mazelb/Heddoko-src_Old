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
#include <time.h>
#include <chrono>

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

void UpArOrientation(float CurrentUpArOrientation[][3], float IntitialRotationLocalUpAr[][3], float IntitialRotationGloballUpAr[][3], float IntitialRotationLocalLoAr[][3], float IntitialRotationGlobalLoAr[][3], float NodUpArYaw, float NodUpArPitch, float NodUpArRoll, float NodLoArYaw, float NodLoArPitch, float NodLoArRoll, float StretchSensorLoAr);
void LoArOrientation(float CurrentLoArOrientation[][3], float IntitialRotationLocalUpAr[][3], float IntitialRotationGloballUpAr[][3], float IntitialRotationLocalLoAr[][3], float IntitialRotationGlobalLoAr[][3], float NodUpArYaw, float NodUpArPitch, float NodUpArRoll, float NodLoArYaw, float NodLoArPitch, float NodLoArRoll, float StretchSensorLoAr , int key);

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
	//controller->setBufferPackets(true);
	while (!controller->setup)
	{
		Sleep(100);
	}
	
		            printf("\nfirst controler name %s and second %s: ", controller->names.at(0).c_str(), controller->names.at(1).c_str());
					controller->controlService(SUBSCRIBE_TO_POSE6D, controller->names.at(0));
					controller->controlService(SUBSCRIBE_TO_POSE6D, controller->names.at(1));
					
					typedef std::chrono::high_resolution_clock Clock;
					typedef std::chrono::milliseconds milliseconds;

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
					vec pitch2, pitch1, pitch3, crossp12, fr = { 0 };
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






					float rx[9] = {}, rx2[9] = {}, rx3[9] = {}, rx4[9] = {}, rx5[9] = {}, rx6[9] = {};
					int hii = 44;
					int hiii = 232323;
					int counter = 1;
					std::stringstream strm;




					rb(rbi, hi.nod1.yaw, hi.nod1.pitch, hi.nod1.roll);
					rb(rbi2, hi.nod2.yaw, hi.nod2.pitch, hi.nod2.roll);
					ro(roi, hi.nod1.yaw, hi.nod1.pitch, hi.nod1.roll);
					ro(roi2, hi.nod2.yaw, hi.nod2.pitch, hi.nod2.roll);
					Clock::time_point t0 = Clock::now();
					Clock::time_point t1 = Clock::now();
					int key = 0;
					float time_difference = 0;

					while (true)
					{

						
						
						t1 = Clock::now();
						if (counter == 4000)
						{
							counter = 0;
							milliseconds ms = std::chrono::duration_cast<milliseconds>(t1 - t0);
							t0 = Clock::now();
							time_difference = (float)ms.count();
							key = 1;
						}
						

						
						UpArOrientation(ro3, rbi, roi, rbi2, roi2, hi.nod1.yaw, hi.nod1.pitch, hi.nod1.roll, hi.nod2.yaw, hi.nod2.pitch, hi.nod2.roll, 1);
						LoArOrientation(ro4, rbi, roi, rbi2, roi2, hi.nod1.yaw, hi.nod1.pitch, hi.nod1.roll, hi.nod2.yaw, hi.nod2.pitch, hi.nod2.roll, time_difference/1000 , key);
						
						key = 0;
						counter++;
						
						
					}

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
		a[2][0] = (sin(roll) *sin(yaw) + cos(yaw) *sin(pitch)*cos(roll));
		a[0][1] = cos(pitch)*sin(yaw);
		a[1][1] = sin(roll)*sin(yaw)*sin(pitch) + cos(yaw) * cos(roll);
		a[2][1] = (cos(roll)*sin(yaw)*sin(pitch) - cos(yaw) * sin(roll));
		a[0][2] = -sin(pitch);
		a[1][2] = cos(pitch)*sin(roll);
		a[2][2] = (cos(pitch)*cos(roll));

	}


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



	void apply(float a[], float b[][3])
	{
		int i, j, k;
		for (i = 0; i < 3; i++)
		{
			for (j = 0; j < 3; j++)
			{


				a[3 * i + j] = b[i][j];

			}
		}
	}



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

	void apply3(float a[], float b[][3])
	{
		int i, j, k;
		a[0] = -b[0][1];
		a[1] = -b[1][1];
		a[2] = -b[2][1];
		a[3] = -b[0][0];
		a[4] = -b[1][0];
		a[5] = -b[2][0];
		a[6] = -b[0][2];
		a[7] = -b[1][2];
		a[8] = -b[2][2];
	}



	/**
	* UpArOrientation()
	*	@This Fuction Provides The Final compensated Update for the UpAr Orientation
	*	@param CurrentUpArOrientation[][3]: The final UpAr orientation
	*	@param float IntitialRotationLocalUpAr[][3], this is the information of the initial frame for UpAr joint
	*	@param float IntitialRotationGloballUpAr[][3],  this is the information of the initial frame for UpAr joint
	*	@param float IntitialRotationLocalLoAr[][3], this is the information of the initial frame for LoAr joint
	*	@param float IntitialRotationGlobalLoAr[][3], this is the information of the initial frame for LoAr joint
	*	@param float NodUpArYaw , .......... UpAr and LoAr Nods Inputs
	*  @param float StretchSensorLoAr    Stretch Sensor data for LoAr
	*	@return void
	*/
	void UpArOrientation(float CurrentUpArOrientation[][3], float IntitialRotationLocalUpAr[][3], float IntitialRotationGloballUpAr[][3], float IntitialRotationLocalLoAr[][3], float IntitialRotationGlobalLoAr[][3], float NodUpArYaw, float NodUpArPitch, float NodUpArRoll, float NodLoArYaw, float NodLoArPitch, float NodLoArRoll, float StretchSensorLoAr)

	{
		//Intermediate arrays until achive final orienation for UpAr and LoAr, they are Taged with F (forward rotation) and B (Backward rotation) and are numbered consecutively

		float UpArF1[3][3] = {};
		float UpArF2[3][3] = {};
		float UpArF3[3][3] = {};
		float UpArF4[3][3] = {};
		float UpArF5[3][3] = {};
		float UpArF6[3][3] = {};
		float UpArF7[3][3] = {};

		float UpArB1[3][3] = {};
		float UpArB2[3][3] = {};
		float UpArB3[3][3] = {};
		float UpArB4[3][3] = {};


		float LoArF1[3][3] = {};
		float LoArF2[3][3] = {};
		float LoArF3[3][3] = {};
		float LoArF4[3][3] = {};

		float LoArB1[3][3] = {};
		float LoArB2[3][3] = {};
		float LoArB3[3][3] = {};
		float LoArB4[3][3] = {};



		float OrientationError = 0;
		float CompensationAngle = 0;



		float CompensationRotationLoAr[3][3] = {};
		float CompensationRotationUpAr[3][3] = {};
		float CurrentLoArOrientation[3][3] = {};

		vec pitchUpAr, pitchLoAr, NcrossUpArLoAr, RollUpAr, RollLoAr, NcrossUpArLoArRoll, fr, yawLoAr, yawLoAr2;






		/////////// Initial Frame Adjustments ///////////////////
		RotationGlobal(UpArF1, NodUpArYaw, NodUpArPitch, NodUpArRoll);
		RotationGlobal(LoArF1, NodLoArYaw, NodLoArPitch, NodLoArRoll);

		RotationLocal(UpArB1, NodUpArYaw, NodUpArPitch, NodUpArRoll);
		RotationLocal(LoArB1, NodLoArYaw, NodLoArPitch, NodLoArRoll);

		multi(UpArF1, IntitialRotationLocalUpAr, UpArF2);
		multi(LoArF1, IntitialRotationLocalLoAr, LoArF2);

		multi(IntitialRotationGloballUpAr, UpArB1, UpArB2);
		multi(IntitialRotationGlobalLoAr, LoArB1, LoArB2);



		///////////// Yaw Compensation Step ///////////////////


		// finding compensated yaw in one plane with upperarm
		yawLoAr.x = LoArB2[0][1] - (UpArB2[0][2] * LoArB2[0][1] + UpArB2[1][2] * LoArB2[1][1] + UpArB2[2][2] * LoArB2[2][1]) * UpArB2[0][2];
		yawLoAr.y = LoArB2[1][1] - (UpArB2[0][2] * LoArB2[0][1] + UpArB2[1][2] * LoArB2[1][1] + UpArB2[2][2] * LoArB2[2][1]) * UpArB2[1][2];
		yawLoAr.z = LoArB2[2][1] - (UpArB2[0][2] * LoArB2[0][1] + UpArB2[1][2] * LoArB2[1][1] + UpArB2[2][2] * LoArB2[2][1]) * UpArB2[2][2];
		yawLoAr.l = sqrt(yawLoAr.x * yawLoAr.x + yawLoAr.y * yawLoAr.y + yawLoAr.z*yawLoAr.z);
		yawLoAr.x = yawLoAr.x / yawLoAr.l;
		yawLoAr.y = yawLoAr.y / yawLoAr.l;
		yawLoAr.z = yawLoAr.z / yawLoAr.l;

		// 

		// Finding yaw compensation Angle


		OrientationError = yawLoAr.x * LoArB2[0][1] + yawLoAr.y * LoArB2[1][1] + yawLoAr.z* LoArB2[2][1];
		CompensationAngle = acos(OrientationError> 1.00 ? 1 : OrientationError);
		yawLoAr2.x = LoArB2[0][1];
		yawLoAr2.y = LoArB2[1][1];
		yawLoAr2.z = LoArB2[2][1];

		NcrossUpArLoAr = Ncross(yawLoAr2, yawLoAr);

		RotationVector(CompensationRotationUpAr, NcrossUpArLoAr, CompensationAngle);
		RotationVector(CompensationRotationLoAr, NcrossUpArLoAr, -CompensationAngle);

		// Applying Pitch Compensation 
		multi(LoArF2, CompensationRotationLoAr, LoArF3);

		multi(CompensationRotationUpAr, LoArB2, LoArB3);








		///////////// LoAr 180 degree Constriant ///////////////////
		RollUpAr.x = UpArB2[0][0];
		RollUpAr.y = UpArB2[1][0];
		RollUpAr.z = UpArB2[2][0];

		yawLoAr.x = LoArB3[0][1];
		yawLoAr.y = LoArB3[1][1];
		yawLoAr.z = LoArB3[2][1];

		yawLoAr2.x = UpArB2[0][1];
		yawLoAr2.y = UpArB2[1][1];
		yawLoAr2.z = UpArB2[2][1];

		NcrossUpArLoArRoll = Ncross(yawLoAr2, yawLoAr);
		if (dot(RollUpAr, yawLoAr) < 0) /// this case when not obey 180 degree constraint
		{

			OrientationError = UpArB2[0][1] * LoArB3[0][1] + UpArB2[1][1] * LoArB3[1][1] + UpArB2[2][1] * LoArB3[2][1];

			
			// Finding yaw compensation Angle
			if (acos(OrientationError > 1.00 ? 1 : OrientationError) > (PI / 2))
			{
				CompensationAngle = acos(OrientationError> 1.00 ? 1 : OrientationError) - PI;
			}
	         else
			{
				//Finding yaw compensation Angle
		     	CompensationAngle = acos(OrientationError > 1.00 ? 1 : OrientationError);
			}


			// Building yaw compensation rotation matrices
			RotationVector(CompensationRotationUpAr, NcrossUpArLoArRoll, +0.5* CompensationAngle);
			RotationVector(CompensationRotationLoAr, NcrossUpArLoArRoll, -0.5* CompensationAngle);

			// Applying yaw Compensation 
			multi(LoArF3, CompensationRotationUpAr, LoArF4);
			multi(UpArF2, CompensationRotationLoAr, UpArF3);
			multi(CompensationRotationLoAr, LoArB3, LoArB4);
			multi(CompensationRotationUpAr, UpArB2, UpArB3);

		}
		else  /// this case when obey 180 degree constraint just to improve LoAr angle estimation
		{

			OrientationError = UpArB2[0][1] * LoArB3[0][1] + UpArB2[1][1] * LoArB3[1][1] + UpArB2[2][1] * LoArB3[2][1];

			// Finding yaw compensation Angle
			CompensationAngle = 0;

			// Building yaw compensation rotation matrices
			RotationVector(CompensationRotationUpAr, NcrossUpArLoArRoll, +0.5* CompensationAngle);
			RotationVector(CompensationRotationLoAr, NcrossUpArLoArRoll, -0.5* CompensationAngle);

			// Applying yaw Compensation 
			multi(LoArF3, CompensationRotationUpAr, LoArF4);
			multi(UpArF2, CompensationRotationLoAr, UpArF3);
			multi(CompensationRotationLoAr, LoArB3, LoArB4);
			multi(CompensationRotationUpAr, UpArB2, UpArB3);

		}







		////////////////// setting to Final Body orientation ///////////////////////////////

		fr.x = UpArB3[0][0];
		fr.y = UpArB3[1][0];
		fr.z = UpArB3[2][0];

		rvector(CompensationRotationUpAr, fr, 3.1415 / 2);

		multi(UpArF3, CompensationRotationUpAr, UpArF4);

		fr.x = UpArB3[0][2];
		fr.y = UpArB3[1][2];
		fr.z = UpArB3[2][2];

		rvector(CompensationRotationUpAr, fr, 3.1415 / 2);

		multi(UpArF4, CompensationRotationUpAr, UpArF5);

		fr.x = UpArB3[0][1];
		fr.y = UpArB3[1][1];
		fr.z = UpArB3[2][1];

		rvector(CompensationRotationUpAr, fr, 3.1415);

		multi(UpArF5, CompensationRotationUpAr, UpArF6);

		fr.x = 0;
		fr.y = 1;
		fr.z = 0;

		rvector(CompensationRotationUpAr, fr, 3 * 3.1415 / 2);

		multi(UpArF6, CompensationRotationUpAr, CurrentUpArOrientation);



	}



	/**
	* LoArOrientation()
	*	@This Fuction Provides The Final Compensated Update for the UpAr Orientation
	*	@param CurrentLoArOrientation[][3]: The final LoAr orientation
	*	@param float IntitialRotationLocalUpAr[][3], this is the information of the initial frame for UpAr joint
	*	@param float IntitialRotationGloballUpAr[][3],  this is the information of the initial frame for UpAr joint
	*	@param float IntitialRotationLocalLoAr[][3], this is the information of the initial frame for LoAr joint
	*	@param float IntitialRotationGlobalLoAr[][3], this is the information of the initial frame for LoAr joint
	*	@param float NodUpArYaw , .......... UpAr and LoAr Nods Inputs
	*   @param float StretchSensorLoAr   Stretch Sensor data for LoAr
	*	@return void
	*/
	void LoArOrientation(float CurrentLoArOrientation[][3], float IntitialRotationLocalUpAr[][3], float IntitialRotationGloballUpAr[][3], float IntitialRotationLocalLoAr[][3], float IntitialRotationGlobalLoAr[][3], float NodUpArYaw, float NodUpArPitch, float NodUpArRoll, float NodLoArYaw, float NodLoArPitch, float NodLoArRoll, float StretchSensorLoAr, int key)

	{

		float UpArF1[3][3] = {};
		float UpArF2[3][3] = {};
		float UpArF3[3][3] = {};
		float UpArF4[3][3] = {};

		float UpArB1[3][3] = {};
		float UpArB2[3][3] = {};
		float UpArB3[3][3] = {};
		float UpArB4[3][3] = {};


		float LoArF1[3][3] = {};
		float LoArF2[3][3] = {};
		float LoArF3[3][3] = {};
		float LoArF4[3][3] = {};
		float LoArF5[3][3] = {};
		float LoArF6[3][3] = {};
		float LoArF7[3][3] = {};

		float LoArB1[3][3] = {};
		float LoArB2[3][3] = {};
		float LoArB3[3][3] = {};
		float LoArB4[3][3] = {};



		float OrientationError = 0;
		float CompensationAngle = 0;



		float CompensationRotationLoAr[3][3] = {};
		float CompensationRotationUpAr[3][3] = {};
		float CurrentUpArOrientation[3][3] = {};

		vec pitchUpAr, pitchLoAr, NcrossUpArLoAr, RollUpAr, RollLoAr, NcrossUpArLoArRoll, fr, yawLoAr, yawLoAr2;



		/////////// Initial Frame Adjustments ///////////////////
		RotationGlobal(UpArF1, NodUpArYaw, NodUpArPitch, NodUpArRoll);
		RotationGlobal(LoArF1, NodLoArYaw, NodLoArPitch, NodLoArRoll);

		RotationLocal(UpArB1, NodUpArYaw, NodUpArPitch, NodUpArRoll);
		RotationLocal(LoArB1, NodLoArYaw, NodLoArPitch, NodLoArRoll);

		multi(UpArF1, IntitialRotationLocalUpAr, UpArF2);
		multi(LoArF1, IntitialRotationLocalLoAr, LoArF2);

		multi(IntitialRotationGloballUpAr, UpArB1, UpArB2);
		multi(IntitialRotationGlobalLoAr, LoArB1, LoArB2);






		///////////// Yaw Compensation Step ///////////////////


		// finding compensated yaw in one plane with upperarm
		yawLoAr.x = LoArB2[0][1] - (UpArB2[0][2] * LoArB2[0][1] + UpArB2[1][2] * LoArB2[1][1] + UpArB2[2][2] * LoArB2[2][1]) * UpArB2[0][2];
		yawLoAr.y = LoArB2[1][1] - (UpArB2[0][2] * LoArB2[0][1] + UpArB2[1][2] * LoArB2[1][1] + UpArB2[2][2] * LoArB2[2][1]) * UpArB2[1][2];
		yawLoAr.z = LoArB2[2][1] - (UpArB2[0][2] * LoArB2[0][1] + UpArB2[1][2] * LoArB2[1][1] + UpArB2[2][2] * LoArB2[2][1]) * UpArB2[2][2];
		yawLoAr.l = sqrt(yawLoAr.x * yawLoAr.x + yawLoAr.y * yawLoAr.y + yawLoAr.z*yawLoAr.z);
		yawLoAr.x = yawLoAr.x / yawLoAr.l;
		yawLoAr.y = yawLoAr.y / yawLoAr.l;
		yawLoAr.z = yawLoAr.z / yawLoAr.l;

		// 

		// Finding yaw compensation Angle


		OrientationError = yawLoAr.x * LoArB2[0][1] + yawLoAr.y * LoArB2[1][1] + yawLoAr.z* LoArB2[2][1];
		CompensationAngle = acos(OrientationError> 1.00 ? 1 : OrientationError);
		yawLoAr2.x = LoArB2[0][1];
		yawLoAr2.y = LoArB2[1][1];
		yawLoAr2.z = LoArB2[2][1];

		NcrossUpArLoAr = Ncross(yawLoAr2, yawLoAr);

		RotationVector(CompensationRotationUpAr, NcrossUpArLoAr, CompensationAngle);
		RotationVector(CompensationRotationLoAr, NcrossUpArLoAr, -CompensationAngle);

		// Applying Pitch Compensation 
		multi(LoArF2, CompensationRotationLoAr, LoArF3);

		multi(CompensationRotationUpAr, LoArB2, LoArB3);



		///////////// LoAr 180 degree Constriant ///////////////////
		RollUpAr.x = UpArB2[0][0];
		RollUpAr.y = UpArB2[1][0];
		RollUpAr.z = UpArB2[2][0];

		yawLoAr.x = LoArB3[0][1];
		yawLoAr.y = LoArB3[1][1];
		yawLoAr.z = LoArB3[2][1];

		yawLoAr2.x = UpArB2[0][1];
		yawLoAr2.y = UpArB2[1][1];
		yawLoAr2.z = UpArB2[2][1];

		
		
		
		NcrossUpArLoArRoll = Ncross(yawLoAr2, yawLoAr);
		if (dot(RollUpAr, yawLoAr) < 0) /// this case when not obey 180 degree constraint
		{

			OrientationError = UpArB2[0][1] * LoArB3[0][1] + UpArB2[1][1] * LoArB3[1][1] + UpArB2[2][1] * LoArB3[2][1];

            //printf("Angle %f   ", acos(OrientationError > 1.00 ? 1 : OrientationError) * 180 / PI);

			//Finding yaw compensation Angle
			if (acos(OrientationError > 1.00 ? 1 : OrientationError) > (PI / 2))
			{
				CompensationAngle = acos(OrientationError> 1.00 ? 1 : OrientationError) - PI;
			}
			else
			{
				// Finding yaw compensation Angle
				CompensationAngle = acos(OrientationError > 1.00 ? 1 : OrientationError);
			}


			

			// Building yaw compensation rotation matrices
			RotationVector(CompensationRotationUpAr, NcrossUpArLoArRoll, +0.5* CompensationAngle);
			RotationVector(CompensationRotationLoAr, NcrossUpArLoArRoll, -0.5* CompensationAngle);

			// Applying yaw Compensation 
			multi(LoArF3, CompensationRotationUpAr, LoArF4);
			multi(UpArF2, CompensationRotationLoAr, UpArF3);
			multi(CompensationRotationLoAr, LoArB3, LoArB4);
			multi(CompensationRotationUpAr, UpArB2, UpArB3);

		}
		else  /// this case when obey 180 degree constraint just to improve LoAr angle estimation
		{

			OrientationError = UpArB2[0][1] * LoArB3[0][1] + UpArB2[1][1] * LoArB3[1][1] + UpArB2[2][1] * LoArB3[2][1];

			// Finding yaw compensation Angle
			CompensationAngle = 0;
			//printf("Angle %f   ", acos(OrientationError > 1.00 ? 1 : OrientationError) * 180 / PI);
			// Building yaw compensation rotation matrices
			RotationVector(CompensationRotationUpAr, NcrossUpArLoArRoll, +0.5* CompensationAngle);
			RotationVector(CompensationRotationLoAr, NcrossUpArLoArRoll, -0.5* CompensationAngle);

			// Applying yaw Compensation 
			multi(LoArF3, CompensationRotationUpAr, LoArF4);
			multi(UpArF2, CompensationRotationLoAr, UpArF3);
			multi(CompensationRotationLoAr, LoArB3, LoArB4);
			multi(CompensationRotationUpAr, UpArB2, UpArB3);

		}



	
		////////////////// Elbow angular calculation ///////////////////////////////
	
		
		float vAngleElbow = 0;
		float vAngularVelocityElbow = 0;
		float vAngularAccelerationElbow = 0;

		static float svAngleElbowOld = 0;
		static float svAngularVelocityElbowOld = 0;
		static float svAngularAccelerationElbowOld = 0;

		if (key == 1)
		{
			OrientationError = UpArB2[0][1] * LoArB3[0][1] + UpArB2[1][1] * LoArB3[1][1] + UpArB2[2][1] * LoArB3[2][1];
			vAngleElbow = acos(OrientationError > 1.00 ? 1 : OrientationError) * 180 / PI;
			vAngularVelocityElbow = (vAngleElbow - svAngleElbowOld) / StretchSensorLoAr;
			vAngularAccelerationElbow = (vAngularVelocityElbow - svAngularVelocityElbowOld) / StretchSensorLoAr;
			//printf("Angle: %f,  Velocity: %f,   Acceleration:%f \n", vAngleElbow, vAngularVelocityElbow, vAngularAccelerationElbow);
			svAngleElbowOld = vAngleElbow;
			svAngularVelocityElbowOld = vAngularVelocityElbow;
			svAngularAccelerationElbowOld = vAngularAccelerationElbow;
		}
		
		
		
		
		
		
		
		
		/// This step calculates the rotation of foerarm around its yaw axis by valuating the changes in the pitch
		////////////////// Lower Arm angle roll estimation ///////////////////////////////

		
		float vAngleLoArRoll = 0;
		float vAngularVelocityLoArRoll = 0;
		float vAngularAccelerationLoArRoll = 0;

		static float svAngleLoArRollOld = 0;
		static float svAngularVelocityLoArRollOld = 0;
		static float svAngularAccelerationLoArRollOld = 0;

		if (key == 1)
		{
			yawLoAr.x = LoArB3[0][2];
			yawLoAr.y = LoArB3[1][2];
			yawLoAr.z = LoArB3[2][2];

			yawLoAr2.x = UpArB2[0][2];
			yawLoAr2.y = UpArB2[1][2];
			yawLoAr2.z = UpArB2[2][2];


			vAngleLoArRoll = acos(dot(yawLoAr, yawLoAr2) > 1.00 ? 1 : dot(yawLoAr, yawLoAr2)) * 180 / PI;
			vAngularVelocityLoArRoll = (vAngleLoArRoll - svAngleLoArRollOld) / StretchSensorLoAr;
			vAngularAccelerationLoArRoll = (vAngularVelocityLoArRoll - svAngularVelocityLoArRollOld) / StretchSensorLoAr;
			printf("Angle: %f,  Velocity: %f,   Acceleration:%f \n", vAngleLoArRoll, vAngularVelocityLoArRoll, vAngularAccelerationLoArRoll);
			svAngleLoArRollOld = vAngleLoArRoll;
			svAngularVelocityLoArRollOld = vAngularVelocityLoArRoll;
			svAngularAccelerationLoArRollOld = vAngularAccelerationLoArRoll;
		}



	
	

		////////////////// setting to Final Body orientation ///////////////////////////////

		fr.x = LoArB4[0][0];
		fr.y = LoArB4[1][0];
		fr.z = LoArB4[2][0];

		rvector(CompensationRotationLoAr, fr, 3.1415 / 2);

		multi(LoArF4, CompensationRotationLoAr, LoArF5);

		fr.x = LoArB4[0][2];
		fr.y = LoArB4[1][2];
		fr.z = LoArB4[2][2];

		rvector(CompensationRotationLoAr, fr, 3.1415 / 2);

		multi(LoArF5, CompensationRotationLoAr, LoArF6);

		fr.x = LoArB4[0][1];
		fr.y = LoArB4[1][1];
		fr.z = LoArB4[2][1];

		rvector(CompensationRotationLoAr, fr, 3.1415);

		multi(LoArF6, CompensationRotationLoAr, LoArF7);

		fr.x = 0;
		fr.y = 1;
		fr.z = 0;

		rvector(CompensationRotationLoAr, fr, 3 * 3.1415 / 2);

		multi(LoArF7, CompensationRotationLoAr, CurrentLoArOrientation);


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
