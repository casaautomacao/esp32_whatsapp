// =========================
// ENVIO GENÉRICO
// =========================
void enviarParaNodeRed(String payload) {
  WiFiClientSecure client;
  client.setInsecure(); // ignora certificado HTTPS

  HTTPClient http;
  http.begin(client, NODE_RED_URL);

  http.addHeader("x-api-key", senhaNode);
  http.addHeader("Content-Type", "application/json");

  int httpCode = http.POST(payload);

  Serial.println("HTTP Code: " + String(httpCode));
  Serial.println("Resposta:");
  Serial.println(http.getString());

  http.end();
}

// =========================
// MENU WHATSAPP (BOTÕES)
// =========================
void enviarMenuWhatsApp() {

  String payload = String("{") +
    "\"messaging_product\":\"whatsapp\"," +
    "\"to\":\"" + String(WHATSAPP_TO) + "\"," +
    "\"type\":\"interactive\"," +
    "\"interactive\":{" +
      "\"type\":\"button\"," +
      "\"body\":{" +
        "\"text\":\"Escolha uma opcao:\"" +
      "}," +
      "\"action\":{" +
        "\"buttons\":[" +

          "{\"type\":\"reply\",\"reply\":{\"id\":\"mantem\",\"title\":\"1 - Mantem\"}}," +

          "{\"type\":\"reply\",\"reply\":{\"id\":\"desabilita\",\"title\":\"2 - Desabilita\"}}," +

          "{\"type\":\"reply\",\"reply\":{\"id\":\"seguranca\",\"title\":\"3 - Liga Seguranca\"}}" +

        "]" +
      "}" +
    "}" +
  "}";

  enviarParaNodeRed(payload);
}



void checarComando() {
  WiFiClientSecure client;
  client.setInsecure();

  HTTPClient http;
  http.begin(client, "https://api.crescerengenharia.com/get-comando");

  // 🔐 envia a senha
  http.addHeader("x-api-key", senhaNode);

  int httpCode = http.GET();

  if (httpCode == 200) {
    String payload = http.getString();

    Serial.println(payload);

    if (payload.indexOf("mantem") >= 0) {
      acaoMantem();
    }
    else if (payload.indexOf("desabilita") >= 0) {
      acaoDesabilita();
    }
    else if (payload.indexOf("seguranca") >= 0) {
      acaoSeguranca();
    }
  } else {
    Serial.println("Erro HTTP: " + String(httpCode));
  }

  http.end();
}





void acaoMantem() {
  setPolling = 0;
  Serial.println("MANTEM");

 digitalWrite(32, 1);    
 digitalWrite(4, 0);   

}

void acaoDesabilita() {
   setPolling = 0;
  Serial.println("DESABILITA");
 digitalWrite(32, 0);    

 setaAlarme = 0;


}

void acaoSeguranca() {
   setPolling = 0;
  Serial.println("SEGURANCA");

setaAlarme = 1;

}
