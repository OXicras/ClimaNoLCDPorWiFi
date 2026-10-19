#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd (0x27, 20, 4);

String pedirParaApiClima(String);

const char* nomeRedeWiFi = "SENAI EDUCACIONAL";
const char* senhaRedeWiFi = "info@134";
const String enderecoAPI = "https://wttr.in/Sao+Caetano+do+Sul?m&lang=pt&format=";

void setup() 
{
  lcd.init();
  lcd.backlight();
  Serial.begin(115200);
  WiFi.begin(nomeRedeWiFi, senhaRedeWiFi);

  Serial.print("\n\r Iniciando conexão WiFi\n\r");
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print("#");
    delay(500);
  }

  Serial.printf("\n\rConectado com sucesso\n\r");
  Serial.printf("Endereco IP: %s\n\r", WiFi.localIP().toString().c_str());
  Serial.printf("Endereco MAC: %s\n\r", WiFi.macAddress().c_str());

  String clima;
  int temperatura, sensacaoTermica;
  clima = pedirParaApiClima("%C");
  temperatura = pedirParaApiClima("%t").toInt();
  sensacaoTermica = pedirParaApiClima("%f").toInt();

  Serial.printf("Condicão climática para hoje: %s\n\r", clima.c_str());
  Serial.printf("Temperatura Atual %d° C e sensacão térmica de %d° C", temperatura, sensacaoTermica);
}

void loop() {}

String pedirParaApiClima(String codigo)
{
  HTTPClient http;
  http.begin(enderecoAPI + codigo);
  
  int codigoHTTP = http.GET();
  String resposta;

  if(codigoHTTP == 200)
  {
    resposta = http.getString();
  }
  else
  {
    Serial.printf("\n\rDeu erro. Código: %d", codigoHTTP);
  }
  http.end();
  return resposta;
}