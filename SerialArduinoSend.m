function [  ] = SerialArduinoSend( dado, porta_serial )
    
    %Converte o numero double a ser enviado em string
    dataInArduinoString = num2str(dado,10); 
     
    %Envia o string para o Matlab
    fprintf (porta_serial,'%s*',dataInArduinoString);
        
end

