#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include "secrets.h"
#include <Crescer.h>

int vetorOpto[8] = { 23, 18, 14, 27, 15, 2, 5, 19 };


unsigned long lastPoll = 0;
const long interval = 2000;  // 2 segundos
int setPolling = 0;
int setaAlarme = 0;

Tempora temp1;
Tempora temp2;
Tempora temp3;


WiFiServer server(80);

void setup() {
  Serial.begin(115200);

  temp1.defiSP(500);
  temp2.defiSP(2000);
  temp3.defiSP(500);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();



  pinMode(32, OUTPUT);
  pinMode(33, OUTPUT);
  pinMode(4, OUTPUT);



  Serial2.begin(115200, SERIAL_8N1, 16, 17);


  //29ma



  for (int i = 0; i < 8; i++) {
    pinMode(vetorOpto[i], INPUT_PULLUP);
  }

  //pinMode(vetorOpto[4], INPUT);
  analogReadResolution(12);
  //testSerial.begin(9600, SWSERIAL_8N1, 26, 25, false, 95, 11);

  Serial1.begin(115200, SERIAL_8N1, 26, 25);
  pinMode(13, OUTPUT);
}

int value = 0;


String acum = "";
String acum2 = "";
int leitura36, leitura39;

int statusSerial = 0;


void loop() {

  delay(10);


  leitura36 = analogRead(36);
  leitura39 = analogRead(39);

  //testSerial.println("Ola");
  Serial1.write('o');
  Serial1.write('l');
  Serial1.write('a');


  while (Serial1.available() > 0) {
    char leitura = Serial1.read();
    acum2 += leitura;
    //Serial.write(testSerial.read());
    delay(2);
  }

  statusSerial = 0;

  if (acum2.indexOf("ola") != -1)  //temp1.Saida(1)
  {

    statusSerial = 1;
  }

  acum2 = "";

  while (Serial.available() > 0) {
    Serial2.write(Serial.read());
    delay(10);
  }
  while (Serial2.available() > 0) {
    Serial.write(Serial2.read());
    delay(10);
  }

  if (temp1.Saida(1)) {

    for (int i = 0; i < 8; i++) {
      Serial.print(digitalRead(vetorOpto[i]));
    }
    Serial.print("    ");
    Serial.print(leitura36 * 5.00 / 4096);
    Serial.print(",");
    Serial.print((leitura39 * 5.00 / 4096) + 0.3);

    Serial.print(",");
    Serial.print(statusSerial);


    Serial.println();
    temp1.Saida(0);
  }



  if (temp2.Saida(1)) {
    digitalWrite(13, 1 - digitalRead(13));
    temp2.Saida(0);
  }




  if (!digitalRead(vetorOpto[0])) {

    // envia o menu ao iniciar
    enviarMenuWhatsApp();

    delay(2000);
    setPolling = 1;
  }

  if (millis() - lastPoll > interval and setPolling) {
    lastPoll = millis();
    checarComando();
  }





  if (!digitalRead(vetorOpto[2])) {

    digitalWrite(32, HIGH);
  }


  if (setaAlarme) {
    if (temp3.Saida(1)) {

      digitalWrite(4, 1 - digitalRead(4));

      temp3.Saida(0);
    }
  }

  /*

  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
         //   client.print("Opto 8 <a href=\"/L19\">Liga</a> <a href=\"/H19\">Desl</a> to turn the LED on pin 19 on.<br><br>");
         //   client.print("Opto 7 <a href=\"/L5\">Liga</a> <a href=\"/H5\">Desl</a> to turn the LED on pin 5 on.<br><br>");
          //  client.print("Opto 6 <a href=\"/L2\">Liga</a> <a href=\"/H2\">Desl</a> to turn the LED on pin 2 on.<br><br>");
          //  client.print("Opto 5 <a href=\"/L15\">Liga</a> <a href=\"/H15\">Desl</a> to turn the LED on pin 15 on.  USE ESTE PINO PARA TESTE DO RS485 <br><br>");

            client.print("RELE 1 <a href=\"/H32\">Liga</a><a href=\"/L32\">Desl</a> to turn the LED on pin 32 off.<br><br>");

            client.print("RELE 2 <a href=\"/H33\">Liga</a><a href=\"/L33\">Desl</a> to turn the LED on pin 33 off.<br><br>");

            client.print("RELE 3 <a href=\"/H4\">Liga</a><a href=\"/L4\">Desl</a> to turn the LED on pin 4 off.<br>");


            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }


        if (currentLine.endsWith("GET /H32")) {
          digitalWrite(32, HIGH);               // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /L32")) {
          digitalWrite(32, LOW);                // GET /L turns the LED off
        }
        if (currentLine.endsWith("GET /H33")) {
          digitalWrite(33, HIGH);               // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /L33")) {
          digitalWrite(33, LOW);                // GET /L turns the LED off
        }
        if (currentLine.endsWith("GET /H4")) {
          digitalWrite(4, HIGH);               // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /L4")) {
          digitalWrite(4, LOW);                // GET /L turns the LED off
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
*/
}
