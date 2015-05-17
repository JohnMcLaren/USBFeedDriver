# USB FEED DRIVER

> _ARMv7 Cortex™ M3 / STM32F103_

>																				15.05.15

## Key features

- Device uses FEED protocol, appears on USB host as a standard USB **CDC** ACM port, so for the "install" device need only **inf** file (for Windows) 

- The device **can** use *interrupt* provided USB CDC class 

- High transfer speed: 4,5 Mbit / s. And this is not the limit, as the size of the buffer *Bulk* point only 62 bytes (**Full Speed** - 12 Mbit / s), which may be increased up to the maximum permitted by the USB standard in 1024 and the device is transferred to the mode of **High Speed** (480 Mbit / s).

- The small size of the code *device* driver ~ 1,5 kB (ARMv7 Cortex ™ M3 - STM32F103 / optimization -O2). 
The driver does **not use** the standard libraries (STM32F10x_StdPeriph_Lib_Vxxx / STM32_USB-FS-Device_Lib_Vxxx)

- A high ratio of the sizes of the payload to the service information in the frame. 
For example, from 255 bytes of allowable frame size, only 6 bytes are service (2.35%) for 31-bit interface addresses and  3 bytes (1.17%) for 8-bit addresses.

- Small  traffic of transfers. The minimum size frame 3 bytes (**read** operation from **8-bit** interface address).   
- Large range of interface addresses - 2 147 483 648 (31-bit addressing) 

- FEED driver frees the devices developer from having to study the work of USB and presents it **simply** as a communications link 

- FEED protocol with minimal changes can be extended to **any** other communications link 

- The integrity of each frame is protected by **CRC8**

![processfeed](https://github.com/JohnMcLaren/USBFeedDriver/blob/master/img/FeedBuffers.png)

Feed test tool (Qt example USB FEED application and uses CUSBFeed class)

![testterminal](https://github.com/JohnMcLaren/USBFeedDriver/blob/master/img/TestTerminalQt.png)

More about Feed Driver work on project page http://johnmclaren.github.io/USBFeedDriver
