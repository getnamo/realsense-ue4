realsense-ue4
====================

An event-driven [RealSense](http:/www.intel.com/realsense) plugin for the Unreal Engine 4.

Main method of use is by subscribing to events within your blueprint, but it also supports polling through functions called on the RealSense Component. You can extend functionality to any blueprint through adding the RealSenseInterface to the blueprint interfaces and adding the RealSense Component to the components of that blueprint. This same architecture is available to C++ if you prefer, which also supports both event-driven and polling style use.

This is a very early release and neither optimized nor a complete feature set. Use with caution. 

As per RSSDK, currently only supports Windows 8 x64.

##How to install it##

1. Download [prerequisites](https://software.intel.com/en-us/intel-realsense-sdk/download): [RealSense Runtime - Direct Link](http://registrationcenter-download.intel.com/akdlm/irc_nas/7787/intel_rs_sdk_runtime_websetup_6.0.21.6598.exe)
2. Create new or open a project. 
3. Browse to your project root (typically found at *Documents/Unreal Projects/{Your Project Root}*)
4. Copy *Plugins* folder into your Project root.
5. Copy *Binaries* folder into your Project root.
6. Restart the Editor and open your project again.
7. Select Window->Plugins. Click on Installed and you should see a category called Input and a plugin called RealSense now available. It should be automatically enabled, if not, Select Enabled. The Editor will warn you to restart, click restart.
8. The plugin should be enabled and ready to use.

##How to Use - Blueprint Quick Setup##
<ol>
<li>Open desired blueprint where you want to receive the real sense events e.g. character, pawn, playercontroller, or custom actor.</li>
<li>Click on Blueprint Props and Scroll down in the details panel to Interfaces. </li>

<img src="http://i.imgur.com/HDTC69Q.png">

<li>Add the interface called RealSenseInterface</li>
<li>Add Real Sense Component to your blueprint
Add directly through components tab
<img src="http://i.imgur.com/A727g2t.png">

<li>You're ready to go.

</ol>

###Blueprint - Example Joint Debug Hands###

Copy the following graph to get a basic debug hand to show up in your editor

<img src="http://i.imgur.com/WErnWH6.png">

You can easily modify the graph to output the hands to a more realistic location by changing the added origin from actor origin to a hand offset component attached to the actor.

###Blueprint - Example Gestures###

To receive gestures you need to enable gestures before using them, e.g. in your begin play.

<img src="http://i.imgur.com/tgT2KwT.png">

Then simply subscribe to the event you are interested in. In the above example we output the body side appended to a verbose thumbs up print. This will show every time the gesture is detected.

Please note that there is currently a performance penalty when you enable gesture detection.

##Blueprint - Reference##

####Events Available####
<img src="http://i.imgur.com/B2Po9QG.png">

####Hand Properties####
Obtained via hand moved event. Polling currently not supported.
<img src="http://i.imgur.com/5fogrxv.png">

####Joint Properties####
Obtained via hand or event joint moved.
<img src="http://i.imgur.com/KlrsTbO.png">

##Credits##
Event-driven plugin by Getnamo available under MIT license.

Respective parts of this plugin fall under RSSDK licence and UE4 license.
