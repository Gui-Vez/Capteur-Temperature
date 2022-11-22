// Inclure les librairies OneWire et DallasTemperature
#include <OneWire.h>
#include <DallasTemperature.h>

// Créer une variable integer où le capteur de température DS18B20 est connecté au GPIO 4
const int oneWireBus = 4;     


// Configurer une variable oneWire pour communiquer avec n'importe quel appareil OneWire
OneWire oneWire(oneWireBus);

// Passer une référence oneWire au capteur de température Dallas
DallasTemperature sensors(&oneWire);


void setup()
{
  // Initialiser le moniteur de série à un taux de 115200 bauds
  Serial.begin(115200);
  
  // Démarer le capteur DS18B20
  sensors.begin();
}

void loop()
{
  // Appeler la fonction qui permet de récupérer la température
  sensors.requestTemperatures();


  // Obtenir la température en Celsius en utilisant la fonction getTempCByIndex
  float temperatureC = sensors.getTempCByIndex(0);

  // Obtenir la température en Fahrenheit en utilisant la fonction getTempFByIndex
  float temperatureF = sensors.getTempFByIndex(0);


  // Écrire les résultats de la température en Celsius
  Serial.print(temperatureC);
  Serial.println("ºC");

  // Écrire les résultats de la température en Fahrenheit
  Serial.print(temperatureF);
  Serial.println("ºF");

  // Exécuter un délai de 5000 millisecondes (5s)
  delay(5000);
}
