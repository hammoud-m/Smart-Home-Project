#include "system_sam3x.h"
#include "at91sam3x8.h"

int col, row;
int val, value;
int number;

void Delay(int value)
{
    
    for(int i=0; i < value; i++)
    asm("nop");
} 

unsigned char Read_Status_Display()
{
    unsigned char temp;
    
    *AT91C_PMC_PCER=(3<<13); // port c and port d clock
    
    *AT91C_PIOC_PER = 0xffffffff;
    *AT91C_PIOC_PPUDR = 0xfffffff;
    
    *AT91C_PIOD_PER=1;
    *AT91C_PIOD_PPUDR=1;
    
    *AT91C_PIOC_ODR=(255<<2); // output disable pin 34 to 41
    *AT91C_PIOC_OER=(253<<12); //output enable pin 44 to 51 (without 50)
    *AT91C_PIOC_OER=(1<<13); // output enabel pin 50
    *AT91C_PIOC_SODR=(1<<13); //  pin 50 (dir)
    *AT91C_PIOD_OER=1; // output enable pin 25
    
    *AT91C_PIOC_CODR=(1<<12); //clear OE (enabel the chip)
    *AT91C_PIOC_SODR=(1<<14); // set C/D
    
    *AT91C_PIOC_CODR=(1<<15); //clear CE chip select display
    *AT91C_PIOC_CODR=(1<<16); // clear read display
    
   //Delay(10);
    
    //
    temp=((*AT91C_PIOC_PDSR)&(255<<2));
    //
    
    *AT91C_PIOC_SODR=(1<<15); //set CE chip select display
    *AT91C_PIOC_SODR=(1<<16); // set read display
    *AT91C_PIOC_SODR=(1<<12); //set OE (disable) the chip
    *AT91C_PIOC_CODR=(1<<13); //  pin 50 (dir)
    return temp;
}

void Write_Command_2_Display(unsigned char Command) 
{
   
    while((Read_Status_Display()) & (2) == 2){}
  
      
    *AT91C_PIOC_OER=(255<<2); // output enable pin 34 to 41
    *AT91C_PIOC_CODR=(255<<2); // clear databus
    
    *AT91C_PIOC_SODR=(unsigned int)Command<<2;// write comman to databus
    
    *AT91C_PIOC_CODR=(1<<13);             // pin 50 (DIR)
    *AT91C_PIOC_CODR=(1<<12);            // enabel output (clear OE)
    *AT91C_PIOC_OER=(255<<2);           // output enable pin 34 to 41
    *AT91C_PIOC_SODR=(1<<14);          // set C/D
    *AT91C_PIOC_CODR=(1<<15);         //clear CE chip select display
    *AT91C_PIOC_CODR=(1<<17);        // clear write display
    Delay(20);
     
     *AT91C_PIOC_SODR=(1<<15);          //set chip enable display
     
     *AT91C_PIOC_SODR=(1<<17);         // set write display
     *AT91C_PIOC_SODR=(1<<12);        // disable output (set OE)
     *AT91C_PIOC_ODR=(255<<2);       // output disable pin 34 to 41
}

void Write_Data_2_Display(unsigned char Data)
{
  
  while((Read_Status_Display()) & (2) == 2){}
  
      
  *AT91C_PIOC_OER=(255<<2);            // output disable pin 34 to 41
  *AT91C_PIOC_CODR=(255<<2);          // clear databus
  
  *AT91C_PIOC_SODR=(unsigned int)Data<<2;//set data to databus
  
  *AT91C_PIOC_CODR=(1<<13);                 // pin 50 (DIR)
  *AT91C_PIOC_CODR=(1<<12);                // enabel output (clear OE)
  *AT91C_PIOC_OER=(255<<2);               // output enable pin 34 to 41
  *AT91C_PIOC_CODR=(1<<14);              // clear C/D
  *AT91C_PIOC_CODR=(1<<15);             //clear CE chip select display
  *AT91C_PIOC_CODR=(1<<17);            // clear write display
  Delay(20);
  

  *AT91C_PIOC_SODR=(1<<15);           //set chip enable display
 
  
  *AT91C_PIOC_SODR=(1<<17);          // set write display
  *AT91C_PIOC_SODR=(1<<12);         // disable output (set OE)
  *AT91C_PIOC_ODR=(255<<2);        // output disable pin 34 to 41
}


void Init_Display(void)
{
  *AT91C_PIOD_CODR=1; // clear reset (pin 25)
  Delay(20000);
  *AT91C_PIOD_SODR=1; // set reset (pin 25)
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
  
  //
  Write_Data_2_Display(0);
  Write_Data_2_Display(0); 
  Write_Command_2_Display(0x24);
  
  for(int i=0;i<500;i++) // clear display
  {
    Write_Data_2_Display(0);
    Write_Command_2_Display(0xC0);
  }
  
  Write_Data_2_Display(0);
  Write_Data_2_Display(0); 
  Write_Command_2_Display(0x24);
  //
}

//configure the keypad
void configure()
{
    *AT91C_PMC_PCER=(3<<13);//port D and port C clock Enabel
   
    //pin 27 (OE)
    *AT91C_PIOD_PER=(1<<2);             // Enable
    *AT91C_PIOD_PPUDR=(1<<2);           //disable pull up resistor
    *AT91C_PIOD_OER=(1<<2);            //pin 27 (output)
     
    //pin 34 to 41
    *AT91C_PIOC_PER=(239<<2);//enable 
    *AT91C_PIOC_PPUDR=(239<<2);//disable pull up resistor
    
    //pin34 to 37 (keypad's rows)
    *AT91C_PIOC_ODR=(15<<1);//as input
    
    //pin 39 to 41  (keypad's column)
    *AT91C_PIOC_OER=(7<<7);//as output 
    *AT91C_PIOC_SODR=(7<<7);//set high
}

int program()
{
  value =0;
  
  *AT91C_PIOC_OER=(7<<7);//pin 39-40-41 as output
  *AT91C_PIOD_CODR=(1<<2);// turn on the octal bus
  *AT91C_PIOC_SODR=(7<<7); // set the column as high
  
  for(col=0; col<3; col++)
  {
     *AT91C_PIOC_CODR=(1<<col+7);//clear bit 7,8 and 9= pin 39,40 and 41
      
     for(row=0;row<4;row++)
     {
        if(((*AT91C_PIOC_PDSR)&(1<<row+2))==0) //check rows
          value=row*3+col+1;
     }
     
      *AT91C_PIOC_SODR=(7<<7);// set the column as high  
  }
  
  *AT91C_PIOC_ODR=(7<<7); //pin 39-40-41 as input
  
  return value;
}

  
int main()
{
  SystemInit();
  Init_Display(); 
  configure(); 
  
  
  while(1)
  {
      val=program();
      
      if( val<=9 && val >0)
      {
        Write_Data_2_Display(0x10+val);
        Write_Command_2_Display(0xC0);
      }
      else if(val==10)
      {
        Write_Data_2_Display(0x0A);
        Write_Command_2_Display(0xC0);
      }
      else if(val==11)
      {
        Write_Data_2_Display(0x10);
        Write_Command_2_Display(0xC0);
      }
      else if (val==12)
      {
        Write_Data_2_Display(0x03);
        Write_Command_2_Display(0xC0);
      }
      Delay(2000000);
  }
  
}
 
  


