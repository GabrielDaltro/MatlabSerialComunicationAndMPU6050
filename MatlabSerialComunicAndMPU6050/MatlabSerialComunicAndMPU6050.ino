#include<math.h>
#include<Servo.h>

#include <Chauvenet.h>
#include <MPU6050.h>
#include <SerialMatlab.h>

#define TAM 10 // Quantida de amostras lidas do MPU6050

Chauvenet chauvenet; // Objeto chauvenet que irá verificar a necessidade de rejeição de amostras
SerialMatlab serialmatlab1; // objeto serialmatlab1 que será utilizado para se comuncar com o matlab
Servo myServo; // objeto myServo utilizado para controlar o servo motor 
MPU6050 mpu6050; // objeto mpu6050 utilizado para realizar leituras do sensor MPU6050 

int offset = 0; // offset de calibração do ângulo do servo para se aproximar do ângulo lido pelo MPU6050
float x,y; // armazenam as projeções do vetor g nos eixos x e y respectivamente
double anguloLido; // armazena o ângulo inclinação do sensor no plano xy
int anguloDestino; // armazena o ângulo que o servo irá tomar 
double VetAngulosLidos[TAM]; // armarzena TAM amostras do ângulo inclinação do sensor no plano xy


void setup() 
{
    serialmatlab1.begin(&Serial); // Inicializa a comunicação com o Matlab
    mpu6050.begin(); // Inicializa o MPU6050
    myServo.attach (8); // Configura o pino de comunicação entre o arduino e o servo
}

void loop() 
{
      // Recebe um valor inteiro enviado pelo matlab que representa o ângulo que o servo deve assumir
      anguloDestino = serialmatlab1.receiverInt(); 
      
      offset = 0;
      anguloDestino = 180 - (anguloDestino + offset); // calibração para ficar igual a leitura do MPU6050
      
      //Envia o ângulo para o servo motor
      myServo.write(anguloDestino);
      //aguarda o servo assumir o ângulo desejado
      delay (2000);

      // realiza TAM  leituras do MPU6050 e as salva no vetor VetAngulosLidos para verificar a posição angular do servo
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

      // VetAngulosLidos é passado para o objeto chauvenet para verificar se alguma amostra pode ser descartada 
      chauvenet.setAmostras(VetAngulosLidos,TAM);
      chauvenet.verificaAmostras();

     //Envia a média das amostras não descartadas para o matlab
     serialmatlab1.send( chauvenet.getMediaAposDescarte() );
}
