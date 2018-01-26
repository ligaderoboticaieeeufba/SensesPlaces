/*
Esse código é o responsável por abrir as portas no C.I e enviar as medições para os wemos que controla os 3 equipamentos.

*/

// ----- TERMISTOR ----
#include <Thermistor.h>

Thermistor temp(0);

// --- MQTT ---

#include <PubSubClient.h>

const char* mqtt_server = "iot.eclipse.org";

const char* temp_analogico = "danca/termistor/analogico/fan";
const char* piezo_analogico = "danca/piezo/analogico/fumaca";
const char* batimento_coracao = "danca/infravermelho/analogico/luz";
const char* status1 = "danca/status/dancarino/1";

const char* mqtt_ClientID = "wemos1";

// --- ESP8266 ---

#include <ESP8266WiFi.h>

const char* ssid = "AndroidAP";
const char* password = "teste123";

WiFiClient espClient;
PubSubClient client(espClient);

#define Z A0

// pinos para selecionar os canais
int pin_seletores[] = {12, 13, 15}; // A(D6), B(D7), C(D8)

//configura o WiFi
void setup_wifi() {

  delay(500);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Conectando a: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Conectado ao WiFi");
  Serial.println("Endereço de IP: ");
  Serial.println(WiFi.localIP());

  client.publish(status1, "Dançarino 1 conectado.", true);
}

// função para reconectar o wemos
void reconecta(){
  while(!client.connected()){
    client.publish(status1, "Reconectando...");
    if(client.connect(mqtt_ClientID)){
      client.publish(status1, "Conectado");
    } else{
      client.publish(status1, "Falha. Tentaremos novamente em 2 segundos.");
      
    }
  }
}

void setup() {
  
  Serial.begin(115200);
  setup_wifi();

  Serial.println("");
  
  Serial.println("Criando o servidor..");
  
  delay(3000);
  client.setServer(mqtt_server, 1883);
  
  Serial.println("Servidor criado");
  
  for(int pin = 0; pin < 3; pin++){
    pinMode(pin_seletores[pin], OUTPUT);
  }
  Serial.println("Pinos setados");
}

void publicaTemperatura(){

  int analogico_temp = 0;
  
   //  C = 0   B = 0   A = 1 ----> TEMPERATURA
  digitalWrite(pin_seletores[0], HIGH);
  digitalWrite(pin_seletores[1], LOW);
  digitalWrite(pin_seletores[2], LOW);
   
  int temperatura_lida = temp.getTemp();
  String val = String(temperatura_lida).c_str();
  String celsius = val + "*C";
   
  Serial.print("A temperatura é: ");
  Serial.println(celsius);
  
  //client.publish(temp_celsius, String(celsius).c_str());
  
  analogico_temp = analogRead(Z);
  Serial.print("Leitura analógica da temperatura: ");
  Serial.println(analogico_temp);
  
  // publica o valor analogico da temperatura
  client.publish(temp_analogico, String(analogico_temp).c_str());
  
  delay(1000);
}

void publicaBatimentos(){

  int analogico_infraRed = 0;
 
  // A = 0 B = 1 C = 0 ---> CORAÇAO
  digitalWrite(pin_seletores[0], LOW);
  digitalWrite(pin_seletores[1], HIGH);
  digitalWrite(pin_seletores[2], LOW);

  //Serial.println("Pino 2 aberto");

  analogico_infraRed = analogRead(Z);
  Serial.print("Leitura analógica do coração: ");
  Serial.println(analogico_infraRed);
  
  //publica o valor analogico das batidas analogicas
  client.publish(batimento_coracao, String(analogico_infraRed).c_str());
  delay(500);

}

void publicaVibracoes(){
   
   int analogico_piezo = 0;
   
  // C = 0  B = 0   C = 0 ---- > PIEZO
  digitalWrite(pin_seletores[0], LOW);
  digitalWrite(pin_seletores[1], LOW);
  digitalWrite(pin_seletores[2], LOW);

  //Serial.println("porta 0 aberta");

  analogico_piezo = analogRead(Z);
  Serial.print("Leitura analógica do piezo: ");
  Serial.println(analogico_piezo);
  Serial.println("----------------------");
  
  
  //publica o valor analogico do piezo
  client.publish(temp_analogico, String(analogico_piezo).c_str());

}

void loop() {
   
  if(!client.connected()){
    reconecta();
  }
  
  publicaTemperatura();
  //publicaBatimentos();
 // publicaVibracoes();
 
}
