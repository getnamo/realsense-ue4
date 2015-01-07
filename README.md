realsense-ue4
====================

An event-driven [RealSense](http:/www.intel.com/realsense) plugin for the Unreal Engine 4.

Main method of use is by subscribing to events within your blueprint, but it also supports polling through functions called on the RealSenseComponent. You can extend functionality to any blueprint through adding the RealSenseInterface to the blueprint interfaces and adding the RealSenseComponent to the components of that blueprint. This same architecture is available to C++ if you prefer, which also supports both event-driven and polling style use.

This is a very early release and neither optimized nor a complete feature set. Use with caution.

##How to install it##

1. Create new or open a project. 
2. Browse to your project root (typically found at *Documents/Unreal Projects/{Your Project Root}*)
3. Copy *Plugins* folder into your Project root.
4. Copy *Binaries* folder into your Project root.
5. Restart the Editor and open your project again.
6. Select Window->Plugins. Click on Installed and you should see a category called Input and a plugin called RealSense now available. It should be automatically enabled, if not, Select Enabled. The Editor will warn you to restart, click restart.
7. The plugin should be enabled and ready to use.