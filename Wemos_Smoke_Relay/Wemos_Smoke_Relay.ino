/* Código para ligação do Wemos no Relé e ativação da Máquina de Fumaça.
O sensor a ser utilizado para controlar a Máquina de Fumaça será um Piezo.


Usar a saída 'r' por questão de pinagem diferente do Wemos D1 mini para o Arduino. 
No caso caso o pino D1 do Wemos correspondo ao pino 5 do Arduino. Ver mapeação das pinagens 
em: https://github.com/esp8266/Arduino/issues/1243
   */

// --- ESP8266 ---
#include <ESP8266WiFi.h>

const char* ssid = "AndroidAP";
const char* password = "teste123";

WiFiClient wemosd1mini;

// --- MQTT ---
#include <PubSubClient.h>

const char* mqtt_server = "iot.eclipse.org";
const char* piezo = "danca/termistor/analogico/fan";
const char* wemos2 = "danca/status/wemos/2/fumaca";
const char* esp = "fumaca";

PubSubClient client(wemosd1mini);

   
#define r 5 // o pino 5 corresponde ao pino D1 no Wemos  
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

  client.publish(wemos2, "Wemos da fumaça conectado", true);
  
}

void reconnect(){

  while(!(client.connected())){

    client.publish(wemos2, "Reconectando...", true);

    if(client.connect(esp)){
      client.publish(wemos2, "Conectado");
      client.subscribe(piezo);
    } else {
      
      String clientstatus = String(client.state()).c_str();
      String erro = "Falha ao reconectar, rc = " + clientstatus;
      client.publish(wemos2, String(erro).c_str());
      delay(1500);
      client.publish(wemos2, "Tentaremos de novo em 2 segundos");
      delay(2000);
      
    }
  } 
}

void ligaFumaca(int valoranalogico){


 /* O valor de XX deve ser bastante alto, afim de fazer a maquina de fumaça funcionar só em grande
 variação de saída analógica.
*/
  if (valoranalogico >= 90 ){  
    digitalWrite(r, HIGH);
    delay(2000);
    digitalWrite(r, LOW);
    delay(2000);
  }
  
}

void callback(char* topic, byte* payload, unsigned int length) {
  
  Serial.print("Mensagem recebida [");
  Serial.print(topic);
  Serial.print("]: ");

  //for(int i = 0; i < length; i++){
   // Serial.print((char)payload[i]);
  //}
  //Serial.println("");
  //Serial.println("--------------------------");

  int val = 0;

  for(int i = 0; i < length; i++){
    val += (int)payload[i];
  }
  val = val - 48;

  Serial.println(val);
  Serial.println("--------------------------------------------");

  ligaFumaca(val);
  
}

void loop(){
  
  if(!(client.connected())){
    reconnect();
  }
  
   client.loop();

}
