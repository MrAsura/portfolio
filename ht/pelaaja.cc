// Toteutus luokan pelaaja funktioille

#include <set>
#include <cstdlib>
#include <iostream>
#include <string>
#include "pelaaja.h"

using namespace std;


//toteutus Pelaaja-luokan oletus rakentajalle
Pelaaja::Pelaaja(): nimi_("/0"),sijainti_(0),pisteet_(0),p_num_(0)
{
}

//toteutus Pelaaja-luokan rakentajalle
Pelaaja::Pelaaja( string p_nimi, Aurinkokunta* sij, int p_num,
                  int pist ):
nimi_( p_nimi ),sijainti_( sij ),pisteet_(pist),p_num_(p_num)
{
   kayty_.insert(sij);
}

//Purkaja pelaaja luokalle
Pelaaja::~Pelaaja()
{
}

//Toteutus pNum funktiolle
int Pelaaja::pNum()const
{
   return p_num_;
}

//Toteutus funktiolle nimi
string Pelaaja::nimi()const
{  
   return nimi_;
}

//toteutus pisteet funktiolle
int Pelaaja::pisteet()const
{
   return pisteet_;
}

//toteutus pisteet funktiolle
void Pelaaja::pisteet( int maara )
{
   pisteet_ += maara;
   return;
}

//toteutus sijainti funktiolle
Aurinkokunta* Pelaaja::sijainti()const
{
   return sijainti_;
}

//toteutus kayty funktiolle
bool Pelaaja::kayty( Aurinkokunta* aurinko )
{
   set<Aurinkokunta*>::iterator it;
   
   it = kayty_.find(aurinko);

   // it on kayty_end() vain jos aurinkoa ei löytytyt
   if( it == kayty_.end() )
   {
      return false;
   }
       
   return true;
}

//toteutus nollaasiirtokunnat funktiolle
void Pelaaja::nollaaSiirtokunnat()
{
   set<Aurinkokunta*>::iterator it;
   it = kayty_.begin();
   
   for( unsigned int i = 0 ; i < kayty_.size() ; ++i )
   {
      (*it)->tuhoaSiirtokunnat( this );
   }
   
   return;
}

//toteutus liiku funktiolle
void Pelaaja::liiku( Aurinkokunta* kohde )
{
   cout << nimi_ << " lensi aurinkokuntaan nimeltä "
     << kohde->nimi() << "." << endl;
   
   //nollataan aurinkokunnan, josta lähdettiin, oleskelija
   sijainti_->oleskelija( this );
   
   sijainti_ = kohde;
   
   kohde->oleskelija( this );
      
   kayty_.insert( kohde );
   
   return;
}
