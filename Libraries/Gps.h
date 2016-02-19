
/** @archivo			Gps.h
  * @descripcion	mbed GTM 202 GPS module Library
  *
	* @creacion			04/02/2016
	* @autor				Juan Pablo Garcia
  */
	
	#ifndef GPS_H
	#define GPS_H
	
	#include "mbed.h"
	#include "MODSERIAL.h"
	
	#define DEBUG_ON true
	
	#define GPS_BAUDRATE 	9600
	
	MODSERIAL Gps(P0_15, P0_16, 1024, 1024);
	Timeout gpsTimeout;
	Timeout realGpsDataTimeout;
	
	
	
	const char UbxCfgRxmMPM[10] = {0xB5,0x62,0x06,0x11,0x02,0x00,0x08,0x00,0x21,0x91};
  //10 datos de respuesta      		B5 		62 	05 		01 	02 		00 	 06   11   1F   48
	const char UbxCfgRxmEM[10] = {0xB5,0x62,0x06,0x11,0x02,0x00,0x08,0x04,0x25,0x95};
	//10 datos de respuesta      B5 62 05 01 02 00 06 11 1F 48
	const char UbxCfgNav5SHT[44]	= {0xB5,0x62,0x06,0x24,0x24,0x00,0xFF,0xFF,0x00,0x03,0x00,
																	 0x00,0x00,0x00,0x10,0x27,0x00,0x00,0x05,0x00,0xFA,0x00,
																	 0xFA,0x00,0x64,0x00,0x2C,0x01,0xC8,0x00,0x00,0x00,0x00,
																	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD8,0x00
	};// 2.00 m/s
		
  const char mtkSetStaticNav[18]="$PMTK386,2.0*3F\r\n";
	
	bool gpsTimeoutStatus = false;
	bool gpsStatusChanged = false;
	bool gpsStatusLastValue = true;
	bool gpsStatus = false;
														 
	bool realGpsData = false;

	char gpsStr[1024]= "";	
	float gpsUtc= false;
	char gpsFix = NULL;
	double gpsLatitude = 0.0;
	char gpsNs = NULL;
	double gpsLongitude = 0.0;
	char gpsEw = NULL;
	float gpsSog = 0.0;
	float gpsCog = 0.0;
	int gpsDate = 0;
	
	char *trash;
	
	void gpsTimeoutHandler(void);
	void gpsTimeoutHandlerError(void);
	void gpsConfig(void);
	bool gpsCmdReady(void);
	void gpsMaxPerfModeUblox(void);
	void gpsStaticHoldModeUblox(void);
	void gpsStaticHoldModeMtk(void);
	int gpsProcess(void);
	bool gpsGetRmc(void);
	void gpsProcessData(void);
	
	void fakeGpsData(void);
	void fakeGpsRmc(void);
	void realDataGpsTimeoutHandler(void);
	
	void gpsTimeoutHandler(void){
		gpsTimeoutStatus = true;
	}
	
	void GpsTimeoutHandlerError(void){
		gpsTimeout.attach(&gpsTimeoutHandler,1.5);
		gpsStatus = false;
		if(gpsStatusLastValue != gpsStatus){
			gpsStatusChanged = true;
//			gpsLedStatus = ledStatusError;
//			gpsUpdateLedStatus();
		}
		gpsStatusLastValue = gpsStatus;
		return;
	}
	
	void gpsConfig(void){
		#ifdef DEBUG_ON
//		Debug.print("---------\r\n");
//		Debug.printf("Gps Init\r\n");
		#endif
		
		if(gpsCmdReady())
			gpsStaticHoldModeMtk();
		else
			return;
		if(gpsCmdReady())
			gpsStaticHoldModeUblox();
		else
			return;
		if(gpsCmdReady())
			gpsMaxPerfModeUblox();
		return;
	}
	
	bool gpsCmdReady(void){
		
		gpsTimeoutStatus = false;
		gpsTimeout.attach(&gpsTimeoutHandler,1.0);
		char c = NULL;
		while(1){
			if(Gps.readable()){
				gpsTimeout.attach(&gpsTimeoutHandler,0.1);
				c = Gps.getc();
				if(c == '\n'){
					gpsTimeout.attach(&gpsTimeoutHandler,1.5);
					gpsStatus = true;
					if(gpsStatusLastValue != gpsStatus){
						gpsStatusChanged = true;
//					gpsLedStatus = ledStatusOff;
//					gpsUpdateLedStatus();
					}
					gpsStatusLastValue = gpsStatus;
					return true;
				}
			}
			else if(gpsTimeoutStatus){
				gpsTimeout.attach(&gpsTimeoutHandler,1.5);
					gpsStatus = false;
					if(gpsStatusLastValue != gpsStatus){
						gpsStatusChanged = true;
//					gpsLedStatus = ledStatusError;
//					gpsUpdateLedStatus();
					}
					gpsStatusLastValue = gpsStatus;
					return false;
			}
		}
	}
	void gpsMaxPerfModeUblox(void){
		#ifdef DEBUG_ON
//		Debug.printf("Gps Maximun Performance Mode - Ublox\r\n");
		#endif
		for(int GMPMUCounter = 0; GMPMUCounter < 10; GMPMUCounter++)
				Gps.putc(UbxCfgRxmMPM[GMPMUCounter]);
		return;
}

	void gpsStaticHoldModeUblox(void){
		#ifdef DEBUG_ON
//		Debug.printf("Gps Static Hold Mode - Mtk\r\n");
		#endif
		for (int GSHMUCounter = 0; GSHMUCounter <44; GSHMUCounter++)
		Gps.putc(mtkSetStaticNav[GSHMUCounter]);
		return;
	}
	
	void gpsStaticHoldModeMtk(void){
		#ifdef DEBUG_ON
//		Debug.printf("Gps Static Hold Mode - Mtk\r\n");
		#endif
		for (int GSHMMCounter = 0; GSHMMCounter <18; GSHMMCounter++)
		Gps.putc(mtkSetStaticNav[GSHMMCounter]);
		return;
	}
	
	int gpsProcess(void){
		if(!Gps.readable()){
			if(!realGpsData){
				fakeGpsData();
				return 1;
			}
			return 0;
		}
		
		if(!gpsGetRmc()){
			if(!realGpsData){
				fakeGpsRmc();
				return 1;
			}
			return 0;
		}
		
		gpsProcessData();
		
		#ifdef DEBUG_ON
//		Debug.printf("Data Processed - Mtk\r\n");
		#endif
		
		return 1;
	}
	
	void fakeGpsData(void)
	{
		gpsLatitude = 9999.0;
		gpsLongitude = -202.0;
		gpsSog  = 0.0;
		gpsCog = 0.0;
		gpsDate = 0;
		gpsUtc = 0.0;
		
		return;
	}
	void fakeGpsRmc(void)
	{
		gpsLatitude = 101.0;
		gpsLongitude = -201.0;
		gpsSog  = 0.0;
		gpsCog = 0;
		gpsUtc = 0.0;
		
		return;
	}
	
	
	bool gpsGetRmc(void){
		gpsTimeoutStatus = false;
		gpsTimeout.attach(&gpsTimeoutHandler, 1.0);
		char c = NULL;
		int GGLIndex = 0;
		memset(gpsStr, NULL, sizeof(gpsStr));
		while(1){
			if(Gps.readable()){
				c = Gps.getc();
				if(c == '$'){
					gpsTimeout.attach(&gpsTimeoutHandler, 1.5);
					gpsStatus = true;
					if(gpsStatusLastValue != gpsStatus){
						gpsStatusChanged = true;
//						gpsLedStatus = ledStatusOff;
//						gpsUpdateLedStatus();
					}
					gpsStatusLastValue = gpsStatus;
					break;
				}
			}
			else if(gpsTimeoutStatus){
				gpsTimeout.attach(&gpsTimeoutHandler,1.5);
					gpsStatus = false;
					if(gpsStatusLastValue != gpsStatus){
						gpsStatusChanged = true;
//					gpsLedStatus = ledStatusError;
//					gpsUpdateLedStatus();
					}
					gpsStatusLastValue = gpsStatus;
					return false;
			}
		}
		gpsStr[GGLIndex++] = c;
		while(1){
			if(Gps.readable()){
				c = Gps.getc();
				if(c == '\n'){
					gpsTimeout.attach(&gpsTimeoutHandler,1.5);
					gpsStatus = true;
					if(gpsStatusLastValue != gpsStatus){
						gpsStatusChanged = true;
//					gpsLedStatus = ledStatusOff;
//					gpsUpdateLedStatus();
					}
					gpsStatusLastValue = gpsStatus;
					
					gpsStr[GGLIndex] = c;
					gpsStr[GGLIndex+1] = NULL;
					if(strncmp(gpsStr, "$GPRMC", 6) == 0){
						return true;
					}
					return false;
				}
				gpsStr[GGLIndex++] = c;
			}
			else if (gpsTimeoutStatus){
				gpsTimeout.attach(&gpsTimeoutHandler,1.5);
					gpsStatus = false;
					if(gpsStatusLastValue != gpsStatus){
						gpsStatusChanged = true;
//					gpsLedStatus = ledStatusError;
//					gpsUpdateLedStatus();
					}
					gpsStatusLastValue = gpsStatus;
					return false;
			}
		}
	}
	
	void gpsProcessData(void){
		char *rmcPtr = strchr (gpsStr,',') + 1;
		if (rmcPtr[0] == ',')
			gpsUtc = 0.0;
		else
			scanf(rmcPtr,"%f",&gpsUtc);
		
		rmcPtr = strchr (gpsStr,',') + 1;
		if (rmcPtr[0] == ',')
			gpsFix = 0;
		else
			scanf(rmcPtr,"%f",&gpsFix);
		
		rmcPtr = strchr (gpsStr,',') + 1;
		if (rmcPtr[0] == ',')
			gpsLatitude = 0.0;
		else
			scanf(rmcPtr,"%f",&gpsLatitude);
		
		rmcPtr = strchr (gpsStr,',') + 1;
		if (rmcPtr[0] == ',')
			gpsNs = NULL;
		else
			scanf(rmcPtr,"%f",&gpsNs);
		
		rmcPtr = strchr (gpsStr,',') + 1;
		if (rmcPtr[0] == ',')
			gpsLongitude = 0.0;
		else
			scanf(rmcPtr,"%f",&gpsLongitude);
		
		rmcPtr = strchr (gpsStr,',') + 1;
		if (rmcPtr[0] == ',')
			gpsEw = 0;
		else
			scanf(rmcPtr,"%f",&gpsEw);
		
		rmcPtr = strchr (gpsStr,',') + 1;
		if (rmcPtr[0] == ',')
			gpsSog = 0.0;
		else
			scanf(rmcPtr,"%f",&gpsSog);
		
		rmcPtr = strchr (gpsStr,',') + 1;
		if (rmcPtr[0] == ',')
			gpsCog = 0.0;
		else
			scanf(rmcPtr,"%f",&gpsCog);
		
		rmcPtr = strchr (gpsStr,',') + 1;
		if (rmcPtr[0] == ',')
			gpsDate = 0;
		else
			scanf(rmcPtr,"%f",&gpsDate);
		
		if (gpsFix == 'A'){
			if(gpsNs != 'N')
				gpsLatitude *=-1;
			if(gpsEw != 'E')
				gpsLongitude *=-1;
			
			int Degrees = gpsLatitude/100;
			float Minutes = fmod(gpsLatitude,100)/60.0;
			gpsLatitude = Degrees + Minutes;
			
			Degrees = gpsLongitude/100;
			Minutes = fmod(gpsLongitude,100)/60.0;
			gpsLongitude = Degrees + Minutes;
			gpsSog *= 1.852;
			
//			if(GpsLedStatus != LED_STAT_OK)
//				{
//						GpsLedStatus = LED_STAT_OK;
//						GpsUpdateLedStatus();
//				}
			
		}
		else{
			gpsSog = 0.0;
			gpsCog = 0.0;
			gpsLatitude = -100.0;
			gpsLongitude = -200.0;
			
//			if(GpsLedStatus != LED_STAT_OFF)
//				{
//						GpsLedStatus = LED_STAT_OFF;
//						GpsUpdateLedStatus();
//				}
		}
		realGpsData = true;
		realGpsDataTimeout.attach(&realDataGpsTimeoutHandler, 1.5);
		return;
	}
	
	void realDataGpsTimeoutHandler(void){
		if(realGpsData == true)
			realGpsData = false;
	}
	
	#endif
