//TiXmlDocument*//
std::string Buildheader() {  //build header with the gloals specified
    std::string temp ="<message><header><sender>"+ wemosNaam +"</sender><receiver>"+ server +"</receiver></header>"; 
    return temp;
}
/*std::string buildcapabilities() {
  std::string temp = "<capabilities>";

  for(int i = 0;i<AMOUNTOFSENSORS;i++){ //voeg elke keer neeiwe sensot toe
    temp += "<func><type>" + sensorNames[i][0] + "</type><funcName>" + sensorNames[i][1] + "</funcName></func>";
  }
  
   for(int i = 0;i<AMOUNTOFACTUATORS;i++){ //voeg elke keer neeiwe actuator toe
    temp += "<func><type>" + actuatorNames[i][0] + "</type><funcName>" + actuatorNames[i][1] + "</funcName></func>";
  }
  
  temp+= "</capabilities>";
  return temp;
} dam this isnt even necesary anymore*/ 


std::string buildAuthenticationMsg() {
   //dammit mad this entire thing only to notice im using the wrong file.............................
   //std::string temp = Buildheader() +"<function>authentication</function><context><password>"+wachtwoord+"</password><clientName>"+wemosNaam+"</clientName><type>"+type+"</type>"+ buildcapabilities()+" </context> </message>";
   std::string temp = Buildheader() + " <function>authentication</function><context><password>" +wachtwoord+ "</password><clientName>" + wemosNaam + "</clientName><type>" + type + "</type></context></message>" ;
   return temp;
}

std::string buildStatusMsg(){
  std::string temp = Buildheader();
  temp += "<function>sensorUpdate</function><context><password>" +wachtwoord+ "</password><type>stoel</type>";
  
    for(int i = 0;i<AMOUNTOFSENSORS;i++){ //voeg elke keer neeiwe sensot toe
      delay(0);
      
       std::string roundd = intToString( i); // int too string
       std::string worth = intToString(sensor[i][0]);
       
      temp +=  "<data"+roundd+">"+ worth +"</data"+roundd+">";
      delay(0);
  }
  
  delay(0);
  temp +=   "</context></message>";
  
  return temp;
}



std::string intToString(int i){ //conver int to string
    std::stringstream str;
   str << i;
   std::string s;
   s += str.str();
   return s;
}
