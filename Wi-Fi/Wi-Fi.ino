

#include <SoftwareSerial.h>
#include "ESP8266WiFi.h"

 
 SoftwareSerial miSerial(D2,D0);
 float n=0;
 float medidas[100];
 int cont = 0;
 WiFiServer server(80);
 
 
void setup() {
  Serial.begin(115200);
  miSerial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin("Red Molina", "Molina_2018");
  Serial.println("Comenzando Conexión");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectado");
  Serial.println(WiFi.localIP());
  server.begin();
}
 
void loop() {
  String m;
  if (miSerial.available()){
    n=miSerial.read();
    if(n>30)
      n=0;
    Serial.println(n);
    medidas[cont] = n;
    cont++;
    if(cont>99)
       cont=0;
  }
  WiFiClient client = server.available();
    if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    while (client.connected()) {            // loop while the client's connected      
            client.println("<!DOCTYPE html><html>");
            client.println("<head><title>prueba</title></head>");
            client.println("<body><h1>Medidas</h1>");
            for(int i = 0; i<=cont; i++){
              m = String(medidas[i]);
              client.println("<p>Distancia: " + m + "</p>");
     
            }  
            client.println("</body></html>");
            client.println();
            break;
          }
          client.stop();
          Serial.println("Client disconnected.");
          Serial.println("");
        } 
}
