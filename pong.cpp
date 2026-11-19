#include <iostream>
#include <ncurses.h>
#include <windows.h>
using namespace std;

const int panjang = 70;
const int lebar = 20;
int pemain1 = lebar / 2;
int pemain2 = lebar / 2;

int bolaX = panjang / 2;
int bolaY = lebar / 2;
int bolaFX = 1;
int bolaFY = 1;
int skor1 = 0;
int skor2 = 0;

const int ukuran_papan = 4;
const int skor_akhir = 5;

void resetbola() {
    bolaX = panjang / 2;
    bolaY = lebar / 2;

    bolaFX = 2;
    bolaFY = 1;
}

void arena() {
    erase();

    mvprintw(0, 20, "PONG GAME");
    mvprintw(1, 15, "PLAYER 1: %d", skor1);
    mvprintw(1, 40, "PLAYER 2: %d", skor2);

    for (int x = 0; x < panjang; x++) {
        mvaddch(3, x + 2, '-');
        mvaddch(lebar + 4, x + 2, '-');
    }

    for (int y = 4; y < lebar + 4; y++) {
        mvaddch(y, panjang / 2 + 2, '|');
    }

    for (int i = 0; i < ukuran_papan; i++) {
        mvaddch(pemain1 + i + 4, 4, '|');
    }

    for (int i = 0; i < ukuran_papan; i++) {
        mvaddch(pemain2 + i + 4, WIDTH - 1, '|');
    }

    mvaddch(bolaY + 4, bolaX + 2, 'O');

    mvprintw(lebar + 6, 10,
             "P1: W/S | P2: UP/DOWN | Q: QUIT");

    refresh();
}
