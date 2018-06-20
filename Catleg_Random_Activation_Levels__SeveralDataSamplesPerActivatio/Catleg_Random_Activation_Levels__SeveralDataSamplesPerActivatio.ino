
#include <Encoder.h>
#include <PinChangeInterrupt.h>
#include <stdio.h>
#include <SPI.h>


//Pins for absolute encoders
    //Encoder 1
    #define CS 53 //Chip or Slave select 
    //Encoder 2
    #define CS2 7 //Chip or Slave select 

///////////Variables

    int averagingWindowSize=2;
    int dataSamples=1000;
    int samplingInterval=50; //Real sampling interval sampling interval+12 ms
    int numberOfSamplesPerSwept=100;
    int totalSwepts;
    int swept=1;

    int absEncoder=1;
    
    float position1;
    float position2;
    
    uint16_t ABSposition = 0;
    uint16_t ABSposition_last = 0;
    
    uint8_t temp[1];
    
    float deg = 0.00;

    int count = 1;
    int count2= 0;
    int header2=1; //To print only once the header1
    int header1=1; //To print only once the header2
    int startTrial=1;
    
    //For Random Arrays
    int numberOfElements=500;
    int randomArray1[500];
    int randomArray2[500];
    int randomArray3[500];
    
    int softRandomArray=0;
    
    int theSoftRandomArray1[500];
    int theSoftRandomArray2[500];
    int theSoftRandomArray3[500];


//Motors

    int PWM[3];

// Motor A******
    
    int enA=4;  // PWM pin
    int in3=30;
    int in4=31;

// Motor B******

    int enB=5; // PWM pin
    int in1=32;
    int in2=33;

// Motor C

    int enC=6; // PWM pin
    int in5=34;
    int in6=35;

    
void setup()
                        {
                          Serial.begin(9600);
                          pinMode(CS,OUTPUT);//Slave Select
                          pinMode(CS2,OUTPUT);//Slave Select
                
                          digitalWrite(CS,HIGH);
                          digitalWrite(CS2,HIGH);
                
                          randomSeed(analogRead(0));
                          
                          //For absolute encoders:
                          SPI.begin();
                          SPI.setBitOrder(MSBFIRST);
                          SPI.setDataMode(SPI_MODE0);
                          SPI.setClockDivider(SPI_CLOCK_DIV32);
                          
                          //Serial.println("starting");
                          Serial.flush();
                          delay(100);
                          SPI.end();
                
                          //Motors:
                          pinMode(enA,OUTPUT);
                          pinMode(enB,OUTPUT);
                          pinMode(enC,OUTPUT);
                          
                          pinMode(in1,OUTPUT);
                          pinMode(in2,OUTPUT);
                          pinMode(in3,OUTPUT);
                          pinMode(in4,OUTPUT);
                          pinMode(in5,OUTPUT);
                          pinMode(in6,OUTPUT);
                
                          digitalWrite(in1, LOW);
                          digitalWrite(in2, HIGH);
                          
                          digitalWrite(in3, LOW);
                          digitalWrite(in4, HIGH);
                            
                          digitalWrite(in5, LOW);
                          digitalWrite(in6, HIGH);
                                  
                        }

void loop() {

if (header1==1){Serial.println("Enter averagingWindowSize    totalSwepts  dataSamples per Swept      samplingInterval (ms)\n");header1--;}

while (Serial.available() > 0) {
  averagingWindowSize=Serial.parseInt(); String averagingWindowSizeString = String(averagingWindowSize);
  totalSwepts=Serial.parseInt();     String totalSweptsString = String(totalSwepts);
  dataSamples=Serial.parseInt();         String dataSamplesString = String(dataSamples);
  samplingInterval=Serial.parseInt();     String samplingIntervalString = String(samplingInterval);

  
  if(header2==1){Serial.println("averagingWindowSize: "+ averagingWindowSizeString + "Total Swepts" + totalSweptsString + ", dataSamples per swept:  "+dataSamplesString+", samplingInterval:"+ samplingIntervalString+
                                " (ms)\n\nLine_Number HIP(deg), KNEE(deg), PWM1(%ActLev), PWM2(%ActLev), PWM3(%ActLev)\n");header2--;
  }
   
    while(swept<totalSwepts){
     //while (count<totalSwepts){
     for (int i=0; i<numberOfElements; i++){
     randomArray1[i]=random(100);
     randomArray2[i]=random(100);
     randomArray3[i]=random(100);
     //count++;
  
         }
//To print the unsoftened random arrays uncomment next lines
    /*for (int i=0; i<numberOfElements; i++){Serial.print(randomArray1[i]);Serial.print(",");}
    Serial.println();
    for (int i=0; i<numberOfElements; i++){Serial.print(randomArray2[i]);Serial.print(",");}
    Serial.println();
    for (int i=0; i<numberOfElements; i++){Serial.print(randomArray3[i]);Serial.print(",");}
    Serial.println();*/


          //count=0;
          
//Softening the randomArray1
          for (int i=(averagingWindowSize-1); i<numberOfElements; i++){
            softRandomArray=0;
            softRandomArray=randomArray1[i]+softRandomArray;
            for (int j=i-1; j>=i-(averagingWindowSize-1); j--){
              
              softRandomArray=randomArray1[j]+softRandomArray;
              }
            softRandomArray=softRandomArray/averagingWindowSize;
            theSoftRandomArray1[i]= softRandomArray;  
            //Serial.print(",");Serial.print(theSoftRandomArray1[i]);   
          }
          //Serial.println();
          
//Softening the randomArray2
          for (int i=(averagingWindowSize-1); i<numberOfElements; i++){
            softRandomArray=0;
            softRandomArray=randomArray2[i]+softRandomArray;
            for (int j=i-1; j>=i-(averagingWindowSize-1); j--){
              softRandomArray=randomArray2[j]+softRandomArray;
            }
            softRandomArray=softRandomArray/averagingWindowSize;
            theSoftRandomArray2[i]= softRandomArray;  
            //Serial.print(",");Serial.print(theSoftRandomArray2[i]);   
          }
          //Serial.println();

//Softening the randomArray3
          for (int i=(averagingWindowSize-1); i<numberOfElements; i++){
            softRandomArray=0;
            softRandomArray=randomArray3[i]+softRandomArray;
            for (int j=i-1; j>=i-(averagingWindowSize-1); j--){
              softRandomArray=randomArray3[j]+softRandomArray;
            }
            softRandomArray=softRandomArray/averagingWindowSize;
            theSoftRandomArray3[i]= softRandomArray;  
            //Serial.print(",");Serial.print(theSoftRandomArray3[i]);   
          }
          //Serial.println();
//Asigning PWM Values

          for (int j=(averagingWindowSize-1); j<=numberOfElements;j++){ 
            if (j<= totalSwepts)
            {
            //Serial.println(j);         
                  for (int i = 0; i<3; i++){
                  //Serial.println(theSoftRandomArray1[j]);
                  if (i==0){PWM[0] = theSoftRandomArray1[j];}
                  if (i==1){PWM[1] = theSoftRandomArray2[j];}
                  if (i==2){PWM[2] = theSoftRandomArray3[j];}
                  }
                  
                  float pwmValA=(PWM[0]*255)/100;
                  float pwmValB=.5*(PWM[1]*255)/100;
                  float pwmValC=.60*(PWM[2]*255)/100;
                 
                  /* Motor A */
                  //analogWrite (enA,100);
                  analogWrite (enA, pwmValA);digitalWrite(in1,LOW);digitalWrite(in2,HIGH);
      
                  /* Motor B */
                  //analogWrite (enB, 100);
                  analogWrite (enB, pwmValB); digitalWrite(in3,LOW);digitalWrite(in4,HIGH);
                
                  /* Motor C */
                  //analogWrite (enC, 255);
                  analogWrite (enC, pwmValC); digitalWrite(in5,HIGH);digitalWrite(in6,LOW);

                  for (int wey=0; wey<dataSamples; wey++) { 
                  delay(samplingInterval);
                  PrintEncodersLecture(count,PWM[0],PWM[1],PWM[2]);
                  //Serial.println(wey);
                  }
                  count++;
            }
                  
                  
          }
                  
                  swept++;
                  //Serial.println("Dario");
                  //if (swept==totalSwepts){break;}
    }
    
          //count=1;
          //swept=1;
          
      /*for (int i=1; i=<dataSamples; i++){
        PrintEncodersLecture(count2,PWM[0],PWM[1],PWM[2]);
        }*/
  }
            

//count2++;


}


                                                  ////////////FUNCTION FOR ABS ECODERS (Level 0):    
                                                  //Line breakes and comments have been removed to save space, for a clearer sintax revise previous programs
                                                  uint8_t SPI_T (uint8_t msg, int CSNum)    // Repetive SPI transmit sequence
                                                          {
                                                             uint8_t msg_temp = 0;  //vairable to hold recieved data
                                                             if (CSNum==1)
                                                             {
                                                                     digitalWrite(CS2,LOW);msg_temp = SPI.transfer(msg);digitalWrite(CS2,HIGH);return(msg_temp);      
                                                             }
                                                             else if (CSNum==2)
                                                             {
                                                                     digitalWrite(CS,LOW);msg_temp = SPI.transfer(msg);digitalWrite(CS,HIGH);return(msg_temp);
                                                             }
                                                             
                                                          }
                                                  
                                                  //////////////FUNCTION FOR ABS ECODERS (Level 1):
                                                  //Line breakes and comments have been removed to save space, for a clearer sintax revise previous programs
                                                  float SPI_ReadPosition (int absEncoder)  
                                                          {
                                                                  uint8_t recieved = 0xA5;    //just a temp vairable
                                                                  ABSposition = 0;    //reset position vairable
                                                                
                                                                SPI.begin();    //start transmition
                                                                           
                                                                                   SPI_T(0x10,absEncoder);recieved = SPI_T(0x00,absEncoder);while (recieved != 0x10)
                                                                                   {
                                                                                     recieved = SPI_T(0x00,absEncoder);delay(2);    //wait a bit
                                                                                   }
                                                                                   
                                                                                   temp[0] = SPI_T(0x00,absEncoder);    //Recieve MSB
                                                                                   temp[1] = SPI_T(0x00,absEncoder);    // recieve LSB
                                                                 
                                                                 SPI.end();    //end transmition
                                                                                   
                                                                                    temp[0] &=~ 0xF0;ABSposition = temp[0] << 8;ABSposition += temp[1];
                                                                                    
                                                                    if (ABSposition != ABSposition_last)    //if nothing has changed dont wast time sending position
                                                                    {
                                                                        ABSposition_last = ABSposition;deg = ABSposition;deg = deg * 0.08789;
                                                                        return deg;
                                                                    }   
                                                                                  
                                                                delay(10);    //wait a bit till next check
                                                          }

                                                  ////////////FUNCTION TO GET ALL THE ENCODERS' POSITION (Lecture)(Level 3)
                                                  void PrintEncodersLecture(int count,float pwmValA, float  pwmValB, float  pwmValC)
                                                          {
                                                   
                                                               position1=SPI_ReadPosition (1)-217.62;  //(1) or (2) to asign an specific encoder
                                                               //delay(1);
                                                               position2=SPI_ReadPosition (2)-233.88;
                                                               //delay(1);
                                                                                                               
                                                               String encoderLecture1 = String(position1,2 );
                                                               String encoderLecture2 = String(position2,2 );
                                                               String stringPwmValA = String(pwmValA);
                                                               String stringPwmValB = String(pwmValB);
                                                               String stringPwmValC = String(pwmValC);
                                                               
                                                               //String lineCount=String(count+",");
                                                               String encoderLectures= String( encoderLecture1+ "," + encoderLecture2+ ",");
                                                               String pwmValues= String( stringPwmValA+","+ stringPwmValB+","+ stringPwmValC+",");
                                                      
                                                               Serial.print(count);
                                                               Serial.print(","+encoderLectures+pwmValues);
                                                               Serial.println();
                                                           
                                                          }
