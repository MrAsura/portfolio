//Toteutus luokalle Peli

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>
#include <cctype>
#include <iomanip>
#include <cmath>
#include "peli.h"

using namespace std;


//Oletus rakentaja luokalle-peli
Peli::Peli():pelaajia_(0),koko_x_(0),koko_y_(0),vuoro_(0),
pelaajat_(0),galaxi_(0)
{
}

//Oletus purkaja
Peli::~Peli()
{
   for( unsigned int i = 0 ; i < pelaajat_.size() ; ++i )
   {
      delete pelaajat_.at(i);
      pelaajat_.at(i) = 0;
   }
   pelaajat_.clear();
   
   for( unsigned int i = 0 ; i < galaxi_.size() ; ++i )
   {      
      delete galaxi_.at(i);
      galaxi_.at(i) = 0;
   }   
   galaxi_.clear();

}

//toteutus funktiolle onko
int Peli::onko( string aurinkokunta )const
{   
   bool sama = true;
   
   for( unsigned int i = 0 ; i < galaxi_.size() ; ++i )
   {
      sama = true;
      
      // jos aurinkokunta on pidempi kuin verrattavan nimi niin
      //ei voi olla sama
      if( aurinkokunta.size() > galaxi_.at(i)->nimi().size() )
      {
         sama = false;
      }
      
      string sub_verrattava =
        galaxi_.at(i)->nimi().substr(0,aurinkokunta.size());
      
      //verrataan kirjain kirjaimelta ja kirjaimet muutettu pieniksi
      for( unsigned int n = 0 ; n < aurinkokunta.size() && sama ; ++n )
      {
         if( tolower( sub_verrattava.at(n) ) != tolower( aurinkokunta.at(n) ) )
         {
            sama = false;
         }
      }
      
      if( sama )
      {
         return static_cast<int> (i);
      }
   }
   
   return -1;
}

//toteutus listaa funktiolle
void Peli::listaa( string aurinkokunta )const
{
   //jos parametri tyhj‰ listataan oletuksena sijainti aurinkokunta
   if( aurinkokunta.empty() )
   {
      cout << vuoro_->sijainti()->nimi() << endl;
      vuoro_->sijainti()->pListaa();
      
      return;
   }
      
   int sijainti = onko( aurinkokunta );
   
   if( sijainti <= -1 )
   {
      cerr << EI_OLE_A_VIRHE << endl;
      return;
   }
     
   //jos pelaaja ei ole k‰ynyt aurinkokunnassa ennen tulostetaan vain nimi
   if( !(vuoro_->kayty(galaxi_.at(sijainti))) )
   {
      cout << galaxi_.at(sijainti)->nimi() << endl;
      return;
   }
      
   cout << galaxi_.at(sijainti)->nimi() << endl;
   galaxi_.at(sijainti)->pListaa();
   
   return;
} 

//Toteutus seuraavaP funktiolle
void Peli::seuraavaP()
{
   unsigned int sijainti = 0;
   Pelaaja* apu = 0;
   
   //etsit‰‰n vuorossa olevan pelajaan sijainti
   while( pelaajat_.at(sijainti) != vuoro_ )
   {
      ++sijainti;
   }
   
   //Jos vuorossa on vektorin viimeinen pelaaja, pyor‰ht‰‰ vuoro taas 
   //ensimm‰iselle
   if( (sijainti + 1) >= pelaajat_.size() )
   {
      apu = pelaajat_.at(0);
   }
   else
   {
      apu = pelaajat_.at(sijainti + 1);
   }
   
   vuoro_ = apu;
   
   return;
}

//toteutus voitto funktiolle
void Peli::voitto()const
{
   int voitto_pisteet = 0;
   vector<unsigned int> indeksit(0); //voittajien indeksit
   
   //etsit‰‰n kenell‰ on eniten pisteit‰ ja ne keill‰ on eniten
   //merkit‰‰n muistiin
   for( unsigned int i = 0 ; i < pelaajat_.size() ; ++i )
   {
      if( pelaajat_.at(i)->pisteet() > voitto_pisteet )
      {
         voitto_pisteet = pelaajat_.at(i)->pisteet();
         
         indeksit.clear();
         indeksit.push_back( i );
      }
      else if( pelaajat_.at(i)->pisteet() == voitto_pisteet )
      {
         indeksit.push_back( i );
      }
   }
   
   //jos yksi voittaja
   if( indeksit.size() == 1 )
   {
      cout << pelaajat_.at(indeksit.at(0))->nimi()
        << " voitti pistein " << voitto_pisteet << "." << endl;
      
      return;
   }
   
   //jos tasapeli
   for( unsigned int i = 0 ; i < indeksit.size() ; ++i )
   {
      cout << pelaajat_.at(indeksit.at(i))->nimi();
      
      if( i < indeksit.size() - 1 )
      {
         cout << ", ";
      }
      else
      {
         cout << " ";
      }
   }
   
   cout << "p‰‰tyiv‰t tasapeliin pistein " << voitto_pisteet << "." << endl;

   return;
}

//Toteutus lopputilan tarkastulselle
bool Peli::lopputilanne()const
{   
   //Testataan onko p‰‰ttymis ehto voimassa
   
   // Jos vain yksi pelaaja peli p‰‰ttyy
   if( pelaajat_.size() == 1 )
   {
      voitto();
      
      return true;
   }
   
   bool kaikki_asutettu = true;
   
   //Jos kaikkien aurinkokuntien kaikki planeetat on asutettu peli p‰‰ttyy
   for( unsigned int i = 0 ; i < galaxi_.size() ; ++i )
   {      
      if( !galaxi_.at(i)->taysi() )
      {         
         kaikki_asutettu = false;
         break;
      }      
   }
     
   if( kaikki_asutettu )
   {
      voitto();
      return true;
   }
   
   return false;
}


//Toetutus pattitilanteelle
bool Peli::pattitilanne()
{
   Pelaaja* apu = vuoro_;
   
   //pattitilanne
   bool patti = true;
   
   //voidaanko asuttaa/muuntaa/rakentaa
   if( !apu->sijainti()->taysi( true ) )
   {
      patti = false;
   }
   //voidaanko liikkua
   if( patti )
   {
      int sij = -1;
      
      for( unsigned int i = 0 ; i < galaxi_.size() ; ++i )
      {
         if( matkaTarkistus( galaxi_.at(i)->nimi(), sij, false ) )
         {
            patti = false;
         }
      }
   }
   
   bool loppu = false;
   
   if( patti )
   {
      apu = 0;
      loppu = luovuta( PATTITILANNE ); //vuoro vaihdetaan aliohjelmassa
   }
   
   return loppu;
}

//Toteutus seuraavaVuoro
bool Peli::seuraavaVuoro()
{
   seuraavaP();
   
   bool loppu = lopputilanne();
   
   if( !loppu )
   {      
      loppu = pattitilanne();
   }
   
   return loppu;
}

//toteutus luovuta funktiolle
bool Peli::luovuta( string viesti )
{
   Pelaaja* luovuttaja = vuoro_;
   
   cout << vuoro_->nimi() << viesti << endl;
   
   seuraavaP();
   
   luovuttaja->nollaaSiirtokunnat();
   
   //poistetaan pelaaja aurinkokunnasta
   luovuttaja->sijainti()->oleskelija( luovuttaja );
   
   //etsit‰‰n pelaajan sijainti ja poistetaan se pelaajat_ vektorista
   int indeksi = 0;
   
   while( pelaajat_.at(indeksi) != luovuttaja )
   {
      ++indeksi;
   }
   
   pelaajat_.erase( pelaajat_.begin() + indeksi ); 
   
   delete luovuttaja;
   luovuttaja = 0;

   //Tehd‰‰n viel‰ vuoron vaihtumis tarkistukset
   
   bool loppu = lopputilanne();
   
   if( !loppu )
   {      
      loppu = pattitilanne();
   }
   
   return loppu;
}

//Toteutus vuorolle
Pelaaja* Peli::vuoro()const
{
   return vuoro_;
}

//Toteutus j‰senfunktiolle pelaaja
void Peli::pelaajat()const
{
   //etsit‰‰n pisin nimi
   int pisin = pelaajat_.at(0)->nimi().size();
   
   for( unsigned int i = 1 ; i < pelaajat_.size() ; ++i )
   {
      if( static_cast<unsigned int> (pisin) < pelaajat_.at(i)->nimi().size() )
      {
         pisin = pelaajat_.at(i)->nimi().size();
      }
   }
   ++pisin;
   
   cout << " " << setw(pisin + 3) << left << "Pelaajat" << "Pisteet" << endl;
   
   //tulostetaan pelaajat riveitt‰in
   for( unsigned int i = 0 ; i < pelaajat_.size() ; ++i )
   {
      if( pelaajat_.at(i) == vuoro_ )
      {
         cout << ">";
      }
      else
      {
         cout << " ";
      }
      
      cout <<  pelaajat_.at(i)->pNum() << ": ";
      
      cout << setw(pisin) << left << pelaajat_.at(i)->nimi() << setw(6)
        << right << pelaajat_.at(i)->pisteet() << "p" << endl;
   }
   
   return;
}

//Toteutus funktiolle tulostaRivi
void Peli::tulostaRivi( string r_pohja, int r_numero )const
{
   string rivi(r_pohja);
   
   //k‰yd‰‰n l‰pi riville tulevat aurinkokunnat
   for( unsigned int n = 0 ; n < galaxi_.size() ; ++n )
   {
      if( galaxi_.at(n)->y() == r_numero )
      {
         rivi.at( 2 * (galaxi_.at(n)->x() - 1) ) =
           galaxi_.at(n)->nimi().at(0);
         
         //tutkitaan mik‰ merkki tulostetaan. ? jos ei k‰yty oleskelijan numero jos
         // on k‰yty tai vain tyhj‰ jos ei ole oleskelijaa
         if( !(vuoro_->kayty(galaxi_.at(n))) )
         {
            rivi.at( 2 * (galaxi_.at(n)->x() - 1) + 1 ) = '?';
         }
         else if( galaxi_.at(n)->oleskelija() != 0 )
         {           
            rivi.at( 2 * (galaxi_.at(n)->x() - 1) + 1 ) =
              static_cast<char> ( galaxi_.at(n)->oleskelija()->pNum() + 
                                  static_cast<int> ('0') );
         }
      }
   }
         
   cout << rivi;
   
   //Tulostetaan pelaaja
   
   cout << "  ";
   
   if( static_cast<unsigned int> (r_numero) <= pelaajat_.size() )
   {            
      if( pelaajat_.at(r_numero - 1) == vuoro_ )
      {
         cout << ">";
      }
      else
      {
         cout << " ";
      }
      
      cout << pelaajat_.at(r_numero - 1)->pNum() << ": "
        << pelaajat_.at(r_numero - 1)->nimi() << endl;
   }
   else
   {
      cout << endl;
   }
      
   return;
}

//toteutus kartta funktiolle
void Peli::kartta()const
{
   cout << "  ";
   
   int apu = 0;
   
   //tulostetaan koordinaatit
   for( int i = 1 ; i <= koko_x_ ; ++i )
   {
      if( i - apu >= 10 )
      {
         apu += 10;
      }
            
      cout << i - apu << " ";
   }
   
   cout << "   Pelaajat" << endl;
   
   //tehd‰‰n aluksi rivin pohja
   
   string r_pohja;
   
   for( int i = 1 ; i <= koko_x_ ; ++i )
   {
      r_pohja += ". ";
   }
   
   apu = 0;
   
   for( int i = 1 ; i <= koko_y_ ; ++i )
   {
      //sijaitetaan pelaajat ja aurinkokunnat
           
      if( i - apu >= 10 )
      {         
         apu += 10;
      }
      
      cout << i - apu << " ";
      
      tulostaRivi( r_pohja, i );
   }
   
   //Tutkitaan onko viel‰ tulostettavia pelaajia ja tulostetaan ne
   if( pelaajia_ > koko_y_ )
   {
      for( int i = koko_y_ ; i < pelaajia_ ; ++i )
      {
         cout << setw(koko_x_*2 + 5) << right;
         
         if( pelaajat_.at(i) == vuoro_ )
         {
            cout << ">";
         }
         else
         {
            cout << "";
         }
         
         cout  << pelaajat_.at(i)->pNum() << ": "
           << pelaajat_.at(i)->nimi() << endl;
      }
   }
      
   return;
}

//toteutus matkaa funktiolle
bool Peli::matkaa( string kohde )
{
   //tarkistaa onnistuuko matka ja palauttaa viitteen‰ sijainnin
   int matkakohde = -1;
   
   if( !matkaTarkistus( kohde, matkakohde ) )
   {
      return false;
   }
   
   //jos matka on mahdollinen kutsutaan liiku j‰senfunktiota
   
   vuoro_->liiku( galaxi_.at(matkakohde) );
   
   return true;
}

//toteutus matkaTarkistukselle
bool Peli::matkaTarkistus( string kohde, int& sijainti, bool tulosteet )const
{
   sijainti = onko( kohde );
   
   //virhe tarkistus
   if( sijainti <= -1 )
   {
      if( tulosteet )
      {
         cerr << EI_OLE_A_VIRHE << endl;
      }      
      return false;
   }
   
   // lasketaan v‰limatka
   int energiaa = vuoro_->sijainti()->energia(vuoro_);
   
   double x1 = vuoro_->sijainti()->x();
   double y1 = vuoro_->sijainti()->y();
   
   double x2 = galaxi_.at(sijainti)->x();
   double y2 = galaxi_.at(sijainti)->y();
   
   int matka =
     static_cast<int> (sqrt( (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1) ));
   
   if( matka > energiaa )
   {
      // tuloste taan viestit vain jos tulosteet on sallittu
      if( tulosteet )
      {      
         //Easteregg
         if( galaxi_.at(sijainti)->nimi() == "Mordor" )
         {
            cout << "One does not simply fly into Mordor." << endl;
         }
         
         cerr << VALIMATKAVIRHE << endl;
      }     
      return false;
   }
   
   //virhetarkistus
   if( galaxi_.at(sijainti)->oleskelija() != 0 )
   {
      if( tulosteet )
      {         
         cerr << MAHDUVIRHE << endl;
      }
      return false;
   }
   
   return true;
}
   
//Toteutus funktiolle alusta
bool Peli::alusta( const char* alustustiedosto, bool& loppu )
{
   ifstream tiedosto( alustustiedosto );
   
   if( !tiedosto.is_open() || !tiedosto )
   {
      cerr << LUKUVIRHE_  << endl;
      
      return false;
   }
   
   string tagi = "/0";
   int apu_x = 0;
   int apu_y = 0;
   int apu_pelaajia = 0;
   
   //Tallennetaan yleiset osion tiedot
   tiedosto >> tagi;
   
   if( tagi != "<yleiset>" )
   {
      cerr << SYNTAKSIVIRHE_ << endl;
      
      return false;
   }
   
   tiedosto >> apu_x >> apu_y >> apu_pelaajia;
   
   if( (apu_x < 2 || apu_x > 20) || (apu_y < 2 || apu_y > 20) )
   {
      cerr << Y_KOKOVIRHE_ << endl;
      
      return false;
   }
   
   if( apu_pelaajia < 2 || apu_pelaajia > 9 )
   {
      cerr << Y_PELAAJAVIRHE_ << endl;
   
      return false;
   }
   
   koko_x_ = apu_x;
   koko_y_ = apu_y;
   pelaajia_ = apu_pelaajia;
   
   tiedosto >> tagi;
      
   if( tagi != "</yleiset>" )
   {
      cerr << SYNTAKSIVIRHE_ << endl;
      
      return false;
   }
   
   //luetaan aurinkokuntien tiedot
   
   int aurinkokuntia = 0;
   
   tiedosto >> tagi;

   
   //m‰‰rittelem‰tˆn tagi tai odottamaton
   if( tagi != "<aurinkokunta>" && tagi != "</aurinkokunta>" )
   {
      cerr << SYNTAKSIVIRHE_  << endl;
      
      return false;
   }
  
   
   string a_nimi = "/0";
   int a_x = 0;
   int a_y = 0;
   int planeettoja = 0;
   char planeetta = ' ';
   char k_tyyppi = ' ';
   Planeetta apu_p = { KAASU, EI, 0 };
   vector<Planeetta> apu_planeetat(0);
   Aurinkokunta* a_osoitin = 0;

   while( tagi == "<aurinkokunta>" )
   {
      getline( tiedosto, a_nimi ); //otetaan v‰lit pois
      getline( tiedosto, a_nimi );
                  
      if( !tiedosto )
      {        
         cerr << SYNTAKSIVIRHE_  << endl;
         
         return false;
      }
      
      if( a_nimi.size() < 1 || a_nimi.size() > 79 || !isupper(a_nimi.at(0)) )
      {
         cerr << A_NIMIVIRHE_  << endl;
         
         return false;
      }
      
      //Tutkitaan sis‰lt‰‰kˆ laittomia merkkej‰
      for( unsigned int i = 1 ; i < a_nimi.size() ; ++i )
      {
         if( !isalnum(a_nimi.at(i)) && !isspace(a_nimi.at(i)) )
         {
            cerr << A_NIMIVIRHE_  << endl;
            
            return false;
         }
      }
      
      //samanimi tarkistus
      for( unsigned int i = 0 ; i < galaxi_.size() ; ++i )
      {
         if( galaxi_.at(i)->nimi().at(0) == a_nimi.at(0) )
         {
            cerr << A_SAMANIMIVIRHE_  << endl;
            
            return false;
         }
      }
      
      tiedosto >> a_x;
      tiedosto >> a_y;
                  
      if( !tiedosto )
      {        
         cerr << SYNTAKSIVIRHE_  << endl;
         
         return false;
      }
      
      if( a_x < 0 || a_y < 0 || a_x > koko_x_ || a_y > koko_y_ )
      {
         cerr << A_SIJAINTIVIRHE_  << endl;
         
         return false;
      }
      
      for( unsigned int i = 0 ; i < galaxi_.size() ; ++i )
      {
         if( galaxi_.at(i)->x() == a_x && galaxi_.at(i)->y() == a_y )
         {
            cerr << A_SIJAINTIVIRHE_  << endl;
            
            return false;
         }
      }
      
      tiedosto >> planeettoja;
      
            
      if( !tiedosto )
      {        
         cerr << SYNTAKSIVIRHE_  << endl;
         
         return false;
      }
      
      if( planeettoja < 0 || planeettoja > 5 )
      {
         cerr << A_KOKOVIRHE_  << endl;
         
         return false;
      }
      
      //luetaan planeettojen tiedot
      for( int i = 0 ; i < planeettoja ; ++i )
      {
         tiedosto >> planeetta;
         tiedosto >> k_tyyppi;
         
         if( !tiedosto )
         {        
            cerr << SYNTAKSIVIRHE_  << endl;
            
            return false;
         }
         
         switch( planeetta )
         {
          case 'A': apu_p.tyyppi = ASUTTAVA;
            break;
          case 'K': apu_p.tyyppi = KAASU;
            break;
          default: cerr << A_PTYYPPIVIRHE_ << endl;
            return false;
         }
         
         switch( k_tyyppi )
         {            
          case 'E': apu_p.tila = EI;
            break;
          case 'K': apu_p.tila = ON;
            break;
          default: cerr << A_KTYYPPIVIRHE_ << endl;
            return false;
         }
         
         apu_planeetat.push_back( apu_p );
      }
      
      //sijoitetaan luetut tiedot
      Aurinkokunta apu_aurinko( a_x, a_y, a_nimi, apu_planeetat );
      apu_planeetat.clear();
      
      a_osoitin = new Aurinkokunta;  //varataan tila uudelle aurinkokiunnalle
      
      *a_osoitin = apu_aurinko;  //talletetaan aurinkokunnan tiedot
                                 //varattuun alueeseen
      galaxi_.push_back( a_osoitin );
            
      tiedosto >> tagi;
      
      if( tagi != "</aurinkokunta>" )
      {
         cerr << SYNTAKSIVIRHE_ << endl;
         
         return false;
      }
      
      tiedosto >> tagi;
      
      ++aurinkokuntia;
      
   }
   
   if( aurinkokuntia < pelaajia_ || aurinkokuntia > koko_x_ * koko_y_ )
   {      
      cerr << SYNTAKSIVIRHE_ << endl;
      
      return false;
   }
   
   
   //m‰‰rittelem‰tˆn tagi tai odottamaton
   if( tagi != "<pelaaja>" && tagi != "</pelaaja>" )
   {
      cerr << SYNTAKSIVIRHE_  << endl;
      
      return false;
   }
   
   
   // Luetaan pelaajien tiedot
   // 
   // luetaan tagi jos edell‰ ei ole viel‰ luettu sit‰
      
   string p_nimi = "/0";
   string aloitus_a = "/0";
   int a_indeksi = 0; //alkuaurinkokunnan indeksi
   int koti_p = -1;
   int pelaaja_tageja = 0;
   Pelaaja *p_osoitin = 0;
   
   int p_num = 1;
   
   while( tagi == "<pelaaja>" )
   {
      getline( tiedosto, p_nimi ); //luetaan v‰lit pois
      getline( tiedosto, p_nimi );
          
      if( p_nimi.size() < 1 || p_nimi.size() > 37 )
      {
         cerr << P_NIMIVIRHE_ << endl;
         
         return false;
      }

      for( unsigned int i = 0 ; i < pelaajat_.size() ; ++i )
      {
         if( p_nimi == pelaajat_.at(i)->nimi() )
         {
            cerr << P_NIMIVIRHE_ << endl;
            
            return false;
         }
      }      
      
      getline( tiedosto, aloitus_a );
            
      if( !tiedosto )
      {         
         cerr << SYNTAKSIVIRHE_ << endl;
         
         return false;
      }
      
      //tutkitaan onko pelaajan aloitus aurinkokunta olemassa
      bool on_a = false;
      
      for( unsigned int i = 0 ; i < galaxi_.size() ; ++i )
      {         
         if( galaxi_.at(i)->nimi() == aloitus_a )
         {
            on_a = true;
            
            a_indeksi = i;
            
            break;
         }         
      }      
      
      if( !on_a )
      {            
         cerr << P_AURINKOKUNTAVIRHE_ << endl;
         
         return false;
      }         
      
      if( galaxi_.at(a_indeksi)->oleskelija() != 0 )
      {
         cerr << P_ALKUPAIKKAVIRHE_ << endl;
         
         return false;
      }
      
      tiedosto >> koti_p;
      
      if( !tiedosto )
      {         
         cerr << SYNTAKSIVIRHE_ << endl;
         
         return false;
      }
            
      if( koti_p < 1 || koti_p > galaxi_.at(a_indeksi)->planeettoja()  
          || galaxi_.at(a_indeksi)->pTyyppi(koti_p - 1) == KAASU )
      {
         cerr << P_ALKUPLANEETTAVIRHE_ << endl;
         
         return false;
      }
      
      //pelaajan tietojen tallennus
      Pelaaja apu( p_nimi, galaxi_.at(a_indeksi), p_num );
      ++p_num;
      
      p_osoitin = new Pelaaja; //varataan pelaajalle tilaa
      
      *p_osoitin = apu; //talletetaan pelaaja varattuun tilaan
      
      pelaajat_.push_back( p_osoitin );
      
      //p‰ivitet‰‰‰n aurinkokunnan osoittimet
      galaxi_.at(a_indeksi)->oleskelija( pelaajat_.back() );
      //Asuttava pelaaja saadaan oleskelijasta
      galaxi_.at(a_indeksi)->asuta( koti_p, true );
    
      
      tiedosto >> tagi;
      
      if( tagi != "</pelaaja>" )
      {
         cerr << SYNTAKSIVIRHE_ << endl;
         
         return false;
      }
      
      tiedosto >> tagi;
      
      ++pelaaja_tageja;
   }
   
   if( pelaaja_tageja != pelaajia_ )
   {
      cerr << P_MAARAVIRHE_ << endl;
      
      return false;
   }
   
   //Tarkistetaan lopputilanne ja pattitilanne ja merkit‰‰n ensimm‰inen pelaaja vuorossa olevaksi jos onnistuu.

   loppu = lopputilanne();
   
   if( !loppu )
   {
      vuoro_ = pelaajat_.at(0);
      
      loppu = pattitilanne();
   }
   
   return true;
}
