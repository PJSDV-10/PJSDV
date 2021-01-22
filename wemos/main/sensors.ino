
void setupPins() {
  // sets the wemos' pins in output, input or input_pullup mode depending on actuator/sensor type.
  // not needed anymore, ddr on wib is always 00001111.
  setupSensors();
  setupActuators();
  Serial.println("pin setup complete");
}

void setupSensors() {
  // puts the pins in input or input_pullup mode depending on the type of sensor
  // TODO: also enables the needed TWI if we are using a TWI sensor.
  /*
  for (int i = 0; i < AMOUNTOFSENSORS; i++) {
      delay(1);
      Serial.print(sensorNames[i][1].c_str());
      Serial.print(" was initialised using: ");
    if(sensorNames[i][1].compare("pushButton") == 0) {

      pinMode(sensor[i][2],INPUT_PULLUP);
      Serial.println("input_pullup");
    }else {
      pinMode(sensor[i][2],INPUT);
      Serial.println("input");
    }
  }
  */
}

void setupActuators() {
  // sets the actuator's pins in output mode
  /*
  for (int i = 0; i < AMOUNTOFACTUATORS; i++) {
    delay(1);
    Serial.print(sensorNames[i][1].c_str());
    Serial.print(" was initialised using: ");
    pinMode(actuator[i][2],OUTPUT);
    Serial.println("output");
  }
  */
}
  void readSensors(){
    
    for (int i = 0; i < AMOUNTOFSENSORS; i++) {
    if (sensorNames[i][0].compare("bool") == 0) {
      sensor[i][0] = digitalRead(sensor[i][2]);
      delay(0);
    } else if (sensorNames[i][0].compare("int") == 0) {
      sensor[i][0] = analogRead(sensor[i][2]);
    }
    if (sensor[i][0] != sensor[i][1]) {
      Serial.print("The sensor ");
      Serial.print(sensorNames[i][1].c_str());
      Serial.print(" changed from: ");
      Serial.print(sensor[i][1]);
      Serial.print(" to: ");
      Serial.println(sensor[i][0]);
    }
  }
  
}



void updateActuators() {
    for(int i = 0; i < AMOUNTOFACTUATORS; i++){
        delay(1);
    if (actuator[i][1] != actuator[i][0]) { // if the wanted value != current value we have to change the current value
      
        Serial.print("we changed the ");
        Serial.print(actuatorNames[i][1].c_str());
        Serial.print("'s value to: ");
        Serial.print(actuator[i][1]);
        
      // change the current value 
      if (actuatorNames[i][0] == "bool"){ // if the actuator is boolean, write a bool to the pin.
        digitalWrite(actuator[i][2],actuator[i][1]);
        Serial.println(" using digitalWrite.");
        
      } else if(actuatorNames[i][0] == "int"){ // if the actuator is analogue, write using PWM.
        if(actuator[i][1] >= 255) // failsafe to catch values higher then 255.
          actuator[i][1] = 255;
        analogWrite(actuator[i][2],actuator[i][1]); // write the wanted value to the current value using analogwrite
        Serial.println(" using analogWrite.");
      }
      
      actuator[i][0] = actuator[i][1]; //update the currentvalue
    }
  }
}
