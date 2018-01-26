/* Código para ligação do Arduino no Relé e ativação do Alto Falante.
O Arduino receberá o sinal de um Wemos para ativar o Alto Falante.

Esse Wemos não estará ligado diretamente ao relay e sim a um Arduino Uno que, usando a função
tone(), acionará um alto falante.
 
*/
#include <Tone.h>
Tone tone1;
 
void setup() {
  pinMode(12, INPUT);
  tone1.begin(13);
}

void loop() {
  digitalRead(12);
  if(digitalRead(12) == 1){
    tone1.play(900); 
  }
  else {
    tone1.stop();
  }
}
