#include <Thermistor.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

Thermistor temp(0);

const char* ssid = "AndroidAP";
const char* password = "teste123";
const char* mqtt_server = "iot.eclipse.org";

const char* temp_analogico = "teste/temperatura/analogico";
const char* temp_celsius = "teste/temperatura/celsius";
const char* piezo_analogico = "teste/piezo/analogico";
const char* batimento_coracao = "teste/coracao";
const char* mqtt_ClientID = "testeCI";

WiFiClient espClient;
PubSubClient client(espClient);

#define Z A0

int pin_seletores[] = {12, 13, 15}; // A(D6), B(D7), C(D8)

// heart Piezo 
int threshold = 60;
int oldvalue = 0;
int newvalue = 0;
unsigned long oldmillis = 0;
unsigned long newmillis = 0;
int cnt = 0;
int timings[16];
int heartrate;

void  heartP(){
    oldvalue = newvalue;
    newvalue = 0;
    
    for(int i=0; i<64; i++){ // Average over 16 measurements
        newvalue += analogRead(Z);
    }
    newvalue = newvalue/64;
    
    // find triggering edge
    if(oldvalue<threshold && newvalue>=threshold){
        oldmillis = newmillis;
        newmillis = millis();
        // fill in the current time difference in ringbuffer
        timings[cnt%16]= (int)(newmillis-oldmillis);
        int totalmillis = 0;
        // calculate average of the last 16 time differences
        for(int i=0;i<16;i++){
            totalmillis += timings[i];
        }
        // calculate heart rate
        heartrate = 60000/(totalmillis/16);
        Serial.print("Valor do batimento cardiaco: ");
        Serial.println(heartrate);
        client.publish(batimento_coracao, String(heartrate).c_str(), true);
        cnt++;
    }


}



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
}

void reconecta(){
  while(!client.connected()){
    client.connect(mqtt_ClientID);
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


void loop() {

  int analogico_infraRed = 0;
  int analogico_piezo = 0;
  int analogico_temp = 0;
  
  if(!client.connected()){
    reconecta();
  }
  /*
  // C = 0  B = 0   C = 0
  digitalWrite(pin_seletores[0], LOW);
  digitalWrite(pin_seletores[1], LOW);
  digitalWrite(pin_seletores[2], LOW);

  //Serial.println("porta 0 aberta");

  analogico_piezo = analogRead(Z);
  Serial.print("Leitura analógica do piezo: ");
  Serial.println(analogico_piezo);
  //publica o valor analogico do piezo
  client.publish(piezo_analogico, String(piezo_analogico).c_str(), true);
  */
  
  //  C = 0   B = 0   A = 1
  /*
  digitalWrite(pin_seletores[0], HIGH);
  digitalWrite(pin_seletores[1], LOW);
  digitalWrite(pin_seletores[2], LOW);
  
  //Serial.println("Pino 1 aberto");
  
  int temperatura_lida = temp.getTemp();
    
  Serial.print("A temperatura é: ");
  Serial.print(temperatura_lida);
  Serial.println("*C");
  
  String temperatura = String(temperatura_lida).c_str();
  String celsius = temperatura + "*C";
  // publica o valor em celsius da temperatura
  client.publish(temp_celsius, String(celsius).c_str(), true);
  
  analogico_temp = analogRead(Z);
  Serial.print("Leitura analógica da temperatura: ");
  Serial.println(analogico_temp);
  
  // publica o valor analogico da temperatura
  client.publish(temp_analogico, String(analogico_temp).c_str(), true);
  
  */
  // A = 0 B = 1 C = 0
  digitalWrite(pin_seletores[0], LOW);
  digitalWrite(pin_seletores[1], HIGH);
  digitalWrite(pin_seletores[2], LOW);

  heartP();

  //Serial.println("Pino 2 aberto");

  //analogico_infraRed = analogRead(Z);
  //Serial.print("Leitura analógica do coração: ");
  //Serial.println(analogico_infraRed);
  
  //publica o valor analogico das batidas analogicas
  
  //client.publish(batimento_coracao, String(analogico_infraRed).c_str(), true);
  
  
  Serial.println("----------------------");
  delay(500);
 
}
