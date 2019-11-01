#word kbd = getenv("SFR:PORTB")

char const KEYS[4][4] = {{'1','2','3','A'},
                         {'4','5','6','B'},
                         {'7','8','9','C'},
                         {'*','0','#','D'}};
                        
void kbd_init(){
   set_tris_b(0x0F00 | (0x00FF & get_tris_b()));
   set_pullup(true,PIN_B8);
   set_pullup(true,PIN_B9);
   set_pullup(true,PIN_B10);
   set_pullup(true,PIN_B11);
   kbd = (0x0000 | (0x00FF & kbd));
}

char kbd_getc(){
   int1 kbd_down = 0;
   int1 idd = 0;
   char last_key;
   unsigned int8 col = 0;
   unsigned int8 kchar = '\0';
   unsigned int8 row;
   
   set_tris_b(0x0F00 | (0x00FF & get_tris_b()));
   
   while(idd == 0){
      delay_us(100); //this delay adds de-bounce
      if(col == 3){
         kbd = (0xEF00 | (0x00FF & kbd));} //set D1, 1st col, low  
      else if(col == 2){
         kbd = (0xDF00 | (0x00FF & kbd));} //set D2, 2nd col, low
      else if(col == 1){
         kbd = (0xBF00 | (0x00FF & kbd));} //set D3, 3rd col, low
      else if(col == 0){
         kbd = (0x7F00 | (0x00FF & kbd));} //set D4, 4th col, low
         
         
      if(kbd_down){
         if((kbd & 0x0F00) == 0x0F00){
         kbd_down=FALSE;
         kchar=last_key;
         last_key='\0';
         idd=1;
         }
      }
      else
      {
         if((kbd & 0x0F00) != 0x0F00){   //if a key is pressed, one of row will be low
            if((kbd & 0x0800) == 0) row = 0;
            else if((kbd & 0x0400) == 0) row = 1;
            else if((kbd & 0x0200) == 0) row = 2;
            else if((kbd & 0x0100) == 0) row = 3;
            last_key = KEYS[col][row];
            kbd_down = TRUE;
         }
         else{
            ++col;
            if(col == 4) col = 0;
         }
      }
   }
   return(kchar);
}

