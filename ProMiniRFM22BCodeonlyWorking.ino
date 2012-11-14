

//  Lib's for the RFM22 Radio
#include <SPI.h>
#include <RFM22.h>

#define RFM_SDN 3  // The Arduinio pin which the RFM ShutDowN pin is connected to.  High = Shutdown!
#define RFM_NSEL 10  // The Arduino pin which the RFM Serial Interface Select input pin is connected to.

#define GPS_PWR 2  //  The Arduino pin which provides Power to the GPS module

//Setup radio on SPI with NSEL on pin 10
rfm22 radio1(RFM_NSEL);

#include <string.h>
#include <util/crc16.h>

//*********************************************************************************
//
// setup()
//
//**********************************************************************************
 
void setup() {   
  
  //Turn off GPS
  pinMode(GPS_PWR,OUTPUT);
  digitalWrite(GPS_PWR, LOW); //Make sure GPS is turned off
  
  //Debugger.println(F("setup() Set up the RFM22B Radio"));
  setupRadio();
  
  radio1.write(0x07, 0x08); // turn tx on
  delay(1000);
  
  //Debugger.println(F("setup() Transmit Test String"));
  rtty_txstring("ProMini v0.1\n");

  //Debugger.println(F("setup() Finished Setup\r\n\r\n\r\n\r\n"));
  delay(1000);
    
}



//*********************************************************************************
//
// loop()
//
//**********************************************************************************


void loop() {
  
  radio1.write(0x07, 0x08); // turn tx on
  delay(1000);
  rtty_txstring("ProMini ProMini ProMini ProMini ProMini v0.11\n");
  delay(1000);

} //end of Loop()



//*********************************************************************************
//
// Other Functions
//
//**********************************************************************************


void rtty_txstring (char * string)
{
 
  char c;
 
  c = *string++;
 
  //Debugger.print(F("rtty_txstring() Send: "));
  while ( c != '\0')
  {
    //Debugger.print(c);
    rtty_txbyte (c);
    c = *string++;
  }
  //Debugger.println("");
}
 
 
void rtty_txbyte (char c)
{
  /* Simple function to sent each bit of a char to 
   	** rtty_txbit function. 
   	** NB The bits are sent Least Significant Bit first
   	**
   	** All chars should be preceded with a 0 and 
   	** proceded with a 1. 0 = Start bit; 1 = Stop bit
   	**
   	*/
 
  int i;
 
  rtty_txbit (0); // Start bit
 
  // Send bits for for char LSB first	
 
  for (i=0;i<7;i++) // Change this here 7 or 8 for ASCII-7 / ASCII-8
  {
    if (c & 1) rtty_txbit(1); 
 
    else rtty_txbit(0);	
 
    c = c >> 1;
 
  }
 
  rtty_txbit (1); // Stop bit
  rtty_txbit (1); // Stop bit
}

// RFM22b txbit
void rtty_txbit (int bit)
{
		if (bit)
		{
		  // high
                  radio1.setFrequency(434.2015);  //  KW High & Low freq's reversed from original code so that we dont have to use the Rv button in dl-fldigi
		}
		else
		{
		  // low
                  radio1.setFrequency(434.2010);
		}
                delayMicroseconds(19500); // 10000 = 100 BAUD 20150
 
}
 
 

// RFM22B Radio Setup
void setupRadio(){
  
  pinMode(RFM_SDN, OUTPUT);
  digitalWrite(RFM_SDN, LOW);
 
  delay(1000);
 
  rfm22::initSPI();
 
  radio1.init();
 
  radio1.write(0x71, 0x00); // unmodulated carrier
 
  //This sets up the GPIOs to automatically switch the antenna depending on Tx or Rx state, only needs to be done at start up
  radio1.write(0x0b,0x12);
  radio1.write(0x0c,0x15);
 
  radio1.setFrequency(434.201);
 
}

