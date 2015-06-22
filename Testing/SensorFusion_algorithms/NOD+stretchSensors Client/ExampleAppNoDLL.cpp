#include "stdafx.h"
#include "ExampleDelegate.h"
#include <iostream>
#include <windows.h>
using namespace std;



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


		
		
			int index =1 ;
			printf("\nfirst controler name %s and second %s: ", controller->names.at(0).c_str(), controller->names.at(1).c_str());
		
			/*if (strcmp(input.c_str(), "pointer") == 0)
			{
				printf("subscribe 2D %s\n", controller->names.at(index).c_str());
				controller->subscribeToPointer(controller->names.at(index));
			}
			if (strcmp(input.c_str(), "gesture") == 0)
			{
				printf("subscribe gesture %s\n", controller->names.at(index).c_str());
				controller->subscribeToGesture(controller->names.at(index));
			}
			if (strcmp(input.c_str(), "button") == 0)
			{
				printf("subscribe button %s\n", controller->names.at(index).c_str());
				controller->subscribeToButton(controller->names.at(index));
			}*/
			
			wcout << "Connecting to pipe..." << endl;

			// Open the named pipe
			// Most of these parameters aren't very relevant for pipes.
			HANDLE pipe = CreateFile(
				L"\\\\.\\pipe\\my_pipe",
				GENERIC_READ, // only need read access
				FILE_SHARE_READ | FILE_SHARE_WRITE,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL
				);

			if (pipe == INVALID_HANDLE_VALUE) {
				wcout << "Failed to connect to pipe." << endl;
				// look up error code here using GetLastError()
				system("pause");
				return 1;
			}

			wcout << "Reading data from pipe..." << endl;

			// The read operation will block until there is data to read
			int buffer;
			DWORD numBytesRead = 0;
			BOOL result;

			while (true)
			{
				result = ReadFile(
					pipe,
					&buffer, // the data from the pipe will be put here
					sizeof(int), // number of bytes allocated
					&numBytesRead, // this will store number of bytes actually read
					NULL // not using overlapped IO
					);

				if (result) {
					//buffer[numBytesRead / sizeof(wchar_t)] = '\0'; // null terminate the string
					//wcout << "Number of bytes read: " << numBytesRead << endl;
					printf("Y:%f,P:%f,R:%f  Y:%f,P:%f,R:%f   %d \n", hi.nod1.yaw, hi.nod1.pitch, hi.nod1.roll, hi.nod2.yaw, hi.nod2.pitch, hi.nod2.roll,buffer);
				}
				else {
					wcout << "Failed to read data from the pipe." << endl;
				}
			}

			// Close our pipe handle
			CloseHandle(pipe);

			wcout << "Done." << endl;

			system("pause");
			return 0;
				
				
		
		

}



