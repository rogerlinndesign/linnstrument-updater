LinnStrument Firmware Updater
=============================

This is a GUI tool written in Juce to easily update the firmware of a LinnStrument.

The firmware update file should be placed alongside the updater application and will be automatically detected. 

To get this .bin file, go to the Arduino IDE preferences and enable "Show verbose output during: compilation". Then, when you press the Verify button (the left-most) when the linnstrument-firmware sketch is open, it will compile the firmware and the before-last line will end with something like: /var/folders/nz/x2b_qbpj6j94wjq9jf8r1sx00000gn/T/build8779920126312548776.tmp/linnstrument.cpp.bin. This is the location of the compiled firmware. Copy the .bin file from there to the folder with the LinnStrument Updater and give it a name like linnstrument-firmware-103.bin. This name isn't necessary per-se, it will work with any .bin file for convenience, but it is a nice naming scheme to adopt.

On Windows, the compiled executable expects a 'tools' directory alongside it with the 'bossac.exe' binary. This is needed to perform the actual firmware update with a standard tool. The Visual Studio build steps will automatically create this in the build directories, just make sure to correctly more the 'tools' directory with 'LinnStrument Updater.exe'.

Thanks to Tod E. Kurt (http://todbot.com/blog/) and his usbSearch project for inspiration of how to detect Arduino devices: https://github.com/todbot/usbSearch

Thanks to the node-serialport project for some of the USB serial device detection code on MacOSX: https://github.com/voodootikigod/node-serialport

The bossac tool was copied from the Arduino IDE distribution and was created by ShumaTech http://www.shumatech.com/web/products/bossa

In order to codesign on windows, we use the following command with the eToken dongle connected:
signtool sign /n "Roger Linn Design" /tr http://timestamp.globalsign.com/tsa/r6advanced1 /td SHA256 /fd SHA256 "LinnStrument Updater.exe"