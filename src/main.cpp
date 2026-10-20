#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include <time.h>

LiquidCrystal_I2C lcd (0x27, 20, 4);

String pedirParaApiClima(String);

const char* nomeRedeWiFi = "SENAI EDUCACIONAL";
const char* senhaRedeWiFi = "info@134";
const String enderecoAPI = "https://wttr.in/Sao+Caetano+do+Sul?m&lang=pt&format=";

//TODO =========== Servidores ===========
const char* NTP1 = "a.st1.ntp.br";
const char* NTP2 = "pool.ntp.org";
const char* FUSO = "<-03>3";
const char* DIAS[] = {"Dom", "Seg", "Ter", "Qua", "Qui", "Sex", "Sab"};

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
  delay(300);

  //? Configuracao de fuso e servidores NTP e inicia a sincronizacao
  configTzTime(FUSO, NTP1, NTP2);

  struct tm agora;
  lcd.setCursor(0, 0);
  lcd.print("Sincronizando hora");
  while(!getLocalTime(&agora)) //? o parametro é o endereco da estrutura tipo tm
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
  lcd.print("Sincronizado");
  delay(300);
}

void loop() 
{
  
  struct tm agora;
  if(getLocalTime(&agora))
  {
    lcd.setCursor(0, 1);
    lcd.printf("%s, %02d:%02d", DIAS[agora.tm_wday], agora.tm_hour, agora.tm_min);
  }
  
  //char condicaoClimatica[] = pedirParaApiClima("%C").c_str();
  lcd.setCursor(0, 0);
  lcd.print("Condicao Climatica:");
  lcd.setCursor(0, 1);
  //lcd.print();
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