#include "Gra.h"
using namespace std;
using namespace globalType;

void Gra::start(){//*3
//==============================================================================================================
    try{
        komunikat.ustawienie_wielkosci_obrazu();
        while(true)
            menu_gry();
//#########################################################################
    }
    catch(obslugiwany_blad &e){
        e.komunikat = __PRETTY_FUNCTION__ + string(" >>\n") + e.komunikat;
        throw;
    }
}
    void Gra::menu_gry(){//*2
//==============================================================================================================
    try{
        vector<string> menu_gry{"1. zagraj", "2. ustawienia kolorow", "3. wyjdz"};
        switch(komunikat.pole_wyboru(menu_gry))
        {
            case 1:  uruchom_partie(); break;
            case 2:  menu_ustawien();  break;
            case 3:  exit(0);
            default: throw invalid_argument("Blad wyboru opcji.");
        }
//#########################################################################
    }
    catch(const invalid_argument &e){
        obslugiwany_blad blad;
        blad.komunikat = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw blad;
    }
    catch(obslugiwany_blad &e){
        e.komunikat = __PRETTY_FUNCTION__ + string(" >>\n") + e.komunikat;
        throw;
    }
}
        void Gra::uruchom_partie(){//1
//==============================================================================================================
    try{
        bool kolor;
        menu_koloru_urzytkownika(kolor);
        Partia(kolor).zagraj_z_urzytkownikiem();
//#########################################################################
    }
    catch(obslugiwany_blad &e){
        e.komunikat = __PRETTY_FUNCTION__ + string(" >>\n") + e.komunikat;
        throw;
    }
}
            void Gra::menu_koloru_urzytkownika(bool &kolor){//0+
//==============================================================================================================
    try{
        vector<string> menu_koloru_urzytkownika{"1. kolorem bialym", "2. kolorem czarnym", "3. kolorem losowym"};
        switch(komunikat.pole_wyboru(menu_koloru_urzytkownika))
       {
           case 1:  kolor = 1;                  break;
           case 2:  kolor = 0;                  break;
           case 3:  kolor = losowanie_koloru(); break;
           default: throw invalid_argument("Blad wyboru opcji.");
       }
//#########################################################################
    }
    catch(const invalid_argument &e){
        obslugiwany_blad blad;
        blad.komunikat = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw blad;
    }
}
                int Gra::losowanie_koloru()noexcept{
//==============================================================================================================
    srand(static_cast<unsigned int>(time(nullptr)));
    return std::rand() % 2;
}
        void Gra::menu_ustawien(){//*0
//==============================================================================================================
    try{
        vector<string> menu_kolorowego_elementu{"1. kolor zaznaczonego pola", "2. kolor menu/komunikatow", "3. kolor wybieranej opcji w menu", "4. kolor notacji"};
        vector<string> menu_kolorow{"1. czerwony", "2. zielony", "3. zolty", "4. niebieski"};

        int wybrany_element = komunikat.pole_wyboru(menu_kolorowego_elementu);
        szachownica.usun_pole_wyboru(293, 53, false);

        SystemInfo::Kolor wybrany_kolor = static_cast<SystemInfo::Kolor>(komunikat.pole_wyboru(menu_kolorow));
        switch(wybrany_element)
       {
           case 1:  SystemInfo::zaznaczone_pole = wybrany_kolor; break;
           case 2:  SystemInfo::menu            = wybrany_kolor; break;
           case 3:  SystemInfo::wybierana_opcja = wybrany_kolor; break;
           case 4:  SystemInfo::notacja         = wybrany_kolor; break;
           default: throw invalid_argument("Blad wyboru opcji.");
       }
       szachownica.usun_pole_wyboru(205, 53, 0);
//#########################################################################
    }
    catch(const invalid_argument &e){
        obslugiwany_blad blad;
        blad.komunikat = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw blad;
    }
    catch(obslugiwany_blad &e){
        e.komunikat = __PRETTY_FUNCTION__ + string(" >>\n") + e.komunikat;
        throw;
    }
}
























