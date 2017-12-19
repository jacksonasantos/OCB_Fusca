///////////////////////
// Introducao do OCB //
///////////////////////
void telaIntroducao()  // Tela 1 - Introducao
{
  int vcol = 25;
  int vlin = 0;
  monitor.fillScreen(WHITE);
/*  
  monitor.fillRect(20,20,monitor.width()-40,monitor.height()-40,WHITE);   
  monitor.setTextSize(2);
  monitor.setTextColor(BLUE);  
  monitor.setCursor(vcol, 40);
  monitor.println( "OnBoard");  
  monitor.setCursor(vcol, 100);
  monitor.println( "Computer");  
  monitor.setCursor(vcol, 160);
  monitor.println( "Beetle");  
*/
 //delay(5000);
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
  int16_t vcolMin = 0;
  int16_t vlinMin = 0;
  int16_t vcolMax = monitor.width();
  int16_t vlinMax = monitor.height();
  
  // Limpa a area util
  monitor.fillRect(vcolMin, vlinMin+33, vcolMax, 262, BLACK);  
  
}

/////////////////////////////////////////////////////////////
// Monitor Termometro Generico                             //
// p_titulo      - Titulo do Termometro (3 ou 4 caracteres // 
// p_valor       - Valor de medicao representado em branco //
// p_tamanho     - Tamanho termometro repres. cinza claro  //
// p_referencia  - Vlr limite para pintar de vermelho      //
// p_col_baixo   - coluna de baixo para inicio termometro  //
// p_lin_baixo   - linha de baixo do termometro            //
/////////////////////////////////////////////////////////////
void mostraTermometro(String p_titulo, int16_t p_valor, int16_t p_tamanho, int16_t p_referencia, int16_t p_col_baixo, int16_t p_lin_baixo, int16_t p_largura )
{
  int16_t _lin = p_lin_baixo;
  int16_t _cor = WHITE;
  int16_t _passo = 15;

  monitor.setTextSize(2);
  int16_t _linTit = p_col_baixo - (((p_titulo.length()*12) - p_largura-6) / 2);
  monitor.setCursor(_linTit, _lin);
  monitor.println(p_titulo);
 
  _lin   = _lin - 15;
  _passo = 15 / p_tamanho;
  for (byte i = 0; i < 15; i++)   
  {
    if (i < p_valor) 
      if ( i >= p_referencia )
         _cor = RED;
      else
         _cor = WHITE;
    else
       _cor = LIGHTGREY;
    monitor.fillRect( p_col_baixo, _lin-_passo, p_largura, 10, _cor);
    monitor.drawFastHLine(p_col_baixo, _lin-_passo, p_largura+5, WHITE);
    _lin-=10;
  }
}



