//////////////////////////////////////////////////
// Retorna os valores do GPS                    //
//////////////////////////////////////////////////
void getGps()
{  
  while (Serial1.available())
    if (gps.encode(Serial1.read()))
    {
    #ifdef DEBUG 
      Serial.print(F(" GPS "));
      Serial.print(F(" Location: "));   Serial.print(gps.location.lat(), 6);
      Serial.print(F(","));             Serial.print(gps.location.lng(), 6);
      Serial.print(F(" Altitude: "));   Serial.print(gps.altitude.meters()); 
      Serial.print(F(" Satelites: "));  Serial.print(gps.satellites.value());
      Serial.print(F(" Idade Inf.: ")); Serial.print(gps.location.age());
      char sz[32];
      sprintf(sz, "%02d/%02d/%02d ", gps.date.day(), gps.date.month(), gps.date.year());
      Serial.print(F(","));             Serial.print(sz);
      sprintf(sz, "%02d:%02d:%02d ", gps.time.hour(), gps.time.minute(), gps.time.second());
      Serial.print(F(","));             Serial.print(sz);
      Serial.print(F(" Cardinal: "));   Serial.print(gps.course.isValid() ? TinyGPSPlus::cardinal(gps.course.value()) : "*** ");
      Serial.print(F(" HDOP: "));       Serial.print(gps.hdop.value()); 

      unsigned long distanceKmToHome = (unsigned long)TinyGPSPlus::distanceBetween( gps.location.lat(), gps.location.lng(), HOME_LAT, HOME_LON) / 1000;
      Serial.print(F(" Dist. CASA: ")); Serial.print(distanceKmToHome, 9);
      double courseToHome = TinyGPSPlus::courseTo( gps.location.lat(), gps.location.lng(), HOME_LAT, HOME_LON);
      Serial.print(F(" Curso CASA: ")); Serial.print(courseToHome);
      const char *cardinalToHome = TinyGPSPlus::cardinal(courseToHome);
      Serial.print(F(" Card. CASA: ")); Serial.print(gps.location.isValid() ? cardinalToHome : "*** ");
      Serial.println("");
    #endif
    }
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    #ifdef DEBUG 
      Serial.println(F("No GPS detected: check wiring."));
    #endif
  }
}  
//////////////////////////////////////////////////
// Retorna o nivel de Oxigencio na Sonda Lambda //
// Valores entre 0 e 1                          //
//////////////////////////////////////////////////
float getNivelOxigenio(uint8_t pinoOxig)
{
  int _lambdaSensor = analogRead (pinoOxig);
  
  // Convert to voltage
  float _lambdaVoltage = _lambdaSensor * (vg_volt_placa / 1023.0);
  int _lambdaAFR = (_lambdaVoltage * 2) + 10;
#ifdef DEBUG  
  Serial.print(" Lambda  : Pino(");Serial.print(pinoOxig);Serial.print(") ");Serial.print(_lambdaSensor);
  Serial.print(" Voltage : "); Serial.print(_lambdaVoltage);
  Serial.print(" AFR     : "); Serial.println(_lambdaAFR);
#endif
  _lambdaAFR= _lambdaAFR>>2;
  return _lambdaAFR;
}
/////////////////////////////////////////////////////////////
// Retorna a temperatura do sensor NTC10k em Graus Celcius //
/////////////////////////////////////////////////////////////
float getTemp_NTC10k(){
  float _sensorNTC10k = tempNTC10k.getTemp() ;              // leitura do valor do sensor analogico

  //v_temp_Read = v_temp_Read  * 0.0488;
  if (_sensorNTC10k >= v_temp_max)	                     // Gera os valores minimos e maximos
  {
    v_temp_max = _sensorNTC10k;
  }
  if (_sensorNTC10k <= v_temp_min)
  {
    v_temp_min = _sensorNTC10k;
  }  
#ifdef DEBUG  
  Serial.print(" Temp Ext: Pino(");Serial.print(" ");Serial.print(") ");Serial.print(_sensorNTC10k);Serial.println ("°C");
#endif    
  return (_sensorNTC10k);
}
//////////////////////////////////////////////////////////////
// Retorna a temperatura do sensor DS18S20 em Graus Celcius //
//////////////////////////////////////////////////////////////
float getTemp_DS18S20(){
  byte data[12];
  byte addr[8];

  if ( !tempDS18S20.search(addr)) {       // no more sensors on chain, reset search
      tempDS18S20.reset_search();
      return -99;
  }
  if ( OneWire::crc8( addr, 7) != addr[7]) {
#ifdef DEBUG    
      Serial.println("CRC is not valid!");
#endif
      return -99;
  }
  if ( addr[0] != 0x10 && addr[0] != 0x28) {
#ifdef DEBUG    
      Serial.print("Device is not recognized");
#endif
      return -99;
  }
  tempDS18S20.reset();
  tempDS18S20.select(addr);
  tempDS18S20.write(0x44,1);     // start conversion, with parasite power on at the end

  byte present = tempDS18S20.reset();
  tempDS18S20.select(addr);    
  tempDS18S20.write(0xBE);       // Read Scratchpad
  
  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = tempDS18S20.read();
  }
  
  tempDS18S20.reset_search();
  
  byte  MSB      = data[1];
  byte  LSB      = data[0];
  int   tempRead = ((MSB << 8) | LSB); //using two's compliment
  float   TempSum  = tempRead / 16;
#ifdef DEBUG  
  Serial.print(" Temp Int: Pino(");Serial.print(" ");Serial.print(") ");Serial.print(TempSum);Serial.println ("°C");
#endif   
  return TempSum;
}
////////////////////////////////////////////////////////
// Retorna a voltagem de um pino de leitura Analogico //
////////////////////////////////////////////////////////
float getTensao(uint8_t pinoTensao){

  float _relPorta = vg_volt_placa/1023;   // referencia de leitura da porta Analógica
  
  float _leitura  = analogRead(pinoTensao);  
  float _tensao   = ((_leitura*_relPorta)*vg_volt_placa);
#ifdef DEBUG  
  Serial.print(" Tensao  : Pino(");Serial.print(pinoTensao);Serial.print(") ");Serial.print(_tensao);Serial.println ("V");
#endif  
  return _tensao;
}

////////////////////////////////////////////////////////
// Retorna a corrente de um pino de leitura Analogico //
////////////////////////////////////////////////////////
float getCorrente(uint8_t pinoCorrente){

  float _aRef    = 111.1;                   // Valor para Calibragem
  
  emon1.current(pinoCorrente, _aRef);       // Current: input pin, calibration.

  emon1.calcVI(20,2000);                    // Calculate all. No.of half wavelengths (crossings), time-out
  //emon1.serialprint();                      // Print out all variables (realpower, apparent power, Vrms, Irms, power factor)
  
  float _corrente       = emon1.Irms;             //extract Irms into Variable
  
#ifdef DEBUG  
  Serial.print(" Corrente: Pino(");Serial.print(pinoCorrente);Serial.print(") ");Serial.print(_corrente);Serial.println ("A");
//  float realPower       = emon1.realPower;        //extract Real Power into variable
//  float apparentPower   = emon1.apparentPower;    //extract Apparent Power into variable
//  float powerFActor     = emon1.powerFactor;      //extract Power Factor into Variable
//  float supplyVoltage   = emon1.Vrms;             //extract Vrms into Variable
#endif  
    return _corrente;
}

