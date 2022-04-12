  double freqValue = 80;
  int offset = 5;
  int freqValueA = freqValue/2+offset;
  int freqValueB = freqValue/2-offset;
  float freq = 16000000/(freqValue*2);
  String input;
  bool isRunning = false;


void setup() {
  Serial.begin(9600);
  pinMode(9, OUTPUT); //output A
  pinMode(10, OUTPUT); //output B

  clearTimers(); //resetar timer register

  //ICR1 and Prescaler sets frequency
  //no prescaler .0625 us per count @ 16Mh
  //80 counts x .0625 = 5 us = 200Khz => ICR1 =40

  TCCR1B |= _BV(CS10); //no prescaler
  //ICR1 = 40;//PWM mode counts up and back down for 80 counts

  //OCR1A = 21; //Pin 9 match
  //output A set rising/clear falling
  //Rise at TCNT 21 upslope, High 38 counts, Fall at TCNT 21 downslope
  //47,5% Duty Cycle Pulse centered on TCNT 40. High 38 Low 42
  //OCR1B = 19; //Pin 10 match
  //output B clear rising/set falling
  //Fall at TCNT 19 upslope, Low 42, Rise at TCNT 19 downslope
  //47.5% Duty Cycle Pulse centered on TCNT 0. High 38 Low 42
  //startOutput();

  TCCR1B |= _BV(WGM13); //PWM mode with ICR1 Mode 10
  TCCR1A |= _BV(WGM11); //WGM13:WGM10 set 1010
  //setFreq(1000);
  
}

void loop() {
  if(Serial.available()){
    input = Serial.readStringUntil('\n');
    switch(input.toInt()){
        case 1:
          if(!isRunning){
              startOutput();
              isRunning=true;
              Serial.println("JA");
            }
          
        break;

        case 0:
          if(isRunning){
              stopOutput();
              isRunning=false;
              Serial.println("Nej");
            }
          stopOutput();
        break;

        default:
        if(isRunning){
        setFreq(input.toFloat()); 
        
      
        }
      }
    
    
    
    }



  
  /* if(Serial.available()){
        input = Serial.readStringUntil('\n');
        if(input == "off"){
          stopOutput();
          while(!start){
            if(Serial.available()){
              input = Serial.readStringUntil('\n');
              if(input == "on"){
                start=true;
                setup();
                }
              }
            }
          }
        else{
          setFreq(input.toFloat()); 
          }
        
        //stopOutput();
   
       
    }*/
/*freq=250;
  setFreq(freq);
  while(freq>15000){
    freq=freq-200;
    setFreq(freq);
    delay(50);
    }
*/


  }

void setFreq(float newFreq){ //Går att skicka in som lägst 122.5 Hz annars överstiger det 65536 (2^16)
  freq= newFreq;
  freqValue= 8000000/freq;
  freqValueA = freqValue/2+offset;
  freqValueB = freqValue/2-offset;
  ICR1 = freqValue;
  OCR1A = freqValueA;
  OCR1B = freqValueB;
  
  
  Serial.println(String(freq,1) + ": " + 16000000/(ICR1*2) + ": " + freqValue);
  }

void clearTimers(){ //resetar timer register
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
  }

void stopOutput(){
    TCCR1A &= !_BV(COM1A1) | !_BV(COM1A0) | !_BV(COM1B1);
  }

void startOutput(){
  TCCR1A |= _BV(COM1A1) | _BV(COM1A0) | _BV(COM1B1); //output A set rising/clear falling, output B clear rising/set falling
  }
  
