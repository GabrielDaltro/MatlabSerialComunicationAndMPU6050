%Autor: Gabriel Daltro Duarte
%Data: 08/06/2017
%Local: João Pessoa, Paraíba - Brasil



    fclose (porta_serial);
    
    clc;
    
    %Dados a serem enviados ao Arduino no formato double    
    for i = 1:10
        dataInArduinoDouble(i) = i*1.1;
    end
     %dataInArduinoDouble(1) = 115.123;

    porta_serial = serial ('COM6', 'BaudRate', 9600);

    fopen(porta_serial);

    %Aguarda que o Arduino envie o string 'SIM' indicando que sua porta serial
    %foi incicializada
    while ( (strcmp (fscanf(porta_serial,'%s'),'SIM')) == 0 ) 
       %disp ('Esperando SIM');
    end
   
    
    %flushinput(porta_serial); %limpa o buffer da porta serial
    
    for i=1:180 
     %i = 1;   
     %dataInArduinoDouble(i) = 30;           
              dataInArduinoString = num2str(i,10); %Converte o numero double a ser enviado em string

            %for i = 1:length(dataInArduinoString)
            %    fwrite(porta_serial,dataInArduinoString(i),'char'); 
            %end
            %fwrite(porta_serial,'c','char'); 

            fprintf (porta_serial,'%s*',dataInArduinoString); %Envia o string para o Matlab
            disp ( 'Enviei: '); %Exibe o string enviado
            disp(dataInArduinoString);

            a = '0';
            while (a ~= '*')
            %while (a ~= '\0')
               % while ( porta_serial.BytesAvailable < 0 ) %aguarda a resposta do Arduino
               % end

               if ( porta_serial.BytesAvailable > 0 )
                  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                  a = fscanf(porta_serial,'%s'); %Exibe o string enviado pelo arduino
                  disp ( 'Recebi: ');
                  disp( a );
               end
            end

            a = fscanf(porta_serial,'%s'); %Exibe o string enviado pelo arduino
            disp ( 'Final: ');
            disp( a );
            dataOutArduinoDouble(i+1) = str2double(a);
            
            %while ( (strcmp (fscanf(porta_serial,'%s'),'SIM')) == 0 ) 
                %disp ('Esperando SIM');
            % end
    
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
