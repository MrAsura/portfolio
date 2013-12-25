<Peliohjeet>

-Tavoite-
Pelin tavoite on on ker‰t‰ mahdollisimman paljon pisteit‰ asuttamalla planeettoja eri aurinkokunnissa.
Jokainen pelaaja toi suorittaa -Komennot- kohdassa kuvattuja toimintoja vuorollaan.
Peli p‰‰ttyy kun pelaajilla ei ole en‰‰n suoritettavia toimintoja.

-Toimintoja-
Pelaaja voi vuorollaan asuttaa asutettavan planeetan (jos planeetta on kaasuplaneetta tulee se ensiksi muuntaa asutettavaksi), rakentaa kuutukikohdan itse asuttamansa planeetan kuuhun, jos planeetalla on kuu, tai siirty‰ johonkin toiseen aurinkokuntaan. Vain yksi pelaaja voi olla aurinkokunnassa kerrallaan. Siirtymiset‰isyyten vaikuttaa sen hetkisess‰ aurinkokunnassa olevat pelaajan asuttamat planeetat ja kuutukikohdat. Pelaaja n‰kee vain vierailemiensa aurinkokuntien tiedot. Kun aurinkokunna kaikki planeetat on asutettu, siirtokuntien omistus menee sille kenell‰ on eniten siirtokuntia kyseisess‰ aurinkokunnassa (tasapeliss‰ ei tapahdu mit‰‰n).


-Komennot-

asuta <int> //asuttaa parametrina annetun asutettavan planeetan 

kartta //tulostaa kartan galaxista. Ei kuluta vuoroa.

kuutukikohta <int> //rakentaa tukikohdan parametrina annetun asutetun planeetan kuuhun

liiku <aurinkokunnan nimi> //Liikkuu annettuun aurinkokuntaan mik‰li mahdollista

luovuta //vuorossa oleva pelaaja poistetaan pelist‰

muunna <int> // muuntaa sen hetkisen aurinkokunnan kaasuplaneetan asutettavaksi jonka j‰rjestysnumero annetaan parametrina.

pelaajat  //tulostaa listan pelaajista ja niiden pisteist‰. Ei kuluta vuoroa

aurinkokunta <aurinkokunnan nimi> //komento listaa parametrina annetun aurinkokunnan planeetat. Ilman parametria tulostetetaan sen hetkisen aurinkokunnan planeetat. Ei kuluta vuoroa

lopeta //Lopettaa pelin

(kommennoista tarvitsee kirjoittaa vain sen verran ett‰ ne ymm‰rt‰‰ yksik‰sitteisesti)


-Huomioita-

Pelin karttaa ja pelaajia voi muuttaa muokkaamalla 'alustus.alustus' nimist‰ tiedostoa.

/*
yleist‰ osio sis‰lt‰‰ kartan koon ja pelaajien m‰‰r‰n
Aurinkokunta sis‰lt‰‰ yksik‰sitteisen nimen, koordinaatit planeettojen m‰‰r‰n ja niiden tyypin (A/K K/E) eli kaasu vai asutettava ja kuu vai ei
pelaaja sis‰lt‰‰ nimen aloitus aurinkkokunnan ja planeetan numeron( aloitusplaneetta oltava sutettava).
*/