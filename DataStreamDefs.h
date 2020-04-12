
  #pragma once

  //Arduino clock frequency
  #define F_CPU               16000000L

  //Length in bytes of vechicles data stream
  #define DATA_LEN            44

  //Num of bytes before actual data in the stream
  //3 in old design. Something weird with Serial1.readBytes();
  #define DATA_OFFSET         2

  //Defines each byte in the mode1 data stream
  #define CTS                 4 //Coolant temp raw
  #define BATT_V              5 //Battery voltage
  #define TPS_V               6 //Throttle position voltage
  #define TPS_P               7 //Throttle position percent
  #define MAP_KPA             8 //Manifold absolute pressure Kpa
  #define VSS_MPH             9 //Vehicle speed MPH
  #define AFR_TARGET          10 //Air fuel ratio target
  #define INT                 11 //Integrator (short term fuel trim)
  #define BLM                 12 //Block learn multiplier (long term fuel trim)
  #define BLM_CELL            13 //"Lookup table" for different conditions (fuel trims)
  #define BPW_MSB_MSEC        14 //Base pulse width (fuel injectors) milli seconds
  #define BPW_LSB_MSEC        15
  #define O2S_MV              16 //Oxygen sensor milli volts
  #define IDLE_DSRD_RPM       17 //Desired idle speed RPM
  #define EGR_PWM_P           18 //Exhaust gas recirculation valve percent
  #define IAC_POS_STEPS       19 //Idler air control valve steps
  #define RICH_LEAN           20 //Is it running rich or lean 
  #define SPRK_ADV_MSB        21 //Spark advance If wrong, use LSB only
  #define SPRK_ADV_LSB        22
  #define RPM_MSB             23 //Engine RPM
  #define RPM_LSB             24
  #define ENG_RUN_MSB_SEC     25 //Engine run time seconds
  #define ENG_RUN_LSB_SEC     26
  #define FUEL_PUMP_V         27 //Fuel pump voltage
  #define STRT_CTS            28 //Startup coolant temp raw
  #define IAC_DSRD_POS_STEPS  29 //Idler air control valve position steps
  #define REF_PULS_MSB_MSEC   30 //time between reference pulse -- square wave for distributor milli seconds
  #define REF_PULS_LSB_MSEC   31 //
  
  #define ESC_ACTVTY_MSB_SEC  34 //Electronic spark control activity (Advancing or retarding timing)
  #define ESC_ACTVTY_LSB_SEC  35
  #define KNOCK_RETARD_MSB    36 //Knock retard
  #define KNOCK_RETARD_LSB    37
  #define ASYNC_PW_MSB_MSEC   38 //Asynchronous pulse width milli seconds
  #define ASYNC_PW_LSB_MSEC   39
  #define AFR_TIMEOUT         40 //Air fuel ratio timeout
  #define BAROMETRIC_KPA      41 //Barometric pressure Kpa


  //Defines each byte in the mode2 data stream
  #define OS_CAL1_MSB         1 //Oxygen sensor calibration 1
  #define OS_CAL1_LSB         2
  #define OS_CAL2_MSB         3 //Oxygen sensor calibration 2
  #define OS_CAL2_LSB         4
  #define ENG_CAL1_MSB        5 //Engine calibration 1
  #define ENG_CAL1_LSB        6
  #define ENG_CAL2_MSB        7 //Engine calibration 2
  #define ENG_CAL2_LSB        8
  #define SPEDO_CAL1_MSB      9 //Speedometer calibration 1
  #define SPEDO_CAL1_LSB      10
  #define SPEDO_CAL2_MSB      11 //Speedometer calibration 2
  #define SPEDO_CAL2_LSB      12
  #define EBCM_CAL1_MSB       13 //Electronic brake control module calibration 1
  #define EBCM_CAL1_LSB       14
  #define EBCM_CAL2_MSB       15 //Electronic brake control module calibration 2
  #define EBCM_CAL2_LSB       16
  #define HVAC_CAL1_MSB       17 //Heating Ventilation and air conditionin calibration 1
  #define HVAC_CAL1_LSB       18
  #define HVAC_CAL2_MSB       19 //Heating Ventilation and air conditionin calibration 2
  #define HVAC_CAL2_LSB       20
  #define VIN1                21 //VIN number dump. Digits 1-17
  #define VIN2                22
  #define VIN3                23
  #define VIN4                24
  #define VIN5                25
  #define VIN6                26
  #define VIN7                27
  #define VIN8                28
  #define VIN9                29
  #define VIN10               30
  #define VIN11               31
  #define VIN12               32
  #define VIN13               33
  #define VIN14               34
  #define VIN15               35
  #define VIN16               36
  #define VIN17               37
              
  
