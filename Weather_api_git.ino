#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid     = "........";
const char* password = "........";
String API_key=".........";  //Your Open Weather API key
String zipcode=".........";  // Your Zip code
String country_code="....."; //Your country code Ex: us

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
}

void loop() {
  // put your main code here, to run repeatedly:
  HTTPClient client;

  client.begin("http://api.openweathermap.org/data/2.5/weather?zip="+zipcode+","+country_code+"&appid="+API_key);
  int httpCode=client.GET();

  if(httpCode >0)
  {
    String payload = client.getString();
    Serial.println(payload);
    char string_to_array[1000];

   // payload.replace(" ", "");
   // payload.replace("\n", "");
   // payload.trim();
   // payload.remove(0,1);
    payload.toCharArray(string_to_array,1000);

    //convert array to json format
    StaticJsonDocument<1000> doc;
    deserializeJson(doc,string_to_array);

    float temperature=doc["main"]["temp"];
    temperature=temperature-273.15;
    float pressure=doc["main"]["pressure"];
    pressure=pressure/33.86;//33.863886666667;
    unsigned int humidity=doc["main"]["humidity"];
    String description=doc["weather"][0]["description"];
    String name_city =doc["name"];
    Serial.println("Place:"+String(name_city));
    Serial.println("Description:"+String(description));
    Serial.println("Temp:"+String(temperature));
    Serial.println("Pressure:"+String(pressure));
    Serial.println("Humidity:"+String(humidity));

    if(description == "clear sky")
    {
      Serial1.print("page 1");
      Serial1.write(0xFF);
      Serial1.write(0xFF);
      Serial1.write(0xFF);

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
    }
    

    client.end();
  }
  else
  {
    Serial.println("Error http request");
  }
 delay(10000);
}
