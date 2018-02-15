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

/*  float _aRef    = 111.1;                   // Valor para Calibragem
  
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
*/
}

