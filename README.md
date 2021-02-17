# Raproto
Application for sensor collection from Tizen-based smart watches.





# Using Raproto

Raproto is designed to be used with the Thingsboard (https://thingsboard.io/) -- although any MQTT broker/client combo can be made compatible.

The general reference API for Raproto follows the Thingsboard MQTT API: https://thingsboard.io/docs/reference/mqtt-api/

Raproto requires shared attributes to configure.  The following is a list of shared attributes available for Raproto:
In the following we write "key:Int:{x,y,z} - description" to be the key (string) and potential values Int values x,y,z and a description of the key.

ACC:INT:{-1,0,...} - accelerometery sample rate in milliseconds (-1 = off).  Default = 100. 

BATTERY:INT:{-1,0,...} - battery level sample rate in seconds (-1 = off).  Default = 60.

NAME:STR:{any name} - Name of the application. Default = ""

POWER:INT:{1} - Power optimization level (functionality not yet available)

QOS:INT:{0,1,2} - MQTT QOS level.  Default = 1.

SAVE_RATE:INT:{-1,0,...} - Autosave rate in seconds for data (functionality not yet available)

TX_RATE:INT:{-1,0,...} - Transmit rate in seconds for data. Default = 60.

WIFI:INT:{0,1,2} - WIFI configuration mode.  WIFI=0 never tries to connect/disconnect.  WIFI=1 only tries to connect.  WIFI=2 will connect/disconnect to optimize power.  Default = 1.





# Installation Instructions

1.A) Install Tizen Studio: https://developer.tizen.org/ko/development/tizen-studio/download?langredirect=1
1.B) Install the 4.0 wearable package (needed for the Samsung Galaxy Watch Active)

2.A) Clone the JSMN JSON parser: https://github.com/zserge/jsmn
2.B) copy JSMN/inc/jsmn.h to Raproto/RaprotoService/inc/jsmn.h

3.A) Download the PahoMQTT C library: https://github.com/eclipse/paho.mqtt.c/releases/tag/v1.3.8c
3.B) Compile for the target ARM architecture linking with OPENSSL (see Cross Compiling for target below)
3.C) Copy static library to Raproto/RaprotoService/lib 





# Cross Compiling MQTT Paho for Target w/ OPENSSL Linking
These instructions provide a (relatively) straight forward way to cross compile 
the PahoMQTT release for the target ARM Architecture.  There may be another way.

1) Open Tizen Studio. File -> New -> Tizen Project.  Select "Template", then click next.

2) Select version (Samsung Galaxy Watch Active = wearable 4.0).  click next.

3) Select "static library". Click Next.

4) choose your package name / ID, click finish.

5) right click the library package name in the Tizen Studio Project Explorer (right panel), select "Properties"

6) In the left panel of the properties pop-up, click C/C++ Build -> Settings  

7) In the main panel of the properties pop-up, click C compiler -> Preprocessor.

8) In the Defined Symbols (-D) window, click the "green plus sign icon" and type "OPENSSL" in the pop-up. Click Okay. 

9) Click "Apply and Close" at the lower right corner of the properties pop-up.  The pop-up will close.

10) Copy the PahoMQTT library to the "src" directory (just copy the folder)

11) After copying the entire pahomqtt directory into source, delete the following directories (they won't compile and aren't needed):
	a) src/pahomqtt/test
	b) src/pahomqtt/test_package
	c) src/pahomqtt/src/samples

12) Copy the VersionInfo.h file (Raproto/VersionInfo.h) containing the following and save it in the "inc" directory of the Tizen project for the library.

13) right click the library package name in the Tizen Studio Project Explorer (right panel), select "Build Project" (you may want to clean it first.)

14) A directory called "Archives" should have been created.  Inside is the static library (.a file).  

15) Copy the static library to Raproto/RaprotoService/lib (Same as Step 3.C in installation instructions).






