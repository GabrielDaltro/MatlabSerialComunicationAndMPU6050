function [ receivedDouble ] = SerialArduinoReadDouble( porta_serial )
    a = '0';
    i = 1;
     while (a ~= '*') %espera o caracter de parada *
           if ( porta_serial.BytesAvailable > 0 ) %espera a chegada de dados no buffer para realizar a leitura
              a = fread (porta_serial,1,'char'); %lê um byte do buffer da serial
              receivedVet(i) = char (a); %converte os bytes recebidos para char e os concatena formando um string
              i = i + 1;
          end
     end
     receivedVet(i-1) = ''; %sobrescreve o '*' com o caracter vazio
     receivedDouble = str2double(receivedVet); %Converto o número de string para double e o retorna     
end
