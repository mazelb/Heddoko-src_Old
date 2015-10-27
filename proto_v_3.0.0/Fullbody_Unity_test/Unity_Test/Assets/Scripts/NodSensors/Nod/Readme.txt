Current version 2.0.0 beta
Published 9/14/2015

This is an early access beta version of the Nod Unity SDK plugin that supports positional tracking. Currently only works with a backspin using the backface camera of a Samsung Galaxy S6.

Getting Started:
Check out our example scenes in Assets\Nod\Examples\Scenes\
You will need to pair your Nod device to your system through its bluetooth settings and have the Nod Config app installed on your system.

Platforms notes:
Android: The only platform currently to support backspin positional tracking.  Only works on a Samsung Galaxy S6.
Our plugin requires some modifications to the AndroidManifest.xml file to enable bluetooth support, the external camera, and reference the nod service app.
There should be a preexisting AndroidManifest.xml you can use as under: Assets\Plugins\Android\
Windows: No positional tracking support.
OSX/iOS: currently not supported, support to come later.

Nod Unity SDK basics:
Since bandwidth over Bluetooth LE is limited you should conditionally enable what information you need, both to save battery on the device and to speed up the update interval.
Here is a break down of what services you can subscribe to from a Nod Devices:
ButtonMode - Sends buttons.
EulerMode - The orientation of the device relative to magnetic north.
GameControlMode - backspin only. Joystick and trigger values.
GestureMode - ring only.  See website for how to perform gestures.  Will report for one frame the most recent gesture.  Up, Down, Left, Right, Clockwise, Counter Clockwise.
PointerMode - ring only.  Mouse emulation.
AccelMode - raw Acceleration values
GyroMode - raw gyroscopic values.

Positional Tracking:
Positional tracking is in meters and uses Unitys cordinate system for relative translations in x,y,z.
Have your GameObject be a child of the camera then translations can be achieved by doing something like this:
transform.localPosition = initialLocalPosition + nodDevice.GetNodCVPosition();
You can use nod.IsTrackingLed() to tell if the backspin is in the field of view or not.

Examples:
