Current version 1.0.7
Published 6/1/2015

Thank you for downloading the Nod Unity SDK.
If you do not currently own a Nod ring visit www.nod.com for more info.


Getting Started:
To get started browse to Assets\Nod\Examples\Scenes\ and load the various example scenes we have there to see
examples of how you can use our APIs.

Example1_Pos6d: Shows you how to read the rotation from the ring and apply it to a GameObject in Unity, how to read the button state, and battery percentage.
Example2_TouchToMove: Shows you have to use Position 2D mode and Gestures.

If you want to quickly apply rotation from the ring to your own GameObjects for your own projects you can drag/drop the "Example1.cs" script from Assets\Nod\Examples\Scripts\ onto it.
When writing your own apps you can pick and choose what data you want from each ring by subscribing to the various services you are interested in.
Subscribe(NodSubscriptionType.Orientation);
for example will report the orientation of the ring.

You probable want to subscribe only to services you will use to minimize bluetooth traffic. If you just want button data from one ring and Pose6D data from another ring,
instead of subscribing both rings to pose6d and buttons you can subscribe one to pse6D and the other to buttons so both rings arn't broadcasting data they do not need.

Make sure to unsubscribe to everything you subscribed to when you are done with it.
Unsubscribe(NodSubscriptionType.Orientation);
for example to unsubscribe to orientation updates.


Supported Platforms:
We currently support PC, iOs, and Android targets in Unity 5.x.

PC:
Make sure you have the PC client installed from: https://github.com/openspatial/openspatial-windows/tree/master/Nod%20Installer
Windows 8.1 or greater is required for blue tooth LE support.
It has a dependancy on the Visual Studio 2013 runtime c++ redistributable, which can be downloaded here:
https://www.microsoft.com/en-us/download/confirmation.aspx?id=40784

iOs:
If you are building for iOs you will need to add the "CoreBluetooth.framework" to xcode.
To do this bring up the Project Navigator, select your project, under Build Phases expand out Link Binary With Libraries, and click on the "+" sign.
Type in "CoreBluetooth.framework"
iOS min requirements are iOs 8, and a device that supports Bluetooth LE.

Android:
To use our ring for Android you need to install a custom Nod app.  You can get the app
by signing up for a dev account under http://developer.nod.com/
Our plugin requires some modifications to the AndroidManifest.xml file, this might take some effort to merge these with other plugins that also modify it.
To assist you in this effort we have provided AndroidManifest.xml files that we know work for a variety of different platforms.
You can find these under Assets\Nod\Support\AndroidVR\...
If you want help merging our plugin with other Android plugins let us know on our forums at http://developer.nod.com/
Android min requirements 4.3 Jellybean, and your device must support Bluetooth LE.

If you would like to see support for a different platform let us know  http://developer.nod.com/


Revision History:
Version 1.0.7
Exposed battery percentage

Version 1.0.4
Fixed a crash on some Android devices that would happen when a Nod ring would get disconnected from the host system.

Version 1.0.4
Updated all AndroidManifest.xml files to add an entry to call out the app as using Nod.

Version 1.0.3
Documentation update on system requirements, no code change.

Version 1.0.2
Bug fix for Mac builds

Version 1.0.1
Added Beta support for Mac, only works in Unity 5.x Beta 18 or greater

Version 1.0.0
Initial release