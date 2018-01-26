/* Código para ligação do Wemos no Relé e ativação do ventilador.
O sensor a ser utilizado para controlar o ventilador será um Termistor


Usar a saída 'r' por questão de pinagem diferente do Wemos D1 mini para o Arduino. 
No caso caso o pino D1 do Wemos correspondo ao pino 5 do Arduino. Ver mapeação das pinagens 
em: https://github.com/esp8266/Arduino/issues/1243
   */
   
#define r 5   
void setup (){
	pinMode(r, OUTPUT);
	digitalWrite(r, LOW);
}
void loop(){
  int valoranalogico; // valor a ser recebido pela transmissão de dados

// Os valores de XX, YY e ZZ devem ser colocados após verificação experimental

	if (valoranalogico >= XX ){ // Para temperatura alta
	  digitalWrite(r, HIGH);
    delay(10000);
    digitalWrite(r, LOW);
    delay(2000);
	}

  else if(valoranalogico <= XX && valor analogico >= YY){ // Para Temperatura mediana
    digitalWrite(r, HIGH);
    delay(3000);
    digitalWrite(r, LOW);
    delay(2000);
  }
  else (valoranalogico <= ZZ){ // Para temperatura baixa
    digitalWrite(r, HIGH);
    delay(1000);
    digitalWrite(r, LOW);
    delay(4000);
  }
}
