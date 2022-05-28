#include <Trade\Trade.mqh>
CTrade trade;
MqlRates rates[];
input int in_takeProfitProportion = 2;
double tickSize;
input int  in_lotsPercent=2;
input int VerySlow_MA_period=200;
input int Fast_MA_period=20;
input int Slow_MA_period=80;
int EA_Magic = 123459;
input double percentmin=0.005;
double Fast_Ma_Buff[];
double Slow_Ma_Buff[];
double VerySlow_Ma_Buff[];
int Fast_Ma_Handle;
int Slow_Ma_Handle;
int VerySlow_Ma_Handle;
double ask;
double bid;
double distance;
input bool VSlowMa=false;
input int LOTE=300;
input double Perda_Maxima  = 0.2;
input double Kelly     = 0.5;
input bool   Kelly_ON =true;

int OnInit()
  {

tickSize = SymbolInfoDouble(Symbol(),SYMBOL_TRADE_TICK_SIZE);   

Fast_Ma_Handle=iMA(_Symbol,PERIOD_CURRENT,Fast_MA_period,0,MODE_EMA, PRICE_CLOSE);
Slow_Ma_Handle=iMA(_Symbol,PERIOD_CURRENT,Slow_MA_period,0,MODE_EMA, PRICE_CLOSE);

if (VSlowMa==true){
VerySlow_Ma_Handle=iMA(_Symbol,PERIOD_CURRENT,VerySlow_MA_period,0,MODE_EMA, PRICE_CLOSE);
ArraySetAsSeries(VerySlow_Ma_Buff,true);
}

ArraySetAsSeries(Fast_Ma_Buff,true);
ArraySetAsSeries(rates,true);
ArraySetAsSeries(Slow_Ma_Buff,true);

   return(INIT_SUCCEEDED);
  }
void OnTick()
  {

if (VSlowMa==true){
CopyBuffer(VerySlow_Ma_Handle,0,0,7,VerySlow_Ma_Buff);
}

CopyBuffer(Fast_Ma_Handle,0,0,7,Fast_Ma_Buff);
CopyBuffer(Slow_Ma_Handle,0,0,7,Slow_Ma_Buff);
CopyRates(_Symbol,PERIOD_CURRENT,0,7,rates);


ask=NormalizeDouble(SymbolInfoDouble(_Symbol,SYMBOL_ASK),_Digits);
bid=NormalizeDouble(SymbolInfoDouble(_Symbol,SYMBOL_BID),_Digits);

if(PositionsTotal()<=0 ){
     
         if  (
             
             
             (Fast_Ma_Buff[1]>Slow_Ma_Buff[1])&&
             (Fast_Ma_Buff[2]<Slow_Ma_Buff[2])
      //       && (Fast_Ma_Buff[0]>VerySlow_Ma_Buff[0])
             )
         
                   
             {                     
            trade.Buy(CalcLots(),NULL,0.0,0.0,0.0);
            
             }
      
        if  (                       
             (Fast_Ma_Buff[1]<Slow_Ma_Buff[1])&&
             (Fast_Ma_Buff[2]>Slow_Ma_Buff[2])
        //     && (Fast_Ma_Buff[0]<VerySlow_Ma_Buff[0])           
            )
            {
  
          // trade.Sell(CalcLots(),NULL,0.0,0.0,0.0);
            
            }              
            }
          
         
      if(PositionsTotal()<=1 ){
      Rastreia();
      }
  } 
  int CalcLots()
  {
   if  (Kelly_ON ==false){
   
   return LOTE;

   }

    double balance = AccountInfoDouble(ACCOUNT_EQUITY);
   
    double CapitalKelly;
    double QTD_asset;
  
    CapitalKelly= (Kelly*balance)/Perda_Maxima;
    QTD_asset= CapitalKelly/ask; 
    Print("CapitalKelly", CapitalKelly);
    Print("ask ", ask);
    Print("QTD_asset", QTD_asset);
    
    int lot_arrendondado = MathRound(QTD_asset/100) * 100;
    Print("lot_arrendondado ", lot_arrendondado);
    return lot_arrendondado;
  }
      
  void Rastreia()
   {
   
   for (int i=PositionsTotal()-1;i>=0;i--) 
   {
     string symbol=PositionGetSymbol(i);
     if (symbol==Symbol())
   {
      ulong ticket= PositionGetInteger(POSITION_TICKET);
   
      if (PositionGetInteger(POSITION_TYPE)== POSITION_TYPE_BUY)
   {
      
      if (
      
              (Fast_Ma_Buff[1]<Slow_Ma_Buff[1])&&
              (Fast_Ma_Buff[2]>Slow_Ma_Buff[2])
         )
      
   {
      
      trade.PositionClose(ticket,0);
      
   }
   }
   
      if (PositionGetInteger(POSITION_TYPE)== POSITION_TYPE_SELL)   
   {
   
          if (
      
              (Fast_Ma_Buff[1]>Slow_Ma_Buff[1])&&
              (Fast_Ma_Buff[2]<Slow_Ma_Buff[2])
             )
             
             {
      
      trade.PositionClose(ticket,0);
      
             } 
   }  
   }       
   }
   }