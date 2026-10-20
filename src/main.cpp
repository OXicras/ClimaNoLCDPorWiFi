#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include <time.h>
#include <string.h>
#include <esp_sntp.h>

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
  sntp_set_sync_interval(60*60*1000); //? intervalo de atualizacao da hora na internet padrao 1*60*60*1000 (1 hora)
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
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Conectado");
  delay(500);

  //? Configuracao de fuso e servidores NTP e inicia a sincronizacao
  configTzTime(FUSO, NTP1, NTP2);

  struct tm agora;
  lcd.clear();
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
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sincronizado");
  delay(500);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.printf("%s, %02d:%02d", DIAS[agora.tm_wday], agora.tm_hour, agora.tm_min);
}

void loop() 
{
  unsigned long now = millis();
  static unsigned long tempoInicial = 0;
  const long umMinuto = 60000;

  struct tm agora;
  if(getLocalTime(&agora) && now >= tempoInicial + umMinuto)
  {
    lcd.setCursor(0, 0);
    lcd.printf("%s, %02d:%02d", DIAS[agora.tm_wday], agora.tm_hour, agora.tm_min);
    tempoInicial = now;
  }

  String condicaoClimatica = pedirParaApiClima("%C");
  for(int i = 0; i < sizeof(condicaoClimatica)-4; i++)
  {
    lcd.setCursor(0, 1);
    String textoCortado = condicaoClimatica.substring(0+i,20+i);
    lcd.printf("Condicao climatica: %s", textoCortado);
    delay(1000);
  } 
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