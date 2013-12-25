//Toteutus komentotulkille

#include "tulkki.h"
#include "peli.h"
#include "aurinkokunta.h"
#include "pelaaja.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

void komentotulkki( bool& lopeta, Peli& moottori )
{
   int para_l = 0;
   string para_m = "/0";
   vector<string> komento = komentoLuku();
   
   //tulkitsee komennon
   switch( analysoi( komento, para_l, para_m ) )
   {
    case ASUTA:
      if( moottori.vuoro()->sijainti()->asuta(para_l) )
      {
         lopeta = moottori.seuraavaVuoro();
      }
      break;
    
    case KUU:
      if( moottori.vuoro()->sijainti()->kuutukikohta(para_l) )
      {
         lopeta = moottori.seuraavaVuoro();
      }
      break;
      
    case MUUNNA: 
      if( moottori.vuoro()->sijainti()->muunna(para_l) )
      {         
          lopeta = moottori.seuraavaVuoro();
      }
      break;
      
    case LIIKU:
      if( para_m.empty() )
      {         
         cerr << PARAVIRHE << endl;
         break;
      } 
      if( moottori.matkaa( para_m ) )
      {
         lopeta = moottori.seuraavaVuoro();
      }
      break;
      
    case LISTAA: moottori.listaa(para_m);
      break;
      
      //Vuoro vaihdetaan jo luovuta jäsenfunktiossa
    case LUOVUTA: lopeta = moottori.luovuta();
      break;
      
    case KARTTA: moottori.kartta();
      break;
    
    case PELAAJAT:
      if( !para_m.empty() )
      {
         cerr << PARAVIRHE << endl;
         break;
      }
      moottori.pelaajat();
      break;
    
    case LOPETA:
      if( !para_m.empty() )
      {         
         cerr << PARAVIRHE << endl;
         break;
      }      
      lopeta = true;
      return;
      break;//stylee varten
    
    case TUNTEMATON: cerr << TUNTEMATON_KOM << endl;
      break;
    
    case PARAMETRIVIRHE: cerr << PARAVIRHE << endl;
      break;
    
    case MONISELITTEINEN: cerr << MONIVIRHE << endl;
      break;
    
    default:
      break;
   }
   
   return;
}
