// serialRead_test.cpp : main project file.

#include "stdafx.h"

using namespace System;
using namespace System::IO::Ports;

int main(array<System::String ^> ^args)
{
    Console::WriteLine(L"Hello World");
	findports();
    return 0;
}

private void findports(void){

	array<Object^>^ objectArray = SerialPort::GetPortName();
	this.ToList().ForEach(Console.WriteLine);

}