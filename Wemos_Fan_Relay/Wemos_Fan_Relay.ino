/* Código para ligação do Wemos no Relé e ativação do ventilador.
O sensor a ser utilizado para controlar o ventilador será um Termistor


Usar a saída 'r' por questão de pinagem diferente do Wemos D1 mini para o Arduino.
No caso caso o pino D1 do Wemos correspondo ao pino 5 do Arduino. Ver mapeação das pinagens
em: https://github.com/esp8266/Arduino/issues/1243
*/


// --- ESP8266 ---
#include <ESP8266WiFi.h>

const char* ssid = "";           // Wi-fi name
const char* password = "";      // Wi-fi password

WiFiClient wemosd1mini;         // Wi-fi object

// --- MQTT ---
#include <PubSubClient.h>

const char* mqtt_server = "iot.eclipse.org";
const char* termistor = "danca/termistor/analogico/fan";
const char* wemos3 = "danca/status/wemos/3/fan";
const char* esp = "termistor";

PubSubClient client(wemosd1mini);

#define r 5 // The pin 5 corresponding to pin D1 in Wemos
void setup (){

  Serial.begin(115200);

  setupWiFi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

	pinMode(r, OUTPUT);
	digitalWrite(r, LOW);
}

void setupWiFi(){
  delay(100);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");

  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  client.publish(wemos3, "Wemos do ventilador conectado", true);

}

void reconnect(){

  while(!(client.connected())){

    client.publish(wemos3, "Reconectando...", true);

    if(client.connect(esp)){
      client.publish(wemos3, "Conectado");
      client.subscribe(termistor);
    } else {

      String clientstatus = String(client.state()).c_str();
      String erro = "Falha ao reconectar, rc = " + clientstatus;
      client.publish(wemos3, String(erro).c_str());
      delay(1500);
      client.publish(wemos3, "Tentaremos de novo em 2 segundos");
      delay(2000);

    }
  }
}

void ligaVentilador(int valoranalogico){


// Os valores de XX, YY e ZZ devem ser colocados após verificação experimental

  if (valoranalogico >= 619 ){ // Para temperatura alta
    digitalWrite(r, HIGH);
    delay(10000);
    digitalWrite(r, LOW);
    delay(2000);
  }

  else if(valoranalogico <= 618 && valoranalogico >= 571){ // Para Temperatura mediana
    digitalWrite(r, HIGH);
    delay(3000);
    digitalWrite(r, LOW);
    delay(2000);
  }
  else if(valoranalogico <= 570){ // Para temperatura baixa
    digitalWrite(r, HIGH);
    delay(1000);
    digitalWrite(r, LOW);
    delay(4000);
  }

}

void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Mensagem recebida [");
  Serial.print(topic);
  Serial.print("]: ");

  for(int i = 0; i < length; i++){
    Serial.print((char)payload[i]);
  }
  Serial.println("");
  Serial.println("--------------------------");

  int val = 0;

  for(int i = 0; i < length; i++){
    val += (int)payload[i];
  }

  ligaVentilador(val);

}


void loop(){

  if(!(client.connected())){
    reconnect();
  }

  client.loop();

}
