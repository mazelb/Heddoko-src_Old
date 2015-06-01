#using <System.dll>

using namespace System;
using namespace System::IO::Ports;
using namespace System::Threading;

public ref class PortChat
{
private:
	static bool _continue;
	static SerialPort^ _serialPort;

public:
	static int ch1, ch2, ch3, ch4, ch5, time;
	static void Main()
	{
		String^ name;
		String^ message;
		StringComparer^ stringComparer = StringComparer::OrdinalIgnoreCase;
		Thread^ readThread = gcnew Thread(gcnew ThreadStart(PortChat::Read));

		// Create a new SerialPort object with default settings.
		_serialPort = gcnew SerialPort();

		// Allow the user to set the appropriate properties.
		_serialPort->PortName = SetPortName(_serialPort->PortName);
		_serialPort->BaudRate = 115200;
		//_serialPort->Parity = SetPortParity(_serialPort->Parity);
		//_serialPort->DataBits = SetPortDataBits(_serialPort->DataBits);
		//_serialPort->StopBits = SetPortStopBits(_serialPort->StopBits);
		//_serialPort->Handshake = SetPortHandshake(_serialPort->Handshake);

		// Set the read/write timeouts
		_serialPort->ReadTimeout = 500;
		_serialPort->WriteTimeout = 500;

		_serialPort->Open();
		_continue = true;
		readThread->Start();
		_serialPort->WriteLine("#s\n");

		/*Console::Write("Name: ");
		name = Console::ReadLine();

		Console::WriteLine("Type QUIT to exit");

		while (_continue)
		{
			message = Console::ReadLine();

			if (stringComparer->Equals("quit", message))
			{
				_continue = false;
			}
			else
			{
				_serialPort->WriteLine(
					String::Format("<{0}>: {1}", name, message));
			}
		}
		*/
		readThread->Join();
		_serialPort->Close();
	}

	static void Read()
	{
	
		while (_continue)
		{
			try
			{
				String^ message = _serialPort->ReadLine();
	
			
				//if (int(message[0])){
					
				//if (message.Length < 33) continue;
				ch1 = 0; ch2 = 0, ch3 = 0, ch4 = 0, ch5 = 0;
				for (int i = 1; i < 5; i++)
				{
					ch1 = (message[i] - '0') + ch1 * 10;
			
				}
				for (int i = 5; i < 9; i++)
				{
					ch2 = (message[i] - '0') + ch2 * 10;
					
				}
				for (int i = 9; i < 13; i++)
				{
					ch3 = (message[i] - '0') + ch3 * 10;
				
				}
				for (int i =13; i < 17; i++)
				{
					ch4 = (message[i] - '0') + ch4 * 10;
				}
				for (int i = 17; i < 21; i++)
				{
					ch5 = (message[i] - '0') + ch5* 10;
				}
				Console::WriteLine("CH1: {0} , CH2: {1}, CH3: {2}, CH3: {3}, CH3: {4}" ,ch1, ch2, ch3, ch4 , ch5);
				//Console::WriteLine(ch2);
				//}
			}
			catch (TimeoutException ^) {}
		}
	}

	static String^ SetPortName(String^ defaultPortName)
	{
		String^ portName;

		Console::WriteLine("Available Ports:");
		for each (String^ s in SerialPort::GetPortNames())
		{
			Console::WriteLine("   {0}", s);
		}

		Console::Write("COM port({0}): ", defaultPortName);
		portName = Console::ReadLine();

		if (portName == "")
		{
			portName = defaultPortName;
		}
		return portName;
	}

	/*static Int32 SetPortBaudRate(Int32 defaultPortBaudRate)
	{
		String^ baudRate;

		Console::Write("Baud Rate({0}): ", defaultPortBaudRate);
		baudRate = Console::ReadLine();

		if (baudRate == "")
		{
			baudRate = defaultPortBaudRate.ToString();
		}

		return Int32::Parse(baudRate);
	}

	static Parity SetPortParity(Parity defaultPortParity)
	{
		String^ parity;

		Console::WriteLine("Available Parity options:");
		for each (String^ s in Enum::GetNames(Parity::typeid))
		{
			Console::WriteLine("   {0}", s);
		}

		Console::Write("Parity({0}):", defaultPortParity.ToString());
		parity = Console::ReadLine();

		if (parity == "")
		{
			parity = defaultPortParity.ToString();
		}

		return (Parity)Enum::Parse(Parity::typeid, parity);
	}

	static Int32 SetPortDataBits(Int32 defaultPortDataBits)
	{
		String^ dataBits;

		Console::Write("Data Bits({0}): ", defaultPortDataBits);
		dataBits = Console::ReadLine();

		if (dataBits == "")
		{
			dataBits = defaultPortDataBits.ToString();
		}

		return Int32::Parse(dataBits);
	}

	static StopBits SetPortStopBits(StopBits defaultPortStopBits)
	{
		String^ stopBits;

		Console::WriteLine("Available Stop Bits options:");
		for each (String^ s in Enum::GetNames(StopBits::typeid))
		{
			Console::WriteLine("   {0}", s);
		}

		Console::Write("Stop Bits({0}):", defaultPortStopBits.ToString());
		stopBits = Console::ReadLine();

		if (stopBits == "")
		{
			stopBits = defaultPortStopBits.ToString();
		}

		return (StopBits)Enum::Parse(StopBits::typeid, stopBits);
	}

	static Handshake SetPortHandshake(Handshake defaultPortHandshake)
	{
		String^ handshake;

		Console::WriteLine("Available Handshake options:");
		for each (String^ s in Enum::GetNames(Handshake::typeid))
		{
			Console::WriteLine("   {0}", s);
		}

		Console::Write("Handshake({0}):", defaultPortHandshake.ToString());
		handshake = Console::ReadLine();

		if (handshake == "")
		{
			handshake = defaultPortHandshake.ToString();
		}

		return (Handshake)Enum::Parse(Handshake::typeid, handshake);
	}*/
};

int main()
{
	PortChat::Main();
}