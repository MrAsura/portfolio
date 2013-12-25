// Toteutus Aurinkokunta-luokan metodeille
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include "aurinkokunta.h"
#include "peli.h"

using namespace std;

// Aurinkokunta-luokan oletus rakentaja
Aurinkokunta::Aurinkokunta(): x_(0),y_(0),nimi_("/0"),planeetat_(0),
oleskelija_(0)
{
}

//Rakentaja Aurinkokunta-luokalle
Aurinkokunta::Aurinkokunta( int x_koordinaatti, int y_koordinaatti,
                            string a_nimi, vector<Planeetta> planeetat ):
x_( x_koordinaatti ), y_( y_koordinaatti ), nimi_( a_nimi ),
planeetat_(planeetat),oleskelija_(0)
{
}

//Aurinkokunta-luokan purkaja
Aurinkokunta::~Aurinkokunta()
{
}

//Toteutus nimi funktiolle
string Aurinkokunta::nimi()const
{
   return nimi_;
}

// toteutus x-funktiolle
int Aurinkokunta::x()const
{
   return x_;
}

//toteutus y-funktiolle
//
int Aurinkokunta::y()const
{
   return y_;
}

//Toteutus oleskelijalle
Pelaaja* Aurinkokunta::oleskelija()const
{
   return oleskelija_;
}

//Toteutus oleskelijalle
void Aurinkokunta::oleskelija( Pelaaja* uusi )
{
   //jos uusi pelaaja on sama kuin vanha niin tulkitaan se tarkoittamaan, että
   //kukaan ei oleskele aurinkokunnassa, koska nollaa pointteria
   //ei voi välittää
   
   if( uusi == oleskelija_ )
   {
      oleskelija_ = 0;
      
      return;
   }
   
   oleskelija_ = uusi;
   
   return;
}

//Toteutus planeettoja funktiolle
int Aurinkokunta::planeettoja()const
{
   return planeetat_.size();
}

//Toteutus Ptyyppille
Ptyyppi Aurinkokunta::pTyyppi( int planeetta )const
{
   return planeetat_.at(planeetta).tyyppi;
}

//Toteutus asuta funktiolle
bool Aurinkokunta::asuta( int planeetta, bool alustus )
{
   --planeetta;

   //virhetarkistukset
   if( static_cast<unsigned int> (planeetta) >= planeetat_.size() )
   {
      cerr << PLANEETTAVIRHE << endl;
      return false;
   }
   
   if( planeetat_.at(planeetta).tyyppi == KAASU )
   {
      cerr << ASUTUSVIRHE << endl;
      return false;
   }
   
   if( planeetat_.at(planeetta).siirtokunta != 0 )
   {
      cerr << SIIRTOKUNTAVIRHE << endl;
      return false;
   }      

   // viestit tulostetaan vain jos ei ole alustuksesta kyse
   if( !alustus )
   {
      cout << oleskelija_->nimi()
        << " perusti uuden siirtokunnan aurinkokuntaan "
        << nimi_ << "." << endl;
   }
   
   planeetat_.at(planeetta).siirtokunta = oleskelija_;
   
   oleskelija_->pisteet(1);
   
   //tutkitaan onko kaikki planeetat asutettu ja onko jollain ylivoima
   if( taysi() )
   {
      multimap<Pelaaja*,unsigned int> omistajat;
      unsigned int suurin = 0;
      multimap<Pelaaja*,unsigned int>::iterator it;
      Pelaaja* mahtavin = 0;
      
      //Kootaan lista pelaajista joilla on siirtokuntia aurinkokunnassa
      for( unsigned int i = 0 ; i < planeetat_.size() ; ++i )
      {
         omistajat.insert( pair<Pelaaja*,unsigned int>
                           ( planeetat_.at(i).siirtokunta, i ) );
      }
            
      //tutkitaan kenellä on eniten siirtokuntia
      for( it = omistajat.begin() ; it != omistajat.end() ; ++it )
      {
         if( omistajat.count(it->first) > suurin )
         {
            suurin = omistajat.count(it->first);
            mahtavin = it->first;
         }
      }
           
      for( it = omistajat.begin() ; it != omistajat.end() ; ++it )
      {
         if( omistajat.count(it->first) == suurin && it->first != mahtavin )
         {
            // vallitsee tasapaino eli ei tehdä muutoksia
            return true;
         }
      }
      //muuten mahtavin saa kaikki planeetat
      for( unsigned int i = 0 ; i < planeetat_.size() ; ++i )
      {
         planeetat_.at(i).siirtokunta->pisteet(-1);
         
         planeetat_.at(i).siirtokunta = mahtavin;
                  
         planeetat_.at(i).siirtokunta->pisteet(1);
      }
   }
        
   return true;
}

//Toteutus kuutukikohta:lle
bool Aurinkokunta::kuutukikohta( int planeetta )
{
   --planeetta;
   
   //virhe tarkistuksia
   if( static_cast<unsigned int> (planeetta) >= planeetat_.size() )
   {      
      cerr << PLANEETTAVIRHE << endl;
      return false;
   }
   
   if( planeetat_.at(planeetta).siirtokunta == 0 )
   {      
      cerr << EIOLEVIRHE << endl;
      return false;
   }
   
   if( oleskelija_ != planeetat_.at(planeetta).siirtokunta )
   {      
      cerr << VARATTUVIRHE << endl;
      return false;
   }
   
   if( planeetat_.at(planeetta).tila == EI )
   {      
      cerr << EIKUUTAVIRHE << endl;
      return false;
   }
   
   if( planeetat_.at(planeetta).tila == TUKIKOHTA )
   {      
      cerr << ONJOVIRHE << endl;
      return false;
   }

   planeetat_.at(planeetta).tila = TUKIKOHTA;
   
   cout << oleskelija_->nimi()
     << " perusti uuden avaruuslentotukikohdan." << endl;
   
   return true;
}

//toteutus muunna funktiolle
bool Aurinkokunta::muunna( int planeetta )
{
   --planeetta;
   
   //virhetarkistuksia
   if( static_cast<unsigned int> (planeetta) >= planeetat_.size() )
   {      
      cerr << PLANEETTAVIRHE << endl;
      return false;
   }
   
   if( planeetat_.at(planeetta).tyyppi == ASUTTAVA )
   {      
      cerr << EIKAASUVIRHE << endl;
      return false;
   }
   
   planeetat_.at(planeetta).tyyppi = ASUTTAVA;
   
   cout << oleskelija_->nimi() << " muokkasi kaasuplaneetasta asuinkelpoisen."
     << endl;
   
   return true;
}

//toteutus funktiolle pListaa
void Aurinkokunta::pListaa()const
{
   unsigned int pisin_nimi = 0;
   
   //määrittää pisimmän nimen
   for( unsigned int i = 0 ; i < planeetat_.size() ; ++i )
   {
      if( planeetat_.at(i).siirtokunta != 0 )
      {
         if( planeetat_.at(i).siirtokunta->nimi().size() > pisin_nimi )
         {
            pisin_nimi = planeetat_.at(i).siirtokunta->nimi().size();
         }
      }
   }
   
   bool asutettu = false;
   
   //tulostetaan planeettojen tíedot
   for( unsigned int i = 0 ; i < planeetat_.size() ; ++i )
   {      
      asutettu = false;
      
      cout << i + 1 << ". ";
      
      if( planeetat_.at(i).tyyppi == ASUTTAVA )
      {
         cout << "asutettava";
      }
      else
      {
         cout << setw(10) << left << "kaasu";
      }
      
      if( planeetat_.at(i).siirtokunta != 0 )
      {
         cout << "   " << setw(pisin_nimi)
           << planeetat_.at(i).siirtokunta->nimi();
         asutettu = true;
      }
            
      if( planeetat_.at(i).tila == ON || planeetat_.at(i).tila == TUKIKOHTA )
      {
         if( !asutettu )
         {
            cout << "   " << setw(pisin_nimi) << "";
         }
         
         cout << "   " <<  "kuu";
         
         if( planeetat_.at(i).tila == TUKIKOHTA )
         {            
            cout << ", lentotukikohta";
         }
      }
      
      cout << endl;
   }
}

//toteutus tuhoaSiirtokunnat funktiot
void Aurinkokunta::tuhoaSiirtokunnat( Pelaaja* kohde )
{
   // nollataan tietyn pelaajan siirtokunnat
   for( unsigned int i = 0 ; i < planeetat_.size() ; ++i )
   {
      if( planeetat_.at(i).siirtokunta == kohde )
      {
         planeetat_.at(i).siirtokunta = 0;
      }
   }
   
   return;
}

//toteutus enerkialle
int Aurinkokunta::energia( Pelaaja* omistaja )
{
   int energy = 0;
   
   //lasketaan pelaajan omistamien siirtokuntien ja kuutukikohtien määrä
   for( unsigned int i = 0 ; i < planeetat_.size() ; ++i )
   {
      if( planeetat_.at(i).siirtokunta == omistaja )
      {
         ++energy;
         
         if( planeetat_.at(i).tila == TUKIKOHTA )
         {
            ++energy;
         }
      }
   }
   
   return energy;
}

//toteutus täysi funktiolle
bool Aurinkokunta::taysi( bool kuut )const
{   
   for( unsigned int i = 0 ; i < planeetat_.size() ; ++i )
   {
      if( planeetat_.at(i).siirtokunta == 0 ||
          ( kuut && planeetat_.at(i).tila == ON ) )
      {
         return false;
      }
   }
   
   return true;
}
