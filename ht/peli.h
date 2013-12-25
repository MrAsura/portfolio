//Header peli-luokalle
#ifndef HH_PELI
#define HH_PELI
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include "pelaaja.h"
#include "aurinkokunta.h"

using namespace std;

//Luovutus viesti
const string LUOVUTAVIESTI =
  " siirtyy nauttimaan lokoisista el‰kep‰ivist‰ Andromedan galaksiin.";

class Peli
{
 public:
   Peli();
   ~Peli();
   
   //Funktio: alusta
   //Parametrit: char* alustustiedosto: tiedoston nimi joka avataan.
   //            bool& loppu: palauttaa tiedon ollaanko jo lopputilanteessa
   //Paluuarvo: palauttaa totuusarvon onnistumisesta.
   //Toiminta: Lukee annetun alustustiedoston ja
   //asettaa annetut arvot tietorakenteisiin.
   bool alusta( const char* alustustiedosto, bool& loppu );

   //Funktio: listaa
   //Parametrit: string aurinkokunta: aurinkokunta jonka planeetat listataan.
   //Toiminta: Tulostaa aurinkokunnan tiedot jos pelalaaja on k‰ynyt
   //kyseisess‰ aurinkokunnassa
   void listaa( string aurinkokunta )const;
      
   //Funktio: seuraavaP
   //Toiminta: vaihtaa vuoro_:n seuraavaksi peliss‰ olevaksi pelaajaksi
   void seuraavaP();
   
   //Tarkistaa lopetus ehdot ja palauttaa true jos peli p‰‰ttyy
   bool lopputilanne()const;
   
   //Tarkistaa ollaanko pattitilanteessa ja palauttaa lopetetaanko peli
   bool pattitilanne();
   
   //hoitaa vuoroon vaihtamiseen liittyv‰t asiat
   bool seuraavaVuoro();
   
   //Funktio: kartta
   //Toiminta: tulostaa kartan galaxista ja listaa pelaajat
   void kartta()const;
   
   //palauttaa vuoro_ muuttuja
   Pelaaja* vuoro()const;

   //Funktio: pelaajat 
   //Toiminta: tulostaa pelaajat ja pisteet
   void pelaajat()const;

   //Funktio: luovuta
   //Parametrit: string viesti: viesti joka tulostetaan kun pelaaja poistetaan
   //Toiminta: vuorossa oleva pelaaja poistetaan pelist‰ ja vuoro siirtyy
   //seuraavalle
   //Paluuarvo: palauttaa lopputilanteen paluuarvon
   bool luovuta( string viesti = LUOVUTAVIESTI );
   
   //Funktio: matkaa
   //Parametri: string kohde: aurinkokunnan nimi johon lennet‰‰n
   //Toiminta: hoitaa matkaamiseen liittyv‰t menettelyt
   //Paluuarvo: kertoo onnistuuko matka.
   bool matkaa( string kohde );
   
   //Funktio: voitto
   //Toiminta: tulostaa voittajan/t
   void voitto()const;
   
 private:
   
   //Funktio: matkaTarkistus
   //Parametri: string kohde: aurinkokunnan nimi johon lennet‰‰n
   //           int& sijainti: aurinkokunnan indeksi galaxissa
   //Toiminta: hoitaa matkaamiseen liittyv‰t tarkistukset
   //Paluuarvo: kertoo onnistuuko matka.
   bool matkaTarkistus( string kohde, int& sijainti, bool tulosteet = true )
     const;
      
   //kartta funktion apu funktio joka tulostaa sille annetun rivi pohjan
   //mukaan kartan rivin sis‰llˆn ja pelaajan kartan riville.
   //Parametrina se saa kartan rivin pohjan ja rivin numeron
   void tulostaRivi( string r_pohja, int r_numero )const;
   
   int pelaajia_;  //pelaajia, p‰ivitet‰‰n jos pelaaja luovuttaa/h‰vi‰‰
   int koko_x_;   //galaxin koko
   int koko_y_;
   Pelaaja* vuoro_;
   
   vector<Pelaaja*> pelaajat_;  //peliss‰ olevat pelaajat ja niiden tiedot
   vector<Aurinkokunta*> galaxi_; //peliss‰ olevat aurinkokunnat ja tiedot

   //tarkistaa onko annettu aurinkokunta olemassa ja palauttaa sen sijainnin
   //jos ei ole palauttaa -1
   int onko( string aurinkokunta )const;
   
};

//virheiden tulosteita ja muita vakioita
//alustus:
const string LUKUVIRHE_ = 
  "Alustusvirhe: Alustustiedostoa ei ole tai sit‰ ei saa auki.";
const string SYNTAKSIVIRHE_ = 
  "Alustusvirhe: Virheellinen alustustiedosto.";

//Yleiset
const string Y_KOKOVIRHE_ = 
  "Alustusvirhe: Galaksin koko ei ole sallituissa rajoissa.";
const string Y_PELAAJAVIRHE_ = 
  "Alustusvirhe: Galaksin koko ei ole sallituissa rajoissa.";

//aurinkokunta
const string A_NIMIVIRHE_ = 
  "Alustusvirhe: Virheellinen aurinkokunnan nimi.";
const string A_SAMANIMIVIRHE_ = 
  "Alustusvirhe: Virheellinen aurinkokunnan nimi (sama).";
const string A_SIJAINTIVIRHE_ = 
  "Alustusvirhe: Virheelliset aurinkokunnan koordinaatit.";
const string A_KOKOVIRHE_ = 
  "Alustusvirhe: Aurinkokunnalla liikaa planeettoja.";
const string A_PTYYPPIVIRHE_ = 
  "Alustusvirhe: Virheellinen planeetta.";
const string A_KTYYPPIVIRHE_ = 
  "Alustusvirhe: Virheellinen kuu.";

//pelaaja
const string P_NIMIVIRHE_ = 
  "Alustusvirhe: Virheellinen pelaajan nimi.";
const string P_AURINKOKUNTAVIRHE_ =
  "Alustusvirhe: Virheellinen aurinkokunnan nimi (pelaaja).";
const string P_ALKUPAIKKAVIRHE_ = 
  "Alustusvirhe: Virheellinen kotiaurinkokunta.";
const string P_ALKUPLANEETTAVIRHE_ = 
  "Alustusvirhe: Virheellinen kotiplaneetta.";
const string P_MAARAVIRHE_ = 
  "Alustusvirhe: Virheellinen pelaajam‰‰r‰.";

//muita virheit‰
const string EI_OLE_A_VIRHE = "Virhe: Virheellinen aurinkokunnan nimi.";
const string VALIMATKAVIRHE = "Virhe: V‰limatka on liian pitk‰.";
const string MAHDUVIRHE =
  "Virhe: Yhteen aurinkokuntaan ei mahdu useampia laivoja samaan aikaan.";
const string PATTITILANNE = " j‰i jumiin ja putosi pelist‰ pois.";

#endif
