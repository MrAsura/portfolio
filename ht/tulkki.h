//headder tulkille

#ifndef TULKKI_HH
#define TULKKI_HH
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include "peli.h"

using namespace std;

//mahdolliset tilat
enum Nimi{ LISTAA, LOPETA, KARTTA, ASUTA, KUU, LIIKU, LUOVUTA, MUUNNA,
     PELAAJAT, TUNTEMATON, PARAMETRIVIRHE, MONISELITTEINEN, END };

//Parametrin tyyppi
enum Paratyyppi{LUKU, MERKKI_TAI_TYHJA};

//komennon pohja
struct KomentoTiedot
{
   string nimi;
   Nimi komento;
   Paratyyppi tyyppi;
};

const int KMAARA = 21; //Komentojen m‰‰r‰

//Tuetut komennot
const KomentoTiedot KOMENNOT[KMAARA] =
{
   { "asuta", ASUTA, LUKU },
   { "perusta", ASUTA, LUKU },
   { "kartta", KARTTA, MERKKI_TAI_TYHJA },
   { "galaksi", KARTTA, MERKKI_TAI_TYHJA },
   { "kuutukikohta", KUU, LUKU },
   { "avaruuslentotukikohta", KUU, LUKU },
   { "liiku", LIIKU, MERKKI_TAI_TYHJA },
   { "lenna", LIIKU, MERKKI_TAI_TYHJA },
   { "matkusta", LIIKU, MERKKI_TAI_TYHJA }, 
   { "tutki", LIIKU, MERKKI_TAI_TYHJA },
   { "luovuta", LUOVUTA, MERKKI_TAI_TYHJA },
   { "hanskat-tiskiin", LUOVUTA, MERKKI_TAI_TYHJA },
   { "muunna", MUUNNA, LUKU },
   { "tuunaa", MUUNNA, LUKU },
   { "pelaajat", PELAAJAT, MERKKI_TAI_TYHJA },
   { "aurinkokunta", LISTAA, MERKKI_TAI_TYHJA },
   { "planeetat", LISTAA, MERKKI_TAI_TYHJA },
   { "listaa", LISTAA, MERKKI_TAI_TYHJA },
   { "lopeta", LOPETA, MERKKI_TAI_TYHJA },
   { "quit", LOPETA, MERKKI_TAI_TYHJA },
   { "sommoro", LOPETA, MERKKI_TAI_TYHJA }
};

//Virheiden viestit
const string PARAVIRHE = "Virhe: Virheelliset parametrit.";
const string TUNTEMATON_KOM = "Virhe: Tuntematon komento.";
const string MONIVIRHE = "Virhe: Komento ei ole yksiselitteinen.";

//Lukee rivin kerrallaan ja sijoittaa komennon ja parametrit vektoriin
//ja palauttaa sen
vector<string> komentoLuku();

//tutkii parametri vektorin ja vertaa komentoihin ja tallettaa parametrit
//niit‰ vastaavaan muuttujaan. Palauttaa komennon enumin.
Nimi analysoi( vector<string>& tutkittava, int& luku_parametri,
               string& merkki_parametri );

//Aliohjelma joka suorittaa komentojen toiminnot eli varsinainen komentotulkki
void komentotulkki( bool& lopeta, Peli& moottori );

#endif
