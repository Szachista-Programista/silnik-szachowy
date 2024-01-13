#ifndef GRA_H
#define GRA_H
#include "Partia.h"




class Gra{
public:

    Komunikat komunikat;

    void start();
        void menu_gry();
    void uruchom_partie();
        void menu_koloru(bool &kolor);
        int losowanie_koloru();




};






#endif
