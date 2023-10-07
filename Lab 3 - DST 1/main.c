#include "system_sam3x.h"

#include "at91sam3x8.h"

#include <string.h>

int m=0;

int x=0;

int tempF;

int tempC;

unsigned int a;

unsigned int b;

int c;

unsigned int y;

 

int voltageArr[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

int rotatedDegree = 0;

int tempArr[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

int tempC=0;

int read;

 

 

 

void Delay(int Value){

int i;

for(i=0;i<Value;i++)

asm("nop");

   

}

 

void Init_temp(){

 

*AT91C_PMC_PCER=(0x8001000); //12 for piob and 27 for TC Parallel I/O Controller B  // Timer counter 0

*AT91C_TC0_CMR =(0x60000); // (TC) Edge: falling edge of TIOA  // (TC) Edge: rising edge of TIOA

*AT91C_TC0_CCR=(0x5); //counter is reset and the clock is start. 1= enable clock counter (page 897)

 

*AT91C_PIOB_PER=(1<<25);

*AT91C_PIOB_ODR=(1<<25);

 

 

NVIC_ClearPendingIRQ(TC0_IRQn);

NVIC_SetPriority(TC0_IRQn,2);

NVIC_EnableIRQ(TC0_IRQn);

}

 

void mesurementTemp(){

 

*AT91C_PIOB_OER = (1<<25);

*AT91C_PIOB_SODR = (1<<25);  

  Delay(25);

*AT91C_PIOB_CODR = (1<<25);

*AT91C_PIOB_ODR = (1<<25);

*AT91C_TC0_CCR=(0x4); //(1<<2)=counter is reset and the clock is start.

if(*AT91C_TC0_SR & (0x60) == (0x60)){

    NVIC_ClearPendingIRQ(TC0_IRQn);

}

*AT91C_TC0_IER = (1<<6);

}

 

void TC0_Handler(){

  *AT91C_TC0_IDR=(1<<6); // (TC0) Interrupt Disable Register

  read = *AT91C_TC0_SR;

   x = 1;

}

 

void Init_photo(){

 

*AT91C_PMC_PCER=(1<<11);

*AT91C_PMC_PCER1=(1<<5); // 11 for PIOA, 37 for ADC

*AT91C_PIOA_PER= (0x18); // pins A5 and A6

*AT91C_PIOA_ODR=(0x18); 

*AT91C_PIOA_SODR=(0x18);

 

*AT91C_ADCC_MR=(2<<8); // (ADCC) ADC Mode Register

*AT91C_ADCC_SR;

NVIC_ClearPendingIRQ(ADC_IRQn);

NVIC_SetPriority(ADC_IRQn,0);

NVIC_EnableIRQ(ADC_IRQn);

}

 

void mesurementPhot(){

*AT91C_ADCC_CHER=(0x6); //channel 1 and 2

*AT91C_ADCC_CR=(1<<1);

 

*AT91C_ADCC_IER = (1<<24);

}

 

void Init_PWM(){

*AT91C_PMC_PCER=(1<<12);

*AT91C_PMC_PCER1=(1<<4);

*AT91C_PIOB_PDR=(1<<17);

*AT91C_PIOB_ABMR=(1<<17);

*AT91C_PWMC_ENA=(1<<1);

*AT91C_PWMC_CH1_CMR=(0x5);

*AT91C_PWMC_CH1_CPRDR=(0xCD14);

*AT91C_PWMC_CH1_CDTYR=(1340);

}

 

void ADC_Handler(){

  *AT91C_ADCC_IDR = (1<<24);

  *AT91C_ADCC_SR;

  m=1;

}

 

unsigned char Read_Status_Display(){

    unsigned char Temp;

   

     *AT91C_PMC_PCER=(3<<13); //  <13 SAM3X8E Parallel I/O Controller C (PIOC)

    

    *AT91C_PIOC_PER= (1<<2) | (1<<3)| (1<<4) |(1<<5) |(1<<6) |(1<<7) |(1<<8) |(1<<9) ; 

    *AT91C_PIOC_PPUDR = (1<<2) | (1<<3)| (1<<4) |(1<<5) |(1<<6) |(1<<7) |(1<<8) |(1<<9) ;

    *AT91C_PIOD_PER=1; // (PIOD) PIO Enable Register

    *AT91C_PIOD_PPUDR=1; // (PIOD) Pull-up Disable Register

 

    *AT91C_PIOC_ODR=(1<<2) | (1<<3)| (1<<4) |(1<<5) |(1<<6) |(1<<7) |(1<<8) |(1<<9); // output disable register from pin 34 to 41

    *AT91C_PIOC_OER=(255<<12); //output enable register from pin 44 to 51

 

    *AT91C_PIOC_OER=(1<<13); // output enabel pin 50

    *AT91C_PIOC_SODR=(1<<13); //  pin 50 (dir)

    *AT91C_PIOD_OER=1; // output enable pin 25

   

    *AT91C_PIOC_CODR=(1<<12); //clear OE (enabel the chip)

    *AT91C_PIOC_SODR=(1<<14); // set C/D

   

    *AT91C_PIOC_CODR=(1<<15); //clear CE chip select display

    *AT91C_PIOC_CODR=(1<<16); // clear read display

   

    Temp=((*AT91C_PIOC_PDSR)&(255<<2));

   

    

    *AT91C_PIOC_SODR=(1<<15); //set CE chip select display

    *AT91C_PIOC_SODR=(1<<16); // set read display

    *AT91C_PIOC_SODR=(1<<12); //set OE (disable) the chip

    *AT91C_PIOC_CODR=(1<<13); //  pin 50 (dir)

    return

      

    Temp;

}

 

void Write_Command_2_Display(unsigned char Command) {

  

  while((Read_Status_Display() & (3<<2)) != (3<<2)); //  Read Status Display

    *AT91C_PIOC_OER=(255<<2); //  pin 34 to 41

    *AT91C_PIOC_CODR=(255<<2); // clear databus

   

    

    *AT91C_PIOC_SODR=(unsigned int)Command<<2;// write to databus

   

    

    *AT91C_PIOC_CODR=(1<<13); // pin 50 PC13(DIR on 74HC245)

    *AT91C_PIOC_CODR=(1<<12); // clear OE Pin 51 PC12

    *AT91C_PIOC_OER=(1<<2) | (1<<3)| (1<<4) |(1<<5) |(1<<6) |(1<<7) |(1<<8) |(1<<9); // pin 34 to 41

    *AT91C_PIOC_SODR=(1<<14); // Register select signal. CD=1: Command, CD=0: Data

    *AT91C_PIOC_CODR=(1<<15);  //clear CE

    *AT91C_PIOC_CODR=(1<<17); // clear write

    Delay(5);

    

    

     *AT91C_PIOC_SODR=(1<<15);// set chip enable display

     

     *AT91C_PIOC_SODR=(1<<17);// set write

     *AT91C_PIOC_SODR=(1<<12); // set OE

     *AT91C_PIOC_ODR=(1<<2) | (1<<3)| (1<<4) |(1<<5) |(1<<6) |(1<<7) |(1<<8) |(1<<9); //  pin 34 to 41

}

                                    

void Write_Data_2_Display(unsigned char Data){

  while((Read_Status_Display() & (3<<2)) != (3<<2)); //  Read Status Display (3<<2)

 

    *AT91C_PIOC_OER=(1<<2) | (1<<3)| (1<<4) |(1<<5) |(1<<6) |(1<<7) |(1<<8) |(1<<9); //  pin 34 to 41

    *AT91C_PIOC_CODR=(1<<2) | (1<<3)| (1<<4) |(1<<5) |(1<<6) |(1<<7) |(1<<8) |(1<<9); // clear databus

   

    

    *AT91C_PIOC_SODR=(unsigned int)Data<<2;// write to databus

   

    

    *AT91C_PIOC_CODR=(1<<13); // pin 50 PC13(DIR on 74HC245)

    *AT91C_PIOC_CODR=(1<<12); // clear OE Pin 51 PC12

    *AT91C_PIOC_OER=(1<<2) | (1<<3)| (1<<4) |(1<<5) |(1<<6) |(1<<7) |(1<<8) |(1<<9); // pin 34 to 41

    *AT91C_PIOC_CODR=(1<<14); // clear signal. CD=1: Command, CD=0: Data

    *AT91C_PIOC_CODR=(1<<15);  //clear CE

    *AT91C_PIOC_CODR=(1<<17); // clear write

    Delay(5);

    

    

     *AT91C_PIOC_SODR=(1<<15);// set chip enable display

    

     *AT91C_PIOC_SODR=(1<<17);// set write

     *AT91C_PIOC_SODR=(1<<12); // set OE

     *AT91C_PIOC_ODR=(1<<2) | (1<<3)| (1<<4) |(1<<5) |(1<<6) |(1<<7) |(1<<8) |(1<<9); //  pin 34 to 41 //

}

 

void Init_Display(void){

  *AT91C_PIOD_CODR=1; // clear reset display pin 25

  Delay(3000); // make a delay

  *AT91C_PIOD_SODR=1; // set reset display pin 25

  Write_Data_2_Display(0x00);

  Write_Data_2_Display(0x00);

  Write_Command_2_Display(0x40);//Set text home address

  Write_Data_2_Display(0x00);

  Write_Data_2_Display(0x40);

  Write_Command_2_Display(0x42); //Set graphic home address

  Write_Data_2_Display(0x1e);

  Write_Data_2_Display(0x00);

  Write_Command_2_Display(0x41); // Set text area

  Write_Data_2_Display(0x1e);

  Write_Data_2_Display(0x00);

  Write_Command_2_Display(0x43); // Set graphic area

  Write_Command_2_Display(0x80); // text mode

  Write_Command_2_Display(0x94); // Text on graphic off

  

 

  Write_Data_2_Display(0);

  Write_Data_2_Display(0);

  Write_Command_2_Display(0x24); // Lines

 

  for(int i=0;i<500;i++) // clear display

  {

    Write_Data_2_Display(0);

    Write_Command_2_Display(0xC0);// v-Dots

  }

 

  Write_Data_2_Display(0);

  Write_Data_2_Display(0);

  Write_Command_2_Display(0x24); // Lines

 

}

 

 

void addScreen(int displayKey){

      if(displayKey != 0){

              

      if(displayKey < 10 && displayKey > 0){ //  0 - 10

        Write_Data_2_Display(0x10 + displayKey); // Sets data to bus (font table)

        Write_Command_2_Display(0xC0); // Data write and Increment ADP

      }else if(displayKey == 10){ // * = 1010

        Write_Data_2_Display(0x0A);

        Write_Command_2_Display(0xC0);

      }else if(displayKey == 11){ // 0

        Write_Data_2_Display(0x10);

        Write_Command_2_Display(0xC0);

      }else if(displayKey == 12){ // #

        Write_Data_2_Display(0x03);

        Write_Command_2_Display(0xC0);

      }

    }

}

 

void toDisplay(char string[]){

  int stringLength = strlen(string);

  int font= 0;

  for(int c = 0; c < stringLength; c++){

   font = string[c] - 0x20;

    Write_Data_2_Display(font); // data to bus

    Write_Command_2_Display(0xC0);

  }

}

 

void Init_keypad(){

    *AT91C_PMC_PCER=(3<<13);//clock Enabel

  

    

    *AT91C_PIOD_PER=(1<<2);//enabel pin 27

    *AT91C_PIOD_PPUDR=(1<<2);// (PIOD) Pull-up Disable Register pin 27

    *AT91C_PIOD_OER=(1<<2); // (PIOD) Output Enable Register pin 27

    

    *AT91C_PIOC_PER=(239<<2);//enable pin 34-41

    *AT91C_PIOC_PPUDR=(239<<2);//disable pull up resistor

   

    *AT91C_PIOC_ODR=(15<<1);//as input rows

   

    *AT91C_PIOC_OER=(7<<7);//as output column

    *AT91C_PIOC_SODR=(7<<7);

}

 

int func(){

  int value =0;

 

  *AT91C_PIOC_OER=(7<<7);//column

  *AT91C_PIOD_CODR=(1<<2);// octalbus

  *AT91C_PIOC_SODR=(7<<7); // column as high

 

  

  for(int col=0; col<3; col++)

  {

     *AT91C_PIOC_CODR=(1<<col+7);//pin 39,40,41 (7,8,9)

     

     for(int row=0;row<4;row++)

     {

        if(((~*AT91C_PIOC_PDSR)&(1<<row+2))==(1<<row+2)) // pin 34 to 37 (2,3,4,5)

          value=row*3+col+1;

     }

    

      *AT91C_PIOC_SODR=(7<<7);// set high

    

  }

 

  *AT91C_PIOC_ODR=(7<<7); 

  *AT91C_PIOD_SODR = 1<<2; // Turn off keypad bus

  return value;

}

 

void extract_number(int num, int arr[]){

  int index = 9;

  if(num == 0){

    arr[index] = 0;

  }

  while(num > 0){

      int mod = num % 10;  //extract last digit from number

      arr[index] = mod;

      index--;

      num = num / 10;    //divide num by 10.

  }

}

 

void displayInfo(int* val, char* SI){

   for(int n = 0; n < 10; n++){

    if(val[n] != -1){

      if(val[n] == 0){

         addScreen(11);

      }else{

         addScreen(val[n]);

      }

      val[n] = -1;

    }

  }

  toDisplay(SI);

}

 

void servo(int degree){

rotatedDegree = degree * 10;

  double conDegrees = 1340 + ((262.3)*(degree));

if(degree <= 0){

   *AT91C_PWMC_CH1_CDTYR = 1340;

}else if(degree >= 180){

   *AT91C_PWMC_CH1_CDTYR = 6400;

}else{

   *AT91C_PWMC_CH1_CDTYR = conDegrees;

}

Delay(10000);

}

 

 

void main() {

  SystemInit();

  Init_temp();

  Init_photo();

  Init_PWM();

  Init_Display(); //initialize display

  Init_keypad(); //initialize keypad

  

  while(1) {

   mesurementTemp ();

  

   Delay(38000);

  if(x == 1) {

     a=*AT91C_TC0_RA;

     b=*AT91C_TC0_RB;

     tempF=b-a;

     if(tempF > 0x200){

    tempC= (int)((((tempF*(1/42.0))/5.0)-273.15));

  }

  } 

  

   

   Write_Data_2_Display(0); // 0 at Y

   Write_Data_2_Display(0); // 0 at X

   Write_Command_2_Display(0x24); // Sets new line

   

   extract_number(tempC, tempArr);

   displayInfo(tempArr, "C");

  

   

   Write_Data_2_Display(60); // 60 at Y

   Write_Data_2_Display(0); // 0 at X

   Write_Command_2_Display(0x24); // Sets new line

   

   

   mesurementPhot();

     if(m==1)

      {

    y = *AT91C_ADCC_LCDR & (0xFFF); //read LCDR value

    m=0;

    }

   y = y /1023;

   extract_number(y, voltageArr);

   displayInfo(voltageArr, "V");

   Write_Data_2_Display(0);

   Write_Command_2_Display(0xC0);

   Write_Data_2_Display(0);

   Write_Command_2_Display(0xC0);

  

   

     int key = func();

   if(key != 0){

     if(key == 11){

        servo(0);

     }else{

        servo(key);     

     }

   }

  

       Delay(100000);

  }

}