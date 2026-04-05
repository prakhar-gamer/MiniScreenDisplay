#include <Arduino.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <WiFi.h>
#include <SpotifyEsp32.h>
#include <SPI.h>
#include <TJpg_Decoder.h>
#include<HTTPClient.h>
#define TFT_CS 1
#define TFT_RST 2
#define TFT_DC 3
#define TFT_SCLK 4
#define TFT_MOSI 5


const char* CLIENT_ID = "not gonna leak it buddy";
const char* CLIENT_SECRET = "Almost leaked this forsure";
Spotify rakhs(CLIENT_ID, SLIENT_SECRET);

Adafruit_ST7735 screen = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

String lastArtist;
String lastTrackName;

void setup() {
  Serial.begin(115200);

  screen.initR(INITR_BLACKTAB);
  screen.setRotation(1);
  Serial.println("Screen Set!");

  char* SSID = "dontwannaleakthistoo";
  char* PASSWORD = "hhahahahahhaha";

  Wifi.begin(SSID, PASSWORD);
  Serial.print("Connecting...");
  while(Wifi.status() != WL_CONNETED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.print("Connected!");

  tft.setCuresor(0,0);
  tft.write(Wifi.localIP().toString().c_str());
  
  rakhs.begin();
  while(!sp.is_auth()) {
    sp.handle_client();
  }
  Serial.print("Connected to Spotify!");
}

void loop() {
  String artName = rakhs.current_artist_names();
  String currentTrackName = rakhs.current_track_name();

  JsonDocument filter; 
  filter["progress_ms"] = true;
  filter["item"]["duration_ms"] = true;

  response r = spotify.get_currently_playing_track(filter);

  long currentTime = r.reply["progress_ms"] | 0;
  long totalTime = r.reply["item"]["duration_ms"] | 0;

  if (lastArtist != artName && artName != "Something went wrong" && !artName.isEmpty()){
    tft.fillScreen(ST77XX_BLACK);
    lastArtist = artName;
    tft.setCursor(10,10);
    tft.write(lastArtist.c_str());
  }

  if (lastTrackName != currentTrackName && currentTrackName != "Something went wrong && !currentTrackName.isEmpty()){
    lastTrackName = currentTrackName;
    tft.setCursor(10,40);
    tft.write(lastTrackName.c_str());
    tft.setCursor(10,70);
    String time = currentTime + ":" + totalTime;
    tft.write(time.c_str());
  }
}

