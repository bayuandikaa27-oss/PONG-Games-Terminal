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
void tampilkan_menu() {
    nodelay(stdscr, FALSE); 
    int pilihan = 0;
    const char* opsi[3] = {
        "1. Mulai Game",
        "2. Cara Bermain",
        "3. Keluar"
    };

    while (true) {
        erase();
        mvprintw(3, 25, "======================");
        mvprintw(4, 25, "      PONG GAME       ");
        mvprintw(5, 25, "======================");

        for (int i = 0; i < 3; i++) {
            if (i == pilihan) {
                attron(A_REVERSE); 
                mvprintw(8 + i * 2, 27, "%s", opsi[i]);
                attroff(A_REVERSE);
            } else {
                mvprintw(8 + i * 2, 27, "%s", opsi[i]);
            }
        }

        mvprintw(16, 18, "Gunakan PANAH ATAS/BAWAH & ENTER untuk memilih");
        refresh();

        int input = getch();
        if (input == KEY_UP) {
            pilihan = (pilihan - 1 + 3) % 3;
        } else if (input == KEY_DOWN) {
            pilihan = (pilihan + 1) % 3;
        } else if (input == 10 || input == KEY_ENTER) { 
            if (pilihan == 0) {
                break; 
            } else if (pilihan == 1) {
                
                erase();
                mvprintw(4, 25, "=== CARA BERMAIN ===");
                mvprintw(7, 15, "Player 1 (Kiri)  : Tombol W (Atas) & S (Bawah)");
                mvprintw(9, 15, "Player 2 (Kanan) : Panah ATAS & Panah BAWAH");
                mvprintw(11, 15, "Keluar saat main : Tombol Q");
                mvprintw(13, 15, "Pemain pertama mencapai %d poin menang!", skor_akhir);
                mvprintw(17, 18, "Tekan tombol apa saja untuk kembali...");
                refresh();
                getch();
            } else if (pilihan == 2) {
                endwin();
                exit(0);
            }
        }
    }
    nodelay(stdscr, TRUE); 
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
        mvaddch(pemain2 + i + 4, panjang - 1, '|');
    }

    mvaddch(bolaY + 4, bolaX + 2, 'O');

    mvprintw(lebar + 6, 10,
             "P1: W/S | P2: UP/DOWN | Q: QUIT");

    refresh();
}

void gerakin_papan() {
    int input = getch();
    switch (input) {
        case 'w':
        case 'W':
            if (pemain1 > 4) {
                pemain1--;
            }
            break;
        case 's':
        case 'S':
            if (pemain1 < lebar + 3 - ukuran_papan) {
                pemain1++;
            }
            break;
        case KEY_UP:
            if (pemain2 > 4) {
                pemain2--;
            }
            break;
        case KEY_DOWN:
            if (pemain2 < lebar + 3 - ukuran_papan) {
                pemain2++;
            }
            break;
        case 'q':
        case 'Q':
            endwin();
            exit(0);
    }
}

void gerakin_bola() {
    bolaX += bolaFX;
    bolaY += bolaFY;

    if (bolaY <= 4 || bolaY >= lebar + 3) {
        bolaFY *= -1;
    }

    if (bolaX == 5) {
        if (bolaY >= pemain1 + 4 && bolaY < pemain1 + 4 + ukuran_papan) {
            bolaFX *= -1;
        } else {
            skor2++;
            resetbola();
        }
    }

    if (bolaX == panjang + 1) {
        if (bolaY >= pemain2 + 4 && bolaY < pemain2 + 4 + ukuran_papan) {
            bolaFX *= -1;
        } else {
            skor1++;
            resetbola();
        }
    }
}
int main() {
    
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE); 

    tampilkan_menu();

    while (skor1 < skor_akhir && skor2 < skor_akhir) {
        arena();
        gerakin_papan();
        gerakin_bola();
        napms(60);
    }

    
    nodelay(stdscr, FALSE);
    erase();
    if (skor1 >= skor_akhir) {
        mvprintw(10, 25, "PLAYER 1 MENANG!");
    } else {
        mvprintw(10, 25, "PLAYER 2 MENANG!");
    }
    mvprintw(12, 20, "Tekan tombol apa saja untuk keluar...");
    refresh();
    getch();

    endwin();
    return 0;
}
