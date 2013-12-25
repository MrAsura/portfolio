//Header aurinkokunta luokalle

#ifndef HH_AURINKOKUNTA
#define HH_AURINKOKUNTA

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <ostream>
#ifndef HH_PELAAJA
#include "pelaaja.h"
#endif

using namespace std;

enum Ptyyppi{ASUTTAVA,KAASU};

enum Kuu{ON,EI,TUKIKOHTA};

class Pelaaja;

struct Planeetta
{
   Ptyyppi tyyppi;
   Kuu tila;
   Pelaaja* siirtokunta; //Jos ei asutettu siirtokunta == NULL
};

class Aurinkokunta
{
 public:
   Aurinkokunta();
   
   //rakentaja joka asettaa heti alkuarvot
   Aurinkokunta( int x_koordinaatti, int y_koordinaatti, string a_nimi,
                 vector<Planeetta> planeetat );
   
   ~Aurinkokunta();
   
   //Funktio: asuta
   //Parametrit: int planeetta: monesko planeetta asutetaan
   //Toiminto: asuttaa parametrina annetun planeetan ja 
   //kasvattaa pelaaja pisteit‰ yhdell‰.
   //Paluuarvo: palauttaa false jos ep‰onnistuu
   bool asuta( int planeetta, bool alustus = false );
   
   //Funktio: muunna
   //Parametrit: int planeetta: monesko planeetta muunnetaan
   //Toiminto: muuntaa parametrina annetun planeetan ja
   //Paluuarvo: palauttaa false jos ep‰onnistuu
   bool muunna( int planeetta );
   
   //Funktio: kuutukukohta
   //Parametrit: int planeetta: monennenko planeettan kuuhun rakennetaan
   //Toiminto: rakentaa kuutukikohdan parametrina annetun planeetan kuuhun.
   //Paluuarvo: palauttaa false jos ep‰onnistuu
   bool kuutukikohta( int planeetta );
   
   //Funktio: pListaa
   //Toiminta: listaa aurinkokunnan planeetat
   void pListaa()const;
   
   //Palauttaa aurinkokunnan nimen
   string nimi()const;
   
   //Funktiot x- ja y-koordinaattien palautukseen
   int x()const;
   int y()const;
   
   //Palauttaa osoittimen aurinkokunnassa olevaan pelaajaan
   Pelaaja* oleskelija()const;
   
   //Muuttaa oleskelijan annetuksi ja jos uusi == oleskelija_,
   //nollataan oleskelija_ ( k‰ytet‰‰n kun l‰hdet‰‰n aurinkokunnasta)
   void oleskelija( Pelaaja* uusi );
   
   //Palauttaa planeettojen m‰‰r‰n
   int planeettoja()const;
   
   //Palauttaa planeetan tyypin jonka numero saadaan parametrin‰
   Ptyyppi pTyyppi( int planeetta )const;
   
   //Nollaa parametrina annetun pelaajaan siirtokunnat
   void tuhoaSiirtokunnat( Pelaaja* kohde );

   //laskee siirtokuntaien ja kuutukikohtien m‰‰r‰n jotka annettu pelaaja
   //omistaa
   int energia( Pelaaja* omistaja );

   //Kertoo onko kaikki aurinkokunnan planeetat asutettu
   //Jos parametri kuut on true tutkitaan myˆs onko kaikissa kuissa
   //tukikohta
   bool taysi( bool kuut = false )const;
   
 private:
   
   int x_;  //aurinkokunnan koordinaatit
   int y_;
   string nimi_;
   vector<Planeetta> planeetat_;
   Pelaaja* oleskelija_; 
};

//Virhe viestej‰
const string PLANEETTAVIRHE = "Virhe: Virheellinen planeetan numero.";
const string ASUTUSVIRHE = "Virhe: Kaasuplaneettaa ei voi asuttaa.";
const string SIIRTOKUNTAVIRHE = "Virhe: Planeetta on jo asutettu.";
//Kuuvirheviestej‰
const string EIOLEVIRHE =
  "Virhe: Planeetalla tulee olla siirtokunta ennen kuin sen kuulle voi rakentaa avaruuslentotukikohdan.";
const string VARATTUVIRHE =
  "Virhe: Planeetalla on vastapelaajan siirtokunta.";
const string EIKUUTAVIRHE = "Virhe: Planeetalla ei ole kuuta.";
const string ONJOVIRHE = "Virhe: Kuussa on jo lentotukikohta.";
//Muuntovirhe
const string EIKAASUVIRHE = "Virhe: Planeetta ei ole kaasuplaneetta.";

#endif
