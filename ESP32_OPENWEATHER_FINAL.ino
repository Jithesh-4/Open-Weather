//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

const char* ssid = "JITHU4G";
const char* password = "jithu2004";

//URL Endpoint for the API
String URL = "http://api.openweathermap.org/data/2.5/weather?";
String ApiKey = "f844ea07cffb5358e45a8b0a876b189b";

// Replace with your location Credentials
String lat = "12.869760111732981";
String lon = "80.05829176261648";


void setup()
{
  Serial.begin(115200);
  lcd.init();                      // initialize the lcd 
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  /*lcd.setCursor(0,0);
  lcd.print("Hello, world!");
  lcd.setCursor(0,1);
  lcd.print("Ywrobot Arduino!");*/
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void loop()
{
  if (WiFi.status() == WL_CONNECTED) {

    HTTPClient http;

    //Set HTTP Request Final URL with Location and API key information
    http.begin(URL + "lat=" + lat + "&lon=" + lon + "&units=metric&appid=" + ApiKey);

    // start connection and send HTTP Request
    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0) {

      //Read Data as a JSON string
      String JSON_Data = http.getString();
      Serial.println(JSON_Data);

      //Retrieve some information about the weather from the JSON format
      DynamicJsonDocument doc(2048);
      deserializeJson(doc, JSON_Data);
      JsonObject obj = doc.as<JsonObject>();

      //Display the Current Weather Info
      const char* description = obj["weather"][0]["description"].as<const char*>();
      const float temp = obj["main"]["temp"].as<float>();
      const int humidity = obj["main"]["humidity"].as<int>();
      const int pressure = obj["main"]["pressure"].as<int>();
      const float speed = obj["main"]["speed"].as<float>();
      const int sunrise = obj["main"]["sunrise"].as<int>();
      const int sunset = obj["main"]["sunset"].as<int>();


      lcd.setCursor(0,0);
      lcd.print("TEMP:     C");
      lcd.setCursor(5,0);
      lcd.print(temp);
      lcd.setCursor(0,1);
      lcd.print("HUMIDITY:  %");
      lcd.setCursor(9,1);
      lcd.print(humidity);

    } else {
      Serial.println("Error!");
      lcd.clear();
      lcd.print("Can't Get DATA!");
    }

    http.end();
  }
  delay(1000);
}


