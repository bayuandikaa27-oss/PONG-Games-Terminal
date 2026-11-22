#include <iostream>
#include <ncurses.h>
#include <unistd.h>

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

bool mode_ai = false;

const int ukuran_papan = 4;
const int skor_akhir = 7;

void resetbola() {
    bolaX = panjang / 2;
    bolaY = lebar / 2;

    bolaFX = 1;
    bolaFY = 1;
}
void tampilkan_menu() {
    nodelay(stdscr, FALSE);
    int pilihan = 0;
    const char* opsi[4] = {
        "    P1 vs P2      ",
        "    P1 vs COM     ",
        "   CARA BERMAIN   ",
        "      KELUAR      "
        
    };

    while (true) {
        erase();

        
        mvprintw(4,  25, ".");
        mvprintw(5,  88, "*");
        mvprintw(9,  32, "*");
        mvprintw(10, 80, ".");
        mvprintw(14, 22, ".");
        mvprintw(16, 92, "*");
        mvprintw(20, 30, "*");
        mvprintw(21, 85, ".");

        
        attron(A_BOLD);
        mvprintw(6, 51, "P O N G   G A M E");
        attroff(A_BOLD);

        
        for (int i = 0; i < 4; i++) {
            int baris = 8 + (i * 3); 

            if (i == pilihan) {
                
                attron(A_REVERSE | A_BOLD);
                mvprintw(baris,     50, "                    ");
                mvprintw(baris + 1, 50, " %s ", opsi[i]);
                mvprintw(baris + 2, 50, "                    ");
                attroff(A_REVERSE | A_BOLD);
            } else {
                
                mvprintw(baris,     50, "+------------------+");
                mvprintw(baris + 1, 50, "|%s|", opsi[i]);
                mvprintw(baris + 2, 50, "+------------------+");
            }
        }

        
        mvprintw(21, 38, "Gunakan PANAH ATAS/BAWAH & ENTER untuk memilih");
        refresh();

        int input = getch();
        if (input == KEY_UP) {
            pilihan = (pilihan - 1 + 4) % 4;
        } else if (input == KEY_DOWN) {
            pilihan = (pilihan + 1) % 4;
        } else if (input == 10 || input == KEY_ENTER) {
            if (pilihan == 0) {
                mode_ai = false;
                break;
            } else if (pilihan == 1) {
                mode_ai = true;
                break;
            }else if (pilihan == 2) {
                erase();
                attron(A_BOLD);
                mvprintw(6, 48, "C A R A   B E R M A I N");
                attroff(A_BOLD);

                mvprintw(10, 37, "Player 1 (Kiri)  : Tombol W (Atas) & S (Bawah)");
                mvprintw(12, 37, "Player 2 (Kanan) : Panah ATAS & Panah BAWAH");
                mvprintw(14, 37, "Keluar saat main : Tombol Q");
                mvprintw(16, 37, "Pemain pertama mencapai %d poin menang!", skor_akhir);
                mvprintw(20, 41, "Tekan tombol apa saja untuk kembali...");
                refresh();
                getch();
            } else if (pilihan == 3) {
                endwin();
                exit(0);

            }
        }
    }
    nodelay(stdscr, TRUE);
}
void arena() {
    erase();

    mvprintw(0, 32, "PONG GAME");
    mvprintw(1, 15, "PLAYER 1: %d", skor1);
    if (mode_ai) {
    mvprintw(1, 45, "KOMPUTER: %d", skor2);
    }else {
        mvprintw(1, 45, "PLAYER 2: %d", skor2);
    }

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

    if (mode_ai) {
    mvprintw(lebar + 6, 18, "P1: W/S | LAWAN: AI | Q: QUIT");
    }else {
        mvprintw(lebar + 6, 18, "P1: W/S | P2: UP/DOWN | Q: QUIT");
    }

    refresh();
}

void gerakin_papan() {

    int input = getch();
    switch (input) {
        case 'w':
        case 'W':
            if (pemain1 > 0) {
                pemain1--;
            }
            break;
        case 's':
        case 'S':
            if (pemain1 < lebar - ukuran_papan) {
                pemain1++;
            }
            break;
        case KEY_UP:
            if (pemain2 > 0) {
                pemain2--;
            }
            break;
        case KEY_DOWN:
            if (pemain2 < lebar - ukuran_papan) {
                pemain2++;
            }
            break;
        case 'q':
        case 'Q':
            endwin();
            exit(0);
    }
}
void com() {
    if (bolaFX > 0) {
        int tengah_papan = pemain2 + (ukuran_papan / 2);
        if (bolaY < tengah_papan && pemain2 > 0) {
            pemain2--;
        } else if (bolaY > tengah_papan && pemain2 < lebar - ukuran_papan) {
            pemain2++;
        }
    }
}
void gerakin_bola() {
    bolaX += bolaFX;
    bolaY += bolaFY;

    if (bolaY <= 0 || bolaY >= lebar - 1) {
        bolaFY *= -1;
    }

    if (bolaX == 3) {
        if (bolaY >= pemain1 && bolaY < pemain1 + ukuran_papan) {
            bolaFX *= -1;
        } else {
            skor2++;
            resetbola();
        }
    }

    if (bolaX == panjang - 4) {
        if (bolaY >= pemain2 && bolaY < pemain2 + ukuran_papan) {
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
    if (lawan_com) {
        com(); 
    }
    gerakin_bola();
    napms(75); 
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
