int waterLevel = 1,
groundSensor = 2,
power = 3,
pump = 4,
beeper = 11,
led = 13;

int LOW_WATER = 250;
int NO_WATER = 100;
int DRY_GROUND = 720;

int POWER_UP_LATENCY = 1000;              // задержка для прогрева датчиков 1 секунда
int WATER_SUPLY_LATENCY = 3000;           // длительность полива 3 секунды
int MAIN_LOOP_LATENCY = 15000/*60 * 60 * 1000*/;   // задержка между циклами 1 час (60 минут по 60 секунд по 1000 милисекунд)

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pinMode(waterLevel, INPUT);
  pinMode(groundSensor, INPUT);
  pinMode(power, OUTPUT);
  pinMode(pump, OUTPUT);
  pinMode(beeper, OUTPUT);
  pinMode(led, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(); // пустая строка чтобы отделить каждый блок вывода текста

  powerOn();

  int waterLevelData = readWaterLevel();
  
  if (waterLevelData < NO_WATER) {
    printOutOfWater(waterLevelData);
    longBeep();
    powerOff();
  } else {
    if (waterLevelData < LOW_WATER) {
      printLowWater(waterLevelData);
      beep3times();
    }

    int groundSensorData = readGroundSensorData();
    powerOff(); // все показания считали, датчики больше не нужны. отключаем
  
    if (groundSensorData > DRY_GROUND) {
      Serial.println(String("The ground is dry. (") + groundSensorData + ") Pumping water...");
      pumpWater();
    } else {
      Serial.println(String("The ground is wet enough (") + groundSensorData + ")");
    }
  }

  Serial.println("Finished.");
  delay(MAIN_LOOP_LATENCY);
}

void pumpWater() {
  powerOnPump();
  delay(WATER_SUPLY_LATENCY);
  powerOffPump();
}

void powerOnPump() {
  digitalWrite(pump, HIGH);
}

void powerOffPump() {
  digitalWrite(pump, LOW);
}

int readGroundSensorData() {
  Serial.print("Reading ground moisture level... ");
  int result = analogRead(groundSensor);
  Serial.println(result);
  return result;
}

int readWaterLevel() {
  Serial.print("Reading water level... ");
  int result = analogRead(waterLevel);
  Serial.println(result);
  return result;
}

void powerOn() {
  Serial.println("Power on");
  digitalWrite(power, HIGH);
  digitalWrite(led, HIGH);
  delay(POWER_UP_LATENCY);
}

void powerOff() {
  Serial.println("Power off");
  digitalWrite(power, LOW);
  digitalWrite(led, LOW);
}

void printOutOfWater(int value) {
  String message = "";
  message = String("OUT OF WATER! (") + value + ")\n" + 
    "Beep and exit.";
  Serial.println(message);
}

void printLowWater(int value) {
  String message = "";
  message = String("LOW WATER LEVEL! (") + value + ")\n" + 
    "Beeping 3 times and process";
}

void beep3times() {
  int sleepTime = 300;    // 0.3 seconds

  for(int i = 0; i < 3; i++) {
    digitalWrite(beeper, HIGH);
    delay(sleepTime);
    digitalWrite(beeper, LOW);
    delay(sleepTime);
  }
}

void longBeep() {
  digitalWrite(beeper, HIGH);
  delay(1000);  // 1 second
  digitalWrite(beeper, LOW);
}
