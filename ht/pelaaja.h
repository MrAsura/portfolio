// Pelaaja luokan header tiedosto

#ifndef HH_PELAAJA
#define HH_PELAAJA
#include <cstdlib>
#include <iostream>
#include <string>
#include <set>
#ifndef HH_AURINKOKUNTA
#include "aurinkokunta.h"
#endif

using namespace std;

class Aurinkokunta;

class Pelaaja
{
 public:
   Pelaaja();
   // alustaa pelaajan tiedot
   Pelaaja( string p_nimi, Aurinkokunta* sij, int p_num,
            int pist = 0 );
   ~Pelaaja();
   
   //Palauttaa pelaajan nimen
   string nimi()const;  
   
   //palauttaa pelaajan pisteet
   int pisteet()const;
   
   //kasvattaa pelaajan pisteit‰ annetulla m‰‰r‰ll‰
   void pisteet( int maara );
   
   //palauttaa p_num arvon
   int pNum()const;
   
   //Palauttaa sijainnin
   Aurinkokunta* sijainti()const;
 
   //Funktio: kayty
   //Parametri: Aurinkokunta* aurinkokunta: aurinkokunta jonka k‰yty status
   // tutkitaan
   //Toiminta:Palauttaa tiedon siit‰ onko pelaaja k‰ynyt parametrina
   // annetussa aurinkokunnassa
   //Paluuarvo:Palauttaa totuusarvon k‰ynti statuksesta.
   bool kayty( Aurinkokunta* aurinko );
   
   //Nollaa k‰ytyjen aurinkokuntien siirtokunnat, jotka pelaaja omistaa.
   void nollaaSiirtokunnat();

   //Funktio: liiku
   //Parametri: Aurinkokunta* kohde: aurinkokunnan nimi johon lennet‰‰n 
   //Toiminta: liikuttaa pelaaja toiseen aurinkokuntaan
   //HUOM! ei tarkista onko liikkuminen mahdollista eli se on tarkastettava 
   //erikseen
   void liiku( Aurinkokunta* kohde );
   
  private:  
   string nimi_;
   Aurinkokunta* sijainti_;
   int pisteet_;
   set<Aurinkokunta*> kayty_;
   int p_num_; //tarvitaan kun listataan pelaaja
};
#endif
