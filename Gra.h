#ifndef GRA_H
#define GRA_H
#include <cstdlib>
#include <ctime>
#include "Partia.h"

class Gra{
private:Szachownica szachownica;
        Komunikat komunikat;
//********************************************************************************
public: void start();
private:    void menu_gry();
                void uruchom_partie();
                    void menu_koloru_urzytkownika(bool &kolor);
                        int losowanie_koloru()noexcept;
                void menu_ustawien();
};
#endif//GRA_H
