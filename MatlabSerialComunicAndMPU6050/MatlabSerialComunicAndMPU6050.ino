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

/***********PARA DEBUG ********************************************/
 const double* const amostrasDescartadas = chauvenet.getAmostrasRejeitadas();
 const double* const amostrasNaoDescartadas = chauvenet.getAmostrasNaoRejeitadas();  
 /********************************************************/



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

          // PARA DEBUG, APAGAR DEPOIS
              Serial.print ("X: ");
              Serial.println (x);
              Serial.print ("Y: ");
              Serial.println (y);
              Serial.print ("Angulo: ");
              Serial.println (VetAngulosLidos[i]);
         //
      }

      chauvenet.setAmostras(VetAngulosLidos,TAM);
      chauvenet.verificaAmostras();

      //APAGAR DEBUG
            Serial.print ( "\n\nLista de amostras: ");
            for (int i = 0; i < TAM; i++)
            {
               Serial.print ( VetAngulosLidos[i]);
               Serial.print (" ");
            }
            Serial.println ();
        
            // imprime o vetor que armazena as amostras descartadas
             Serial.print ( "\n\nLista de amostras descartadas: ");
              for (int i = 0; i < chauvenet.getQtdAmostrasRejeitadas(); i++)
              {
                 Serial.print ( amostrasDescartadas[i]);
                 Serial.print (" ");
              }
      
            Serial.print ("\n\nLista de amostras NAO descartadas: ");
        
              for (int i = 0; i < chauvenet.getQtdAmostrasNaoRejeitadas(); i++)
              {
                Serial.print (amostrasNaoDescartadas[i]);
                Serial.print (" ");
              }
      
           Serial.print ( "\n\nMedia das amostras apos o descarte: ");
       //
    
     serialmatlab1.send( chauvenet.getMediaAposDescarte() );
}
