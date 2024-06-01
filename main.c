#include "dosya_islemleri.h"
#include "dizin_islemleri.h"
void dosyaIslemleriMenu();
void anaMenuGoster();
void log_massage(const char *format, ...) {
    // Tarih ve saat bilgisini al
    time_t t;
    struct tm *tm_info;
    time(&t);
    tm_info = localtime(&t);

    // Tarih ve saat bilgisini string olarak formatla
    char zaman[20];
    strftime(zaman, 20, "%Y-%m-%d %H:%M:%S", tm_info);

    // Log dosyasını oluştur ve aç
    FILE *logDosyasi = fopen("log.txt", "a");
    if (logDosyasi == NULL) {
        printf("Log dosyasi olusturulamadi.");
        return;
    }

    // Log dosyasına yazılacak içerik
    va_list args;
    va_start(args, format);

    fprintf(logDosyasi, "%s ", zaman);
    vfprintf(logDosyasi, format, args);
    fprintf(logDosyasi, "\n");

    va_end(args);

    // Log dosyasını kapat
    fclose(logDosyasi);
}


void dosyaIslemleriMenu() {
    char *menu[] = {"Temel Islemler", "Gelismis Islemler","Geri"};
    int menu_length = sizeof(menu) / sizeof(menu[0]);
    int current_item = 0;

    while (1) {
        clear();

        // Menüyü ekrana yazdır
        for (int i = 0; i < menu_length; ++i) {
            if (i == current_item) {
                attron(A_REVERSE);
                printw("%s\n", menu[i]);
                attroff(A_REVERSE);
            } else {
                printw("%s\n", menu[i]);
            }
        }

        refresh();

        int key = getch();

        switch (key) {
            case KEY_UP:
                current_item = (current_item - 1 + menu_length) % menu_length;
                break;
            case KEY_DOWN:
                current_item = (current_item + 1) % menu_length;
                break;
            case 10: // Enter tuşu
                // Seçilen menü öğesine göre ilgili işlem fonksiyonunu çağır
                switch (current_item) {
                    case 0:
                        // Dosya Ac fonksiyonu çağır
                        temelIslemler();
                        break;
                    case 1:
                        // Dosya Yaz fonksiyonu çağır
                        gelismisIslemler();
                        break;
                    case 2:
                        anaMenuGoster();
                        break;
                }
                break;
        }
    }
}

void dizinIslemleriMenu() {
    char *menu[] = {"Dizin Islemleri","Arama Fonksiyonlari","Geri"};
    int menu_length = sizeof(menu) / sizeof(menu[0]);
    int current_item = 0;

    while (1) {
        clear();

        // Menüyü ekrana yazdır
        for (int i = 0; i < menu_length; ++i) {
            if (i == current_item) {
                attron(A_REVERSE);
                printw("%s\n", menu[i]);
                attroff(A_REVERSE);
            } else {
                printw("%s\n", menu[i]);
            }
        }

        refresh();

        int key = getch();

        switch (key) {
            case KEY_UP:
                current_item = (current_item - 1 + menu_length) % menu_length;
                break;
            case KEY_DOWN:
                current_item = (current_item + 1) % menu_length;
                break;
            case 10: // Enter tuşu
                // Seçilen menü öğesine göre ilgili işlem fonksiyonunu çağır
                switch (current_item) {
                    case 0:
                        dizin_temelIslemler();
                        break;
                    case 1:
                        dizin_aramaFonksiyonlari();  
                        break;
                    case 2:
                        anaMenuGoster();
                        break;
                }
                break;
        }
    }
}

void anaMenuGoster() {
    char *menu[] = {"Dosya Islemleri", "Dizin Islemleri","Programi Sonlandir"};
    int menu_length = sizeof(menu) / sizeof(menu[0]);
    int current_item = 0;

    while (1) {
        clear();

        // Menüyü ekrana yazdır
        for (int i = 0; i < menu_length; ++i) {
            if (i == current_item) {
                attron(A_REVERSE);
                printw("%s\n", menu[i]);
                attroff(A_REVERSE);
            } else {
                printw("%s\n", menu[i]);
            }
        }

        refresh();

        int key = getch();

        switch (key) {
            case KEY_UP:
                current_item = (current_item - 1 + menu_length) % menu_length;
                break;
            case KEY_DOWN:
                current_item = (current_item + 1) % menu_length;
                break;
            case 10: // Enter tuşu
                // Seçilen menü öğesine göre işlem yap
                switch (current_item) {
                    case 0:
                        // Dosya İşlemleri menüsünü göster
                        dosyaIslemleriMenu();
                        break;
                    case 1:
                        // Dizin İşlemleri menüsünü göster
                        dizinIslemleriMenu();
                        break;
                    case 2:
                        endwin();
                        exit(0);
                        break;
                        
                }
                break;
        }
    }
}

int main() {
    initscr();
    keypad(stdscr, TRUE);
    curs_set(0);

    // Ana menüyü göster
    anaMenuGoster();

    endwin();
    return 0;
}