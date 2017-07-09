%Autor: Gabriel Daltro Duarte
%Data: 08/06/2017
%Local: João Pessoa, Paraíba - Brasil



    fclose (porta_serial);
    %clear;
    clc;

    porta_serial = serial ('COM5', 'BaudRate', 9600);

    fopen(porta_serial);

    %Aguarda que o Arduino envie o string 'SIM' indicando que sua porta serial
    %foi incicializada
    while ( (strcmp (fscanf(porta_serial,'%s'),'SIM')) == 0 ) 
       %disp ('Esperando SIM');
    end
      
    %ang = zeros (1,181,'int16');
    ang (1:2:181) = 0:90;
    ang (180:-2:2) = 91:180;
    
    for i=0:180  
             %converte o número a ser enviado em string
             dataInArduinoString = num2str(ang(i+1),10); %Converte o numero double a ser enviado em string
             %Envia o string para o Matlab
            fprintf (porta_serial,'%s*',dataInArduinoString);
            disp ( 'Enviei: '); 
            %Exibe o string enviado
            disp(dataInArduinoString);

            a = '0';
            while (a ~= '*')
               if ( porta_serial.BytesAvailable > 0 )
                  a = fscanf(porta_serial,'%s'); %Exibe o string enviado pelo arduino
                  disp ( 'Recebi: ');
                  disp( a );
               end
            end

            a = fscanf(porta_serial,'%s'); %Exibe o string enviado pelo arduino
            disp ( 'Final: ');
            disp( a );
            dataOutArduinoDouble( ang(i+1) + 1) = str2double(a);
            
  
    end 
    
    
    dataOutArduinoDoubleRound = round (dataOutArduinoDouble);
    fclose(porta_serial);
    plot (0:180,dataOutArduinoDoubleRound);
    hold on;
    plot ([0 180],[0 180],'r');
    
    maiorErro = abs ( dataOutArduinoDoubleRound(1) - 0);
    posMaiorErro = 0;
    for i = 1:180
        erro = abs(dataOutArduinoDoubleRound(i+1) - i);
        if (erro > maiorErro)
            maiorErro = erro;
            posMaiorErro = i;
        end
    end
    disp ('Maior ERRO: ');
    disp (maiorErro);
    disp ('No Angulo: ');
    disp (posMaiorErro);
