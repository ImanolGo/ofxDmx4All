# ofxDmx4All wraps DMX over serial control for [openFrameworks](http://openframeworks.cc/)

ofxDmx4All is based on ofxDmx from Marek Bereza based on DmxPro from Erik Sjodin. ofxDmx4All has been adapted by Imanol Gómez.

ofxDmx is targeted at the [Dmx4All](http://dmx4all.eu/) running the virtual serial port drivers [available from FTDI](http://www.ftdichip.com/Drivers/VCP.htm).

## About Channel Numbering

DMX is an unusual specification in that channels begin at `1`. There is no `0` channel. This means that if you have 24 channels, they will be numbered `1, 2, 3... 24`. They will not be numbered `0,1,2... 23`. Because of this, you have to more careful when you write for loops. This is correct:

	int total = 16;
	dmx.connect("tty.usbserial-ENS8KBA6", total);
	for(int channel = 1; channel <= total; channel++) {
		dmx.setLevel(channel, ...);
	}

Notice that the for loop starts at `1`, and the stopping condition is `channel <= total` rather than the usual `<` operator.


## NanoDMX USB Interface

The software was tested under MacOS Sierra using [NanoDMX](http://www.shop.dmx4all.de/de/dmx4all-produkte/ethernet-artnet-usb-dmx-interfaces/usb-dmx-interfaces/NanoDMX-USB-Interface.html). 

 - DMX-OUT: up to 512 channels (44 frames/sec. with 512 DMX- channels)
 - PC-Connection: USB, cable length ca. 1m
 - Power supply: 250mA about USB
 - USB-driver: CDC-Driver (virtual COM Port)


## Running the Example Projects

Project files for the examples are not included so you will need to generate the project files for your operating system and development environment using the OF ProjectGenerator which is included with the OpenFrameworks distribution.

To (re)generate project files for an *existing* project:

* click the "Import" button in the ProjectGenerator
* navigate the to base folder for the project ie. "example"
* click the "Update" button

If everything went Ok, you should now be able to open the generated project and build/run the example.

