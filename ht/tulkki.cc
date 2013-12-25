//TULKKI
//toteutus tulkille

#include "tulkki.h"
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cctype>


using namespace std;


//Hoitaa komennon lukemisen
vector<string> komentoLuku()
{   
   string komento = "";
   vector<string> purku(0);
   
   getline( cin, komento );
   
   stringstream apu_virta( komento );
      
   string apu_string = "";
   apu_virta >> apu_string;
   
   purku.push_back( apu_string );
   
   apu_string.clear();
      
   // erotetaan parametrin erilliset osat ja poistetaan whitespace
   apu_virta >> apu_string;
   string apu_apu_string = apu_string;
     
   //kootaan parametri yhdeksi v‰lill‰ eroteltuna
   while( apu_virta >> apu_string )
   {
      apu_apu_string += " ";
      apu_apu_string += apu_string;
   }
         
   purku.push_back( apu_apu_string );   //talletetaan parametritkin vectoriin
   
   apu_virta.clear();
   
   return purku;
}

//analysoi komennon
Nimi analysoi( vector<string>& tutkittava, int& luku_parametri,
               string& merkki_parametri )
{
   //virhetarkistuksia
   if( cin.eof() )
   {
      return END;
   }
   else if( tutkittava.size() == 0 )
   {
      return TUNTEMATON;
   }
   else if( tutkittava.size() > 2 )
   {
      return PARAMETRIVIRHE;
   }
         
   string apu_nimi = "";
   bool epatarkka_lippu = false;  //kertoo onko monta vaihtoehtoa komennolle
   const int tut_pituus = tutkittava.at(0).size();
   int kom_sijainti = -1;
      
   //muutetaan komento pieniksi kirjaimiksi
   for( int i = 0 ; i < tut_pituus ; ++i )
   {
      tutkittava.at(0).at(i) = 
        static_cast<char>(tolower(tutkittava.at(0).at(i)));
   }

   apu_nimi = tutkittava.at(0);
   
   //tutkitaan onko moniselitteinen
   for( int i = 0 ; i < KMAARA ; ++i )
   {
      if( apu_nimi == KOMENNOT[i].nimi.substr( 0, tut_pituus ) )
      {
         if( epatarkka_lippu )
         {
            return MONISELITTEINEN;
         }
         kom_sijainti = i;
         epatarkka_lippu = true;
      }
   }
   
   //jos ei sovi yhteenk‰‰n j‰‰ sijainti nollaksi
   //ei ole lˆydetty yht‰‰n sopivaa
   if( kom_sijainti < 0 )
   {
      return TUNTEMATON;
   }
   
   //k‰sittelee LUKU tyyppiset parametrit
   if( KOMENNOT[kom_sijainti].tyyppi == LUKU )
   {
      string roska = "/0";
      unsigned int apu_luku = 0;
      stringstream s_virta( tutkittava.at(1) );
      
      s_virta >> apu_luku;
      
      if( !s_virta || s_virta >> roska) //jos inttiin luku ep‰onnistuu on
      {                                 //kyseess‰ virheellinen parametri
         return PARAMETRIVIRHE;         //tai jos on lis‰‰ parametrej‰
      }                                 //tulee virhe
      
      luku_parametri = apu_luku;
      
      return KOMENNOT[kom_sijainti].komento;
   }
   
   //merkkityyppisten parametrien k‰sittely
   
   merkki_parametri = tutkittava.at(1);
   
   return KOMENNOT[kom_sijainti].komento;
}
