#include "system_sam3x.h"
#include "at91sam3x8.h"

void Set_Led(unsigned int nLed);
void Button_Set_Led();
void SysTick_Handler(void);
void PIOD_Handler(void);
void Button_Change_Led();
int Counter = 0;
int Button_Interupt = 0;





int main()
{
    SystemInit();
    
    Set_Led(0);
    //SysTick_Config(8400000);
    /*
    Button_Set_Led();
    while (1){
    Button_Change_Led();
    }; */
    
  
    
    (*AT91C_PMC_PCER) = (1<<14);
    //(*AT91C_PIOD_PPUDR) = (1<<1);
    //PIOD_AIMER Additional Interrupt Modes Enable Register
    (*AT91_CAST(AT91_REG *) 0x400E14B0) = (1<<1);
    (*AT91C_PIOD_IFER) = (1<<1);
    //PIOD_DIFSR PIO Debouncing Input Filtering Select Register
    (*AT91_CAST(AT91_REG *) 0x400E1484) = (1<<1);
    //PIOD_SCDR clock for debouncing
    (*AT91_CAST(AT91_REG *) 0x400E148C) = (1<<5);
    (*AT91C_PIOD_ISR);
    static __INLINE void NVIC_ClearPendingIRQ(IRQn_Type IRQn);   
    static __INLINE void NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority);
    static __INLINE void NVIC_EnableIRQ(IRQn_Type IRQn);
    (*AT91C_PIOD_PER) = (1<<1);
    (*AT91C_PIOD_IER) = (1<<1);
    
      
    while (1);
    
    return 0;
}


void PIOD_Handler(void){
  Set_Led(1);
  if (*AT91C_PIOD_ISR == 2){ // button was cause
    Button_Interupt++;
    
  }
}

void SysTick_Handler(void){
  Counter ++;
  if ((*AT91C_PIOD_ODSR) != 8 && Counter > 10 ){
    Set_Led(1);
    Counter = 0;
  }
  else {
    Set_Led(0);
  }
}

/*
void SysTick_Handler(void){
  Counter ++;
  if ((*AT91C_PIOD_ODSR) != 8 && Counter > 10 && Button_Interupt % 2 == 0){
    Set_Led(1);
    Counter = 0;
  }
  else {
    Set_Led(0);
  }
}
*/

void Set_Led(unsigned int nLed) {
  
    (*AT91C_PMC_PCER) = (1<<14);
  
    (*AT91C_PIOD_PER) = (1<<3);
  
    (*AT91C_PIOD_PPUDR) = (1<<3);
  
    (*AT91C_PIOD_OER) = (1<<3);
  
    if (nLed == 1)
      (*AT91C_PIOD_SODR) = (1<<3);
    else
      (*AT91C_PIOD_CODR) = AT91C_PIO_PD3;
}

void Button_Set_Led() {
  
    (*AT91C_PMC_PCER) = (1<<14);
    
    (*AT91C_PIOD_PPUDR) = (1<<1);
    
    (*AT91C_PIOD_PER) = (1<<1);
    
    (*AT91C_PIOD_IFER) = (1<<1);
    
    (*AT91C_PIOD_PER) = (1<<3);
  
    (*AT91C_PIOD_PPUDR) = (1<<3);
  
    (*AT91C_PIOD_OER) = (1<<3);
    Button_Change_Led();
}

void Button_Change_Led() {
  if (*AT91C_PIOD_PDSR == 2147483637)
      (*AT91C_PIOD_SODR) = (1<<3);
    else
      (*AT91C_PIOD_CODR) = AT91C_PIO_PD3;
}