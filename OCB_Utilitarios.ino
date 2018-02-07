///////////////////////////////
// Inicialização do Monitor  //
///////////////////////////////
void iniciaTFT() 
{
#ifdef DEBUG
  Serial.print("Inicializando monitor..... ");
#endif  
  monitor.reset();                                // Reinicia o Monitor
  monitor.begin(vg_identifier);                   // Inicializa Monitor 
  monitor.setRotation(1);                         // Ajusta o monitor para ficar alinhado na horizontal
#ifdef DEBUG
  Serial.println("OK!");
#endif  
}

///////////////////////
// Prepara o Monitor //
///////////////////////
void preparaMonitor() 
{  
  // Desenha cabecalho
  monitor.fillRect(0,  0, monitor.width(), 25, BLUE);   
  monitor.fillRect(0, 25, monitor.width(),  1, DARKGREY);   
  monitor.fillRect(0, 26, monitor.width(),  1, WHITE);   
  monitor.fillRect(0, 27, monitor.width(),  1, DARKGREY);   
  monitor.fillRect(0, 28, monitor.width(),  5, BLUE);   

  // Desenha Rodape
  monitor.fillRect(0, monitor.height()-25, monitor.width(), 25, BLUE); 
} 
///////////////////
// Imprime Texto //
///////////////////
void imprimeTexto(String pTexto, String pAlinhamento, int pLinha)
// pAlinhamento = "E"squerda (DEFAULT) - "C"entro - "D"ireita
{
  int _vcol = 5;
  int _tamFonte = 8;
  
  switch(monitor.getTextSize())
  {
    case 2:
     _tamFonte = 12;
     break;
    case 4:
     _tamFonte = 20;
     break;
  }   
  if (pAlinhamento=="C")
     _vcol = ((monitor.width()-(pTexto.length()*_tamFonte))/2);
  else if (pAlinhamento=="D")
          _vcol = (monitor.width()-pTexto.length()*_tamFonte);
  else if (pAlinhamento=="E")
          _vcol = 5;

  monitor.setCursor(_vcol, pLinha);
  monitor.println( pTexto );
}
////////////////////////////////////////////////////////////////
void limpaArea()
{
  // Limpa a area util
  monitor.fillRect(0, 33,  monitor.width(), monitor.height()-58, BLACK);  
}
