//monitor.println("123456789012345678901234567890123456789");   // fonte 10 x 14 pixels
//////////////////////////////////////////////////////
// Rotina de Apresenta os Sensores de Monitoramento //
//////////////////////////////////////////////////////
void apresentaMonitor()
{
  int16_t vcolMin = 5;
  int16_t vlinMin = 35;
  int16_t vlinMed = 163;
  int16_t vcolMax = monitor.width();     // 480
  int16_t vlinMax = monitor.height()-25; // 320 - 25
  int16_t vlinDis = vlinMin;
  int16_t vcolDis = vcolMin;

  //=================== Intervalos em (ms) ======== 
  const unsigned long _IntervalTemperatura = 5000; 
  const unsigned long _IntervalBateria     = 5000; 
  const unsigned long _IntervalHallmeter   = 50; 
  const unsigned long _IntervalCombustivel = 3000;  
  const unsigned long _IntervalOleo        = 1000;  
  const unsigned long _IntervalGPS         = 5000;  
  const unsigned long _IntervalRPM         = 50;  
  const unsigned long _IntervalKMH         = 50;  
  const unsigned long _IntervalBussola     = 50;  
  //=================== Timers ====================
  unsigned long       _timerTemperatura, _timerBateria, _timerHallmeter, _timerCombustivel, _timerOleo, _timerGPS, _timerRPM, _timerKMH , _timerBussola= millis();
  //=================== Variaveis =================
  float               _tempInt, _tempExt, _tensao, _corrente, _nivelOxig, _combustivel, _tempOleo, _pressOleo, _valorRPM, _valorKMH = 0;

  limpaArea();

  // Monta a grade do monitor
  monitor.drawFastHLine(vcolMin, vlinMed, vcolMax, WHITE);  
  int16_t vcolAux = 120;
  for (uint8_t i=0;i<3;i++)
      monitor.drawFastVLine(vcolAux+(vcolAux*i), vlinMin, vlinMax-vlinMin, WHITE);  

  // Monta Paineis
 // monitor.fillRect(vcolMin, vlinMin, 120, vlinMed-vlinMin, DARKGREY);
  
  while (true)
  {
    monitor.setFont(&Org_01);  
    monitor.setTextSize(2);

////////////////////////////////////////// Temperatura
    vlinDis = vlinMin;
    vcolDis = vcolMin;
    if ((millis() - _timerTemperatura) >= _IntervalTemperatura) {  
      _tempInt = getTemp_DS18S20();
      _tempExt = getTemp_NTC10k();
      monitor.setTextColor(WHITE, BLACK);  
      monitor.setCursor(vcolDis    , vlinDis+10);  monitor.print( "Temperatura");  
      monitor.setCursor(vcolDis+5  , vlinDis+50 );  monitor.print( "Int");  
      monitor.setCursor(vcolDis+5  , vlinDis+75 );  monitor.print( "Ext");   
      monitor.setCursor(vcolDis+5  , vlinDis+100);  monitor.print( "Min");
      monitor.setCursor(vcolDis+65 , vlinDis+100);  monitor.print( "Max"); 
      monitor.setTextColor(LIGHTGREY, BLACK);      
      monitor.fillRect (vcolDis+65 , vlinDis+40, 48, 40, BLACK);
      monitor.setCursor(vcolDis+65 , vlinDis+50 );  monitor.print(_tempInt,0); monitor.print("C");
      monitor.setCursor(vcolDis+65 , vlinDis+75 );  monitor.print(_tempExt,0);  monitor.print("C");
      monitor.fillRect (vcolDis+5  , vlinDis+110, 100, 15, BLACK);
      monitor.setCursor(vcolDis+5  , vlinDis+120);  monitor.print(v_temp_min,0); monitor.print("C");
      monitor.setCursor(vcolDis+65 , vlinDis+120);  monitor.print(v_temp_max,0); monitor.print("C");
      _timerTemperatura = millis();
    }
    
////////////////////////////////////////// Bateria
    vcolDis = vcolMin+120;   
    if ((millis() - _timerBateria) >= _IntervalBateria) {  
      _tensao = getTensao(pinoTensao);
      _corrente = getCorrente(pinoCorrente);    
      
      monitor.setTextColor(WHITE, BLACK);     
      monitor.setCursor(vcolDis    , vlinDis+10 );  monitor.print( "  Bateria" );  
      monitor.setCursor(vcolDis+70 , vlinDis+75 );  monitor.print( " V" );
      monitor.setCursor(vcolDis+70 , vlinDis+100);  monitor.print( " A" ); 
      monitor.fillRect (vcolDis+1  , vlinDis+63, 78, 50, BLACK);
      monitor.setTextColor(LIGHTGREY, BLACK);      
      monitor.setCursor(vcolDis+15 , vlinDis+75 );   monitor.print(_tensao,1 ); 
      monitor.setCursor(vcolDis+15 , vlinDis+100 );  monitor.print(_corrente,1 ); 
      _timerBateria = millis();
    }
////////////////////////////////////////// Hallmeter    
    vcolDis = vcolMin+240;   
    if ((millis() - _timerHallmeter) >= _IntervalHallmeter) {  
      _nivelOxig = getNivelOxigenio(pinoOxig);
      monitor.setTextColor(WHITE, BLACK);     
      monitor.setCursor(vcolDis    , vlinDis+10 );  monitor.print( " Hallmeter" );  
      monitor.setCursor(vcolDis+50 , vlinDis+100);  monitor.print( "mV" );
      monitor.fillRect(vcolDis+7,vlinDis+45,90,45,BLACK);
      monitor.setTextColor(LIGHTGREY, BLACK);      
      monitor.setCursor(vcolDis+50 , vlinDis+75 );  monitor.print( _nivelOxig,0 ); 
      int x = map(_nivelOxig,0,20,1,10);
      int _l = vlinDis+40;
      for ( int i=1;i<=x;i++ ) 
      {      
        int _c = vcolDis+7+(i*8);
        if ( i < 4 )                  {monitor.fillRect(_c, _l, 8, 10, GREEN);}     
        else if( i >= 4 && i <= 7 )   {monitor.fillRect(_c, _l, 8, 10, ORANGE);}     
        else if( i > 7 )              {monitor.fillRect(_c, _l, 8, 10, RED);}  
      }
      _timerHallmeter = millis();   
    }
////////////////////////////////////////// Combustivel
    vcolDis = vcolMin+360;   
    if ((millis() - _timerCombustivel) >= _IntervalCombustivel) {  
      _combustivel = 99999;
      monitor.setTextColor(WHITE, BLACK);     
      monitor.setCursor(vcolDis+5  , vlinDis+10 );  monitor.print( "Combustivel" );  
      monitor.setCursor(vcolDis+7  , vlinDis+100);  monitor.print( "V  1/2  C" );
      monitor.setTextColor(RED, BLACK);  // LIGHTGREY   
      monitor.setCursor(vcolDis+7  , vlinDis+75 );  monitor.print( _combustivel,0 );
      _timerCombustivel = millis();
    }
////////////////////////////////////////// Oleo
    vlinDis = vlinMed+8;
    vcolDis = vcolMin;   
    if ((millis() - _timerOleo) >= _IntervalOleo) {  
      _tempOleo = 50;
      _pressOleo = 2;
      monitor.setTextColor(WHITE, BLACK);     
      monitor.setCursor(vcolDis    , vlinDis+10 );  monitor.print( "   Oleo" );  
      monitor.setCursor(vcolDis    , vlinDis+40 );  monitor.print( "Temperatura" );
      monitor.setCursor(vcolDis+5  , vlinDis+90 );  monitor.print( "Pressao" );
      monitor.setTextColor(RED, BLACK);      //LIGHTGREY
      monitor.setCursor(vcolDis+35 , vlinDis+55 );  monitor.print( 100 );
      monitor.setCursor(vcolDis+35 , vlinDis+105);  monitor.print( 999 );
      _timerOleo = millis();
    }
    
 ////////////////////////////////////////// GPS
    vcolDis = vcolMin+120;   
    if ((millis() - _timerGPS) >= _IntervalGPS) {     
      getGps();
      monitor.setTextColor(WHITE, BLACK);     
      monitor.setCursor(vcolDis    , vlinDis+10 );  monitor.print( "   GPS" );  
      monitor.setCursor(vcolDis+5  , vlinDis+40 );  monitor.print( "Lat" );
      monitor.setCursor(vcolDis+5  , vlinDis+90 );  monitor.print( "Lng" );
      monitor.setTextColor(LIGHTGREY, BLACK);   
      monitor.fillRect(vcolDis+5,vlinDis+45,100,20,BLACK);
      monitor.setCursor(vcolDis+5  , vlinDis+55 );  monitor.print(gps.location.lat(),6);
      monitor.fillRect(vcolDis+5,vlinDis+95,100,20,BLACK);
      monitor.setCursor(vcolDis+5  , vlinDis+105);  monitor.print(gps.location.lng(),6);
      _timerGPS = millis();
    }

////////////////////////////////////////// Bussola
    vcolDis = vcolMin+240;   
    if ((millis() - _timerBussola) >= _IntervalBussola) {  
      
      monitor.setTextColor(WHITE, BLACK);     
      monitor.setCursor(vcolDis    , vlinDis+10 );  monitor.print( "xxxxx" );  
      monitor.setCursor(vcolDis+20 , vlinDis+30 );  monitor.print( "X:" );
      monitor.setCursor(vcolDis+20 , vlinDis+50 );  monitor.print( "Y:" );
      monitor.setCursor(vcolDis+20 , vlinDis+70 );  monitor.print( "Z:" );
      monitor.setCursor(vcolDis+10  , vlinDis+100);  monitor.print("xxxxx");
      monitor.setTextColor(LIGHTGREY, BLACK); 
      monitor.setCursor(vcolDis+60 , vlinDis+30 );  monitor.print( 0 );
      monitor.setCursor(vcolDis+60 , vlinDis+50 );  monitor.print( 0 );
      monitor.setCursor(vcolDis+60 , vlinDis+70 );  monitor.print( 0 );
      monitor.setCursor(vcolDis+10 , vlinDis+115);  monitor.print(0);
      _timerBussola = millis();          
    }

////////////////////////////////////////// KM/h
    vcolDis = vcolMin+360;   
    if ((millis() - _timerKMH) >= _IntervalKMH) {  
      _valorKMH = gps.speed.kmph();
      monitor.setTextColor(WHITE, BLACK);     
      monitor.setCursor(vcolDis    , vlinDis+10 );  monitor.print( "   KM/h" );  
      monitor.setTextColor(LIGHTGREY, BLACK);      
      monitor.setCursor(vcolDis+40 , vlinDis+35 );  monitor.print( _valorKMH,0 );
      _timerKMH = millis();
    }
    if ((millis() - _timerRPM) >= _IntervalRPM) {  
      _valorRPM = 9999;
      monitor.setTextColor(WHITE, BLACK);     
      monitor.setCursor(vcolDis    , vlinDis+65 );  monitor.print( "   RPM" );  
      monitor.setTextColor(RED, BLACK);      //LIGHTGREY
      monitor.setCursor(vcolDis+30 , vlinDis+100);  monitor.print( _valorRPM,0 );
      _timerRPM = millis();
    }
  }
  monitor.setFont();  
}
