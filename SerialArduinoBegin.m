function [ porta_serial ] = SerialArduinoBegin( StringPorta, NumberBaudRate )
   
    porta_serial = serial (StringPorta, 'BaudRate', NumberBaudRate);

    fopen(porta_serial);

    %Aguarda que o Arduino envie o string 'SIM' indicando que sua porta serial
    %foi incicializada
    while ( (strcmp (fscanf(porta_serial,'%s'),'SIM')) == 0 ) 
       %disp ('Esperando SIM');
    end
    
end

