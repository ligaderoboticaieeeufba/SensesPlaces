/* Código para ligação do Wemos no Relé e ativação da Máquina de Fumaça.
O sensor a ser utilizado para controlar a Máquina de Fumaça será um Piezo.


Usar a saída 'r' por questão de pinagem diferente do Wemos D1 mini para o Arduino. 
No caso caso o pino D1 do Wemos correspondo ao pino 5 do Arduino. Ver mapeação das pinagens 
em: https://github.com/esp8266/Arduino/issues/1243
   */
   
#define r 5 // o pino 5 corresponde ao pino D1 no Wemos  
void setup (){
	pinMode(r, OUTPUT);
	digitalWrite(r, LOW);
}
void loop(){
  int valoranalogico; // valor a ser recebido pela transmissão de dados

 /* O valor de XX deve ser bastante alto, afim de fazer a maquina de fumaça funcionar só em grande
 variação de saída analógica.
*/
	if (valoranalogico >= XX ){  
	  digitalWrite(r, HIGH);
    delay(2000);
    digitalWrite(r, LOW);
    delay(2000);
	}

}
