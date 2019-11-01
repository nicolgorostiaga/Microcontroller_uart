
#include <lcd.h>

//Key pressed
int1 keypad_flag = 0;
char key = 0;
#INT_EXT0
void isr_ext(){

   key = kbd_getc();
   keypad_flag = 1;
}

isr_uart();

void lcd_display(char c){
   disable_interrupts(INT_EXT0);
   set_pullup(false);
   lcd_putc(c);
   kbd_init();
   clear_interrupt(INT_EXT0);
   enable_interrupts(INT_EXT0);
}

void main(){
   
   int intkey = 0;
   double operand1 = 0,operand2 = 0;
   double result = 0;
   char sign = '\0';
   int  errorMessage;
   int repeat;
   lcd_init();

   //UART display
   printf("\n\rNicol Anokhin\n\r");
   printf("\n\rA:+ B:- C:* D:/\n\r");
   
   //LCD display
   printf(lcd_putc,"\fNicol Anokhin\n");
   printf(lcd_putc,"A:+ B:- C:* D:/");
   delay_ms(2500);
   printf(lcd_putc,"\f");
   
   
   kbd_init();
   ext_int_edge(L_TO_H);
   clear_interrupt(INT_EXT0);
   enable_interrupts(INT_EXT0);
   enable_interrupts(INTR_GLOBAL);//LCD & Terminal
   
   ext_int_edge(L_TO_H);
   enable_interrupts(INT_RDA2);
   enable_interrupts(INTR_GLOBAL);//LCD & Terminal
   
   while(TRUE){

      if(keypad_flag || serial_flag){
         if(serial_flag){
          key = uart_key;
          serial_flag = 0;
         }
         if(keypad_flag){
          key = key;
          keypad_flag = 0;
         }
         if(key == '*'){
                //Reset calculator
               if(errorMessage == 2){
                  lcd_display('\f');
                  putc('\n');
                  putc('\r');
                   key = '\0';
                   operand1 = 0;
                   operand2 = 0;
                   intkey = 0;
                   sign = '\0'; 
                   result = 0;
                   errorMessage = 0;
                   repeat = 1;
               }
               //Change number to negative
               if(sign == '\0'&& repeat != 1 && repeat == 0){
                  putc('*');
                  lcd_display('\f');
                  operand1 = -1*(operand1);
                  printf(lcd_display,"%f",operand1);
                  printf("\n\r%f",operand1);
                  repeat = 0;
               }
               if(sign != '\0'){
                  putc('*');
                  //Change second number to negative
                   lcd_display('\f');
                   if(sign == 'A'){
                   operand2 = -1*operand2;
                   //LCD display
                   printf(lcd_display,"%f",operand1);
                   printf(lcd_display,"+");
                   printf(lcd_display,"%f",operand2);
                   //UART display
                   printf("\n\r%f",operand1);
                   printf("+");
                   printf("%f",operand2);
                   }
                   if(sign == 'B'){
                   operand2 = -1*operand2;
                   //LCD display
                   printf(lcd_display,"%f",operand1);
                   printf(lcd_display,"-");
                   printf(lcd_display,"%f",operand2);
                   //UART display
                   printf("\n\r%f",operand1);
                   printf("-");
                   printf("%f",operand2);
                   }
                   if(sign == 'C'){
                   operand2 = -1*operand2;
                   // LCD Display
                   printf(lcd_display,"%f",operand1);
                   printf(lcd_display,"*");
                   printf(lcd_display,"%f",operand2);
                   //UART display
                   printf("\n\r%f",operand1);
                   printf("*");
                   printf("%f",operand2);
                   }
                   if(sign == 'D'&& operand2 != 0){
                   operand2 = -1*operand2;
                   //LCD display
                   printf(lcd_display,"%f",operand1);
                   printf(lcd_display,"/");
                   printf(lcd_display,"%f",operand2);
                   //UART display
                   printf("\n\r%f",operand1);
                   printf("/");
                   printf("%f",operand2);
                   }
               }
               //Reset calculator
               if(errorMessage == 1){
                   putc('*');
                  lcd_display('\f');
                   key = '\0';
                   operand1 = 0;
                   operand2 = 0;
                   intkey = 0;
                   sign = '\0'; 
                   result = 0;
                   errorMessage = 0;
               }
               
         }  // end if(key == *)
         else{
            //accept integers
            if((key >= '0' && key <= '9')){
            lcd_display(key);
            putc(key);
             if(sign == 'A'|| sign == 'B' || sign == 'C'||sign == 'D'){
                intKey = key-0x30; //char to int shift to get another number
                operand2 = (10*operand2) + intkey; 
             }
             if(sign == '\0'){
             intKey = key-0x30; //char to int shift to get another number
             operand1 = (10*operand1) + intkey;
             } 
            }    
            //accept the operations
            if( key == 'A'|| key == 'B' || key == 'C'||key == 'D'){
               sign = key;
               switch(sign){
                  case 'A':{ lcd_display('+');
                              putc('+');}
                  break;
                  case 'B':{ lcd_display('-');
                             putc('-');}
                  break;
                  case 'C':{ lcd_display('*');
                              putc('*');}
                  break;
                  default:{lcd_display('/');
                           putc('/');}
                  break;
               }
            }
            //calculate result
            if(key == '#'){
               putc('=');
               switch(sign){
                  case 'A': result = operand1 + operand2;
                  break;
                  case 'B':result = (float)operand1 - operand2;
                  break;
                  case 'C':result = operand1 * operand2;
                  break;
                  default:result = (float)operand1 / operand2;
                  break;
               }
               if(sign == 'D' && operand2 == 0){
                printf(lcd_display,"\nerror");
               // printf("\n\rerror\n\r");
               printf("error");
                errorMessage = 1;
               }
               if(result != 0){
               printf(lcd_display,"\n=%f",result);
              // printf("\n\r=%f\n\r",result);
              putc(result);
               errorMessage = 2;
               }
               if(operand1 != 0 && operand2 != 0 && result == 0){
               printf(lcd_display,"\n=%f",result);
              // printf("\n\r=%f\n\r",result);
              putc(result);
               errorMessage = 2;
               }
             }
         }  
      }// end if(key)
   }// end while loop
}//end main
