#include<math.h>
#include<Servo.h>

#include <Chauvenet.h>
#include <MPU6050.h>
#include <SerialMatlab.h>

#define TAM 10

Chauvenet chauvenet; 
SerialMatlab serialmatlab1;
Servo myServo;
MPU6050 mpu6050;

int offset = 0;
float x,y;
double VetAngulosLidos[TAM];
double anguloLido;
int anguloDestino;


void setup() 
{
    serialmatlab1.begin(&Serial);
    mpu6050.begin();
    myServo.attach (8); 
}

void loop() 
{
      anguloDestino = serialmatlab1.receiverInt();
      
      offset = 0;
      anguloDestino = 180 - (anguloDestino + offset); // calibração para ficar igual a leitura do MPU6050
      myServo.write(anguloDestino);
      
      delay (2000);

      for (int i = 0; i < TAM; i++)
      {
          mpu6050.doReadings();  
          x = mpu6050.getAcX();
          y = mpu6050.getAcY();
          anguloLido = atan2(y,x)*180/3.14;
          
          if ( anguloLido > 0)
              VetAngulosLidos[i] = anguloLido;
          else
               VetAngulosLidos[i] = anguloLido + 360.0;   
         
         delay (50);
      }

      chauvenet.setAmostras(VetAngulosLidos,TAM);
      chauvenet.verificaAmostras();
    
     serialmatlab1.send( chauvenet.getMediaAposDescarte() );
}
