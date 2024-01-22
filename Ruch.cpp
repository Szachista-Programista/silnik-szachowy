#include "Ruch.h"
using namespace std;
using namespace globalType;

Ruch::Ruch(bool k)noexcept: kolor{k}{
//==============================================================================================================
    pokolenie_klasy = 0;

    polozenie_mojego_krola_x       = kolor?3:4;
    polozenie_mojego_krola_y       =         7;
    polozenie_krola_urzytkownika_x = kolor?3:4;
    polozenie_krola_urzytkownika_y =         0;

    czy_moj_krol_sie_ruszyl                   = false;
    czy_moja_lewa_wierza_sie_ruszyla          = false;
    czy_moja_prawa_wierza_sie_ruszyla         = false;
    czy_krol_urzytkownika_sie_ruszyl          = false;
    czy_lewa_wierza_urzytkownika_sie_ruszyla  = false;
    czy_prawa_wierza_urzytkownika_sie_ruszyla = false;
}
Ruch::~Ruch()noexcept{
//==============================================================================================================
    wyzeruj_wektor_na_ruchy();
}
void Ruch::przygotuj_Ruch()noexcept{
//==============================================================================================================
    statusy_materialne_rozpatrywanych_rochow.clear();
    numer_mojego_ruchu       = -1 ;
    index_najgorszego_ruchu  =  0 ;
    najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow = 100.0;
}
double Ruch::oblicz_nastepny_ruch(T_wsk_szachownica &wsk_X){//7
//==============================================================================================================
    try{
        if(wsk_X == nullptr)
            throw invalid_argument("Zerowy adres szachownicy.");
        zweryfikuj_polozenia_krolow(wsk_X);
        sprawdz_czy_wieze_i_krole_byly_ruszane(wsk_X);
        sprawdz_moj_krol_jest_szachowany(wsk_X);
        wykonaj_moj_ruch(wsk_X);
        return znajdz_najleprze_z_posuniec(wsk_X);
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
    void Ruch::zweryfikuj_polozenia_krolow(T_wsk_szachownica wsk_X){//0+
//==============================================================================================================
    try{
        if(wsk_X == nullptr)
            throw invalid_argument("Zerowy adres szachownicy.");
        for(int i=7; i>=0; i--)
            for(int j=0; j<8; j++)
                if(wsk_X[i][j]=='K')
                {
                    polozenie_mojego_krola_x=j;
                    polozenie_mojego_krola_y=i;
                    break;
                }
        for(int i=0; i<8; i++)
            for(int j=0; j<8; j++)
                if(wsk_X[i][j]=='k')
                {
                    polozenie_krola_urzytkownika_x=j;
                    polozenie_krola_urzytkownika_y=i;
                    break;
                }
//#########################################################################
    }
    catch(const invalid_argument &e){
        obslugiwany_blad blad;
        blad.komunikat = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw blad;
    }
}
    void Ruch::sprawdz_czy_wieze_i_krole_byly_ruszane(T_wsk_szachownica wsk_X){//0+
//==============================================================================================================
    try{
        if (wsk_X == nullptr)
            throw invalid_argument("Zerowy adres szachownicy.");
        if(wsk_X[7][0] != 'W') czy_moja_lewa_wierza_sie_ruszyla  = true;
        if(wsk_X[7][7] != 'W') czy_moja_prawa_wierza_sie_ruszyla = true;
        if(wsk_X[0][0] != 'w') czy_lewa_wierza_urzytkownika_sie_ruszyla  = true;
        if(wsk_X[0][7] != 'w') czy_prawa_wierza_urzytkownika_sie_ruszyla = true;
        if(kolor? wsk_X[7][3] != 'K': wsk_X[7][4] != 'K') czy_moj_krol_sie_ruszyl          = true;
        if(kolor? wsk_X[0][3] != 'k': wsk_X[0][4] != 'k') czy_krol_urzytkownika_sie_ruszyl = true;
//#########################################################################
    }
    catch(const invalid_argument &e){
        obslugiwany_blad blad;
        blad.komunikat = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw blad;
    }
}
    void Ruch::sprawdz_moj_krol_jest_szachowany(T_wsk_szachownica wsk_X){//1
//==============================================================================================================
    try{
        czy_moj_krol_jest_szachowany = czy_moje_pole_jest_bite(polozenie_mojego_krola_x, polozenie_mojego_krola_y, wsk_X);
//#########################################################################
    }
    catch(obslugiwany_blad &e){
        e.komunikat = __PRETTY_FUNCTION__ + string(" >>\n") + e.komunikat;
        throw;
    }
}
    double Ruch::znajdz_najleprze_z_posuniec(T_wsk_szachownica &wsk_X){//r
//==============================================================================================================
    try{
        if(pokolenie_klasy < ostatnie_pokolenie)  return obsluga_poczatku_drzewa_poszukowan(wsk_X);
        if(pokolenie_klasy == ostatnie_pokolenie) return obsluga_konca_drzewa_poszukowan();
//#########################################################################
    }
    catch(obslugiwany_blad &e){
        e.komunikat = __PRETTY_FUNCTION__ + string(" >>\n") + e.komunikat;
        throw;
    }
}
        double Ruch::obsluga_poczatku_drzewa_poszukowan(T_wsk_szachownica &wsk_X){//r
//==============================================================================================================
    try{
        double najleprzy_z_najgorszych_status_materialny = -100;
        int index_najleprzego_z_najgorszych_ruchow;
        for(int i=0; i<wektor_na_ruchy.size() && i<ile_ruchow_rozpatrywac[pokolenie_klasy]; i++)
        {
            double najgorszy_status_malerialny_po_ruchu_urzytkownika = 100,
                   bierzacy_status_malerialny_urzytkownika;
            for(int j=1; j<wektor_na_ruchy[i].size() && j<=ile_ruchow_urzytkownika_rozpatrywac[pokolenie_klasy]; j++)
            {
                Ruch(this, bierzacy_status_malerialny_urzytkownika, wektor_na_ruchy[i][j], pokolenie_klasy + 1);
                if(bierzacy_status_malerialny_urzytkownika < najgorszy_status_malerialny_po_ruchu_urzytkownika)
                    najgorszy_status_malerialny_po_ruchu_urzytkownika = bierzacy_status_malerialny_urzytkownika;
            }
            if(najleprzy_z_najgorszych_status_materialny < najgorszy_status_malerialny_po_ruchu_urzytkownika)
            {
                najleprzy_z_najgorszych_status_materialny = najgorszy_status_malerialny_po_ruchu_urzytkownika;
                index_najleprzego_z_najgorszych_ruchow = i;
            }
        }
        obsluga_konca_poszukowan(wsk_X, index_najleprzego_z_najgorszych_ruchow);
        wyzeruj_wektor_na_ruchy();
        return najleprzy_z_najgorszych_status_materialny;
//#########################################################################
    }
    catch(obslugiwany_blad &e){
        e.komunikat = __PRETTY_FUNCTION__ + string(" >>\n") + e.komunikat;
        throw;
    }
}
            void Ruch::obsluga_konca_poszukowan(T_wsk_szachownica &wsk_X, int &index){//2
//==============================================================================================================
    try{
        if(pokolenie_klasy == 0)
        {
            czy_urzytkownik_zakonczyl_gre(wsk_X);
            delete[]wsk_X;
            wsk_X=nullptr;
            czy_maszyna_zakonczyla_gre(wsk_X, index);
            if(wsk_X == nullptr && wektor_na_ruchy.size()!=0)
                wsk_X = skopiuj_szachownice(wektor_na_ruchy[index][0]);
        }
//#########################################################################
    }
    catch(obslugiwany_blad &e){
        e.komunikat = __PRETTY_FUNCTION__ + string(" >>\n") + e.komunikat;
        throw;
    }
}
                void Ruch::czy_urzytkownik_zakonczyl_gre(T_wsk_szachownica &wsk_X){//1
//==============================================================================================================
    try{
        if(wektor_na_ruchy.size()==0)
        {
            if(czy_moje_pole_jest_bite(polozenie_mojego_krola_x, polozenie_mojego_krola_y, wsk_X))
                koniec_gry_wygrana_urzytkownika = true;
            else
                koniec_gry_pat_urzytkownika     = true;
        }
//#########################################################################
    }
    catch(obslugiwany_blad &e){
        e.komunikat = __PRETTY_FUNCTION__ + string(" >>\n") + e.komunikat;
        throw;
    }
}
                void Ruch::czy_maszyna_zakonczyla_gre(T_wsk_szachownica &wsk_X, int &index){//1
//==============================================================================================================
    try{
        for(int i=0, j=0; i<wektor_na_ruchy.size(); i++)
        {
            if(wektor_na_ruchy[i].size()==1) // czy maszyna zamatowala urzytkowanika albo wywolala pata?
            {
                j++;
                if(index == i)
                    for(int k=0; k<wektor_na_ruchy.size(); k++)
                        if(wektor_na_ruchy[k].size() > 1 )
                            index = k;
                if(czy_pole_urzytkownika_jest_bite(polozenie_krola_urzytkownika_x, polozenie_krola_urzytkownika_y, wektor_na_ruchy[i][0]))
                {
                    wsk_X = skopiuj_szachownice(wektor_na_ruchy[i][0]);
                    koniec_gry_wygrana_maszyny = true;
                    break;
                }
                else if(j == wektor_na_ruchy.size())
                {
                    wsk_X = skopiuj_szachownice(wektor_na_ruchy[i][0]);
                    koniec_gry_pat_maszyny     = true;
                    break;
                }
            }
        }
//#########################################################################
    }
    catch(obslugiwany_blad &e){
        e.komunikat = __PRETTY_FUNCTION__ + string(" >>\n") + e.komunikat;
        throw;
    }
}
        double Ruch::obsluga_konca_drzewa_poszukowan()noexcept{
//==============================================================================================================
    double status_materialny = -100;
    for(auto element: statusy_materialne_rozpatrywanych_rochow)
        if(status_materialny < element)
            status_materialny = element;
    wyzeruj_wektor_na_ruchy();
    return status_materialny;
}
            void Ruch::wyzeruj_wektor_na_ruchy()noexcept{
//==============================================================================================================
    for(auto wektorek: wektor_na_ruchy)
    {
        for(auto element: wektorek)
            delete[]element;
    }
    wektor_na_ruchy.clear();
}
        Ruch::Ruch(Ruch* wskaznik, double &status_materialny, T_wsk_szachownica wsk_X, int iteracja): pokolenie_klasy {iteracja}{//r
//==============================================================================================================
    try{
        przepisz_dane_o_ruchach_krolow_i_wiez(wskaznik);
        status_materialny = oblicz_nastepny_ruch(wsk_X);
//#########################################################################
    }
    catch(obslugiwany_blad &e){
        e.komunikat = __PRETTY_FUNCTION__ + string(" >>\n") + e.komunikat;
        throw;
    }
}
            void Ruch::przepisz_dane_o_ruchach_krolow_i_wiez(Ruch* wsk){//0+
//==============================================================================================================
    try{
        if (wsk == nullptr)
            throw invalid_argument("wsk == nullptr.");
        czy_moj_krol_sie_ruszyl                  = wsk->czy_moj_krol_sie_ruszyl;
        czy_moja_lewa_wierza_sie_ruszyla         = wsk->czy_moja_lewa_wierza_sie_ruszyla;
        czy_moja_prawa_wierza_sie_ruszyla        = wsk->czy_moja_prawa_wierza_sie_ruszyla;
        czy_krol_urzytkownika_sie_ruszyl          = wsk->czy_krol_urzytkownika_sie_ruszyl;
        czy_lewa_wierza_urzytkownika_sie_ruszyla  = wsk->czy_lewa_wierza_urzytkownika_sie_ruszyla;
        czy_prawa_wierza_urzytkownika_sie_ruszyla = wsk->czy_prawa_wierza_urzytkownika_sie_ruszyla;
//#########################################################################
    }
    catch(const invalid_argument &e){
        obslugiwany_blad blad;
        blad.komunikat = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw blad;
    }
}
void Ruch::wykonaj_moj_ruch(T_wsk_szachownica wsk_X){//6
//==============================================================================================================
    try{
        if (wsk_X == nullptr)
            throw invalid_argument("Zerowy adres szachownicy.");
        vector<int>iteratory = {3, 4, 2, 5, 1, 6, 0, 7};
        for(int i=0; i<8; i++)
            for (int j: iteratory)
            {
                switch(wsk_X[i][j])
                {
                    case ' ': break;
                    case 'P':                                                       //KWESTIA PIONKA
                    {
                        if(1<i && i<7)                                                  //PIONEK BEZ SZANSY NA PROMOCJE
                        {
                            if(wsk_X[i-1][j]==' ')                                   obsluga_ruchu_maszyny(j,i,j,i-1,'P',wsk_X);    //POSUNIECIE O 1 W PRZOD
                            if(i==6 && wsk_X[5][j]==' ' && wsk_X[4][j]==' ')         obsluga_ruchu_maszyny(j,i,j,4,'P',wsk_X);      //POSUNIECIE O 2 W PRZOD
                            if(j!=0 && 'g'<=wsk_X[i-1][j-1] && wsk_X[i-1][j-1]<='w') obsluga_ruchu_maszyny(j,i,j-1,i-1,'P',wsk_X);  //BICIE W LEWO
                            if(j!=7 && 'g'<=wsk_X[i-1][j+1] && wsk_X[i-1][j+1]<='w') obsluga_ruchu_maszyny(j,i,j+1,i-1,'P',wsk_X);  //BICIE W PRAWO
                        }
                        if(i==1)                                                       //PROMOCJA
                        {
                            if(wsk_X[0][j]==' ')                                                //PROMOCJA PO PROSTU
                            {
                                obsluga_ruchu_maszyny(j,1,j,0,'S',wsk_X);//Z UTWORZENIEM SKOCZKA
                                obsluga_ruchu_maszyny(j,1,j,0,'G',wsk_X);//Z UTWORZENIEM GONCA
                                obsluga_ruchu_maszyny(j,1,j,0,'W',wsk_X);//Z UTWORZENIEM WIEZY
                                obsluga_ruchu_maszyny(j,1,j,0,'H',wsk_X);//Z UTWORZENIEM HETMANA
                            }
                            if(j!=0 && 'g'<=wsk_X[0][j-1] && wsk_X[0][j-1]<='w')                //PROMOCJA W LEWO
                            {
                                obsluga_ruchu_maszyny(j,1,j-1,0,'S',wsk_X);//Z UTWORZENIEM SKOCZKA
                                obsluga_ruchu_maszyny(j,1,j-1,0,'G',wsk_X);//Z UTWORZENIEM GONCA
                                obsluga_ruchu_maszyny(j,1,j-1,0,'W',wsk_X);//Z UTWORZENIEM WIEZY
                                obsluga_ruchu_maszyny(j,1,j-1,0,'H',wsk_X);//Z UTWORZENIEM HETMANA
                            }
                            if(j!=7 && 'g'<=wsk_X[0][j+1] && wsk_X[0][j+1]<='w')                //PROMOCJA W PRAWO
                            {
                                obsluga_ruchu_maszyny(j,1,j+1,0,'S',wsk_X);//Z UTWORZENIEM SKOCZKA
                                obsluga_ruchu_maszyny(j,1,j+1,0,'G',wsk_X);//Z UTWORZENIEM GONCA
                                obsluga_ruchu_maszyny(j,1,j+1,0,'W',wsk_X);//Z UTWORZENIEM WIEZY
                                obsluga_ruchu_maszyny(j,1,j+1,0,'H',wsk_X);//Z UTWORZENIEM HETMANA
                            }
                        }
                        break;
                    }
                    case 'S':                                                       //KWESTIA SKOCZKA
                    {
                        if(0<=i-2 && j+1<=7 && (wsk_X[i-2][j+1]<'G' || 'W'<wsk_X[i-2][j+1])) obsluga_ruchu_maszyny(j,i,j+1,i-2,'S',wsk_X);
                        if(0<=i-1 && j+2<=7 && (wsk_X[i-1][j+2]<'G' || 'W'<wsk_X[i-1][j+2])) obsluga_ruchu_maszyny(j,i,j+2,i-1,'S',wsk_X);
                        if(i+1<=7 && j+2<=7 && (wsk_X[i+1][j+2]<'G' || 'W'<wsk_X[i+1][j+2])) obsluga_ruchu_maszyny(j,i,j+2,i+1,'S',wsk_X);
                        if(i+2<=7 && j+1<=7 && (wsk_X[i+2][j+1]<'G' || 'W'<wsk_X[i+2][j+1])) obsluga_ruchu_maszyny(j,i,j+1,i+2,'S',wsk_X);
                        if(i+2<=7 && 0<=j-1 && (wsk_X[i+2][j-1]<'G' || 'W'<wsk_X[i+2][j-1])) obsluga_ruchu_maszyny(j,i,j-1,i+2,'S',wsk_X);
                        if(i+1<=7 && 0<=j-2 && (wsk_X[i+1][j-2]<'G' || 'W'<wsk_X[i+1][j-2])) obsluga_ruchu_maszyny(j,i,j-2,i+1,'S',wsk_X);
                        if(0<=i-1 && 0<=j-2 && (wsk_X[i-1][j-2]<'G' || 'W'<wsk_X[i-1][j-2])) obsluga_ruchu_maszyny(j,i,j-2,i-1,'S',wsk_X);
                        if(0<=i-2 && 0<=j-1 && (wsk_X[i-2][j-1]<'G' || 'W'<wsk_X[i-2][j-1])) obsluga_ruchu_maszyny(j,i,j-1,i-2,'S',wsk_X);
                        break;
                    }
                    case 'G':                                                       //KWESTIA GONCA
                    {
                        for(int y=i-1, x=j+1; 0<=y && x<=7; y--, x++)                   //RUCH NA 1:30
                        {
                            if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W')                                     break;
                            if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w') {obsluga_ruchu_maszyny(j,i,x,y,'G',wsk_X);  break;}
                            if(wsk_X[y][x]==' ')                      obsluga_ruchu_maszyny(j,i,x,y,'G',wsk_X);
                        }
                        for(int y=i+1, x=j+1; y<=7 && x<=7; y++, x++)                   //RUCH NA 4:30
                        {
                            if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W')                                     break;
                            if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w') {obsluga_ruchu_maszyny(j,i,x,y,'G',wsk_X);  break;}
                            if(wsk_X[y][x]==' ')                      obsluga_ruchu_maszyny(j,i,x,y,'G',wsk_X);
                        }
                        for(int y=i+1, x=j-1; y<=7 && 0<=x; y++, x--)                   //RUCH NA 7:30
                        {
                            if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W')                                     break;
                            if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w') {obsluga_ruchu_maszyny(j,i,x,y,'G',wsk_X);  break;}
                            if(wsk_X[y][x]==' ')                      obsluga_ruchu_maszyny(j,i,x,y,'G',wsk_X);
                        }
                        for(int y=i-1, x=j-1; 0<=y && 0<=x; y--, x--)                   //RUCH NA 10:30
                        {
                            if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W')                                     break;
                            if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w') {obsluga_ruchu_maszyny(j,i,x,y,'G',wsk_X);  break;}
                            if(wsk_X[y][x]==' ')                      obsluga_ruchu_maszyny(j,i,x,y,'G',wsk_X);
                        }
                        break;
                    }
                    case 'W':                                                       //KWESTIA WIEZY
                    {
                        for(int y=i-1; 0<=y; y--)                                       //RUCH NA 12:00
                        {
                            if('G'<=wsk_X[y][j] && wsk_X[y][j]<='W')                                     break;
                            if('g'<=wsk_X[y][j] && wsk_X[y][j]<='w') {obsluga_ruchu_maszyny(j,i,j,y,'W',wsk_X);  break;}
                            if(wsk_X[y][j]==' ')                      obsluga_ruchu_maszyny(j,i,j,y,'W',wsk_X);
                        }
                        for(int x=j+1; x<=7; x++)                                       //RUCH NA 3:00
                        {
                            if('G'<=wsk_X[i][x] && wsk_X[i][x]<='W')                                     break;
                            if('g'<=wsk_X[i][x] && wsk_X[i][x]<='w') {obsluga_ruchu_maszyny(j,i,x,i,'W',wsk_X);  break;}
                            if(wsk_X[i][x]==' ')                      obsluga_ruchu_maszyny(j,i,x,i,'W',wsk_X);
                        }
                        for(int y=i+1; y<=7; y++)                                       //RUCH NA 6:00
                        {
                            if('G'<=wsk_X[y][j] && wsk_X[y][j]<='W')                                     break;
                            if('g'<=wsk_X[y][j] && wsk_X[y][j]<='w') {obsluga_ruchu_maszyny(j,i,j,y,'W',wsk_X);  break;}
                            if(wsk_X[y][j]==' ')                      obsluga_ruchu_maszyny(j,i,j,y,'W',wsk_X);
                        }
                        for(int x=j-1; 0<=x; x--)                                       //RUCH NA 9:00
                        {
                            if('G'<=wsk_X[i][x] && wsk_X[i][x]<='W')                                     break;
                            if('g'<=wsk_X[i][x] && wsk_X[i][x]<='w') {obsluga_ruchu_maszyny(j,i,x,i,'W',wsk_X);  break;}
                            if(wsk_X[i][x]==' ')                      obsluga_ruchu_maszyny(j,i,x,i,'W',wsk_X);
                        }
                        break;
                    }
                    case 'H':                                                       //KWESTIA HETMANA
                    {
                        for(int y=i-1; 0<=y; y--)                                       //RUCH NA 12:00
                        {
                            if('G'<=wsk_X[y][j] && wsk_X[y][j]<='W')                                    break;
                            if('g'<=wsk_X[y][j] && wsk_X[y][j]<='w'){obsluga_ruchu_maszyny(j,i,j,y,'H',wsk_X);  break;}
                            if(wsk_X[y][j]==' ')                     obsluga_ruchu_maszyny(j,i,j,y,'H',wsk_X);
                        }
                        for(int y=i-1, x=j+1; 0<=y && x<=7; y--, x++)                   //RUCH NA 1:30
                        {
                            if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W')                                    break;
                            if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w'){obsluga_ruchu_maszyny(j,i,x,y,'H',wsk_X);  break;}
                            if(wsk_X[y][x]==' ')                     obsluga_ruchu_maszyny(j,i,x,y,'H',wsk_X);
                        }
                        for(int x=j+1; x<=7; x++)                                       //RUCH NA 3:00
                        {
                            if('G'<=wsk_X[i][x] && wsk_X[i][x]<='W')                                    break;
                            if('g'<=wsk_X[i][x] && wsk_X[i][x]<='w'){obsluga_ruchu_maszyny(j,i,x,i,'H',wsk_X);  break;}
                            if(wsk_X[i][x]==' ')                     obsluga_ruchu_maszyny(j,i,x,i,'H',wsk_X);
                        }
                        for(int y=i+1, x=j+1; y<=7 && x<=7; y++, x++)                   //RUCH NA 4:30
                        {
                            if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W')                                    break;
                            if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w'){obsluga_ruchu_maszyny(j,i,x,y,'H',wsk_X);  break;}
                            if(wsk_X[y][x]==' ')                     obsluga_ruchu_maszyny(j,i,x,y,'H',wsk_X);
                        }
                        for(int y=i+1; y<=7; y++)                                       //RUCH NA 6:00
                        {
                            if('G'<=wsk_X[y][j] && wsk_X[y][j]<='W')                                    break;
                            if('g'<=wsk_X[y][j] && wsk_X[y][j]<='w'){obsluga_ruchu_maszyny(j,i,j,y,'H',wsk_X);  break;}
                            if(wsk_X[y][j]==' ')                     obsluga_ruchu_maszyny(j,i,j,y,'H',wsk_X);
                        }
                        for(int y=i+1, x=j-1; y<=7 && 0<=x; y++, x--)                   //RUCH NA 7:30
                        {
                            if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W')                                    break;
                            if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w'){obsluga_ruchu_maszyny(j,i,x,y,'H',wsk_X);  break;}
                            if(wsk_X[y][x]==' ')                     obsluga_ruchu_maszyny(j,i,x,y,'H',wsk_X);
                        }
                        for(int x=j-1; 0<=x; x--)                                       //RUCH NA 9:00
                        {
                            if('G'<=wsk_X[i][x] && wsk_X[i][x]<='W')                                    break;
                            if('g'<=wsk_X[i][x] && wsk_X[i][x]<='w'){obsluga_ruchu_maszyny(j,i,x,i,'H',wsk_X);  break;}
                            if(wsk_X[i][x]==' ')                     obsluga_ruchu_maszyny(j,i,x,i,'H',wsk_X);
                        }
                        for(int y=i-1, x=j-1; 0<=y && 0<=x; y--, x--)                   //RUCH NA 10:30
                        {
                            if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W')                                    break;
                            if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w'){obsluga_ruchu_maszyny(j,i,x,y,'H',wsk_X);  break;}
                            if(wsk_X[y][x]==' ')                     obsluga_ruchu_maszyny(j,i,x,y,'H',wsk_X);
                        }
                        break;
                    }
                    case 'K':                                                       //KWESTIA KROLA
                    {
                        if(0<=i-1 && (wsk_X[i-1][j]<'G' || 'W'<wsk_X[i-1][j]))                    //RUCH NA 12:00
                        if(2<=abs(polozenie_mojego_krola_x-polozenie_krola_urzytkownika_x) || 2<=abs(polozenie_mojego_krola_y-1-polozenie_krola_urzytkownika_y)) //czy krol do krola sie nie przytula
                        {
                            polozenie_mojego_krola_y--;
                            obsluga_ruchu_maszyny(j,i,j,i-1,'K',wsk_X);
                            polozenie_mojego_krola_y++;
                        }
                        if(0<=i-1 && j+1<=7 && (wsk_X[i-1][j+1]<'G' || 'W'<wsk_X[i-1][j+1]))      //RUCH NA 1:30
                        if(2<=abs(polozenie_mojego_krola_x+1-polozenie_krola_urzytkownika_x) || 2<=abs(polozenie_mojego_krola_y-1-polozenie_krola_urzytkownika_y)) //czy krol do krola sie nie przytula
                        {
                            polozenie_mojego_krola_x++;
                            polozenie_mojego_krola_y--;
                            obsluga_ruchu_maszyny(j,i,j+1,i-1,'K',wsk_X);
                            polozenie_mojego_krola_x--;
                            polozenie_mojego_krola_y++;
                        }
                        if(j+1<=7 && (wsk_X[i][j+1]<'G' || 'W'<wsk_X[i][j+1]))                    //RUCH NA 3:00
                        if(2<=abs(polozenie_mojego_krola_x+1-polozenie_krola_urzytkownika_x) || 2<=abs(polozenie_mojego_krola_y-polozenie_krola_urzytkownika_y)) //czy krol do krola sie nie przytula
                        {
                            polozenie_mojego_krola_x++;
                            obsluga_ruchu_maszyny(j,i,j+1,i,'K',wsk_X);
                            polozenie_mojego_krola_x--;
                        }
                        if(i+1<=7 && j+1<=7 && (wsk_X[i+1][j+1]<'G' || 'W'<wsk_X[i+1][j+1]))      //RUCH NA 4:30
                        if(2<=abs(polozenie_mojego_krola_x+1-polozenie_krola_urzytkownika_x) || 2<=abs(polozenie_mojego_krola_y+1-polozenie_krola_urzytkownika_y)) //czy krol do krola sie nie przytula
                        {
                            polozenie_mojego_krola_x++;
                            polozenie_mojego_krola_y++;
                            obsluga_ruchu_maszyny(j,i,j+1,i+1,'K',wsk_X);
                            polozenie_mojego_krola_x--;
                            polozenie_mojego_krola_y--;
                        }
                        if(i+1<=7 && (wsk_X[i+1][j]<'G' || 'W'<wsk_X[i+1][j]))                    //RUCH NA 6:00
                        if(2<=abs(polozenie_mojego_krola_x-polozenie_krola_urzytkownika_x) || 2<=abs(polozenie_mojego_krola_y+1-polozenie_krola_urzytkownika_y)) //czy krol do krola sie nie przytula
                        {
                            polozenie_mojego_krola_y++;
                            obsluga_ruchu_maszyny(j,i,j,i+1,'K',wsk_X);
                            polozenie_mojego_krola_y--;
                        }
                        if(i+1<=7 && 0<=j-1 && (wsk_X[i+1][j-1]<'G' || 'W'<wsk_X[i+1][j-1]))      //RUCH NA 7:30
                        if(2<=abs(polozenie_mojego_krola_x-1-polozenie_krola_urzytkownika_x) || 2<=abs(polozenie_mojego_krola_y+1-polozenie_krola_urzytkownika_y)) //czy krol do krola sie nie przytula
                        {
                            polozenie_mojego_krola_x--;
                            polozenie_mojego_krola_y++;
                            obsluga_ruchu_maszyny(j,i,j-1,i+1,'K',wsk_X);
                            polozenie_mojego_krola_x++;
                            polozenie_mojego_krola_y--;
                        }
                        if(0<=j-1 && (wsk_X[i][j-1]<'G' || 'W'<wsk_X[i][j-1]))                    //RUCH NA 9:00
                        if(2<=abs(polozenie_mojego_krola_x-1-polozenie_krola_urzytkownika_x) || 2<=abs(polozenie_mojego_krola_y-polozenie_krola_urzytkownika_y)) //czy krol do krola sie nie przytula
                        {
                            polozenie_mojego_krola_x--;
                            obsluga_ruchu_maszyny(j,i,j-1,i,'K',wsk_X);
                            polozenie_mojego_krola_x++;
                        }
                        if(0<=i-1 && 0<=j-1 && (wsk_X[i-1][j-1]<'G' || 'W'<wsk_X[i-1][j-1]))      //RUCH NA 10:30
                        if(2<=abs(polozenie_mojego_krola_x-1-polozenie_krola_urzytkownika_x) || 2<=abs(polozenie_mojego_krola_y-1-polozenie_krola_urzytkownika_y)) //czy krol do krola sie nie przytula
                        {
                            polozenie_mojego_krola_x--;
                            polozenie_mojego_krola_y--;
                            obsluga_ruchu_maszyny(j,i,j-1,i-1,'K',wsk_X);
                            polozenie_mojego_krola_x++;
                            polozenie_mojego_krola_y++;
                        }
                        if(!kolor)                                                                      //ROSZADA KIEDY UZYTKOWNIK GRA CZARNYMI
                        {
                            if(czy_moj_krol_sie_ruszyl==false && czy_moja_lewa_wierza_sie_ruszyla==false && wsk_X[7][1]==' ' && wsk_X[7][2]==' ' && wsk_X[7][3]==' ')       //ROSZADA DLUGA
                            {
                                T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                                if(czy_moje_pole_jest_bite(2, 7, kopia)==false && czy_moje_pole_jest_bite(3, 7, kopia)==false && czy_moje_pole_jest_bite(4, 7, kopia)==false)
                                {
                                    kopia[7][0]=' ';
                                    kopia[7][4]=' ';
                                    kopia[7][2]='K';
                                    kopia[7][3]='W';
                                    polozenie_mojego_krola_x-=2;
                                    wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                                    polozenie_mojego_krola_x+=2;
                                }
                                else delete[]kopia;
                            }
                            if(czy_moj_krol_sie_ruszyl==false && czy_moja_prawa_wierza_sie_ruszyla==false && wsk_X[7][5]==' ' && wsk_X[7][6]==' ')                          //ROSZADA KROTKA
                            {
                                T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                                if(czy_moje_pole_jest_bite(4, 7, kopia)==false && czy_moje_pole_jest_bite(5, 7, kopia)==false && czy_moje_pole_jest_bite(6, 7, kopia)==false)
                                {
                                    kopia[7][7]=' ';
                                    kopia[7][4]=' ';
                                    kopia[7][6]='K';
                                    kopia[7][5]='W';
                                    polozenie_mojego_krola_x+=2;
                                    wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                                    polozenie_mojego_krola_x-=2;
                                }
                                else delete[]kopia;
                            }
                        }
                        if(kolor)                                                                       //ROSZADA KIEDY UZYTKOWNIK GRA BIALYMI
                        {
                            if(czy_moj_krol_sie_ruszyl==false && czy_moja_prawa_wierza_sie_ruszyla==false && wsk_X[7][4]==' ' && wsk_X[7][5]==' ' && wsk_X[7][6]==' ')       //ROSZADA DLUGA
                            {
                                T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                                if(czy_moje_pole_jest_bite(3, 7, kopia)==false && czy_moje_pole_jest_bite(4, 7, kopia)==false && czy_moje_pole_jest_bite(5, 7, kopia)==false)
                                {
                                    kopia[7][7]=' ';
                                    kopia[7][3]=' ';
                                    kopia[7][5]='K';
                                    kopia[7][4]='W';
                                    polozenie_mojego_krola_x+=2;
                                    wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                                    polozenie_mojego_krola_x-=2;
                                }
                                else delete[]kopia;
                            }
                            if(czy_moj_krol_sie_ruszyl==false && czy_moja_lewa_wierza_sie_ruszyla==false && wsk_X[7][1]==' ' && wsk_X[7][2]==' ')                          //ROSZADA KROTKA
                            {
                                T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                                if(czy_moje_pole_jest_bite(1, 7, kopia)==false && czy_moje_pole_jest_bite(2, 7, kopia)==false && czy_moje_pole_jest_bite(3, 7, kopia)==false)
                                {
                                    kopia[7][0]=' ';
                                    kopia[7][3]=' ';
                                    kopia[7][1]='K';
                                    kopia[7][2]='W';
                                    polozenie_mojego_krola_x-=2;
                                    wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                                    polozenie_mojego_krola_x+=2;
                                }
                                else delete[]kopia;
                            }
                        }
                        break;
                    }
                default: break;
            }
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
    void Ruch::obsluga_ruchu_maszyny(int zX, int zY, int naX, int naY, char bierka, T_wsk_szachownica wsk_X){//5
//==============================================================================================================
    try{
        if (wsk_X == nullptr)
            throw invalid_argument("Zerowy adres szachownicy.");
        if(zX<0 || 7<zX || zY<0 || 7<zY || naX<0 || 7<naX || naY<0 || 7<naY)
            throw invalid_argument("Wspolzedne szachownicy poza zakresem.");
        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
        kopia[zY][zX] = ' ';
        kopia[naY][naX] = bierka;
        wykonaj_moj_ruch_jak_mozna(zY, zX, kopia);
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
        void Ruch::wykonaj_moj_ruch_jak_mozna(int &y, int &x, T_wsk_szachownica kopia){//4
//==============================================================================================================
    try{
        if (x < 0 || 7 < x || y < 0 || 7 < y)
            throw invalid_argument("Wspolzedne szachownicy poza zakresem.");
        if (kopia == nullptr)
            throw invalid_argument("Zerowy adres szachownicy.");
        if(czy_moj_krol_jest_szachowany==false && x!=polozenie_mojego_krola_x && y!=polozenie_mojego_krola_y && (polozenie_mojego_krola_x+polozenie_mojego_krola_y!=x+y) && (polozenie_mojego_krola_x-polozenie_mojego_krola_y!=x-y))
        {
            numer_mojego_ruchu++;
            wektor_na_ruchy.push_back(vector<T_wsk_szachownica>());
            wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
            wykonaj_ruch_urzytkownika(kopia);
            odrzuc_najgorszy_ruch();
        }
        else if(czy_moje_pole_jest_bite(polozenie_mojego_krola_x, polozenie_mojego_krola_y, kopia)==false)
        {
            numer_mojego_ruchu++;
            wektor_na_ruchy.push_back(vector<T_wsk_szachownica>());
            wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
            wykonaj_ruch_urzytkownika(kopia);
            odrzuc_najgorszy_ruch();
        }
        else delete[]kopia; //jesli nie mozna to uwalniamy pamiec
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
            bool Ruch::czy_moje_pole_jest_bite(const int &x,const int &y, T_wsk_szachownica kopia){//0+
//==============================================================================================================
    try{
        if (x < 0 || 7 < x || y < 0 || 7 < y)
            throw invalid_argument("Wspolzedne szachownicy poza zakresem.");
        if (kopia == nullptr)
            throw invalid_argument("Zerowy adres szachownicy.");
        for(int i=1; 0<=y-i; i++)                                       //12:00 //BITE PRZEZ HETMANA / WIEZE / GONCA
        {
            if(kopia[y-i][x]==' ') continue;
            if(kopia[y-i][x]=='h' || kopia[y-i][x]=='w') return true;
            else break;
        }
        for(int i=1; 0<=y-i && x+i<=7; i++)                             //1:30
        {
            if(kopia[y-i][x+i]==' ') continue;
            if(kopia[y-i][x+i]=='h' || kopia[y-i][x+i]=='g') return true;
            else break;
        }
        for(int i=1; x+i<=7; i++)                                       //3:00
        {
            if(kopia[y][x+i]==' ') continue;
            if(kopia[y][x+i]=='h' || kopia[y][x+i]=='w') return true;
            else break;
        }
        for(int i=1; y+i<=7 && x+i<=7; i++)                             //4:30
        {
            if(kopia[y+i][x+i]==' ') continue;
            if(kopia[y+i][x+i]=='h' || kopia[y+i][x+i]=='g') return true;
            else break;
        }
        for(int i=1; y+i<=7; i++)                                       //6:00
        {
            if(kopia[y+i][x]==' ') continue;
            if(kopia[y+i][x]=='h' || kopia[y+i][x]=='w') return true;
            else break;
        }
        for(int i=1; y+i<=7 && 0<=x-i; i++)                             //7:30
        {
            if(kopia[y+i][x-i]==' ') continue;
            if(kopia[y+i][x-i]=='h' || kopia[y+i][x-i]=='g') return true;
            else break;
        }
        for(int i=1; 0<=x-i; i++)                                       //9:00
        {
            if(kopia[y][x-i]==' ') continue;
            if(kopia[y][x-i]=='h' || kopia[y][x-i]=='w') return true;
            else break;
        }
        for(int i=1; 0<=y-i && 0<=x-i; i++)                             //10:30
        {
            if(kopia[y-i][x-i]==' ') continue;
            if(kopia[y-i][x-i]=='h' || kopia[y-i][x-i]=='g') return true;
            else break;
        }

        if(0<=y-2 && x+1<=7 && kopia[y-2][x+1]=='s') return true;                       //BITE PRZEZ SKOCZKA
        if(0<=y-1 && x+2<=7 && kopia[y-1][x+2]=='s') return true;
        if(y+1<=7 && x+2<=7 && kopia[y+1][x+2]=='s') return true;
        if(y+2<=7 && x+1<=7 && kopia[y+2][x+1]=='s') return true;
        if(y+2<=7 && 0<=x-1 && kopia[y+2][x-1]=='s') return true;
        if(y+1<=7 && 0<=x-2 && kopia[y+1][x-2]=='s') return true;
        if(0<=y-1 && 0<=x-2 && kopia[y-1][x-2]=='s') return true;
        if(0<=y-2 && 0<=x-1 && kopia[y-2][x-1]=='s') return true;

        if(2<=y)                                                                    //BITE PRZEZ PIONKA
        {
            if(1<=x && kopia[y-1][x-1]=='p') return true;
            if(x<=6 && kopia[y-1][x+1]=='p') return true;
        }
        return false;
//#########################################################################
    }
    catch(const invalid_argument &e){
        obslugiwany_blad blad;
        blad.komunikat = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw blad;
    }
}
            void Ruch::odrzuc_najgorszy_ruch(){//1
//==============================================================================================================
    try{
        double najmniejszy_status_materialny_dla_danego_ruchu = 100.0;                                      //WYWALAMY NA BIEZACO NAJGORSZY RUCH
        for(int i=1; i < wektor_na_ruchy[numer_mojego_ruchu].size(); i++) //znajdujemy najkorzyskniejszy dla urzytkownika po jego odpowiedzi
        {
            najmniejszy_status_materialny_dla_danego_ruchu = min(najmniejszy_status_materialny_dla_danego_ruchu, oblicz_status_materialny(wektor_na_ruchy[numer_mojego_ruchu][i]));
        }
        if(ile_ruchow_rozpatrywac[pokolenie_klasy] >= wektor_na_ruchy.size()) //sytuacja, w ktorej ilosc obliczonych ruchow nieprzekracza limitu
        {
            if(najmniejszy_status_materialny_dla_danego_ruchu < najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow)
            {
                najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow = najmniejszy_status_materialny_dla_danego_ruchu;
                index_najgorszego_ruchu = numer_mojego_ruchu;
            }
            statusy_materialne_rozpatrywanych_rochow.push_back(najmniejszy_status_materialny_dla_danego_ruchu);
        }
        else  //sytuacja, w ktorej ilosc obliczonych ruchow przekracza limit i trzeba odrzucic najgorszy ruch
        {
            if(najmniejszy_status_materialny_dla_danego_ruchu <= najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow)//jesli odrzucamy bieżacy ruch
            {
                for(auto element: wektor_na_ruchy[numer_mojego_ruchu])
                    delete[]element;
                wektor_na_ruchy.pop_back();
                numer_mojego_ruchu--;
            }
            else//jesli odrzucamy ruch ktorys z poprzednich
            {
                statusy_materialne_rozpatrywanych_rochow[index_najgorszego_ruchu] = najmniejszy_status_materialny_dla_danego_ruchu;
                for(auto element: wektor_na_ruchy[index_najgorszego_ruchu])
                    delete[]element;
                wektor_na_ruchy[index_najgorszego_ruchu] = wektor_na_ruchy[numer_mojego_ruchu];
                wektor_na_ruchy.pop_back();
                numer_mojego_ruchu--;
                index_najgorszego_ruchu = 0;// od poczatku szukamu ktory ruch jest potencjalnie najgorszym
                for(int i=0; i<ile_ruchow_rozpatrywac[pokolenie_klasy]; i++)
                    if(statusy_materialne_rozpatrywanych_rochow[i] < statusy_materialne_rozpatrywanych_rochow[index_najgorszego_ruchu])
                        index_najgorszego_ruchu = i;
            }
        }
//#########################################################################
    }
    catch(obslugiwany_blad &e){
        e.komunikat = __PRETTY_FUNCTION__ + string(" >>\n") + e.komunikat;
        throw;
    }
}
void Ruch::wykonaj_ruch_urzytkownika(T_wsk_szachownica wsk_X){//3
//==============================================================================================================
    try{
        if (wsk_X == nullptr)
            throw invalid_argument("Zerowy adres szachownicy.");
        czy_krol_urzytkownika_jest_szachowany = czy_pole_urzytkownika_jest_bite(polozenie_krola_urzytkownika_x, polozenie_krola_urzytkownika_y, wsk_X);
        for(int i=0; i<8; i++)
            for(int j=0; j<8; j++)
            {
                switch(wsk_X[i][j])
                {
                    case ' ': break;
                    case 'p':                                                       //KWESTIA PIONKA
                    {
                        if(0<i && i<6)                                                  //PIONEK BEZ SZANSY NA PROMOCJE
                        {
                            if(wsk_X[i+1][j]==' ')                                    obsluga_ruchu_urzytkownika(j,i,j,i+1,'p',wsk_X);   //POSUNIECIE O 1 W PRZOD
                            if(i==1 && wsk_X[2][j]==' ' && wsk_X[3][j]==' ')          obsluga_ruchu_urzytkownika(j,1,j,3,'p',wsk_X);     //POSUNIECIE O 2 W PRZOD
                            if(j!=0 && 'G'<=wsk_X[i+1][j-1] && wsk_X[i+1][j-1]<='W')  obsluga_ruchu_urzytkownika(j,i,j-1,i+1,'p',wsk_X); //BICIE W LEWO
                            if(j!=7 && 'G'<=wsk_X[i+1][j+1] && wsk_X[i+1][j+1]<='W')  obsluga_ruchu_urzytkownika(j,i,j+1,i+1,'p',wsk_X); //BICIE W PRAWO

                        }
                        if(i==6)                                                       //PROMOCJA
                        {
                            if(wsk_X[7][j]==' ')                                                //PROMOCJA PO PROSTU
                            {
                                obsluga_ruchu_urzytkownika(j,6,j,7,'s',wsk_X); //Z UTWORZENIEM SKOCZKA
                                obsluga_ruchu_urzytkownika(j,6,j,7,'g',wsk_X); //Z UTWORZENIEM GONCA
                                obsluga_ruchu_urzytkownika(j,6,j,7,'w',wsk_X); //Z UTWORZENIEM WIEZY
                                obsluga_ruchu_urzytkownika(j,6,j,7,'h',wsk_X); //Z UTWORZENIEM HETMANA
                            }
                            if(j!=0 && 'G'<=wsk_X[7][j-1] && wsk_X[7][j-1]<='W')                //PROMOCJA W LEWO
                            {
                                obsluga_ruchu_urzytkownika(j,6,j-1,7,'s',wsk_X); //Z UTWORZENIEM SKOCZKA
                                obsluga_ruchu_urzytkownika(j,6,j-1,7,'g',wsk_X); //Z UTWORZENIEM GONCA
                                obsluga_ruchu_urzytkownika(j,6,j-1,7,'w',wsk_X); //Z UTWORZENIEM WIEZY
                                obsluga_ruchu_urzytkownika(j,6,j-1,7,'h',wsk_X); //Z UTWORZENIEM HETMANA
                            }
                            if(j!=7 && 'G'<=wsk_X[7][j+1] && wsk_X[7][j+1]<='W')                //PROMOCJA W PRAWO
                            {
                                obsluga_ruchu_urzytkownika(j,6,j+1,7,'s',wsk_X); //Z UTWORZENIEM SKOCZKA
                                obsluga_ruchu_urzytkownika(j,6,j+1,7,'g',wsk_X); //Z UTWORZENIEM GONCA
                                obsluga_ruchu_urzytkownika(j,6,j+1,7,'w',wsk_X); //Z UTWORZENIEM WIEZY
                                obsluga_ruchu_urzytkownika(j,6,j+1,7,'h',wsk_X); //Z UTWORZENIEM HETMANA
                            }
                        }
                        break;
                    }
                    case 's':                                                       //KWESTIA SKOCZKA
                    {
                        if(0<=i-2 && j+1<=7 && (wsk_X[i-2][j+1]<'g' || 'w'<wsk_X[i-2][j+1]))  obsluga_ruchu_urzytkownika(j,i,j+1,i-2,'s',wsk_X);
                        if(0<=i-1 && j+2<=7 && (wsk_X[i-1][j+2]<'g' || 'w'<wsk_X[i-1][j+2]))  obsluga_ruchu_urzytkownika(j,i,j+2,i-1,'s',wsk_X);
                        if(i+1<=7 && j+2<=7 && (wsk_X[i+1][j+2]<'g' || 'w'<wsk_X[i+1][j+2]))  obsluga_ruchu_urzytkownika(j,i,j+2,i+1,'s',wsk_X);
                        if(i+2<=7 && j+1<=7 && (wsk_X[i+2][j+1]<'g' || 'w'<wsk_X[i+2][j+1]))  obsluga_ruchu_urzytkownika(j,i,j+1,i+2,'s',wsk_X);
                        if(i+2<=7 && 0<=j-1 && (wsk_X[i+2][j-1]<'g' || 'w'<wsk_X[i+2][j-1]))  obsluga_ruchu_urzytkownika(j,i,j-1,i+2,'s',wsk_X);
                        if(i+1<=7 && 0<=j-2 && (wsk_X[i+1][j-2]<'g' || 'w'<wsk_X[i+1][j-2]))  obsluga_ruchu_urzytkownika(j,i,j-2,i+1,'s',wsk_X);
                        if(0<=i-1 && 0<=j-2 && (wsk_X[i-1][j-2]<'g' || 'w'<wsk_X[i-1][j-2]))  obsluga_ruchu_urzytkownika(j,i,j-2,i-1,'s',wsk_X);
                        if(0<=i-2 && 0<=j-1 && (wsk_X[i-2][j-1]<'g' || 'w'<wsk_X[i-2][j-1]))  obsluga_ruchu_urzytkownika(j,i,j-1,i-2,'s',wsk_X);
                        break;
                    }
                    case 'g':                                                       //KWESTIA GONCA
                    {
                        for(int y=i-1, x=j+1; 0<=y && x<=7; y--, x++)                   //RUCH NA 1:30
                        {
                            if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w')                                                  break;
                            if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W') {obsluga_ruchu_urzytkownika(j,i,x,y,'g',wsk_X);  break;}
                            if(wsk_X[y][x]==' ')                      obsluga_ruchu_urzytkownika(j,i,x,y,'g',wsk_X);
                        }
                        for(int y=i+1, x=j+1; y<=7 && x<=7; y++, x++)                   //RUCH NA 4:30
                        {
                            if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w')                                                  break;
                            if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W') {obsluga_ruchu_urzytkownika(j,i,x,y,'g',wsk_X);  break;}
                            if(wsk_X[y][x]==' ')                      obsluga_ruchu_urzytkownika(j,i,x,y,'g',wsk_X);
                        }
                        for(int y=i+1, x=j-1; y<=7 && 0<=x; y++, x--)                   //RUCH NA 7:30
                        {
                            if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w')                                                  break;
                            if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W') {obsluga_ruchu_urzytkownika(j,i,x,y,'g',wsk_X);  break;}
                            if(wsk_X[y][x]==' ')                      obsluga_ruchu_urzytkownika(j,i,x,y,'g',wsk_X);
                        }
                        for(int y=i-1, x=j-1; 0<=y && 0<=x; y--, x--)                   //RUCH NA 10:30
                        {
                            if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w')                                                  break;
                            if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W') {obsluga_ruchu_urzytkownika(j,i,x,y,'g',wsk_X);  break;}
                            if(wsk_X[y][x]==' ')                      obsluga_ruchu_urzytkownika(j,i,x,y,'g',wsk_X);
                        }
                        break;
                    }
                    case 'w':                                                       //KWESTIA WIEZY
                    {
                        for(int y=i-1; 0<=y; y--)                                       //RUCH NA 12:00
                        {
                            if('g'<=wsk_X[y][j] && wsk_X[y][j]<='w')                                                  break;
                            if('G'<=wsk_X[y][j] && wsk_X[y][j]<='W') {obsluga_ruchu_urzytkownika(j,i,j,y,'w',wsk_X);  break;}
                            if(wsk_X[y][j]==' ')                      obsluga_ruchu_urzytkownika(j,i,j,y,'w',wsk_X);
                        }
                        for(int x=j+1; x<=7; x++)                                       //RUCH NA 3:00
                        {
                            if('g'<=wsk_X[i][x] && wsk_X[i][x]<='w')                                                  break;
                            if('G'<=wsk_X[i][x] && wsk_X[i][x]<='W') {obsluga_ruchu_urzytkownika(j,i,x,i,'w',wsk_X);  break;}
                            if(wsk_X[i][x]==' ')                      obsluga_ruchu_urzytkownika(j,i,x,i,'w',wsk_X);
                        }
                        for(int y=i+1; y<=7; y++)                                       //RUCH NA 6:00
                        {
                            if('g'<=wsk_X[y][j] && wsk_X[y][j]<='w')                                                  break;
                            if('G'<=wsk_X[y][j] && wsk_X[y][j]<='W') {obsluga_ruchu_urzytkownika(j,i,j,y,'w',wsk_X);  break;}
                            if(wsk_X[y][j]==' ')                      obsluga_ruchu_urzytkownika(j,i,j,y,'w',wsk_X);
                        }
                        for(int x=j-1; 0<=x; x--)                                       //RUCH NA 9:00
                        {
                            if('g'<=wsk_X[i][x] && wsk_X[i][x]<='w')                                                  break;
                            if('G'<=wsk_X[i][x] && wsk_X[i][x]<='W') {obsluga_ruchu_urzytkownika(j,i,x,i,'w',wsk_X);  break;}
                            if(wsk_X[i][x]==' ')                      obsluga_ruchu_urzytkownika(j,i,x,i,'w',wsk_X);
                        }
                        break;
                    }
                    case 'h':                                                       //KWESTIA HETMANA
                    {
                        for(int y=i-1; 0<=y; y--)                                       //RUCH NA 12:00
                        {
                            if('g'<=wsk_X[y][j] && wsk_X[y][j]<='w')                                                  break;
                            if('G'<=wsk_X[y][j] && wsk_X[y][j]<='W') {obsluga_ruchu_urzytkownika(j,i,j,y,'h',wsk_X);  break;}
                            if(wsk_X[y][j]==' ')                      obsluga_ruchu_urzytkownika(j,i,j,y,'h',wsk_X);
                        }
                        for(int y=i-1, x=j+1; 0<=y && x<=7; y--, x++)                   //RUCH NA 1:30
                        {
                            if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w')                                                  break;
                            if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W') {obsluga_ruchu_urzytkownika(j,i,x,y,'h',wsk_X);  break;}
                            if(wsk_X[y][x]==' ')                      obsluga_ruchu_urzytkownika(j,i,x,y,'h',wsk_X);
                        }
                        for(int x=j+1; x<=7; x++)                                       //RUCH NA 3:00
                        {
                            if('g'<=wsk_X[i][x] && wsk_X[i][x]<='w')                                                  break;
                            if('G'<=wsk_X[i][x] && wsk_X[i][x]<='W') {obsluga_ruchu_urzytkownika(j,i,x,i,'h',wsk_X);  break;}
                            if(wsk_X[i][x]==' ')                      obsluga_ruchu_urzytkownika(j,i,x,i,'h',wsk_X);
                        }
                        for(int y=i+1, x=j+1; y<=7 && x<=7; y++, x++)                   //RUCH NA 4:30
                        {
                            if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w')                                                  break;
                            if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W') {obsluga_ruchu_urzytkownika(j,i,x,y,'h',wsk_X);  break;}
                            if(wsk_X[y][x]==' ')                      obsluga_ruchu_urzytkownika(j,i,x,y,'h',wsk_X);
                        }
                        for(int y=i+1; y<=7; y++)                                       //RUCH NA 6:00
                        {
                            if('g'<=wsk_X[y][j] && wsk_X[y][j]<='w')                                                  break;
                            if('G'<=wsk_X[y][j] && wsk_X[y][j]<='W') {obsluga_ruchu_urzytkownika(j,i,j,y,'h',wsk_X);  break;}
                            if(wsk_X[y][j]==' ')                      obsluga_ruchu_urzytkownika(j,i,j,y,'h',wsk_X);
                        }
                        for(int y=i+1, x=j-1; y<=7 && 0<=x; y++, x--)                   //RUCH NA 7:30
                        {
                            if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w')                                                  break;
                            if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W') {obsluga_ruchu_urzytkownika(j,i,x,y,'h',wsk_X);  break;}
                            if(wsk_X[y][x]==' ')                      obsluga_ruchu_urzytkownika(j,i,x,y,'h',wsk_X);
                        }
                        for(int x=j-1; 0<=x; x--)                                       //RUCH NA 9:00
                        {
                            if('g'<=wsk_X[i][x] && wsk_X[i][x]<='w')                                                  break;
                            if('G'<=wsk_X[i][x] && wsk_X[i][x]<='W') {obsluga_ruchu_urzytkownika(j,i,x,i,'h',wsk_X);  break;}
                            if(wsk_X[i][x]==' ')                      obsluga_ruchu_urzytkownika(j,i,x,i,'h',wsk_X);
                        }
                        for(int y=i-1, x=j-1; 0<=y && 0<=x; y--, x--)                   //RUCH NA 10:30
                        {
                            if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w')                                                  break;
                            if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W') {obsluga_ruchu_urzytkownika(j,i,x,y,'h',wsk_X);  break;}
                            if(wsk_X[y][x]==' ')                      obsluga_ruchu_urzytkownika(j,i,x,y,'h',wsk_X);
                        }
                        break;
                    }
                    case 'k':                                                       //KWESTIA KROLA
                    {
                        if(0<=i-1 && (wsk_X[i-1][j]<'g' || 'w'<wsk_X[i-1][j]))                    //RUCH NA 12:00
                        if(2<=abs(polozenie_mojego_krola_x-polozenie_krola_urzytkownika_x) || 2<=abs(polozenie_mojego_krola_y-1-polozenie_krola_urzytkownika_y)) //czy krol do krola sie nie przytula
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[i-1][j]='k';
                            if(czy_pole_urzytkownika_jest_bite(polozenie_krola_urzytkownika_x, polozenie_krola_urzytkownika_y-1, kopia)==false)
                            {
                                polozenie_krola_urzytkownika_y--;
                                wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
                                polozenie_krola_urzytkownika_y++;
                            }
                            else delete[]kopia;
                        }
                        if(0<=i-1 && j+1<=7 && (wsk_X[i-1][j+1]<'g' || 'w'<wsk_X[i-1][j+1]))      //RUCH NA 1:30
                        if(2<=abs(polozenie_mojego_krola_x+1-polozenie_krola_urzytkownika_x) || 2<=abs(polozenie_mojego_krola_y-1-polozenie_krola_urzytkownika_y)) //czy krol do krola sie nie przytula
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[i-1][j+1]='k';
                            if(czy_pole_urzytkownika_jest_bite(polozenie_krola_urzytkownika_x+1, polozenie_krola_urzytkownika_y-1, kopia)==false)
                            {
                                polozenie_krola_urzytkownika_x++;
                                polozenie_krola_urzytkownika_y--;
                                wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
                                polozenie_krola_urzytkownika_x--;
                                polozenie_krola_urzytkownika_y++;
                            }
                            else delete[]kopia;
                        }
                        if(j+1<=7 && (wsk_X[i][j+1]<'g' || 'w'<wsk_X[i][j+1]))                    //RUCH NA 3:00
                        if(2<=abs(polozenie_mojego_krola_x+1-polozenie_krola_urzytkownika_x) || 2<=abs(polozenie_mojego_krola_y-polozenie_krola_urzytkownika_y)) //czy krol do krola sie nie przytula
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[i][j+1]='k';
                            if(czy_pole_urzytkownika_jest_bite(polozenie_krola_urzytkownika_x+1, polozenie_krola_urzytkownika_y, kopia)==false)
                            {
                                polozenie_krola_urzytkownika_x++;
                                wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
                                polozenie_krola_urzytkownika_x--;
                            }
                            else delete[]kopia;
                        }
                        if(i+1<=7 && j+1<=7 && (wsk_X[i+1][j+1]<'g' || 'w'<wsk_X[i+1][j+1]))      //RUCH NA 4:30
                        if(2<=abs(polozenie_mojego_krola_x+1-polozenie_krola_urzytkownika_x) || 2<=abs(polozenie_mojego_krola_y+1-polozenie_krola_urzytkownika_y)) //czy krol do krola sie nie przytula
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[i+1][j+1]='k';
                            if(czy_pole_urzytkownika_jest_bite(polozenie_krola_urzytkownika_x+1, polozenie_krola_urzytkownika_y+1, kopia)==false)
                            {
                                polozenie_krola_urzytkownika_x++;
                                polozenie_krola_urzytkownika_y++;
                                wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
                                polozenie_krola_urzytkownika_x--;
                                polozenie_krola_urzytkownika_y--;
                            }
                            else delete[]kopia;
                        }
                        if(i+1<=7 && (wsk_X[i+1][j]<'g' || 'w'<wsk_X[i+1][j]))                    //RUCH NA 6:00
                        if(2<=abs(polozenie_mojego_krola_x-polozenie_krola_urzytkownika_x) || 2<=abs(polozenie_mojego_krola_y+1-polozenie_krola_urzytkownika_y)) //czy krol do krola sie nie przytula
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[i+1][j]='k';
                            if(czy_pole_urzytkownika_jest_bite(polozenie_krola_urzytkownika_x, polozenie_krola_urzytkownika_y+1, kopia)==false)
                            {
                                polozenie_krola_urzytkownika_y++;
                                wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
                                polozenie_krola_urzytkownika_y--;
                            }
                            else delete[]kopia;
                        }
                        if(i+1<=7 && 0<=j-1 && (wsk_X[i+1][j-1]<'g' || 'w'<wsk_X[i+1][j-1]))      //RUCH NA 7:30
                        if(2<=abs(polozenie_mojego_krola_x-1-polozenie_krola_urzytkownika_x) || 2<=abs(polozenie_mojego_krola_y+1-polozenie_krola_urzytkownika_y)) //czy krol do krola sie nie przytula
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[i+1][j-1]='k';
                            if(czy_pole_urzytkownika_jest_bite(polozenie_krola_urzytkownika_x-1, polozenie_krola_urzytkownika_y+1, kopia)==false)
                            {
                                polozenie_krola_urzytkownika_x--;
                                polozenie_krola_urzytkownika_y++;
                                wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
                                polozenie_krola_urzytkownika_x++;
                                polozenie_krola_urzytkownika_y--;
                            }
                            else delete[]kopia;
                        }
                        if(0<=j-1 && (wsk_X[i][j-1]<'g' || 'w'<wsk_X[i][j-1]))                    //RUCH NA 9:00
                        if(2<=abs(polozenie_mojego_krola_x-1-polozenie_krola_urzytkownika_x) || 2<=abs(polozenie_mojego_krola_y-polozenie_krola_urzytkownika_y)) //czy krol do krola sie nie przytula
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[i][j-1]='k';
                            if(czy_pole_urzytkownika_jest_bite(polozenie_krola_urzytkownika_x-1, polozenie_krola_urzytkownika_y, kopia)==false)
                            {
                                polozenie_krola_urzytkownika_x--;
                                wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
                                polozenie_krola_urzytkownika_x++;
                            }
                            else delete[]kopia;
                        }
                        if(0<=i-1 && 0<=j-1 && (wsk_X[i-1][j-1]<'g' || 'w'<wsk_X[i-1][j-1]))      //RUCH NA 10:30
                        if(2<=abs(polozenie_mojego_krola_x-1-polozenie_krola_urzytkownika_x) || 2<=abs(polozenie_mojego_krola_y-1-polozenie_krola_urzytkownika_y)) //czy krol do krola sie nie przytula
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[i-1][j-1]='k';
                            if(czy_pole_urzytkownika_jest_bite(polozenie_krola_urzytkownika_x-1, polozenie_krola_urzytkownika_y-1, kopia)==false)
                            {
                                polozenie_krola_urzytkownika_x--;
                                polozenie_krola_urzytkownika_y--;
                                wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
                                polozenie_krola_urzytkownika_x++;
                                polozenie_krola_urzytkownika_y++;
                            }
                            else delete[]kopia;
                        }
                        if(!kolor)                                                                      //ROSZADA KIEDY UZYTKOWNIK GRA CZARNYMI
                        {
                            if(czy_krol_urzytkownika_sie_ruszyl==false && czy_lewa_wierza_urzytkownika_sie_ruszyla==false && wsk_X[0][1]==' ' && wsk_X[0][2]==' ' && wsk_X[0][3]==' ')       //ROSZADA DLUGA
                            {
                                T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                                if(czy_pole_urzytkownika_jest_bite(2, 0, kopia)==false && czy_pole_urzytkownika_jest_bite(3, 0, kopia)==false && czy_pole_urzytkownika_jest_bite(4, 0, kopia)==false)
                                {
                                    kopia[0][0]=' ';
                                    kopia[0][4]=' ';
                                    kopia[0][2]='k';
                                    kopia[0][3]='w';
                                    polozenie_krola_urzytkownika_x-=2;
                                    wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
                                    polozenie_krola_urzytkownika_x+=2;
                                }
                                else delete[]kopia;
                            }
                            if(czy_krol_urzytkownika_sie_ruszyl==false && czy_prawa_wierza_urzytkownika_sie_ruszyla==false && wsk_X[0][5]==' ' && wsk_X[0][6]==' ')                          //ROSZADA KROTKA
                            {
                                T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                                if(czy_pole_urzytkownika_jest_bite(4, 0, kopia)==false && czy_pole_urzytkownika_jest_bite(5, 0, kopia)==false && czy_pole_urzytkownika_jest_bite(6, 0, kopia)==false)
                                {
                                    kopia[0][7]=' ';
                                    kopia[0][4]=' ';
                                    kopia[0][6]='k';
                                    kopia[0][5]='w';
                                    polozenie_krola_urzytkownika_x+=2;
                                    wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
                                    polozenie_krola_urzytkownika_x-=2;
                                }
                                else delete[]kopia;
                            }
                        }
                        if(kolor)                                                                       //ROSZADA KIEDY UZYTKOWNIK GRA BIALYMI
                        {
                            if(czy_krol_urzytkownika_sie_ruszyl==false && czy_prawa_wierza_urzytkownika_sie_ruszyla==false && wsk_X[0][4]==' ' && wsk_X[0][5]==' ' && wsk_X[0][6]==' ')       //ROSZADA DLUGA
                            {
                                T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                                if(czy_pole_urzytkownika_jest_bite(3, 0, kopia)==false && czy_pole_urzytkownika_jest_bite(4, 0, kopia)==false && czy_pole_urzytkownika_jest_bite(5, 0, kopia)==false)
                                {
                                    kopia[0][7]=' ';
                                    kopia[0][3]=' ';
                                    kopia[0][5]='k';
                                    kopia[0][4]='w';
                                    polozenie_krola_urzytkownika_x+=2;
                                    wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
                                    polozenie_krola_urzytkownika_x-=2;
                                }
                                else delete[]kopia;
                            }
                            if(czy_krol_urzytkownika_sie_ruszyl==false && czy_lewa_wierza_urzytkownika_sie_ruszyla==false && wsk_X[0][1]==' ' && wsk_X[0][2]==' ')                          //ROSZADA KROTKA
                            {
                                T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                                if(czy_pole_urzytkownika_jest_bite(1, 0, kopia)==false && czy_pole_urzytkownika_jest_bite(2, 0, kopia)==false && czy_pole_urzytkownika_jest_bite(3, 0, kopia)==false)
                                {
                                    kopia[0][0]=' ';
                                    kopia[0][3]=' ';
                                    kopia[0][1]='k';
                                    kopia[0][2]='w';
                                    polozenie_krola_urzytkownika_x-=2;
                                    wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
                                    polozenie_krola_urzytkownika_x+=2;
                                }
                                else delete[]kopia;
                            }
                        }
                        break;
                    }
                default: break;
            }
        }
    odrzuc_najgorsze_ruchy_urzytkownika();
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
    void Ruch::obsluga_ruchu_urzytkownika(int zX, int zY, int naX, int naY, char bierka, T_wsk_szachownica wsk_X){//2
//==============================================================================================================
    try{
        if (wsk_X == nullptr)
            throw invalid_argument("Zerowy adres szachownicy.");
        if(zX<0 || 7<zX || zY<0 || 7<zY || naX<0 || 7<naX || naY<0 || 7<naY)
            throw invalid_argument("Wspolzedne szachownicy poza zakresem.");
        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
        kopia[zY][zX] = ' ';
        kopia[naY][naX] = bierka;
        wykonaj_ruch_urzytkownika_jak_mozna(zY, zX, kopia);
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
        void Ruch::wykonaj_ruch_urzytkownika_jak_mozna(int &y, int &x, T_wsk_szachownica kopia){//1
//==============================================================================================================
    try{
        if (x < 0 || 7 < x || y < 0 || 7 < y)
            throw invalid_argument("Wspolzedne szachownicy poza zakresem.");
        if (kopia == nullptr)
            throw invalid_argument("Zerowy adres szachownicy.");
        if(czy_krol_urzytkownika_jest_szachowany==false && x!=polozenie_krola_urzytkownika_x && y!=polozenie_krola_urzytkownika_y && (polozenie_krola_urzytkownika_x+polozenie_krola_urzytkownika_y!=x+y) && (polozenie_krola_urzytkownika_x-polozenie_krola_urzytkownika_y!=x-y))
            wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
        else if(czy_pole_urzytkownika_jest_bite(polozenie_krola_urzytkownika_x, polozenie_krola_urzytkownika_y, kopia)==false)
            wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
        else delete[]kopia;
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
            bool Ruch::czy_pole_urzytkownika_jest_bite(const int &x, const int &y, T_wsk_szachownica kopia){//0+
//==============================================================================================================
    try{
        if (x < 0 || 7 < x || y < 0 || 7 < y)
            throw invalid_argument("Wspolzedne szachownicy poza zakresem.");
        if (kopia == nullptr)
            throw invalid_argument("Zerowy adres szachownicy.");
        for(int i=1; 0<=y-i; i++)                                       //12:00 //BITE PRZEZ HETMANA / WIEZE / GONCA
        {
            if(kopia[y-i][x]==' ') continue;
            if(kopia[y-i][x]=='H' || kopia[y-i][x]=='W') return true;
            else break;
        }
        for(int i=1; 0<=y-i && x+i<=7; i++)                             //1:30
        {
            if(kopia[y-i][x+i]==' ') continue;
            if(kopia[y-i][x+i]=='H' || kopia[y-i][x+i]=='G') return true;
            else break;
        }
        for(int i=1; x+i<=7; i++)                                       //3:00
        {
            if(kopia[y][x+i]==' ') continue;
            if(kopia[y][x+i]=='H' || kopia[y][x+i]=='W') return true;
            else break;
        }
        for(int i=1; y+i<=7 && x+i<=7; i++)                             //4:30
        {
            if(kopia[y+i][x+i]==' ') continue;
            if(kopia[y+i][x+i]=='H' || kopia[y+i][x+i]=='G') return true;
            else break;
        }
        for(int i=1; y+i<=7; i++)                                       //6:00
        {
            if(kopia[y+i][x]==' ') continue;
            if(kopia[y+i][x]=='H' || kopia[y+i][x]=='W') return true;
            else break;
        }
        for(int i=1; y+i<=7 && 0<=x-i; i++)                             //7:30
        {
            if(kopia[y+i][x-i]==' ') continue;
            if(kopia[y+i][x-i]=='H' || kopia[y+i][x-i]=='G') return true;
            else break;
        }
        for(int i=1; 0<=x-i; i++)                                       //9:00
        {
            if(kopia[y][x-i]==' ') continue;
            if(kopia[y][x-i]=='H' || kopia[y][x-i]=='W') return true;
            else break;
        }
        for(int i=1; 0<=y-i && 0<=x-i; i++)                             //10:30
        {
            if(kopia[y-i][x-i]==' ') continue;
            if(kopia[y-i][x-i]=='H' || kopia[y-i][x-i]=='G') return true;
            else break;
        }

        if(0<=y-2 && x+1<=7 && kopia[y-2][x+1]=='S') return true;                       //BITE PRZEZ SKOCZKA
        if(0<=y-1 && x+2<=7 && kopia[y-1][x+2]=='S') return true;
        if(y+1<=7 && x+2<=7 && kopia[y+1][x+2]=='S') return true;
        if(y+2<=7 && x+1<=7 && kopia[y+2][x+1]=='S') return true;
        if(y+2<=7 && 0<=x-1 && kopia[y+2][x-1]=='S') return true;
        if(y+1<=7 && 0<=x-2 && kopia[y+1][x-2]=='S') return true;
        if(0<=y-1 && 0<=x-2 && kopia[y-1][x-2]=='S') return true;
        if(0<=y-2 && 0<=x-1 && kopia[y-2][x-1]=='S') return true;

        if(y<=5)                                                                    //BITE PRZEZ PIONKA
        {
            if(1<=x && kopia[y+1][x-1]=='P') return true;
            if(x<=6 && kopia[y+1][x+1]=='P') return true;
        }
        return false;
//#########################################################################
    }
    catch(const invalid_argument &e){
        obslugiwany_blad blad;
        blad.komunikat = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw blad;
    }
}
        void Ruch::odrzuc_najgorsze_ruchy_urzytkownika(){//1
//==============================================================================================================
    try{
        int liczba_ruchow_urzytkownika = wektor_na_ruchy[numer_mojego_ruchu].size()-1;
        vector<double>status_materialny;
        for(int i=0; i<liczba_ruchow_urzytkownika; i++)                                                  //obliczamy statusy materialne urzytkownika
            status_materialny.push_back(oblicz_status_materialny(wektor_na_ruchy[numer_mojego_ruchu][i+1]));

        int index_komorki_z_najwieksza_wartoscia;
        while(liczba_ruchow_urzytkownika > ile_ruchow_urzytkownika_rozpatrywac[pokolenie_klasy])
        {
            index_komorki_z_najwieksza_wartoscia = 0;
            for(int i=1; i<liczba_ruchow_urzytkownika; i++)        //szukamu komorki z najwiekszym statusem materialnym
                if(status_materialny[index_komorki_z_najwieksza_wartoscia] < status_materialny[i])
                    index_komorki_z_najwieksza_wartoscia = i;
            status_materialny.erase(status_materialny.begin() + index_komorki_z_najwieksza_wartoscia);  //usuwamy ta komorke
            delete[]wektor_na_ruchy[numer_mojego_ruchu][index_komorki_z_najwieksza_wartoscia+1];        //niezapominajac o zwolnieniu pamieci
            wektor_na_ruchy[numer_mojego_ruchu].erase(wektor_na_ruchy[numer_mojego_ruchu].begin() + index_komorki_z_najwieksza_wartoscia + 1);//wektor na ruchy tez nam sie zmienjsza
            liczba_ruchow_urzytkownika--;
        }
//#########################################################################
    }
    catch(obslugiwany_blad &e){
        e.komunikat = __PRETTY_FUNCTION__ + string(" >>\n") + e.komunikat;
        throw;
    }
}
double Ruch::oblicz_status_materialny(const T_wsk_szachownica wsk_X){//0+
//==============================================================================================================
    try{
        if (wsk_X == nullptr)
            throw invalid_argument("Zerowy adres szachownicy.");
        double status_materialny  = 0.0;
        double wartosc_piona      = 1.0;
        double wartosc_skoczka_z_boku    = 2.9;
        double wartosc_skoczka_na_srodku = 3.0;
        double wartosc_gonca_z_boku      = 2.9;
        double wartosc_gonca_na_srodku   = 3.0;
        double wartosc_wiezy      = 5.0;
        double wartosc_hetmana    = 7.0;

        for(int i=0; i<8; i++)
            for(int j=0; j<8; j++)
            {
                switch(wsk_X[i][j])
                {
                    case 'p': status_materialny += (i==6)? -2*wartosc_piona : -wartosc_piona; break;
                    case 's': status_materialny += (i==0 || i==7 || j==0 || j==7)? -wartosc_skoczka_z_boku : -wartosc_skoczka_na_srodku; break;
                    case 'g': status_materialny += (i==0 || i==7 || j==0 || j==7)? -wartosc_gonca_z_boku   : -wartosc_gonca_na_srodku;   break;
                    case 'w': status_materialny += -wartosc_wiezy;   break;
                    case 'h': status_materialny += -wartosc_hetmana; break;

                    case 'P': status_materialny += (i==1)? 2*wartosc_piona : wartosc_piona; break;
                    case 'S': status_materialny += (i==0 || i==7 || j==0 || j==7)? wartosc_skoczka_z_boku : wartosc_skoczka_na_srodku; break;
                    case 'G': status_materialny += (i==0 || i==7 || j==0 || j==7)? wartosc_gonca_z_boku   : wartosc_gonca_na_srodku;   break;
                    case 'W': status_materialny +=  wartosc_wiezy;   break;
                    case 'H': status_materialny +=  wartosc_hetmana; break;
                    default: break;
                }
            }
        return status_materialny;
//#########################################################################
    }
    catch(const invalid_argument &e){
        obslugiwany_blad blad;
        blad.komunikat = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw blad;
    }
}
T_wsk_szachownica Ruch::skopiuj_szachownice(const T_wsk_szachownica oryginal){//0+
//==============================================================================================================
    try{
        if (oryginal == nullptr)
            throw invalid_argument("Proba skopiowania zerowego adresu szachownicy.");
        T_wsk_szachownica kopia = new char[8][8];
        for(int i=0; i<8; i++)
            for(int j=0; j<8; j++)
                kopia[i][j] = oryginal[i][j];
        return kopia;
//#########################################################################
    }
    catch(const bad_alloc &e){
        obslugiwany_blad blad;
        blad.komunikat = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw blad;
    }
    catch(const invalid_argument &e){
        obslugiwany_blad blad;
        blad.komunikat = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw blad;
    }
}


























