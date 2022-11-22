// Inclure les bibliothèques
#include <DallasTemperature.h>
#include <ThingSpeak.h>
#include <WiFi.h>
#include <FastLED.h>


// Connecter le DS18B20 sur le pin 4
const int busOneWire = 4;


// Instancier le OneWire
OneWire monOneWire(busOneWire);

// Connecter nos capteurs sur le bus
DallasTemperature sensors(&monOneWire);


// Variable integer qui détient le nombre de LEDs
const int nbLeds = 8;

// Variable integer qui contient les données de la pin
const int pinData = 22;

// Variable stockant le strip qui contient les LEDs
CRGB monStrip[nbLeds];


// Configurer les variables Wi-Fi
char* ssid = "cm-public";
const char* password = "sansfil3800";

//Configurer les variables ThingSpeak
char server[] = "api.thingspeak.com";
String writeAPIKey = "UB3X8WOBJQ8F0ZF7";//clé nécessaire


void setup()
{
  // Initialiser le débit de communication à 115200 bauds
  Serial.begin(115200);
  
  // Initialiser les capteurs
  sensors.begin();

  // Construire le strip
  FastLED.addLeds<NEOPIXEL, pinData>(monStrip, nbLeds);
  
  // Se connecter au Wi-Fi
  connectWiFi();
}

void loop()
{
  // Récolter les températures en degrés Celcius
  sensors.requestTemperatures();


  // Stocker la température en Celsius dans une variable
  float temperatureC = sensors.getTempCByIndex(0);

  // Afficher la température en Celsius dans le moniteur série
  Serial.print(temperatureC);
  Serial.println("ºC");


  // Stocker la température en Farenheit dans une variable
  float temperatureF = sensors.getTempFByIndex(0);
  
  // Afficher la température en Farenheit dans le moniteur série
  Serial.print(temperatureF);
  Serial.println("ºF");

  
  // Envoyer une requête http de la température en Celsius
  httpRequest(temperatureC);

  // Envoyer une requête http de la température en Farenheit
  httpRequest(temperatureF);
  

  


  // Régler l'intensité des lumières LED
  FastLED.setBrightness(50);
  
  
  // Si la température en Celsius est plus haute ou égale à 28 degrés et demi,
  if (temperatureC >= 28.5)
  {
    // Envoyer un message au moniteur série
    Serial.print("la maison brûle");
    Serial.println("");


    // Mettre la couleur rouge dans la première LED
    monStrip[0] = CRGB::Red;

    // Mettre la couleur verte dans la deuxième LED
    monStrip[1] = CRGB::Green;

    // Mettre la couleur bleu dans la troisième LED
    monStrip[2] = CRGB::Blue;

    // Mettre la couleur mauve dans la quatrième LED
    monStrip[3] = CRGB::Purple;

    // Mettre la couleur aqua dans la cinquième LED
    monStrip[4] = CRGB::Aqua;

    // Mettre la couleur jaune dans la sixième LED
    monStrip[5] = CRGB::Yellow;

    // Mettre la couleur rose dans la septième LED
    monStrip[6] = CRGB::Pink;

    // Mettre la couleur brune dans la huitième LED
    monStrip[7] = CRGB::SaddleBrown;
  }

  // Sinon,
  else
  {
    // Envoyer un message au moniteur série
    Serial.print("la maison gèle");
    Serial.println("");

    
    // Pour chaque LED du strip,
    for (int i = 0; i < nbLeds; i++)
    {
      // Désactiver la LED
      monStrip[i] = CRGB::Black;
    }
  }

  // Afficher la lumière des LEDs
  FastLED.show();
    
  // Exécuter un délai de 5000 millisecondes
  delay(5000);
}

void connectWiFi()
{
  // Tant que le Wi-Fi n'est pas connecté,
  while (WiFi.status() != WL_CONNECTED)
  {
    // Démarrer l'échantillonnage des variables Wi-Fi
    WiFi.begin(ssid, password);

    // Exécuter un délai de 3000 millisecondes
    delay(3000);
  }
  
  // Si le Wi-Fi est connecté,
  // Envoyer un message de connexion dans le moniteur série
  Serial.println("Connected");
}

void httpRequest(float field1Data)
{
  // Variable WiFiClient
  WiFiClient client;

  // Si le client n'est pas connecté au serveur,
  if (!client.connect(server, 80))
  {
    // Écrire un message d'échec dans le moniteur série
    Serial.println("Connexion a échoué");

    // Arrêter les actions du client
    client.stop();
    
    return;
  }

  // Sinon,
  else
  {
    // Créer une variable de type String pour y mettre la valeur du champ
    String data = "field1=" + String(field1Data) ;

    // Si le client est connecté au serveur à l'aide du port 80,
    if (client.connect(server, 80))
    {
      // Envoyer les données à ThingSpeak
      client.println("POST /update HTTP/1.1");
      client.println("Host: api.thingspeak.com");
      client.println("Connection: close");
      client.println("User-Agent: ESP32WiFi/1.1");
      client.println("X-THINGSPEAKAPIKEY: " + writeAPIKey);
      client.println("Content-Type: application/x-www-form-urlencoded");
      client.print("Content-Length: ");
      client.print(data.length());
      client.print("\n\n");
      client.print(data);

      // Exécuter un délai de 1000 millisecondes pour limiter les sorties
      delay(1000);
    }
  }

  // Arrêter les actions du client
  client.stop();
}
