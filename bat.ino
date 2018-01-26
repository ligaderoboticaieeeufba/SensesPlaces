void setup() {
  // put your setup code here, to run once:
  Serial.begin(11500);

}

void loop() {
  // put your main code here, to run repeatedly:

int  analog_0 = 0;
int counter = 0;
int counter2=0;
int  previous_running_time = 0 ;
int  target = 10000;
int  pulse = 0 ;
int  remind = 0;
int current_running_time= 0;

  while (true){
    
      current_running_time = millis();
      analog_0 = analogRead(0);


      if (remind == 0 ){

          if ( analog_0 >= 920){
            
           counter = counter+1;
           remind = 1;
     
            }

       if (remind == 1) {

          if(analog_0<920){

            remind = 0 ;
            
     
            }   
        }

        
      }
    

    if( ( current_running_time - previous_running_time) >= target){
      
      counter = counter *6;

      Serial.println(counter);
      
      
      
      
      
      }
    


    
    
    
    }
  

}
