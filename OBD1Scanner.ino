  #include "DataStreamDefs.h"
  #include <SoftwareSerial.h>

  int rxControl = 13;
  int payloadOffset =2;//3 in old design. Something weird with Serial1.readBytes();
  bool dataReady = false;
  const int dataStreamLength = 44;
  unsigned int watchdogTimeout = 12000;
  
  unsigned long baudRate = 8192;
  
  unsigned char dataStream[dataStreamLength];
  
  unsigned char mode1[] = {0xF4, 0x57, 0x01, 0x00, 0xB4};
  unsigned char mode2[] = {0xF4, 0x57, 0x01, 0x01, 0xB3};
  unsigned char clearCodes[] = {0xF4, 0x56, 0x0A, 0xAC};
  
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
    getAldlData();
    processData();
  }


  void getAldlData(void) {
    unsigned int watchdog = 0;
  
    digitalWrite(rxControl, HIGH);
    Serial1.write(mode1, mode1Length);
    Serial1.flush();//Must finish sending all data before attempting to read
    digitalWrite(rxControl, LOW);
  
      while ((Serial1.available() == 0) && (watchdog < watchdogTimeout)) {
        watchdog++;
      }
  
      if (watchdog >= watchdogTimeout) {
        softSerial.println("[!] Watchdog Timeout");
        dataReady = false;
      } else {
          Serial1.readBytes(dataStream, dataStreamLength);
          dataReady = true;
      }
    digitalWrite(rxControl, HIGH);
  }
  
  
  void processData()
  {
    if (dataReady) {
    
      float temp;

       // Calculate RPM
      temp = dataStream[RPM_MSB + payloadOffset];
      temp += dataStream[RPM_LSB + payloadOffset] * 10;
      softSerial.print((String)"RPM: " + temp);
      softSerial.print("\t");

      //Calculate Desired RPM
      temp = dataStream[IDLE_DSRD_RPM + payloadOffset] * 25;
      softSerial.print((String)"RPMd: " +temp);
      softSerial.print("\t");
  
      // Calculate MPH
      temp = dataStream[VSS_MPH + payloadOffset];
      softSerial.print((String)"MPH: " + temp);
      softSerial.print("\t");
    
      // Calculate Throttle position sensor percent
      temp = dataStream[TPS_P + payloadOffset];
      softSerial.print((String)"TPS%: " + temp);
      softSerial.print("\t");
  
      // Calculate Manifold air pressure
      temp = (float)(dataStream[MAP_KPA + payloadOffset] * 0.5f);
      softSerial.print((String)"MAP: " + temp);
      softSerial.print("\t");
  
      // Calculate Air fuel reatio target
      temp = dataStream[AFR_TARGET + payloadOffset];
      softSerial.print((String)"AFRt: " + temp) / 10.0f;
      softSerial.print("\t");
  
      temp = dataStream[RICH_LEAN + payloadOffset];
      softSerial.print((String)"R/L: " + temp);
      softSerial.print("\t");
      
      // Calculate coolant temperature C
      temp = (float)dataStream[CTS + payloadOffset] * 0.75f - 40.0f;
      softSerial.print((String)"CTSc: " + temp);
      softSerial.print("\t");
  
      // Calculate coolant temperature F
      temp = (float)dataStream[CTS + payloadOffset] * 1.35f - 40.0f;
      softSerial.print((String)"CTSf: " + temp);
      softSerial.print("\t");
  
      // Calculate Battery voltage
      temp = (float)dataStream[BATT_V + payloadOffset] / 10.0f;
      softSerial.print((String)"BATv: " + temp);
      softSerial.print("\t");

      // Calcualte engine runtime
      temp = dataStream[ENG_RUN_MSB_SEC + payloadOffset];
      temp += dataStream[ENG_RUN_LSB_SEC + payloadOffset];
      softSerial.print((String)"RNTs: " + temp);
      softSerial.println();
    }
  }
  
