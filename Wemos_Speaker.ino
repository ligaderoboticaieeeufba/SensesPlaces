/* Código para ligação do Wemos no Arduino e ativação do Alto Falante.
O sensor a ser utilizado para controlar o ALto Falante será o circuito de detectação de Pulso.

Esse Wemos não estará ligado diretamente ao relay e sim a um Arduino Uno que, usando a função
tone(), acionará um alto falante.
 
Usar a saída 'r' por questão de pinagem diferente do Wemos D1 mini para o Arduino. 
No caso caso o pino D1 do Wemos correspondo ao pino 5 do Arduino. Ver mapeação das pinagens 
em: https://github.com/esp8266/Arduino/issues/1243
   */
   
#define r 5   // o pino 5 corresponde ao pino D1 no Wemos
void setup (){
	pinMode(r, OUTPUT);
	digitalWrite(r, LOW);
}
void loop(){
  int valoranalogico; // valor a ser recebido pela transmissão de dados

 /* O valor de XX deve ser bastante alto, afim de fazer a maquina de fumaça funcionar só em grande
 variação de saída analógica.
*/
	if (valoranalogico >= 800 ){  
	  digitalWrite(r, HIGH);
    delay(500);
    digitalWrite(r, LOW);
	}

}
