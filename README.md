# ESP32 Walkie-Talkie
All firmware, documentation, and resources are available in the following repo. The ESP32 Walkie-Talkies make use of UDP over wifi in order to transmit audio. They function and operate just like every other Walkie-Talkie but with the benefits of a digital connection rather than an analog one.
+++ idk

## Features
* Oled display
* 12 bit speaker
* Built in battery with charge circuit
* Custom PCB design
* Custom 3d printed housing

## How it Works
The Walkie-Talkie makes use of a wifi enabled chip, the **WROOM-32**, in order to do digital communication via **UDP**. The ESP32 devboard has been modified to house a significantly larger antenna in order to increase the range to xxx. 

## Setup
### Arduino IDE
In order to upload the firmware to the ESP32 Walkie-Talkie, you will need to have the Arduino IDE setup for development on Espressif modules/chips**. If you already have the appropriate environemnt setup, skip this step

> Downlaod the [Arduino IDE](https://www.arduino.cc/en/software) for your respective operating system

> Open the Arduino IDE and navigate to **Arduini IDE > Preferences**. Place the following url inside the **Additional board managers URL** box.  
`https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`.

>Navigate to **Tool > Manage Libraries** and install the ***.

### Deploying
> Plug in your ESP32 Devboard and navigate to the **Board Manager** in the top left of your IDE. Select the appropriate board type and port for your respective devboard. If you are using a **WROOM32** based devboard as specified in the **BOM**, use the **WROOM-DA** board manager.
`Note: On some devboards, it may be necesary to hold down the boot button while uploading`

**Clearing Memory**
>Navigate to the **Tools > FlushEEPROM** and open the **FlushEEPROM.ino** file inside the Arduino IDE. We wrote this tool in order to allow us to flush the ESP32's memory. It also validates that all the bytes have been reset. Alternatively, you can use a web version, however, it does do any form of validation.
`https://adafruit.github.io/Adafruit_WebSerial_ESPTool/`

**Flashing Walkie-Talkie firmware**
> Navigate to the main folder and open **Main.Ino** file inside of the Arduino IDE.

## KiCad
The kicad folder contains all the project files, libraries and tools we created throughout the process. The following files can be accessed/opened via KiCad 6.

>Note: In order to properly load the project files in KiCad, you may be required to manually add both the footprint and symbol libraries manualy via the libraries manager in KiCad.

### KiCad Package Manager
Using github for version control can be a little finicky in the case of KiCad. When moving between machines, any non project specific libraries must be manually copied to the repo and then re-copied back in whenever a user pulls. In order to increase iteration time and make the whole process easier, A **CLI** tool was created to handle the copying process

> **SETUP**: When you first load the tool, you will be prompted for a initial setup, simply follow the instructions and input the file paths for both copy paths for KiCad and the Repository. Multiple copy paths can be added by using the "," seperator.

> **Commands:** Libraries can be copied between the source and destination paths by using ***push*** and ***pull*** respectively. The tool can also be reconfigured by running ***config***

