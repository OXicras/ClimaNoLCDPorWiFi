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

  lcd.setCursor(0, 0);
  lcd.print("Conectando Wifi");
  while(WiFi.status() != WL_CONNECTED)
  {
    for(int i = 15; i < 18; i++)
    {
      lcd.setCursor(i, 0);  
      lcd.print(".");
      if(WiFi.status() == WL_CONNECTED)
        break;
      delay(500);
    }
    lcd.setCursor(15, 0);
    lcd.print("   ");
    delay(200);
  }
  lcd.setCursor(0, 0);
  lcd.print("Conectado");
}

void loop() 
{
  lcd.setCursor(0, 0);
  lcd.print("Condicao Climatica:");
  lcd.setCursor(0, 1);
  lcd.print(pedirParaApiClima("%C").c_str());
  lcd.setCursor(0, 2);
  lcd.printf("Temperatura: %d C", pedirParaApiClima("%t").toInt());
  lcd.setCursor(0, 3);
  lcd.printf("Sensacao: %d C", pedirParaApiClima("%f").toInt());
}

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