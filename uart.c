//Keyboard pressed
int1 serial_flag = 0;
char uart_key;

#INT_RDA2
void isr_uart(){
  uart_key = getc();
  serial_flag = 1;
}




