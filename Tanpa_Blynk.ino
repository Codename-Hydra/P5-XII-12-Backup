
#include <DHT.h>
#define LDRpin A1        
#define MQ2pin A2       
#define DHTPIN A0    
#define DHTTYPE DHT22  
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino
  

const int BUZZER_PIN = 6; 
const int LED_PIN1 = 5; 
const int LED_PIN2 = 8;
  
const int GAS_THRESHOLD = 200;     
     
float distance_cm;          
float gasValue;       
int LDRValue;

int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value

void setup() {
  Serial.begin(9600);
  dht.begin();
  
  // Pin Modes
  
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  pinMode(LDRpin, INPUT);
  pinMode(MQ2pin, INPUT);
  pinMode(DHTPIN, INPUT);
  
  
  // Ensure initial states
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN1, LOW);
  digitalWrite(LED_PIN2, LOW);
}


void updateSensorValues() {
  gasValue = analogRead(MQ2pin);
}

bool isAlarmConditionActive() {
  return (gasValue > GAS_THRESHOLD);
}

void playAlarm() {
  while (true) {
    digitalWrite(LED_PIN1, HIGH); // Turn on the LED
    digitalWrite(LED_PIN2, LOW); 
    Serial.println("Alarm is active!");
    
    // Check sensor values before each pattern
    updateSensorValues();
    if (!isAlarmConditionActive()) {
      Serial.println("Conditions returned to normal - stopping alarm");
      stopAlarm();
      break;
    }
    
    
    for (int i = 0; i < 80; i++) {
      digitalWrite(BUZZER_PIN, HIGH);
      delay(1);
      digitalWrite(BUZZER_PIN, LOW);
      delay(1);
      
      
      if (i % 20 == 0) {
        updateSensorValues();
        if (!isAlarmConditionActive()) {
          Serial.println("Conditions returned to normal - stopping alarm");
          stopAlarm();
          return;
        }
      }
    }
    
    
    for (int i = 0; i < 100; i++) {
      digitalWrite(BUZZER_PIN, HIGH);
      delay(2);
      digitalWrite(BUZZER_PIN, LOW);
      delay(2);
      
      if (i % 25 == 0) {
        updateSensorValues();
        if (!isAlarmConditionActive()) {
          Serial.println("Conditions returned to normal - stopping alarm");
          stopAlarm();
          return;
        }
      }
    }
  }
}

void stopAlarm() {
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN1, LOW);
  digitalWrite(LED_PIN2, HIGH);
  Serial.println("Alarm stopped.");
}

void loop() {

  LDRValue = analogRead(LDRpin);
  updateSensorValues();
  
  Serial.print("LDR Value: ");
  Serial.println(LDRValue);
     hum = dht.readHumidity();
    temp= dht.readTemperature();
    //Print temp and humidity values to serial monitor
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.print(" %, Temp: ");
    Serial.print(temp);
    Serial.println(" Celsius");
  Serial.print("Gas Value (Raw): ");
  Serial.println(gasValue);
  
  if (isAlarmConditionActive()) {
    
    Serial.println("Condition met for alarm: Gas leak or fire detected");
    playAlarm();    
    
  } 
   
  else {
    Serial.println("No conditions met for alarm, civilian can roam freely");
    stopAlarm();     
  }
  
  delay(1000);
}
