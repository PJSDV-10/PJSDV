
void setupPins() {
  // sets the wemos' pins in output, input or input_pullup mode depending on actuator/sensor type.
  // not needed anymore, ddr on wib is always 00001111.
  Serial.println("Setting up I2C");
  Wire.beginTransmission(WIBADRESD);
  Wire.write(byte(0x03));         
  Wire.write(byte(0x0F));       
  Wire.endTransmission();
}

void readSensors(){
  unsigned int analogin[2];
  Wire.requestFrom(WIBADRESA, 4);   
  //read AI0
  analogin[0] = Wire.read()&0x03;  //read byte 1
  analogin[0]=analogin[0]<<8;
  analogin[0] = analogin[0]|Wire.read();  // read byte 0
  //read AI1
  analogin[1] = Wire.read()&0x03;  //read byte 1
  analogin[1]=analogin[1]<<8;
  analogin[1] = analogin[1]|Wire.read(); //read byte 0
  
  Wire.beginTransmission(WIBADRESD); 
  Wire.write(byte(0x00));      
  Wire.endTransmission();
  Wire.requestFrom(0x38, 1);   
  unsigned int inputs = Wire.read();

  
  for (int i = 0; i < AMOUNTOFSENSORS; i++) {
    
    if (sensorNames[i][0].compare("bool") == 0) {
      if (inputs & sensor[i][2]) {
        sensor[i][0] = 1;
      } else 
      sensor[i][0] = 0;
    } 
    
    if (sensorNames[i][0].compare("int") == 0) {
      sensor[i][0] = analogin[(sensor[i][2] - 300)];
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
      if (actuatorNames[i][0].compare("bool")==0){ // if the actuator is boolean, write a bool to the pin. 
        //Serial.println("hij is hier");
        delay(0);
        uint8_t stuur=0;
        for(int H = 0; !(H==AMOUNTOFACTUATORS);H++){
          //Serial.println("hallo");
          if(actuatorNames[i][0] == "bool" && actuator[H][1] == 1 ){
                      stuur += (actuator[H][2])  ;  //tel hier alen actuatoren die bool zijn op
          }
           //Serial.println(stuur);
        }
        //Serial.println(stuur);
        Wire.beginTransmission(WIBADRESD);
        Wire.write(byte(0x01));          
        Wire.write(byte(stuur));
        Wire.endTransmission(); 
      } 
      actuator[i][0] = actuator[i][1]; //update the currentvalue
    }
  }
}

/*
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
}*/
