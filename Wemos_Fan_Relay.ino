/* Código para ligação do Wemos no Relé e ativação do ventilador


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

	if (valoranalogico >= XX ){ // Para temperatura alta
	  digitalWrite(r, HIGH);
    delay(10000);
    digitalWrite(r, LOW);
    delay(2000);
	}

  else if(valoranalogico <= XX && valor analogico >= YY){
    digitalWrite(r, HIGH);
    delay(3000);
    digitalWrite(r, LOW);
    delay(2000);
  }
  else (valoranalogico <= ZZ){
    digitalWrite(r, HIGH);
    delay(1000);
    digitalWrite(r, LOW);
    delay(4000);
  }
}
