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
			
			while (true)
			{
				printf("Y:%f,P:%f,R:%f  Y:%f,P:%f,R:%f \n", hi.nod1.yaw, hi.nod1.pitch, hi.nod1.roll, hi.nod2.yaw, hi.nod2.pitch, hi.nod2.roll);
			}
				
			return 0;
				
	
		
		
}



