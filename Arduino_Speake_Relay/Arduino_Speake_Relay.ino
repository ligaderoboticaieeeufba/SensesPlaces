/* Código para ligação do Arduino no Relé e ativação do Alto Falante.
O Arduino receberá o sinal de um Wemos para ativar o Alto Falante.

Esse Wemos não estará ligado diretamente ao relay e sim a um Arduino Uno que, usando a função
tone(), acionará um alto falante.
 
*/
#define i 12
#define o 13
void setup() {
 pinMode(o, OUTPUT);
 pinMode(i, INPUT);
}

void loop() {
  digitalRead(i);
  if(i == HIGH){
    tone(440, o);
    delay(500);
    noTone(o); 
  }
  else if(i == LOW){
    noTone(o);
  }
  
  

}
