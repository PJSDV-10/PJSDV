
void setupPins() {
  // sets up the i2c connection with the Wemos interface board.

  Wire.beginTransmission(WIBADRESD);
  Wire.write(byte(0x03));         
  Wire.write(byte(0x0F));       
  Wire.endTransmission();
}

void readSensors(){
  // the analog input registers are in series, so we can start a session on the first(WIBADRESA) and sequentially read the rest
  // the digital input registers are on a different address, WIBADRESBD so we have to start another session,
  
  
  unsigned int analogin[2]; //will contain both of the analog input variables.
  
  Wire.requestFrom(WIBADRESA, 4);   // begin a reading session at the lowest byte.
  //read AI0
  analogin[0] = Wire.read()&0x03;  //read the high byte
  analogin[0]=analogin[0]<<8; // shift the high byte to the highest 8 bits of the 16 bit integer
  analogin[0] = analogin[0]|Wire.read();  // read the low byte.
  
  //read AI1
  analogin[1] = Wire.read()&0x03;  //read the high byte
  analogin[1]=analogin[1]<<8; // shift the high byte to the highest 8 bits of the 16 bit integer
  analogin[1] = analogin[1]|Wire.read(); // read the low byte.
  
  Wire.beginTransmission(WIBADRESD); 
  Wire.write(byte(0x00));      
  Wire.endTransmission();
  Wire.requestFrom(0x38, 1);   
  unsigned int inputs = Wire.read(); // read the digital inputs.

  
  // now that we have aquired the sensor values from the wemos interface board, put them in the array.
  for (int i = 0; i < AMOUNTOFSENSORS; i++) {
    
    if (sensorNames[i][0].compare("bool") == 0) {
      if (inputs & sensor[i][2]) {// if the bit in the input byte, AND the bit in the address byte are both one, put the bool value as true.
        sensor[i][0] = 1;
      } else // otherwise, put the bool value down as false.
      sensor[i][0] = 0;
    } 
    
    if (sensorNames[i][0].compare("int") == 0) { 
      sensor[i][0] = analogin[(sensor[i][2] - 300)]; // if a sensor is analogue, put an analog value in the array.
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
        Serial.println(actuator[i][1]);
        
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
        //Serial.println(" transmit WIBADRESD");
        //Serial.println(stuur);
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
