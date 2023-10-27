#include "Notatnik.h"
using namespace std;
using T_wsk_szachownica = char(*)[8];







Notatnik::Notatnik()
{
    szachownica = zainicjalizuj_szachownice();
}
Notatnik::~Notatnik()
{
    delete[]szachownica;
}
T_wsk_szachownica Notatnik::zainicjalizuj_szachownice()
{
    return new char[8][8]{{'w','s','g',kolor?'k':'h',kolor?'h':'k','g','s','w'},
                          {'p','p','p','p','p','p','p','p'},
                          {' ',' ',' ',' ',' ',' ',' ',' '},
                          {' ',' ',' ',' ',' ',' ',' ',' '},
                          {' ',' ',' ',' ',' ',' ',' ',' '},
                          {' ',' ',' ',' ',' ',' ',' ',' '},
                          {'P','P','P','P','P','P','P','P'},
                          {'W','S','G',kolor?'K':'H',kolor?'H':'K','G','S','W'}};
}

string Notatnik::podaj_notacje_tego_ruchu(int ruch)
{
    rozszyfruj_ruch(ruch);
    zaktualizuj_polozenia_krolow();
}



void Notatnik::rozszyfruj_ruch(int ruch)
{
    ruch_naY = ruch % 10;
    ruch /=10;
    ruch_naX = ruch % 10;
    ruch /=10;
    ruch_zY  = ruch % 10;
    ruch /=10;
    ruch_zX  = ruch;
    ruch /=10;
    parametr_dodatkowy = ruch;
}


void Notatnik::zaktualizuj_polozenia_krolow()
{
    for(int i=7; i>=0; i--)
        for(int j=0; j<8; j++)
            if(szachownica[i][j]=='K')
            {
                krol_maszyny_x=j;
                krol_maszyny_y=i;
                break;
            }
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
            if(szachownica[i][j]=='k')
            {
                krol_urzytkownika_x=j;
                krol_urzytkownika_y=i;
                break;
            }
}





bool Notatnik::czy_jest_szach_dla_urzytkownika()
{
    int x = krol_urzytkownika_x;
    int y = krol_urzytkownika_y;

    for(int i=1; 0<=y-i; i++)                                       //12:00 //BITE PRZEZ HETMANA / WIEZE / GONCA
    {
        if(szachownica[y-i][x]==' ') continue;
        if(szachownica[y-i][x]=='H' || szachownica[y-i][x]=='W') return true;
        else break;
    }
    for(int i=1; 0<=y-i && x+i<=7; i++)                             //1:30
    {
        if(szachownica[y-i][x+i]==' ') continue;
        if(szachownica[y-i][x+i]=='H' || szachownica[y-i][x+i]=='G') return true;
        else break;
    }
    for(int i=1; x+i<=7; i++)                                       //3:00
    {
        if(szachownica[y][x+i]==' ') continue;
        if(szachownica[y][x+i]=='H' || szachownica[y][x+i]=='W') return true;
        else break;
    }
    for(int i=1; y+i<=7 && x+i<=7; i++)                             //4:30
    {
        if(szachownica[y+i][x+i]==' ') continue;
        if(szachownica[y+i][x+i]=='H' || szachownica[y+i][x+i]=='G') return true;
        else break;
    }
    for(int i=1; y+i<=7; i++)                                       //6:00
    {
        if(szachownica[y+i][x]==' ') continue;
        if(szachownica[y+i][x]=='H' || szachownica[y+i][x]=='W') return true;
        else break;
    }
    for(int i=1; y+i<=7 && 0<=x-i; i++)                             //7:30
    {
        if(szachownica[y+i][x-i]==' ') continue;
        if(szachownica[y+i][x-i]=='H' || szachownica[y+i][x-i]=='G') return true;
        else break;
    }
    for(int i=1; 0<=x-i; i++)                                       //9:00
    {
        if(szachownica[y][x-i]==' ') continue;
        if(szachownica[y][x-i]=='H' || szachownica[y][x-i]=='W') return true;
        else break;
    }
    for(int i=1; 0<=y-i && 0<=x-i; i++)                             //10:30
    {
        if(szachownica[y-i][x-i]==' ') continue;
        if(szachownica[y-i][x-i]=='H' || szachownica[y-i][x-i]=='G') return true;
        else break;
    }

    if(0<=y-2 && x+1<=7 && szachownica[y-2][x+1]=='S') return true;                       //BITE PRZEZ SKOCZKA
    if(0<=y-1 && x+2<=7 && szachownica[y-1][x+2]=='S') return true;
    if(y+1<=7 && x+2<=7 && szachownica[y+1][x+2]=='S') return true;
    if(y+2<=7 && x+1<=7 && szachownica[y+2][x+1]=='S') return true;
    if(y+2<=7 && 0<=x-1 && szachownica[y+2][x-1]=='S') return true;
    if(y+1<=7 && 0<=x-2 && szachownica[y+1][x-2]=='S') return true;
    if(0<=y-1 && 0<=x-2 && szachownica[y-1][x-2]=='S') return true;
    if(0<=y-2 && 0<=x-1 && szachownica[y-2][x-1]=='S') return true;

    if(y<=5)                                                                    //BITE PRZEZ PIONKA
    {
        if(1<=x && szachownica[y+1][x-1]=='P') return true;
        if(x<=6 && szachownica[y+1][x+1]=='P') return true;
    }
    return false;
}
bool Notatnik::czy_jest_szach_dla_maszyny()
{
    int x = krol_maszyny_x;
    int y = krol_maszyny_y;

    for(int i=1; 0<=y-i; i++)                                       //12:00 //BITE PRZEZ HETMANA / WIEZE / GONCA
    {
        if(szachownica[y-i][x]==' ') continue;
        if(szachownica[y-i][x]=='h' || szachownica[y-i][x]=='w') return true;
        else break;
    }
    for(int i=1; 0<=y-i && x+i<=7; i++)                             //1:30
    {
        if(szachownica[y-i][x+i]==' ') continue;
        if(szachownica[y-i][x+i]=='h' || szachownica[y-i][x+i]=='g') return true;
        else break;
    }
    for(int i=1; x+i<=7; i++)                                       //3:00
    {
        if(szachownica[y][x+i]==' ') continue;
        if(szachownica[y][x+i]=='h' || szachownica[y][x+i]=='w') return true;
        else break;
    }
    for(int i=1; y+i<=7 && x+i<=7; i++)                             //4:30
    {
        if(szachownica[y+i][x+i]==' ') continue;
        if(szachownica[y+i][x+i]=='h' || szachownica[y+i][x+i]=='g') return true;
        else break;
    }
    for(int i=1; y+i<=7; i++)                                       //6:00
    {
        if(szachownica[y+i][x]==' ') continue;
        if(szachownica[y+i][x]=='h' || szachownica[y+i][x]=='w') return true;
        else break;
    }
    for(int i=1; y+i<=7 && 0<=x-i; i++)                             //7:30
    {
        if(szachownica[y+i][x-i]==' ') continue;
        if(szachownica[y+i][x-i]=='h' || szachownica[y+i][x-i]=='g') return true;
        else break;
    }
    for(int i=1; 0<=x-i; i++)                                       //9:00
    {
        if(szachownica[y][x-i]==' ') continue;
        if(szachownica[y][x-i]=='h' || szachownica[y][x-i]=='w') return true;
        else break;
    }
    for(int i=1; 0<=y-i && 0<=x-i; i++)                             //10:30
    {
        if(szachownica[y-i][x-i]==' ') continue;
        if(szachownica[y-i][x-i]=='h' || szachownica[y-i][x-i]=='g') return true;
        else break;
    }

    if(0<=y-2 && x+1<=7 && szachownica[y-2][x+1]=='s') return true;                       //BITE PRZEZ SKOCZKA
    if(0<=y-1 && x+2<=7 && szachownica[y-1][x+2]=='s') return true;
    if(y+1<=7 && x+2<=7 && szachownica[y+1][x+2]=='s') return true;
    if(y+2<=7 && x+1<=7 && szachownica[y+2][x+1]=='s') return true;
    if(y+2<=7 && 0<=x-1 && szachownica[y+2][x-1]=='s') return true;
    if(y+1<=7 && 0<=x-2 && szachownica[y+1][x-2]=='s') return true;
    if(0<=y-1 && 0<=x-2 && szachownica[y-1][x-2]=='s') return true;
    if(0<=y-2 && 0<=x-1 && szachownica[y-2][x-1]=='s') return true;

    if(2<=y)                                                                    //BITE PRZEZ PIONKA
    {
        if(1<=x && szachownica[y-1][x-1]=='p') return true;
        if(x<=6 && szachownica[y-1][x+1]=='p') return true;
    }
    return false;

}
