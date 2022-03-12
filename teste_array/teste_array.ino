#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged TO GPIO 4
#define ONE_WIRE_BUS 4

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// Number of temperature devices found
int numberOfDevices;

// We'll use this variable to store a found device address
DeviceAddress tempDeviceAddress[5] = {0x28, 0xFF, 0x64, 0x1, 0xB7, 0xF3, 0xB2, 0x4D,
                                      0x28, 0xFF, 0x64, 0x1, 0xB0, 0x1D, 0x68, 0xB7,
                                      0x28, 0xFF, 0x64, 0x1, 0xB7, 0xF0, 0x23, 0x57,
                                      0x28, 0xFF, 0x64, 0x1, 0xB7, 0xCC, 0xFD, 0x7F,
                                      0x28, 0xFF, 0x64, 0x1, 0xB7, 0xCD, 0x39, 0x10 };

void setup(){
  // start serial port
  Serial.begin(115200);
  
  // Start up the library
  sensors.begin();
  
  // Grab a count of devices on the wire
  numberOfDevices = sensors.getDeviceCount();
  
  // locate devices on the bus
  Serial.print("Localizando dispositivos...");
  Serial.print("Encontrado ");
  Serial.print(numberOfDevices, DEC);
  Serial.println(" dispositivos.");

  // Loop through each device, print out address
  for(int i=0;i<numberOfDevices; i++){
    // Search the wire for address
    if(sensors.isConnected(tempDeviceAddress[i])){
      Serial.print("Dispositivo conectado");
      Serial.print(i, DEC);
      Serial.print(" com endereco: ");
      printAddress(tempDeviceAddress[i]);
      Serial.println();
    } else {
      Serial.print("Encontrado dispositivo fantasma em ");
      Serial.print(i, DEC);
      Serial.print(" mas nao pode detectar endereco. Verifique as conexoes");
    }
  }
}

void loop(){ 
  sensors.requestTemperatures(); // Send the command to get temperatures
  
    for (int i = 0; i < numberOfDevices; i++) { //loop faz a leitura de temperatura em todos os sensores e verifica se algum sensor está detectando um alarme
    if (sensors.hasAlarm(tempDeviceAddress[i])) {
      Serial.println();
      Serial.print("ALARM: ");
      printAddress(tempDeviceAddress[i]); //printa o endereço do sensor no monitor serial
    }
    if (!sensors.hasAlarm(tempDeviceAddress[i])) {
      temperatura(tempDeviceAddress[i], i); //mostra a temperatura atual (média)
    }
  }
}

void temperatura(DeviceAddress deviceAddres, int i){

      Serial.print("Dispositivo ");
      Serial.print(i, DEC);
      float tempC = sensors.getTempC(deviceAddres);
      Serial.print("Temp C: ");
      Serial.print(tempC);
      Serial.print(" Temp F: ");
      Serial.println(DallasTemperature::toFahrenheit(tempC)); // Converts tempC to Fahrenheit
      Serial.println();
      
      Serial.print("");
      Serial.print("********************************************");
      Serial.println();
      delay(500);
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++){
    if (deviceAddress[i] < 16) Serial.print("0");
          Serial.print(deviceAddress[i], HEX);
  }
}
