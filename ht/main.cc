// P‰‰ohjelma

#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <fstream>
#include <windows.h>
#include "peli.h"
#include "tulkki.h"

using namespace std;

int main( int argc, char* argv[] )
{
   cout << "### OHJ-1160 Laaja ohjelmointi 2" << endl
     << "### Jaakko Laitinen" << endl;
   
   Peli moottori;
   bool lopeta = false;
   string init = _pgmptr;

   
   // Virheilmoitus annetaan j‰senfunktiossa
   if( !moottori.alusta( init.replace( init.end()-6, init.end(), "alustus.alustus").c_str(), lopeta ) )
   {
	   system("pause");
       return EXIT_FAILURE;
   }

   /*
   //tutkitaan parametrien oikeellisuus
   if( argc == 3 && argv[2] != 0 )
   {
      if( argv[1][0] == '-' && argv[1][1] == 'a' )
      {                 
          // Virheilmoitus annetaan j‰senfunktiossa
         if( !moottori.alusta( argv[2], lopeta ) )
         {
            return EXIT_FAILURE;
         }
         //moottori.tulosta();
      }
      else
      {
         cerr << "Virhe: Virheelliset komentoriviparametrit." << endl;
         return EXIT_FAILURE;
      }      
   }
   else
   {
      cerr << "Virhe: Virheelliset komentoriviparametrit." << endl;
      return EXIT_FAILURE;
   }
   */
   //Komentotulkki osuus alkaa t‰st‰
   //Ekaksi tallennetaan luettelo pelaajista ja muuta tietoa
   
   while( !lopeta && !cin.eof() )
   {
      cout << moottori.vuoro()->nimi() << "> ";
      
      komentotulkki( lopeta, moottori );
   }
      
   system("pause");
   return EXIT_SUCCESS;
}
