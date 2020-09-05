#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <HardwareSerial.h>
#include <EEPROM.h>

//Fill in your detail here 
const char* ssid     = "........";
const char* password = ".........";
String API_key="..............";
String zipcode=".........";
String country_code="us";

float temperature=0.0;
float pressure=0.0;
unsigned int humidity=0;
String icon="";
float wind_speed=0.0;
float temp_min=0.0;
float temp_max=0.0;

int uartRx_Buff[10];

void setup() {
  // put your setup code here, to run once:
    Serial.begin(115200);
    Serial1.begin(9600,SERIAL_8N1,16,17);
    delay(10);

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    attachInterrupt(16,ISR_UART2_Rx ,CHANGE);
    
}

void loop() {
  // put your main code here, to run repeatedly:
  HTTPClient client;

  client.begin("http://api.openweathermap.org/data/2.5/weather?zip="+zipcode+","+country_code+"&appid="+API_key);
  int httpCode=client.GET();

  if(httpCode >0)
  {
    String payload = client.getString();
    //Serial.println(payload);
    char string_to_array[1000];
    
    payload.toCharArray(string_to_array,1000);

    //convert array to json format
    StaticJsonDocument<1000> doc;
    deserializeJson(doc,string_to_array);

    temperature=doc["main"]["temp"];
    temperature=temperature-273.15;
    pressure=doc["main"]["pressure"];
    pressure=pressure/33.86;//33.863886666667;
    humidity=doc["main"]["humidity"];
    String description =doc["weather"][0]["description"];
    String name_city =doc["name"];
    String icon=doc["weather"][0]["icon"];
    temp_min=doc["main"]["temp_min"];
    temp_min=temp_min-273.15;
    temp_max=doc["main"]["temp_max"];
    temp_max=temp_max-273.15;
    wind_speed=doc["wind"]["speed"];
   
//If its Clear sky day
    if(icon == "01d")
    {
      Serial1.print("page 1");
      Serial1.write(0xFF);
      Serial1.write(0xFF);
      Serial1.write(0xFF);
      SendWeatherData(name_city,description); 
    }
    // If its Clear sky night
   else if(icon == "01n")
    {
      Serial1.print("page 2");
      Serial1.write(0xFF);
      Serial1.write(0xFF);
      Serial1.write(0xFF);
      SendWeatherData(name_city,description); 
    }
    //If its Cloudy day
   else if(icon == "02d" || icon == "03d"|| icon == "04d")
    {
      Serial1.print("page 3");
      Serial1.write(0xFF);
      Serial1.write(0xFF);
      Serial1.write(0xFF);
      SendWeatherData(name_city,description); 
    }
    //If its Cloudy night
   else if(icon == "02n" || icon == "03n"|| icon == "04n")
    {
      Serial1.print("page 4");
      Serial1.write(0xFF);
      Serial1.write(0xFF);
      Serial1.write(0xFF);
      SendWeatherData(name_city,description); 
    }
    //If its Day Rain
   else if(icon == "09d" || icon == "10d")
    {
      Serial1.print("page 5");
      Serial1.write(0xFF);
      Serial1.write(0xFF);
      Serial1.write(0xFF);
      SendWeatherData(name_city,description); 
    }
   //If its Night Rain
   else if(icon == "09n" || icon == "10n")
    {
      Serial1.print("page 6");
      Serial1.write(0xFF);
      Serial1.write(0xFF);
      Serial1.write(0xFF);
      SendWeatherData(name_city,description); 
    }
//If its Thunderstorm
   else if(icon == "11n" || icon == "11d")
    {
      Serial1.print("page 7");
      Serial1.write(0xFF);
      Serial1.write(0xFF);
      Serial1.write(0xFF);
      SendWeatherData(name_city,description); 
    }    

//If its Snow
   else if(icon == "13d" || icon == "13n")
    {
      Serial1.print("page 8");
      Serial1.write(0xFF);
      Serial1.write(0xFF);
      Serial1.write(0xFF);
      SendWeatherData(name_city,description); 
    }  

//If its Mist
   else if(icon == "50d" || icon == "50n")
    {
      Serial1.print("page 9");
      Serial1.write(0xFF);
      Serial1.write(0xFF);
      Serial1.write(0xFF);
      SendWeatherData(name_city,description); 
    }

    
    client.end();
  }
  else
  {
    Serial.println("Error http request");
  }
 delay(10000);
}

void SendWeatherData(String name_city, String description )
{
    Serial1.print("t0.txt=\"");
    Serial1.print(name_city);
    Serial1.print("\"");
    Serial1.write(0xFF);
    Serial1.write(0xFF);
    Serial1.write(0xFF);
  
    Serial1.print("t1.txt=\"");
    Serial1.print(description);
    Serial1.print("\"");
    Serial1.write(0xFF);
    Serial1.write(0xFF);
    Serial1.write(0xFF);
   
    Serial1.print("t4.txt=\"");
    Serial1.print(String(temperature));
    Serial1.print(" C\"");
    Serial1.write(0xFF);
    Serial1.write(0xFF);
    Serial1.write(0xFF);
    
    Serial1.print("t5.txt=\"");
    Serial1.print(String(humidity));
    Serial1.print("%\"");
    Serial1.write(0xFF);
    Serial1.write(0xFF);
    Serial1.write(0xFF);
    
    Serial1.print("t6.txt=\"");
    Serial1.print(String(pressure));
    Serial1.print(" inHg\"");
    Serial1.write(0xFF);
    Serial1.write(0xFF);
    Serial1.write(0xFF);

    Serial1.print("t8.txt=\"");
    Serial1.print(String(temp_min));
    Serial1.print(" C Mn\"");
    Serial1.write(0xFF);
    Serial1.write(0xFF);
    Serial1.write(0xFF);

    Serial1.print("t9.txt=\"");
    Serial1.print(String(temp_max));
    Serial1.print(" C Mx\"");
    Serial1.write(0xFF);
    Serial1.write(0xFF);
    Serial1.write(0xFF);

    Serial1.print("t10.txt=\"");
    Serial1.print(String(wind_speed));
    Serial1.print(" m/s\"");
    Serial1.write(0xFF);
    Serial1.write(0xFF);
    Serial1.write(0xFF);
}

void ISR_UART2_Rx()
{
  int Read_val=Serial1.read();
  Serial.println(Read_val);
}
