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


void loading_screen() {
    erase();

    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_BLUE, COLOR_BLUE);
        init_pair(2, COLOR_WHITE, COLOR_BLACK);
    }

    int lebar_bar = 40;
    int tengah_x = (COLS - lebar_bar - 2) / 2;
    int tengah_y = LINES / 2 - 2;

    attron(A_BOLD);
    mvprintw(tengah_y - 3, (COLS - 9) / 2, "PONG GAME");
    attroff(A_BOLD);

   
    for (int progress = 0; progress <= lebar_bar; progress++) {
       
        attron(COLOR_PAIR(2) | A_BOLD);

        mvaddch(tengah_y, tengah_x, '+');

        for (int x = 1; x <= lebar_bar; x++) {
            mvaddch(tengah_y, tengah_x + x, '-');
        }

        mvaddch(tengah_y, tengah_x + lebar_bar + 1, '+');
        mvaddch(tengah_y + 1, tengah_x, '|');
        mvaddch(tengah_y + 1, tengah_x + lebar_bar + 1, '|');

        mvaddch(tengah_y + 2, tengah_x, '+');

        for (int x = 1; x <= lebar_bar; x++) {
            mvaddch(tengah_y + 2, tengah_x + x, '-');
        }

        mvaddch(tengah_y + 2, tengah_x + lebar_bar + 1, '+');

        attroff(COLOR_PAIR(2) | A_BOLD);
        if (has_colors()) {
            attron(COLOR_PAIR(1));
        }

        for (int x = 0; x < progress; x++) {
            mvaddch(tengah_y + 1, tengah_x + 1 + x, ' ');
        }

        if (has_colors()) {
            attroff(COLOR_PAIR(1));
        }

        attron(A_BOLD);
        mvprintw(tengah_y + 4, (COLS - 12) / 2, "LOADING...");
        attroff(A_BOLD);

        mvprintw(tengah_y + 5, (COLS - 5) / 2,
                 "%3d%%", progress * 100 / lebar_bar);

        refresh();
        napms(60);
    }

    napms(400);
    erase();
    refresh();
}


void hitung_mundur() {
    nodelay(stdscr, FALSE);

    const char* hitungan[] = {"3", "2", "1", "GO!"};

    int tengahX = COLS / 2;
    int tengahY = LINES / 2;

    for (int i = 0; i < 4; i++) {
        erase();

        attron(A_BOLD);
        mvprintw(tengahY - 3, tengahX - 4, "PONG GAME");
        attroff(A_BOLD);

        attron(A_BOLD);
        if (i == 3) {
            mvprintw(tengahY, tengahX - 1, "%s", hitungan[i]); 
        } else {
            mvprintw(tengahY, tengahX, "%s", hitungan[i]);     
        }
        attroff(A_BOLD);

        refresh();
        napms(1000);
    }

    erase();
    refresh();

    nodelay(stdscr, TRUE);
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

        int tengahX = COLS / 2;
        int y_awal = (LINES - 20) / 2;

        mvprintw(y_awal,      tengahX - 35, ".");
        mvprintw(y_awal + 1,  tengahX + 28, "*");
        mvprintw(y_awal + 5,  tengahX - 28, "*");
        mvprintw(y_awal + 6,  tengahX + 20, ".");
        mvprintw(y_awal + 10, tengahX - 38, ".");
        mvprintw(y_awal + 12, tengahX + 32, "*");
        mvprintw(y_awal + 16, tengahX - 30, "*");
        mvprintw(y_awal + 17, tengahX + 25, ".");

        attron(A_BOLD);
        mvprintw(y_awal + 2, tengahX - 8, "P O N G   G A M E");
        attroff(A_BOLD);

        for (int i = 0; i < 4; i++) {
            int baris = y_awal + 4 + (i * 3);

            if (i == pilihan) {
                attron(A_REVERSE | A_BOLD);
                mvprintw(baris,     tengahX - 10, "                    ");
                mvprintw(baris + 1, tengahX - 10, " %s ", opsi[i]);
                mvprintw(baris + 2, tengahX - 10, "                    ");
                attroff(A_REVERSE | A_BOLD);
            } else {
                mvprintw(baris,     tengahX - 10, "+------------------+");
                mvprintw(baris + 1, tengahX - 10, "|%s|", opsi[i]);
                mvprintw(baris + 2, tengahX - 10, "+------------------+");
            }
        }

        mvprintw(y_awal + 17, tengahX - 22, "Gunakan PANAH ATAS/BAWAH & ENTER untuk memilih");
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
            } else if (pilihan == 2) {
                erase();
                int caraX = COLS / 2;
                int caraY = (LINES - 16) / 2;

                attron(A_BOLD);
                mvprintw(caraY, caraX - 11, "C A R A   B E R M A I N");
                attroff(A_BOLD);

                mvprintw(caraY + 4,  caraX - 23, "Player 1 (Kiri)  : Tombol W (Atas) & S (Bawah)");
                mvprintw(caraY + 6,  caraX - 23, "Player 2 (Kanan) : Panah ATAS & Panah BAWAH");
                mvprintw(caraY + 8,  caraX - 23, "Keluar saat main : Tombol Q");
                mvprintw(caraY + 10, caraX - 23, "Pemain pertama mencapai %d poin menang!", skor_akhir);
                mvprintw(caraY + 14, caraX - 19, "Tekan tombol apa saja untuk kembali...");
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

    int x_awal = (COLS - panjang) / 2;   
    int y_awal = (LINES - lebar) / 2 - 2; 

    mvprintw(y_awal - 3, x_awal + 30, "PONG GAME"); 
    mvprintw(y_awal - 2, x_awal + 13, "PLAYER 1: %d", skor1); 
    if (mode_ai) {
        mvprintw(y_awal - 2, x_awal + 43, "KOMPUTER: %d", skor2); 
    } else {
        mvprintw(y_awal - 2, x_awal + 43, "PLAYER 2: %d", skor2); 
    }
    
    for (int x = 0; x < panjang; x++) {
        mvaddch(y_awal, x + x_awal, '-');             
        mvaddch(y_awal + lebar + 1, x + x_awal, '-'); 
    }
    
    for (int y = 1; y < lebar + 1; y++) {
        mvaddch(y + y_awal, panjang / 2 + x_awal, '|'); 
    }

    for (int i = 0; i < ukuran_papan; i++) {
        mvaddch(pemain1 + i + y_awal + 1, x_awal + 2, '|'); 
    }

    for (int i = 0; i < ukuran_papan; i++) {
        mvaddch(pemain2 + i + y_awal + 1, panjang - 3 + x_awal, '|'); 
    }

    mvaddch(bolaY + y_awal + 1, bolaX + x_awal, 'O'); 

    
    if (mode_ai) {
        mvprintw(y_awal + lebar + 3, x_awal + 16, "P1: W/S | LAWAN: AI | Q: QUIT"); 
    } else {
        mvprintw(y_awal + lebar + 3, x_awal + 16, "P1: W/S | P2: UP/DOWN | Q: QUIT"); 
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

    loading_screen();

    while (true) {

        skor1 = 0;
        skor2 = 0;
        pemain1 = lebar / 2;
        pemain2 = lebar / 2;
        resetbola();

        tampilkan_menu();
        hitung_mundur();

        while (skor1 < skor_akhir && skor2 < skor_akhir) {
            arena();
            gerakin_papan();
            if (mode_ai) {
                com();
            }
            gerakin_bola();
            napms(75);
        }

        nodelay(stdscr, FALSE);
        erase();
        int tengahX = COLS / 2;
        int tengahY = LINES / 2;

        attron(A_BOLD);
        mvprintw(tengahY - 1, tengahX - 9, "G A M E   O V E R");
        attroff(A_BOLD);
        mvprintw(tengahY + 1, tengahX - 18, "Tekan ENTER untuk melihat pemenang...");
        refresh();

        int tombol;
        while (true) {
            tombol = getch();
            if (tombol == 10 || tombol == KEY_ENTER) {
                break;
            }
        }

        erase();
        attron(A_BOLD);
        if (skor1 >= skor_akhir) {
            mvprintw(tengahY - 2, tengahX - 8, "PLAYER 1 MENANG!");
        } else {
            if (mode_ai) {
                mvprintw(tengahY - 2, tengahX - 8, "KOMPUTER MENANG!");
            } else {
                mvprintw(tengahY - 2, tengahX - 8, "PLAYER 2 MENANG!");
            }
        }
        attroff(A_BOLD);

        mvprintw(tengahY + 3, tengahX - 21, "Tekan [R] untuk ke Menu  |  [Q] untuk Quit");
        refresh();

        while (true) {
            tombol = getch();
            if (tombol == 'r' || tombol == 'R') {
                break;
            } else if (tombol == 'q' || tombol == 'Q') {
                endwin();
                return 0;
            }
        }

        nodelay(stdscr, TRUE);
    }

    endwin();
    return 0;
}