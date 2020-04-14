  #include "DataStreamDefs.h"
  #include <SoftwareSerial.h>

  int rxControl = 4;
  bool dataReady = false;
  
  unsigned long baudRate = 8192;

  //message ID, length, mode type, unknown, 2's compliment checksum
  unsigned char mode1Cmd[] = {0xF4, 0x57, 0x01, 0x00, 0xB4};
  unsigned char mode2Cmd[] = {0xF4, 0x57, 0x01, 0x01, 0xB3};
  unsigned char clearCodesCmd[] = {0xF4, 0x56, 0x0A, 0xAC};
  
  unsigned int watchdogTimeout = 12000;
  unsigned int mode1CmdLength = sizeof(mode1Cmd) / sizeof(mode1Cmd[0]);
  unsigned int mode2CmdLength = sizeof(mode2Cmd) / sizeof(mode2Cmd[0]);
  unsigned int clearCodesCmdLength = sizeof(clearCodesCmd) / sizeof(clearCodesCmd[0]);
  
  SoftwareSerial softSerial(2, 3); // RX, TX
  
  void setup() {
    Serial1.begin(baudRate);
    softSerial.begin(8192);
    pinMode(rxControl, OUTPUT);
  }

  
  void loop() {
    struct mode1Data stream1;
    struct mode2Data stream2;
    
    unsigned char rawDataStream1[DATA_STREAM_LEN];
    unsigned char rawDataStream2[DATA_STREAM_LEN];

    // Not recommended to call both mode1 and mode2 commands consecutively, but it does work.
    getAldlData(mode1Cmd, mode1CmdLength, rawDataStream1);
    processMode1Data(rawDataStream1, &stream1);
    displayData1(&stream1);

/*
    getAldlData(mode2Cmd, mode2CmdLength, rawDataStream2);
    processMode2Data(rawDataStream2, &stream2);
    displayData2(&stream2);
*/
  }


  void getAldlData(unsigned char *cmd, unsigned int cmdLength, unsigned char *dataBuffer) {
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
          Serial1.readBytes(dataBuffer, DATA_STREAM_LEN);
          dataReady = true;
      }
    digitalWrite(rxControl, HIGH);
  }
  
  
  void processMode1Data(unsigned char *data, struct mode1Data *mode1) {
    if (dataReady) {

      mode1->coolantTempF = data[CTS + DATA_OFFSET] * 1.35f - 40.0f;
      mode1->coolantTempC = data[CTS + DATA_OFFSET] * 0.75f - 40.0f;
      mode1->batteryVoltage = data[BATT_V + DATA_OFFSET] / 10.0f;
      mode1->throttleVoltage = data[TPS_V + DATA_OFFSET] / 10.0f;
      mode1->throttlePercent = data[TPS_P + DATA_OFFSET];
      mode1->MAP = data[MAP_KPA + DATA_OFFSET] * 0.5f;
      mode1->vehicleSpeed = data[VSS_MPH + DATA_OFFSET];
      mode1->targetAFR = data[AFR_TARGET + DATA_OFFSET] / 10.0f;
      mode1->fuelINT = data[INT + DATA_OFFSET];
      mode1->fuelBLM = data[BLM + DATA_OFFSET];
      mode1->fuelBLMcell = data[BLM_CELL + DATA_OFFSET];
      mode1->fuelBPW = ((data[BPW_MSB_MSEC + DATA_OFFSET]) + (data[BPW_LSB_MSEC + DATA_OFFSET])) * 0.015259;
      mode1->oxygenSensor = data[O2S_MV + DATA_OFFSET] * 5;
      mode1->desiredIdle = data[IDLE_DSRD_RPM + DATA_OFFSET] * 25;
      mode1->EGRdutyCycle = data[EGR_PWM_P + DATA_OFFSET] * 0.5f;
      mode1->IACposition = data[IAC_POS_STEPS + DATA_OFFSET];
      mode1->exhaustTransitions = data[RICH_LEAN + DATA_OFFSET];
      mode1->sparkAdvance = ((data[SPRK_ADV_MSB + DATA_OFFSET]) + (data[SPRK_ADV_LSB + DATA_OFFSET])) * 0.062500f;
      mode1->RPM = ((data[RPM_MSB + DATA_OFFSET]) + (data[RPM_LSB + DATA_OFFSET])) * 10;
      mode1->engineRuntime = ((data[ENG_RUN_MSB_SEC + DATA_OFFSET]) + (data[ENG_RUN_LSB_SEC + DATA_OFFSET]));
      mode1->fuelPumpVoltage = data[FUEL_PUMP_V + DATA_OFFSET] / 10.0f;
      mode1->startCoolantTempF = data[STRT_CTS + DATA_OFFSET] * 1.35f - 40.0f;
      mode1->startCoolantTempC = data[STRT_CTS + DATA_OFFSET] * 0.75f - 40.0f;
      mode1->desiredIACposition = data[IAC_DSRD_POS_STEPS + DATA_OFFSET];
      mode1->referencePulseTime = ((data[REF_PULS_MSB_MSEC + DATA_OFFSET]) + (data[REF_PULS_LSB_MSEC + DATA_OFFSET])) * 0.02f;
      mode1->ESCactivity = ((data[ESC_ACTVTY_MSB_SEC + DATA_OFFSET]) + (data[ESC_ACTVTY_LSB_SEC + DATA_OFFSET])) * 0.02f;
      mode1->knockRetard = ((data[KNOCK_RETARD_MSB + DATA_OFFSET]) + (data[KNOCK_RETARD_LSB + DATA_OFFSET])) * 0.0625f;
      mode1->asyncPulseWidth = ((data[ASYNC_PW_MSB_MSEC + DATA_OFFSET]) + (data[ASYNC_PW_LSB_MSEC + DATA_OFFSET])) / 10.0f;
      mode1->AFRtimeout = data[AFR_TIMEOUT + DATA_OFFSET] / 10.0f;
      mode1->barometric = data[BAROMETRIC_KPA + DATA_OFFSET] * 0.5f;
    }
  }


  void processMode2Data(unsigned char *data, struct mode2Data *mode2) {
    if (dataReady) {

      mode2->oxygenCal1 = ((data[OS_CAL1_MSB + DATA_OFFSET]) + (data[OS_CAL1_LSB + DATA_OFFSET]));
      mode2->oxygenCal2 = ((data[OS_CAL2_MSB + DATA_OFFSET]) + (data[OS_CAL2_LSB + DATA_OFFSET]));
      mode2->engineCal1 = ((data[ENG_CAL1_MSB + DATA_OFFSET]) + (data[ENG_CAL1_LSB + DATA_OFFSET]));
      mode2->engineCal2 = ((data[ENG_CAL2_MSB + DATA_OFFSET]) + (data[ENG_CAL2_LSB + DATA_OFFSET]));
      mode2->speedoCal1 = ((data[SPEDO_CAL1_MSB + DATA_OFFSET]) + (data[SPEDO_CAL1_LSB + DATA_OFFSET]));
      mode2->speedoCal2 = ((data[SPEDO_CAL2_MSB + DATA_OFFSET]) + (data[SPEDO_CAL2_LSB + DATA_OFFSET]));
      mode2->EBCMcal1 = ((data[EBCM_CAL1_MSB + DATA_OFFSET]) + (data[EBCM_CAL1_LSB + DATA_OFFSET]));
      mode2->EBCMcal2 = ((data[EBCM_CAL2_MSB + DATA_OFFSET]) + (data[EBCM_CAL2_LSB + DATA_OFFSET]));
      mode2->HVACcal1 = ((data[HVAC_CAL1_MSB + DATA_OFFSET]) + (data[HVAC_CAL1_LSB + DATA_OFFSET]));
      mode2->HVACcal2 = ((data[HVAC_CAL2_MSB + DATA_OFFSET]) + (data[HVAC_CAL2_LSB + DATA_OFFSET]));

      for (unsigned int i = 0; i < VIN_LEN; i++) {
        mode2->VIN[i] = data[(VIN1 + DATA_OFFSET) + i];
      }
      
    }
  }
  

  void displayData1(struct mode1Data *processedData) {
    
    if (dataReady) {
      
      //PRINTING MODE1 DATA
      softSerial.print((String)"RPM: " + processedData->RPM);
      softSerial.print("\t");

      softSerial.print((String)"RPMd: " + processedData->desiredIdle);
      softSerial.print("\t");
  
      softSerial.print((String)"MPH: " + processedData->vehicleSpeed);
      softSerial.print("\t");
    
      softSerial.print((String)"TPS%: " + processedData->throttlePercent);
      softSerial.print("\t");
  
      softSerial.print((String)"MAP: " + processedData->MAP);
      softSerial.print("\t");
  
      softSerial.print((String)"AFRt: " + processedData->targetAFR);
      softSerial.print("\t");
  
      softSerial.print((String)"R/L: " + processedData->exhaustTransitions);
      softSerial.print("\t");
      
      softSerial.print((String)"CTSc: " + processedData->coolantTempC);
      softSerial.print("\t");
  
      softSerial.print((String)"CTSf: " + processedData->coolantTempF);
      softSerial.print("\t");
  
      softSerial.print((String)"BATv: " + processedData->batteryVoltage);
      softSerial.print("\t");

      softSerial.print((String)"RNTs: " + processedData->engineRuntime);
      softSerial.println();
    }
  }


  void displayData2(struct mode2Data *processedData) {
    
    if (dataReady) {

      //PRINTIG MODE2 DATA
      softSerial.print((String)"O2Cal1: " + processedData->oxygenCal1);
      softSerial.print("\t");

      softSerial.print((String)"O2Cal2: " + processedData->oxygenCal2);
      softSerial.print("\t");

      softSerial.print((String)"ENGCal1: " + processedData->engineCal1);
      softSerial.print("\t");

      softSerial.print((String)"ENGCal2: " + processedData->engineCal2);
      softSerial.print("\t");

      softSerial.print((String)"SPDCal1: " + processedData->speedoCal1);
      softSerial.print("\t");

      softSerial.print((String)"SPDCal2: " + processedData->speedoCal2);
      softSerial.print("\t");

      softSerial.print((String)"EBCMCal1: " + processedData->EBCMcal1);
      softSerial.print("\t");

      softSerial.print((String)"EBCMCal2: " + processedData->EBCMcal2);
      softSerial.print("\t");

      softSerial.print((String)"HVACCal1: " + processedData->HVACcal1);
      softSerial.print("\t");

      softSerial.print((String)"HVACCal2: " + processedData->HVACcal2);
      softSerial.print("\t");

      
      softSerial.print("VIN: ");
            for (unsigned int i = 0; i < VIN_LEN; i++) {
        softSerial.print((char)processedData->VIN[i]);
      }
      softSerial.println();
    }
  }

  
