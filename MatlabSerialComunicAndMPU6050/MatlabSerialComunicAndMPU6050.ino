#include<Wire.h>
#include<math.h>
#include<Servo.h>

const int MPU=0x68;  
//Variaveis para armazenar valores dos sensores
float AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
float x,y;

byte in = 0.0; // Variável que armazena os dados recebidos do matlab 
byte out = 0.0;
byte InOut[20];
byte index = 0;
int i = 0;
byte LenthOfString = 0;

int intDataReceiver;
String StringReceiver;

char concatCharReceiver[30];
char charReceiver = '0';

Servo myServo;

void setup() {
   //Configura a porta serial para a taxa de transmissão de 9600 bits/s
    Serial.begin (9600);

    myServo.attach (8);
    
    Wire.begin();
    Wire.beginTransmission(MPU);
    //Inicializa o MPU-6050
    Wire.write(0x6B); 
    Wire.write(0); 
    Wire.endTransmission(false);
    
    //Config Escalas para Gyro e Accel, a maior nas duas
    Wire.beginTransmission(MPU);
    Wire.write(0x1B); 
    Wire.write(24);
    Wire.write(24); 
    Wire.endTransmission(true);
    

   // Envia o string "SIM" para o matlab para indicar que a porta serial já foi incializada
   //Ao reeber "SIM", o matlab sabe que já pode enviar e receber dados do arduino
    Serial.println ("SIM");     
}

void loop() {

   if (Serial.available() > 0) // Condição válida quando o arduino recebe algum byte
   {  
      
      while ( charReceiver != '*')
      {
        while (Serial.available() == 0) {} // Espera que o Buffer tenha algo para realizar a leitura
        charReceiver = Serial.read(); // Lê os dados recebidos 
        Serial.print("charReceiver = "); 
        Serial.println(charReceiver);  
        
        concatCharReceiver[i] = charReceiver;
        
        i++;
      }
      
      concatCharReceiver[i] = '\0';
      i = 0;
      charReceiver = '0';
     
      StringReceiver = concatCharReceiver;
      intDataReceiver = StringReceiver.toInt();

      intDataReceiver = 180 - (intDataReceiver + 1); // calibração para ficar igual a leitura do MPU6050
      myServo.write(intDataReceiver);
      
      delay (2000);
     

      Wire.beginTransmission(MPU);
      Wire.write(0x3B);  // inicia leitura do registrador 0x3B (ACCEL_XOUT_H)
      Wire.endTransmission(false);
      //Solicita os dados do sensor
      Wire.requestFrom(MPU,4,true);//pedindo 14 bytes  
      
      //Armazena o valor dos sensores nas variaveis correspondentes
      AcX=Wire.read()<<8|Wire.read();  //0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
      AcY=Wire.read()<<8|Wire.read();  //0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
     

      x = AcX/2024;
      y = AcY/2024;

      Serial.println( atan2(y,x)*180/3.14 );
                   
    }   
}
