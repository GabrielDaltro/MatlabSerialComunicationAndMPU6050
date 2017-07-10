%Autor: Gabriel Daltro Duarte
%Data: 08/06/2017
%Local: Jo�o Pessoa, Para�ba - Brasil

    clc;

    %Inicicializa a comunica��o com o arduino. porta_serial � o objeto do
    %Tipo porta serial que est� conectado a porta serial vinculada ao
    %arduino
    porta_serial = SerialArduinoBegin('COM5',9600);
     
    %vetor a ser enviado para o arduino
    ang (1:2:181) = 0:90;
    ang (180:-2:2) = 91:180;
    
    for i=0:180
            %envia o angulo desejado para o arduino
            SerialArduinoSend ( ang(i+1), porta_serial );
           
            %Exibe o �ngulo enviado
            disp ( 'Enviei: '); 
            disp(ang(i+1));
            
            %L� o �ngulo enviado pelo arduino e o salva no vetor dataOutArduinoDouble
            dataOutArduinoDouble( ang(i+1) + 1) = SerialArduinoReadDouble( porta_serial );
            
            %Exibe o �ngulo enviado pelo arduino
            disp ( 'Recebi: '); 
            disp (  dataOutArduinoDouble( ang(i+1) + 1) );           
    end 
   
    %Arredonda os �ngulos recebidos
    dataOutArduinoDoubleRound = round (dataOutArduinoDouble);
    
    %fecha a porta serial 
    fclose(porta_serial);
    
    %Plota o gr�fico dos dados recebidos e enviados
    plot (0:180,dataOutArduinoDoubleRound);
    hold on;
    plot ([0 180],[0 180],'r');
    
    %Procura a mair diferen�a entre o �ngulo de destino e o �ngulo medido
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
