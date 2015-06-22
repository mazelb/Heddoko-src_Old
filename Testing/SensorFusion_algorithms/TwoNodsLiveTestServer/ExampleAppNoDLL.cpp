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
			float rp = 0,KneeAngleOpt;
			vec pitch2, pitch1, pitch3, roll2, roll1, roll12, crossp12, fr;
			float rb1[3][3] = {};
			float rb2[3][3] = {};
			float r[3][3] = {}, r2[3][3] = {}, r3[3][3] = {};
			float rb3[3][3] = {};
			float rb4[3][3] = {};
			float rb5[3][3] = {};
			float rb6[3][3] = {};
			float rb9[3][3] = {};
			float rb10[3][3] = {};
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
			float ro11[3][3] = {};
			float ro12[3][3] = {};
			float ro13[3][3] = {};
			float ro14[3][3] = {};
			float ro15[3][3] = {};
			float ro16[3][3] = {};
			float ro17[3][3] = {};
			float ro18[3][3] = {};
			float ro19[3][3] = {};
			float ro20[3][3] = {};
			float roi[3][3] = {};
			float roi2[3][3] = {};


			
			

			while (0)
			{
				
				/*
				
				l1 = (2 * ((result1.w*result1.y) + (result1.x*result1.z)) * 2 * ((result1.w*result1.y) + (result1.x*result1.z)))+(2 * ((result1.z*result1.y) - (result1.x*result1.w)) * 2 * ((result1.z*result1.y) - (result1.x*result1.w)))   + (((result1.w*result1.w) - (result1.x*result1.x) - (result1.y*result1.y) + (result1.z*result1.z))* ((result1.w*result1.w) - (result1.x*result1.x) - (result1.y*result1.y) + (result1.z*result1.z)));

				l2 = (2 * ((result2.w*result2.y) + (result2.x*result2.z)) * 2 * ((result2.w*result2.y) + (result2.x*result2.z)))  +(2 * ((result2.z*result2.y) - (result2.x*result2.w)) * 2 * ((result2.z*result2.y) - (result2.x*result2.w)))  + (((result2.w*result2.w) - (result2.x*result2.x) - (result2.y*result2.y) + (result2.z*result2.z))* ((result2.w*result2.w) - (result2.x*result2.x) - (result2.y*result2.y) + (result2.z*result2.z)));

				param2 = (2 * ((result1.w*result1.y) + (result1.x*result1.z)) * 2 * ((result2.w*result2.y) + (result2.x*result2.z))) + (2 * ((result1.z*result1.y) - (result1.x*result1.w)) * (2 * ((result2.z*result2.y) - (result2.x*result2.w)))) + (((result1.w*result1.w) - (result1.x*result1.x) - (result1.y*result1.y) + (result1.z*result1.z))* ((result2.w*result2.w) - (result2.x*result2.x) - (result2.y*result2.y) + (result2.z*result2.z)));

				*/
				//lnod1yaw = sqrt(		(sin(hi.nod1.pitch)*sin(hi.nod1.pitch))		+	(cos(hi.nod1.pitch)*sin(hi.nod1.roll) * cos(hi.nod1.pitch)*sin(hi.nod1.roll))		+		(cos(hi.nod1.pitch)*cos(hi.nod1.roll)*cos(hi.nod1.pitch)*cos(hi.nod1.roll)));
				// lnod2yaw = sqrt(		(sin(hi.nod2.pitch)*sin(hi.nod2.pitch))		+	(cos(hi.nod2.pitch)*sin(hi.nod2.roll) * cos(hi.nod2.pitch)*sin(hi.nod2.roll))		+		(cos(hi.nod2.pitch)*cos(hi.nod2.roll)*cos(hi.nod2.pitch)*cos(hi.nod2.roll)));
				
				//param = (sin(hi.nod1.pitch)*sin(hi.nod2.pitch)) + (cos(hi.nod1.pitch)*sin(hi.nod1.roll) * cos(hi.nod2.pitch)*sin(hi.nod2.roll)) + (cos(hi.nod1.pitch)*cos(hi.nod1.roll)*cos(hi.nod2.pitch)*cos(hi.nod2.roll));


				//lnod1pitch = sqrt(		(cos(hi.nod1.pitch)*sin(hi.nod1.yaw) * cos(hi.nod1.pitch)*sin(hi.nod1.yaw))		+		(cos(hi.nod1.yaw)*cos(hi.nod1.roll) + sin(hi.nod1.pitch)*sin(hi.nod1.roll) *sin(hi.nod1.yaw)) * (cos(hi.nod1.yaw)*cos(hi.nod1.roll) + sin(hi.nod1.pitch)*sin(hi.nod1.roll) *sin(hi.nod1.yaw))		+		(cos(hi.nod1.roll)*sin(hi.nod1.pitch) *sin(hi.nod1.yaw) - cos(hi.nod1.yaw) *sin(hi.nod1.roll)) *(cos(hi.nod1.roll)*sin(hi.nod1.pitch) *sin(hi.nod1.yaw) - cos(hi.nod1.yaw) *sin(hi.nod1.roll))	);
				//lnod2pitch = sqrt(		(cos(hi.nod2.pitch)*sin(hi.nod2.yaw) * cos(hi.nod2.pitch)*sin(hi.nod2.yaw))		+		(cos(hi.nod2.yaw)*cos(hi.nod2.roll) + sin(hi.nod2.pitch)*sin(hi.nod2.roll) *sin(hi.nod2.yaw)) * (cos(hi.nod2.yaw)*cos(hi.nod2.roll) + sin(hi.nod2.pitch)*sin(hi.nod2.roll) *sin(hi.nod2.yaw))		+		 (cos(hi.nod2.roll)*sin(hi.nod2.pitch) *sin(hi.nod2.yaw) - cos(hi.nod2.yaw) *sin(hi.nod2.roll)) *(cos(hi.nod2.roll)*sin(hi.nod2.pitch) *sin(hi.nod2.yaw) - cos(hi.nod2.yaw) *sin(hi.nod2.roll))	);
				//param2 = (cos(hi.nod1.pitch)*sin(hi.nod1.yaw) * cos(hi.nod2.pitch)*sin(hi.nod2.yaw)) + (cos(hi.nod1.yaw)*cos(hi.nod1.roll) + sin(hi.nod1.pitch)*sin(hi.nod1.roll) *sin(hi.nod1.yaw)) * (cos(hi.nod2.yaw)*cos(hi.nod2.roll) + sin(hi.nod2.pitch)*sin(hi.nod2.roll) *sin(hi.nod2.yaw)) + (cos(hi.nod1.roll)*sin(hi.nod1.pitch) *sin(hi.nod1.yaw) - cos(hi.nod1.yaw) *sin(hi.nod1.roll)) *(cos(hi.nod2.roll)*sin(hi.nod2.pitch) *sin(hi.nod2.yaw) - cos(hi.nod2.yaw) *sin(hi.nod2.roll));


				//lnod1roll = sqrt(		(cos(hi.nod1.pitch)*cos(hi.nod1.yaw) * cos(hi.nod1.pitch)*cos(hi.nod1.yaw))		+		(cos(hi.nod1.yaw)*sin(hi.nod1.roll)*sin(hi.nod1.pitch) - cos(hi.nod1.roll) * sin(hi.nod1.yaw)) * (cos(hi.nod1.yaw)*sin(hi.nod1.roll)*sin(hi.nod1.pitch) - cos(hi.nod1.roll) * sin(hi.nod1.yaw))		+		(sin(hi.nod1.roll) *sin(hi.nod1.yaw) + cos(hi.nod1.yaw) *sin(hi.nod1.pitch)*(cos(hi.nod1.roll))) * (sin(hi.nod1.roll) *sin(hi.nod1.yaw) + cos(hi.nod1.yaw) *sin(hi.nod1.pitch)*(cos(hi.nod1.roll))));
				//lnod2roll = sqrt(		(cos(hi.nod2.pitch)*cos(hi.nod2.yaw) * cos(hi.nod2.pitch)*cos(hi.nod2.yaw))		+		(cos(hi.nod2.yaw)*sin(hi.nod2.roll)*sin(hi.nod2.pitch) - cos(hi.nod2.roll) * sin(hi.nod2.yaw)) * (cos(hi.nod2.yaw)*sin(hi.nod2.roll)*sin(hi.nod2.pitch) - cos(hi.nod2.roll) * sin(hi.nod2.yaw))		+		 (sin(hi.nod2.roll) *sin(hi.nod2.yaw) + cos(hi.nod2.yaw) *sin(hi.nod2.pitch)*(cos(hi.nod2.roll))) * (sin(hi.nod2.roll) *sin(hi.nod2.yaw) + cos(hi.nod2.yaw) *sin(hi.nod2.pitch)*(cos(hi.nod2.roll))));
				//param3 = (cos(hi.nod1.pitch)*cos(hi.nod1.yaw) * cos(hi.nod2.pitch)*cos(hi.nod2.yaw)) + (cos(hi.nod1.yaw)*sin(hi.nod1.roll)*sin(hi.nod1.pitch) - cos(hi.nod1.roll) * sin(hi.nod1.yaw)) * (cos(hi.nod2.yaw)*sin(hi.nod2.roll)*sin(hi.nod2.pitch) - cos(hi.nod2.roll) * sin(hi.nod2.yaw)) + (sin(hi.nod1.roll) *sin(hi.nod1.yaw) + cos(hi.nod1.yaw) *sin(hi.nod1.pitch)*(cos(hi.nod1.roll))) * (sin(hi.nod2.roll) *sin(hi.nod2.yaw) + cos(hi.nod2.yaw) *sin(hi.nod2.pitch)*(cos(hi.nod2.roll)));
				
				//pitch1.x = (sin(hi.nod1.roll) *sin(hi.nod1.yaw) + cos(hi.nod1.yaw) *sin(hi.nod1.pitch)*cos(hi.nod1.roll));
				//pitch1.y = (cos(hi.nod1.roll)*sin(hi.nod1.yaw)*sin(hi.nod1.pitch) - cos(hi.nod1.yaw) * sin(hi.nod1.roll));
				//pitch1.z = (cos(hi.nod1.pitch)*cos(hi.nod1.roll));

			//	pitch2.x = (sin(hi.nod2.roll) *sin(hi.nod2.yaw) + cos(hi.nod2.yaw) *sin(hi.nod2.pitch)*cos(hi.nod2.roll));
				//pitch2.y = (cos(hi.nod2.roll)*sin(hi.nod2.yaw)*sin(hi.nod2.pitch) - cos(hi.nod2.yaw) * sin(hi.nod2.roll));
				//pitch2.z =  cos(hi.nod2.pitch)*cos(hi.nod2.roll);


				//crossp12 = cross(pitch1, pitch2);


			

				//crossp12.x = pitch1.y *pitch2.z - pitch2.y *pitch1.z;
				//crossp12.y = pitch1.z *pitch2.x - pitch2.z *pitch1.x;
				//crossp12.z = pitch1.x *pitch2.y - pitch2.x *pitch1.y;
				//crossp12.l = sqrt(crossp12.x*crossp12.x + crossp12.y * crossp12.y + crossp12.z * crossp12.z);
				//making it a unit vector

				//crossp12.x /= crossp12.l;
				//crossp12.y /= crossp12.l;
				//crossp12.z /= crossp12.l;
				//crossp12.l = sqrt(crossp12.x*crossp12.x + crossp12.y * crossp12.y + crossp12.z * crossp12.z);
				//Checking cross 
				//printf("inner1:%f, inner2:%f\n", crossp12.x* pitch1.x + crossp12.y* pitch1.y + crossp12.z* pitch1.z, crossp12.x* pitch2.x + crossp12.y* pitch2.y + crossp12.z* pitch2.z);

				
				//paramp = (sin(hi.nod1.roll) *sin(hi.nod1.yaw) + cos(hi.nod1.yaw) *sin(hi.nod1.pitch)*cos(hi.nod1.roll)) * (sin(hi.nod2.roll) *sin(hi.nod2.yaw) + cos(hi.nod2.yaw) *sin(hi.nod2.pitch)*cos(hi.nod2.roll))		 +		(cos(hi.nod1.roll)*sin(hi.nod1.yaw)*sin(hi.nod1.pitch) - cos(hi.nod1.yaw) * sin(hi.nod1.roll)) * (cos(hi.nod2.roll)*sin(hi.nod2.yaw)*sin(hi.nod2.pitch) - cos(hi.nod2.yaw) * sin(hi.nod2.roll))		 +		 (cos(hi.nod1.pitch)*cos(hi.nod1.roll) * cos(hi.nod2.pitch)*cos(hi.nod2.roll));
				//paramx = (cos(hi.nod1.pitch)*cos(hi.nod1.yaw) * cos(hi.nod2.pitch)*cos(hi.nod2.yaw))		+		(cos(hi.nod1.pitch)*sin(hi.nod1.yaw) * cos(hi.nod2.pitch)*sin(hi.nod2.yaw))			+		sin(hi.nod1.pitch)*sin(hi.nod2.pitch);
				//paramy = (-cos(hi.nod1.roll) *sin(hi.nod1.yaw) + cos(hi.nod1.yaw) *sin(hi.nod1.pitch)*sin(hi.nod1.roll)) * (-cos(hi.nod2.roll) *sin(hi.nod2.yaw) + cos(hi.nod2.yaw) *sin(hi.nod2.pitch)*sin(hi.nod2.roll))		 +		(sin(hi.nod1.roll)*sin(hi.nod1.yaw)*sin(hi.nod1.pitch) + cos(hi.nod1.yaw) * cos(hi.nod1.roll)) * (sin(hi.nod2.roll)*sin(hi.nod2.yaw)*sin(hi.nod2.pitch) + cos(hi.nod2.yaw) * cos(hi.nod2.roll))		 +		 (cos(hi.nod1.pitch)*sin(hi.nod1.roll) * cos(hi.nod2.pitch)*sin(hi.nod2.roll));
				//paramz = pitch1.x * pitch2.x + pitch1.y * pitch2.y + pitch1.z * pitch2.z;
				

				rb(rb1, hi.nod1.yaw, hi.nod1.pitch, hi.nod1.roll);
				rb(rb2, hi.nod2.yaw, hi.nod2.pitch, hi.nod2.roll);

				pitch1.x = rb1[0][2];
				pitch1.y = rb1[1][2];
				pitch1.z = rb1[2][2];

				pitch2.x = rb2[0][2];
				pitch2.y = rb2[1][2];
				pitch2.z = rb2[2][2];
				
				
				crossp12 = cross(pitch1, pitch2); 

				//printf("inner1:%f, inner2:%f\n", crossp12.x* pitch1.x + crossp12.y* pitch1.y + crossp12.z* pitch1.z, crossp12.x* pitch2.x + crossp12.y* pitch2.y + crossp12.z* pitch2.z);
				//printf("inner1:%f, \n", crossp12.x* crossp12.x + crossp12.y* crossp12.y + crossp12.z* crossp12.z);

				param =	 rb1[0][0] * rb2[0][0] + rb1[1][0] * rb2[1][0] + rb1[2][0] * rb2[2][0];
				param2 = rb1[0][1] * rb2[0][1] + rb1[1][1] * rb2[1][1] + rb1[2][1] * rb2[2][1];
				param3 = rb1[0][2] * rb2[0][2] + rb1[1][2] * rb2[1][2] + rb1[2][2] * rb2[2][2];
				
				rp = acos(param3);

				rvector(r, crossp12, -rp);
				multi(r, rb2, rb3);

				pitch3.x = rb3[0][2];
				pitch3.y = rb3[1][2];
				pitch3.z = rb3[2][2];

				param4 = rb1[0][0] * rb3[0][0] + rb1[1][0] * rb3[1][0] + rb1[2][0] * rb3[2][0];
				param5 = rb1[0][1] * rb3[0][1] + rb1[1][1] * rb3[1][1] + rb1[2][1] * rb3[2][1];
				param6 = rb1[0][2] * rb3[0][2] + rb1[1][2] * rb3[1][2] + rb1[2][2] * rb3[2][2];
				angle = acos(param)  * 180.0 / PI;
				angle2 = acos(param2)  * 180.0 / PI;
				angle3 = acos(param3)  * 180.0 / PI;
				angle4 = acos(param4 > 1.00 ? 1 : param4)  * 180.0 / PI;
				angle5 = acos(param5 > 1.00 ? 1 : param5)  * 180.0 / PI;
				angle6 = acos(param6 > 1.00 ? 1 : param6)  * 180.0 / PI;
			

				printf("x:%f x:%f y:%f y:%f z:%f z:%f \n", (angle + angle5 )/2, angle4, angle2, angle5, angle3, angle6);

				//printf(" %f      %f \n ", rb3[0][0] * rb3[0][0] + rb3[1][0] * rb3[1][0] + rb3[2][0] * rb3[2][0], rb3[0][2] * rb3[0][2] + rb3[1][2] * rb3[1][2] + rb3[2][2] * rb3[2][2]);

				//printf("x:%f x:%f y:%f y:%f z:%f z:%f \n", rb3[0][0], rb3[0][1], pitch3.x, pitch3.y, pitch3.z, pitch3.y);
				
				//testing one pitch data

				//lnod1yaw = sqrt((sin(hi.nod1.roll)*sin(hi.nod1.roll)) + (cos(hi.nod1.roll)*sin(hi.nod1.pitch) * cos(hi.nod1.roll)*sin(hi.nod1.pitch)) + (cos(hi.nod1.roll)*cos(hi.nod1.pitch)*cos(hi.nod1.roll)*cos(hi.nod1.pitch)));
				//lnod2yaw = sqrt((sin(hi.nod2.roll)*sin(hi.nod2.roll)) + (cos(hi.nod2.roll)*sin(hi.nod2.pitch) * cos(hi.nod2.roll)*sin(hi.nod2.pitch)) + (cos(hi.nod2.roll)*cos(hi.nod2.pitch)*cos(hi.nod2.roll)*cos(hi.nod2.pitch)));
				//param = (sin(hi.nod1.roll)*sin(hi.nod2.roll)) + (cos(hi.nod1.roll)*sin(hi.nod1.pitch) * cos(hi.nod2.roll)*sin(hi.nod2.pitch)) + (cos(hi.nod1.pitch)*cos(hi.nod1.roll)*cos(hi.nod2.pitch)*cos(hi.nod2.roll));
			



				//l1pitch = pow((sin(hi.nod1.roll) *sin(hi.nod1.yaw) + cos(hi.nod1.yaw) *sin(hi.nod1.pitch)*(cos(hi.nod1.roll))), 2) + pow((cos(hi.nod1.roll)*sin(hi.nod1.yaw)*sin(hi.nod1.pitch) - cos(hi.nod1.yaw) * sin(hi.nod1.roll)), 2) + pow((cos(hi.nod1.pitch)*cos(hi.nod1.roll)), 2);
				//printf("roll:%f, kknee:%f  \n", pitch1.x*pitch1.x + pitch1.y*pitch1.y + pitch1.z*pitch1.z    , pitch2.x*pitch2.x + pitch2.y*pitch2.y + pitch2.z*pitch2.z);
				//
				
				//printf("knee_x:%f, Knee_y:%f   P_z:%f  \n", angle,angle2, angle3);



				//printf("x:%f x:%f y:%f y:%f z:%f z:%f \n", pitch1.x, pitch2.x, pitch1.y, pitch2.y, pitch1.z, pitch2.z);
				// comparing pitch data 
				//printf("x:%f x:%f y:%f y:%f z:%f z:%f \n", (sin(hi.nod2.roll) *sin(hi.nod2.yaw) + cos(hi.nod2.yaw) *sin(hi.nod2.pitch)*cos(hi.nod2.roll)), (sin(hi.nod1.roll) *sin(hi.nod1.yaw) + cos(hi.nod1.yaw) *sin(hi.nod1.pitch)*cos(hi.nod1.roll)), (cos(hi.nod2.roll)*sin(hi.nod2.yaw)*sin(hi.nod2.pitch) - cos(hi.nod2.yaw) * sin(hi.nod2.roll)), (cos(hi.nod1.roll)*sin(hi.nod1.yaw)*sin(hi.nod1.pitch) - cos(hi.nod1.yaw) * sin(hi.nod1.roll)), (cos(hi.nod2.pitch)*cos(hi.nod2.roll)), (cos(hi.nod1.pitch)*cos(hi.nod1.roll)));
			    
				// comparison two pitches from with vector
			//	printf("x:%f x:%f y:%f y:%f z:%f z:%f \n", (sin(hi.nod1.roll) *sin(hi.nod1.yaw) + cos(hi.nod1.yaw) *sin(hi.nod1.pitch)*cos(hi.nod1.roll)), pitch2.x, (cos(hi.nod1.roll)*sin(hi.nod1.yaw)*sin(hi.nod1.pitch) - cos(hi.nod1.yaw) * sin(hi.nod1.roll)), pitch2.y, (cos(hi.nod1.pitch)*cos(hi.nod1.roll)), pitch2.z);
				//testing one pitch data
				
				
				
				//testing one pitch data
				//printf("yaw:%f pitch:%f  roll:%f \n", (sin(hi.nod1.roll) *sin(hi.nod1.yaw) + cos(hi.nod1.yaw) *sin(hi.nod1.pitch)*(cos(hi.nod1.roll))), (cos(hi.nod1.roll)*sin(hi.nod1.yaw)*sin(hi.nod1.pitch) - cos(hi.nod1.yaw) * sin(hi.nod1.roll)), (cos(hi.nod1.pitch)*cos(hi.nod1.roll)));
			}
				
			
		
			
			
			
			
			
			
			
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
				
						
							   
							
							

							

							ro(ro1, hi.nod1.yaw, hi.nod1.pitch, hi.nod1.roll);
							ro(ro2, hi.nod2.yaw, hi.nod2.pitch, hi.nod2.roll);

							rb(rb1, hi.nod1.yaw, hi.nod1.pitch, hi.nod1.roll);
							rb(rb2, hi.nod2.yaw, hi.nod2.pitch, hi.nod2.roll);



								
								multi(ro1, rbi, ro3);
								multi(ro2, rbi2, ro4);


								multi(roi, rb1, rb3);
								multi(roi2, rb2, rb4);


								
								



								pitch1.x = rb3[0][2];
								pitch1.y = rb3[1][2];
								pitch1.z = rb3[2][2];

								pitch2.x = rb4[0][2];
								pitch2.y = rb4[1][2];
								pitch2.z = rb4[2][2];

								crossp12 = cross(pitch1, pitch2);

								param = rb3[0][2] * rb4[0][2] + rb3[1][2] * rb4[1][2] + rb3[2][2] * rb4[2][2];

								rp = acos(param > 1.00 ? 1 : param);
								angle = acos(param)  * 180.0 / PI;
								
								
								
								
								
								rvector(r, crossp12, .5*rp);
								rvector(r2, crossp12, -.5*rp);


								multi(r,rb3,rb5);
								multi(r2, rb4, rb6);
								
								multi(ro3, r2, ro7);
								multi(ro4, r, ro8);

								roll1.x = rb5[0][0];
								roll1.y = rb5[1][0];
								roll1.z = rb5[2][0];

								roll2.x = rb6[0][0];
								roll2.y = rb6[1][0];
								roll2.z = rb6[2][0];

								pitch1.x = rb5[0][2];
								pitch1.y = rb5[1][2];
								pitch1.z = rb5[2][2];

								

								roll12 = cross(roll1, roll2);
								if (dot(roll12, pitch1) > 0)
								
								{

									param = rb5[0][0] * rb6[0][0] + rb5[1][0] * rb6[1][0] + rb5[2][0] * rb6[2][0];
									rp = acos(param > 1.00 ? 1 : param);																							
									rvector(r, roll12, .5*rp);
									rvector(r2, roll12, -.5*rp);
									multi(r, rb5, rb9);
									multi(r2, rb6, rb10);
									multi(ro7, r2, ro11);
									multi(ro8, r, ro12);
																																		
								}


								else
								{

									param = rb5[0][0] * rb6[0][0] + rb5[1][0] * rb6[1][0] + rb5[2][0] * rb6[2][0];
									rp = acos(param > 1.00 ? 1 : param);
									param2 = rb5[0][0] * rb6[0][0] + rb5[1][0] * rb6[1][0] + rb5[2][0] * rb6[2][0];
									param3 = rb5[0][1] * rb6[0][1] + rb5[1][1] * rb6[1][1] + rb5[2][1] * rb6[2][1];
									KneeAngleOpt = (acos(param2 > 1.00 ? 1 : param2) + acos(param3 > 1.00 ? 1 : param3)) / 2;
									printf("kneeangleopt:   %f  \n", KneeAngleOpt * 180 / PI);
									rp = rp - KneeAngleOpt;
									rvector(r, roll12, .5*rp);
									rvector(r2, roll12, -.5*rp);
									multi(r, rb5, rb9);
									multi(r2, rb6, rb10);
									multi(ro7, r2, ro11);
									multi(ro8, r, ro12);

								}


								



					
								
								fr.x = rb9[0][0];
								fr.y = rb9[1][0];
								fr.z = rb9[2][0];
								
								rvector(r3, fr, 3.1415/2);
								
								multi(ro11, r3, ro13);

								fr.x = rb9[0][2];
								fr.y = rb9[1][2];
								fr.z = rb9[2][2];

								rvector(r3, fr, 3.1415 / 2);

								multi(ro13, r3, ro15);

								fr.x = rb9[0][1];
								fr.y = rb9[1][1];
								fr.z = rb9[2][1];

								rvector(r3, fr, 3.1415 );

								multi(ro15, r3, ro17);

								fr.x =0;
								fr.y = 1;
								fr.z = 0;

								rvector(r3, fr, 3*3.1415 / 2);

								multi(ro17, r3, ro19);
								
							    // printf("Original:%f   compensated:%f \n", angle , angle2);


								fr.x = rb10[0][0];
								fr.y = rb10[1][0];
								fr.z = rb10[2][0];

								rvector(r3, fr, 3.1415 / 2);

								multi(ro12, r3, ro14);

								fr.x = rb10[0][2];
								fr.y = rb10[1][2];
								fr.z = rb10[2][2];

								rvector(r3, fr, 3.1415 / 2);

								multi(ro14, r3, ro16);

								fr.x = rb10[0][1];
								fr.y = rb10[1][1];
								fr.z = rb10[2][1];

								rvector(r3, fr, 3.1415);

								multi(ro16, r3, ro18);
								
								fr.x =0;
								fr.y = 1;
								fr.z = 0;

								rvector(r3, fr,  3* 3.1415 / 2);

								multi(ro18, r3, ro20);


							
								apply2(rx, ro19);
								apply2(rx2, ro20);

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
	a[1] = -b[1][1];
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



void delay(int count)
{
	for (int i = 0; i<count; i++)
		for (int j = 0; j<count; j++);
}


void FinalRotation(float a[][3], float b[][3])
{



}




