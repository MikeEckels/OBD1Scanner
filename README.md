# OBD1Scanner
This device reads the data stream from GM OBD1 vehicles communicating at 8192 baud, with the 12-Pin ALDL connector. This is sometimes also referred to as "OBD 2.1". Using this project,
a simple "dataStream" can be defined to provide a "mapping" of all the byte positions for each available sensor reading on the vehicle. The one provided in this project is
specifically for a 1994 Chevy S-10 Pickup. The dataStream was created based off the A100.ads binary. Other ".ads" definition files can be found [here](https://www.tunerpro.net/downloadBinDefs.htm)
 
 # Electronics Overview
 The heart of this scanner is an arduino UNO microcontroller paired with a simple three-transistor circuit. More information on how the circuit works can be found [here](https://lukeskaff.com/projects/car/gm-obd-i-obd1-aldl-microcontroller-lcd-interface-scan-tool). All credit for this circuit goes to these people. I also created a custom
 PCB, however, this is untested.
 
 <p float="left">
  <img src="https://imgur.com/mVUplII.jpg" width="318" />
 <img src="https://imgur.com/MxWnOnw.jpg" width="500" />
 </p>
 
 # Code Overveiew
 The basic approach of this project revolves around the dataStream. This defines all byte mappings of each sensor so the program can properly decode them. Once a dataStream
 is defined, we must construct a command. A command is a five byte long message sent to the vehicle to initiate a conversation. The first byte is the message ID, the second is
 the length, then the mode type, an unknown value, and finally the two's compliment checksum of the entire command.
 
 ```C
 //message ID, length, mode type, unknown, 2's compliment checksum
 unsigned char mode1Cmd[] = {0xF4, 0x57, 0x01, 0x00, 0xB4};
 ```
 
 Finally, we can simply pass the command, it's length, and a buffer to the "getAldlData()" function, process the data, and display it over the serial monitor with software
 serial.
 
 ```C
 unsigned char rawDataStream1[DATA_STREAM_LEN];
 unsigned char rawDataStream2[DATA_STREAM_LEN];

 // Not recommended to call both mode1 and mode2 commands consecutively, but it does work.
 getAldlData(mode1Cmd, mode1CmdLength, rawDataStream1);
 processMode1Data(rawDataStream1, &stream1);
 displayData1(&stream1);
 ```

