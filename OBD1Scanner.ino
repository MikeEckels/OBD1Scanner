  #include "DataStreamDefs.h"
  #include <SoftwareSerial.h>

  int rxControl = 4;
  bool dataReady = false;
  
  unsigned long baudRate = 8192;
  
  unsigned char mode1[] = {0xF4, 0x57, 0x01, 0x00, 0xB4};
  unsigned char mode2[] = {0xF4, 0x57, 0x01, 0x01, 0xB3};
  unsigned char clearCodes[] = {0xF4, 0x56, 0x0A, 0xAC};
  
  unsigned int watchdogTimeout = 12000;
  unsigned int mode1Length = sizeof(mode1) / sizeof(mode1[0]);
  unsigned int mode2Length = sizeof(mode2) / sizeof(mode2[0]);
  unsigned int clearCodesLength = sizeof(clearCodes) / sizeof(clearCodes[0]);
  
  SoftwareSerial softSerial(2, 3); // RX, TX
  
  void setup() {
    Serial1.begin(baudRate);
    softSerial.begin(8192);
    pinMode(rxControl, OUTPUT);
  }

  
  void loop() {
    unsigned char dataStream[DATA_LEN];
    //getAldlData(mode1, mode1Length, dataStream);
    getAldlData(mode2, mode2Length, dataStream);
    //processMode1Data(dataStream);
    processMode2Data(dataStream);
  }


  void getAldlData(unsigned char *cmd, unsigned int cmdLength, unsigned char *data) {
    unsigned int watchdog = 0;
  
    digitalWrite(rxControl, HIGH);
    Serial1.write(cmd, cmdLength);
    Serial1.flush();//Must finish sending all data before attempting to read
    digitalWrite(rxControl, LOW);
  
      while ((Serial1.available() == 0) && (watchdog < watchdogTimeout)) {
        watchdog++;
      }
  
      if (watchdog >= watchdogTimeout) {
        softSerial.println("[!] Watchdog Timeout");
        dataReady = false;
      } else {
          Serial1.readBytes(data, DATA_LEN);
          dataReady = true;
      }
    digitalWrite(rxControl, HIGH);
  }
  
  
  void processMode1Data(unsigned char *data) {
    if (dataReady) {
    
      float temp;

       // Calculate RPM
      temp = data[RPM_MSB + DATA_OFFSET];
      temp += data[RPM_LSB + DATA_OFFSET] * 10;
      softSerial.print((String)"RPM: " + temp);
      softSerial.print("\t");

      //Calculate Desired RPM
      temp = data[IDLE_DSRD_RPM + DATA_OFFSET] * 25;
      softSerial.print((String)"RPMd: " +temp);
      softSerial.print("\t");
  
      // Calculate MPH
      temp = data[VSS_MPH + DATA_OFFSET];
      softSerial.print((String)"MPH: " + temp);
      softSerial.print("\t");
    
      // Calculate Throttle position sensor percent
      temp = data[TPS_P + DATA_OFFSET];
      softSerial.print((String)"TPS%: " + temp);
      softSerial.print("\t");
  
      // Calculate Manifold air pressure
      temp = (float)(data[MAP_KPA + DATA_OFFSET] * 0.5f);
      softSerial.print((String)"MAP: " + temp);
      softSerial.print("\t");
  
      // Calculate Air fuel reatio target
      temp = data[AFR_TARGET + DATA_OFFSET];
      softSerial.print((String)"AFRt: " + temp) / 10.0f;
      softSerial.print("\t");
  
      temp = data[RICH_LEAN + DATA_OFFSET];
      softSerial.print((String)"R/L: " + temp);
      softSerial.print("\t");
      
      // Calculate coolant temperature C
      temp = (float)data[CTS + DATA_OFFSET] * 0.75f - 40.0f;
      softSerial.print((String)"CTSc: " + temp);
      softSerial.print("\t");
  
      // Calculate coolant temperature F
      temp = (float)data[CTS + DATA_OFFSET] * 1.35f - 40.0f;
      softSerial.print((String)"CTSf: " + temp);
      softSerial.print("\t");
  
      // Calculate Battery voltage
      temp = (float)data[BATT_V + DATA_OFFSET] / 10.0f;
      softSerial.print((String)"BATv: " + temp);
      softSerial.print("\t");

      // Calcualte engine runtime
      temp = data[ENG_RUN_MSB_SEC + DATA_OFFSET];
      temp += data[ENG_RUN_LSB_SEC + DATA_OFFSET];
      softSerial.print((String)"RNTs: " + temp);
      softSerial.println();
    }
  }

  void processMode2Data(unsigned char *data) {
    if (dataReady) {
      char temp;

      temp = data[VIN1 + DATA_OFFSET];
      softSerial.print((String)"VIN #: " + temp);

      temp = data[VIN2 + DATA_OFFSET];
      softSerial.print(temp);

      temp = data[VIN3 + DATA_OFFSET];
      softSerial.print(temp); 

      temp = data[VIN4 + DATA_OFFSET];
      softSerial.print(temp);

      temp = data[VIN5 + DATA_OFFSET];
      softSerial.print(temp);

      temp = data[VIN6 + DATA_OFFSET];
      softSerial.print(temp);

      temp = data[VIN7 + DATA_OFFSET];
      softSerial.print(temp); 

      temp = data[VIN8 + DATA_OFFSET];
      softSerial.print(temp); 

      temp = data[VIN9 + DATA_OFFSET];
      softSerial.print(temp);

      temp = data[VIN10 + DATA_OFFSET];
      softSerial.print(temp); 

      temp = data[VIN11 + DATA_OFFSET];
      softSerial.print(temp);

      temp = data[VIN12 + DATA_OFFSET];
      softSerial.print(temp);

      temp = data[VIN13 + DATA_OFFSET];
      softSerial.print(temp);

      temp = data[VIN14 + DATA_OFFSET];
      softSerial.print(temp); 

      temp = data[VIN15 + DATA_OFFSET];
      softSerial.print(temp);

      temp = data[VIN16 + DATA_OFFSET];
      softSerial.print(temp);

      temp = data[VIN17 + DATA_OFFSET];
      softSerial.print(temp); 
      softSerial.println();
    }
  }
  
