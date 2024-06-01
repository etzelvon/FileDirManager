#include "dosya_islemleri.h"

// Dosya adlarını alıp diziye atayan fonksiyon
char** dosya_adlarini_al(int* dosya_sayisi) {
    // Mevcut dizini aç
    DIR* dizin = opendir(".");
    struct dirent* dosya;

    char** dosya_adi = NULL;
    int sayac = 0;

    if (dizin) {
        // Mevcut dizindeki dosya adlarını al
        while ((dosya = readdir(dizin)) != NULL) {
            // "." ve ".." dosyalarını atla
            if (strcmp(dosya->d_name, ".") != 0 && strcmp(dosya->d_name, "..") != 0) {
                sayac++;
                // Diziyi yeniden boyutlandır
                dosya_adi = (char**)realloc(dosya_adi, sayac * sizeof(char*));
                dosya_adi[sayac - 1] = strdup(dosya->d_name);
            }
        }
        closedir(dizin);
    }

    *dosya_sayisi = sayac;
    return dosya_adi;
}

void dosya_secimi_goster(char** dosya_adi, int dosya_sayisi, int secilen_index, int* secilen_dosyalar_index) {
    // Seçimi tamamla seçeneğini ekrana yazdır
    mvprintw(0, 20, "Seçimi tamamlamak için 'c' tusuna basin\n");
    mvprintw(1, 17, "'f' tusuna basarak dosya adini yazabilirsiniz\n");
    mvprintw(2, 20, "ya da enter tusu ile seccim yapabilirsiniz\n");

    // Dosya adlarını ekrana yazdır
    for (int i = 0; i < dosya_sayisi; ++i) {
        if (i  == secilen_index) {
            attron(A_REVERSE);
            attron(COLOR_PAIR(1)); // Kırmızı rengi etkinleştir
            printw("%s\n", dosya_adi[i]);
            attroff(COLOR_PAIR(1)); // Kırmızı rengi kapat
            attroff(A_REVERSE);
        } else {
            attron(COLOR_PAIR(2)); // Beyaz rengi etkinleştir
            printw("%s", dosya_adi[i]);
            if (secilen_dosyalar_index[i] == 1) {
                printw(" (Seçildi)\n");
            } else {
                printw("\n");
            }
            attroff(COLOR_PAIR(2)); // Beyaz rengi kapat
        }
    }
}

char** dosya_sec() {
    int dosya_sayisi;
    char** dosya_adi = dosya_adlarini_al(&dosya_sayisi);

    // Ncurses ekran başlatma
    initscr();
    // Ekranın karakter girişlerini anlamlandırmasını sağla
    keypad(stdscr, TRUE);
    // Ekranı temizle
    clear();

    // Renk çiftlerini tanımla
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK); // Kırmızı renk
    init_pair(2, COLOR_WHITE, COLOR_BLACK); // Beyaz renk

    // Seçilen dosyaları tutacak dizi
    char** secilen_dosyalar = (char**)malloc(0);
    int secilen_dosya_sayisi = 0;

    int* secilen_dosyalar_index = (int*)calloc(dosya_sayisi, sizeof(int));
    // Dosyaları listele ve seçim yap
    int secilen_index = 0;

    while (1) {
        // Dosyaları renkli bir şekilde ekrana yazdır
        dosya_secimi_goster(dosya_adi, dosya_sayisi, secilen_index, secilen_dosyalar_index);

        refresh();

        int key = getch();

        switch (key) {
            case KEY_UP:
                secilen_index = (secilen_index - 1 + dosya_sayisi) % dosya_sayisi;
                break;
            case KEY_DOWN:
                secilen_index = (secilen_index + 1) % dosya_sayisi;
                break;
            case 'c' : 
                // Seçimi tamamla seçeneği seçildi
                endwin(); // Ncurses'i kapat
                free(dosya_adi); // Bellek sızıntısını önlemek için belleği serbest bırak
                // Sadece seçilen dosyaları iade et
                char** secilenler = (char**)malloc((secilen_dosya_sayisi + 1) * sizeof(char*));
                for (int i = 0; i < secilen_dosya_sayisi; ++i) {
                    secilenler[i] = strdup(secilen_dosyalar[i]);
                    free(secilen_dosyalar[i]);
                }
                secilenler[secilen_dosya_sayisi] = NULL; // Sonlandırıcı ekleyin
                free(secilen_dosyalar);
                return secilenler;
                break;
            case 10: // Enter tuşu
                // Dosya seçildi
                if (secilen_dosyalar_index[secilen_index] == 1) {
                    // Eğer dosya zaten seçilmişse, seçimden çıkar
                    secilen_dosyalar_index[secilen_index] = 0;
                    for (int i = 0; i < secilen_dosya_sayisi; ++i) {
                        if (strcmp(dosya_adi[secilen_index], secilen_dosyalar[i]) == 0) {
                            free(secilen_dosyalar[i]);
                            for (int j = i; j < secilen_dosya_sayisi - 1; ++j) {
                                secilen_dosyalar[j] = secilen_dosyalar[j + 1];
                            }
                            secilen_dosyalar = (char**)realloc(secilen_dosyalar, (secilen_dosya_sayisi - 1) * sizeof(char*));
                            secilen_dosya_sayisi--;
                            break;
                        }
                    }
                } else {
                    // Eğer dosya seçilmemişse, seç
                    secilen_dosyalar_index[secilen_index] = 1;
                    secilen_dosyalar = (char**)realloc(secilen_dosyalar, (secilen_dosya_sayisi + 1) * sizeof(char*));
                    secilen_dosyalar[secilen_dosya_sayisi] = strdup(dosya_adi[secilen_index]);
                    secilen_dosya_sayisi++;
                }
                break;
            case 27: // ESC tuşu
                // Seçim işlemini iptal et
                endwin(); // Ncurses'i kapat
                free(dosya_adi); // Bellek sızıntısını önlemek için belleği serbest bırak
                for (int i = 0; i < secilen_dosya_sayisi; ++i) {
                    free(secilen_dosyalar[i]);
                }
                free(secilen_dosyalar);
                return NULL;
            case 'f': // f tuşu
                {
                    // Dosya seçimi aktifleşti
                    // Dosya adını girmek için bir pencere aç
                    clear();
                    mvprintw(0, 0, "Dosya adini girin ve Enter tusuna basin: ");
                    char dosya_adi_giris[256];
                    echo(); // Kullanıcının girdiği metni ekranda göster
                    getstr(dosya_adi_giris);
                    noecho(); // Kullanıcının girdiği metni ekranda gizle
                    clear();
                    // Girilen dosyanın listede olup olmadığını kontrol et
                    int dosya_var_mi = 0;
                    int secilen_dosya_index = -1; // Seçilen dosyanın listedeki indeksi
                    for (int i = 0; i < dosya_sayisi; ++i) {
                        if (strcmp(dosya_adi_giris, dosya_adi[i]) == 0) {
                            dosya_var_mi = 1;
                            secilen_dosya_index = i;
                            break;
                        }
                    }
                    if (dosya_var_mi) {
                        // Dosya listede varsa seçim işlemi yap
                        // Dosyanın zaten seçili olup olmadığını kontrol et
                        if (secilen_dosyalar_index[secilen_dosya_index] == 0) {
                            // Dosya zaten seçili değilse seç ve işaretle
                            secilen_dosyalar_index[secilen_dosya_index] = 1;
                            secilen_dosyalar = (char**)realloc(secilen_dosyalar, (secilen_dosya_sayisi + 1) * sizeof(char*));
                            secilen_dosyalar[secilen_dosya_sayisi] = strdup(dosya_adi[secilen_dosya_index]);
                            secilen_dosya_sayisi++;
                        } else {
                            // Dosya zaten seçiliyse listeden çıkar ve işaretlemeyi kaldır
                            for (int i = 0; i < secilen_dosya_sayisi; ++i) {
                                if (strcmp(dosya_adi_giris, secilen_dosyalar[i]) == 0) {
                                    free(secilen_dosyalar[i]);
                                    for (int j = i; j < secilen_dosya_sayisi - 1; ++j) {
                                        secilen_dosyalar[j] = secilen_dosyalar[j + 1];
                                    }
                                    secilen_dosyalar = (char**)realloc(secilen_dosyalar, (secilen_dosya_sayisi - 1) * sizeof(char*));
                                    secilen_dosyalar_index[secilen_dosya_index] = 0; // İşaretlemeyi kaldır
                                    secilen_dosya_sayisi--;
                                    break;
                                }
                            }
                        }
                    }
                }
                break;


        }
    }
}

char** tek_dosya_sec() {
    int dosya_sayisi;
    char** dosya_adi = dosya_adlarini_al(&dosya_sayisi);

    // Ncurses ekran başlatma
    initscr();
    // Ekranın karakter girişlerini anlamlandırmasını sağla
    keypad(stdscr, TRUE);
    // Ekranı temizle
    clear();

    // Renk çiftlerini tanımla
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK); // Kırmızı renk
    init_pair(2, COLOR_WHITE, COLOR_BLACK); // Beyaz renk

    // Seçilen dosyaları tutacak dizi
    char** secilen_dosyalar = (char**)malloc(0);
    int secilen_dosya_sayisi = 0;

    int* secilen_dosyalar_index = (int*)calloc(dosya_sayisi, sizeof(int));
    // Dosyaları listele ve seçim yap
    int secilen_index = 0;

    while (1) {
        // Dosyaları renkli bir şekilde ekrana yazdır
        dosya_secimi_goster(dosya_adi, dosya_sayisi, secilen_index, secilen_dosyalar_index);

        refresh();

        int key = getch();

        switch (key) {
            case KEY_UP:
                secilen_index = (secilen_index - 1 + dosya_sayisi) % dosya_sayisi;
                break;
            case KEY_DOWN:
                secilen_index = (secilen_index + 1) % dosya_sayisi;
                break;
            case 'c' : 
                // Seçimi tamamla seçeneği seçildi
                endwin(); // Ncurses'i kapat
                free(dosya_adi); // Bellek sızıntısını önlemek için belleği serbest bırak
                // Sadece seçilen dosyaları iade et
                char** secilenler = (char**)malloc((secilen_dosya_sayisi + 1) * sizeof(char*));
                for (int i = 0; i < secilen_dosya_sayisi; ++i) {
                    secilenler[i] = strdup(secilen_dosyalar[i]);
                    free(secilen_dosyalar[i]);
                }
                secilenler[secilen_dosya_sayisi] = NULL; // Sonlandırıcı
                free(secilen_dosyalar);
                return secilenler;
                break;
            case 10: // Enter tuşu
                // Dosya seçildi
                if (secilen_dosyalar_index[secilen_index] == 1) {
                    // Eğer dosya zaten seçilmişse, seçimi iptal et
                    secilen_dosyalar_index[secilen_index] = 0;
                    for (int i = 0; i < secilen_dosya_sayisi; ++i) {
                        if (strcmp(dosya_adi[secilen_index], secilen_dosyalar[i]) == 0) {
                            free(secilen_dosyalar[i]);
                            for (int j = i; j < secilen_dosya_sayisi - 1; ++j) {
                                secilen_dosyalar[j] = secilen_dosyalar[j + 1];
                            }
                            secilen_dosyalar = (char**)realloc(secilen_dosyalar, (secilen_dosya_sayisi - 1) * sizeof(char*));
                            secilen_dosya_sayisi--;
                            break;
                        }
                    }
                } else {
                    // Eğer başka bir dosya seçilmişse, önceki seçimi iptal et ve yeni dosyayı seç
                    for (int i = 0; i < dosya_sayisi; ++i) {
                        if (i != secilen_index && secilen_dosyalar_index[i] == 1) {
                            // Önceki seçimi iptal et
                            secilen_dosyalar_index[i] = 0;
                            for (int j = 0; j < secilen_dosya_sayisi; ++j) {
                                if (strcmp(dosya_adi[i], secilen_dosyalar[j]) == 0) {
                                    free(secilen_dosyalar[j]);
                                    for (int k = j; k < secilen_dosya_sayisi - 1; ++k) {
                                        secilen_dosyalar[k] = secilen_dosyalar[k + 1];
                                    }
                                    secilen_dosyalar = (char**)realloc(secilen_dosyalar, (secilen_dosya_sayisi - 1) * sizeof(char*));
                                    secilen_dosya_sayisi--;
                                    break;
                                }
                            }
                        }
                    }
                    // Yeni dosyayı seç
                    secilen_dosyalar_index[secilen_index] = 1;
                    secilen_dosyalar = (char**)realloc(secilen_dosyalar, (secilen_dosya_sayisi + 1) * sizeof(char*));
                    secilen_dosyalar[secilen_dosya_sayisi] = strdup(dosya_adi[secilen_index]);
                    secilen_dosya_sayisi++;
                }
                break;

            case 27: // ESC tuşu
                // Seçim işlemini iptal et
                endwin(); // Ncurses'i kapat
                free(dosya_adi); // Bellek sızıntısını önlemek için belleği serbest bırak
                for (int i = 0; i < secilen_dosya_sayisi; ++i) {
                    free(secilen_dosyalar[i]);
                }
                free(secilen_dosyalar);
                return NULL;
            case 'f': // f tuşu
            {
                // Dosya seçimi aktifleşti
                // Dosya adını girmek için bir pencere aç
                clear();
                mvprintw(0, 0, "Dosya adini girin ve Enter tusuna basin: ");
                char dosya_adi_giris[256];
                echo(); // Kullanıcının girdiği metni ekranda göster
                getstr(dosya_adi_giris);
                noecho(); // Kullanıcının girdiği metni ekranda gizle
                clear();

                // Girilen dosyanın listede olup olmadığını kontrol et
                int dosya_var_mi = 0;
                int secilen_dosya_index = -1; // Seçilen dosyanın listedeki indeksi
                for (int i = 0; i < dosya_sayisi; ++i) {
                    if (strcmp(dosya_adi_giris, dosya_adi[i]) == 0) {
                        dosya_var_mi = 1;
                        secilen_dosya_index = i;
                        break;
                    }
                }

                // Dosya listede yoksa oluştur ve listeye ekle
                if (!dosya_var_mi) {
                    // Dosya adını kopyala
                    char *yeni_dosya_adi = strdup(dosya_adi_giris);
                    if (yeni_dosya_adi == NULL) {
                        // Bellek yetersizliği durumunda
                        // Hata mesajı yazdır ve çık
                        mvprintw(3, 0, "Bellek yetersiz!");
                        refresh();
                        break;
                    }

                    // Dosya adlarını tutan diziyi yeniden boyutlandır
                    dosya_adi = realloc(dosya_adi, (dosya_sayisi + 1) * sizeof(char *));
                    if (dosya_adi == NULL) {
                        free(yeni_dosya_adi);
                        mvprintw(3, 0, "Bellek yetersiz!");
                        refresh();
                        break;
                    }

                    // Yeni dosya adını dizide sakla
                    dosya_adi[dosya_sayisi] = yeni_dosya_adi;
                    dosya_sayisi++;
                    secilen_dosya_index = dosya_sayisi - 1;
                    dosya_var_mi = 1;
                    // Ekrana çıktı ekleyelim
                    mvprintw(3, 0, "Dosya adi: %s", dosya_adi[dosya_sayisi - 1]);
                    refresh(); // Ekranı yenilemek için
                }
                // Başka bir dosya seçiliyse önceki seçimi iptal et
                for (int i = 0; i < dosya_sayisi; ++i) {
                    if (i != secilen_dosya_index && secilen_dosyalar_index[i] == 1) {
                        // Önceki seçimi iptal et
                        secilen_dosyalar_index[i] = 0;
                        for (int j = 0; j < secilen_dosya_sayisi; ++j) {
                            if (strcmp(dosya_adi[i], secilen_dosyalar[j]) == 0) {
                                free(secilen_dosyalar[j]);
                                for (int k = j; k < secilen_dosya_sayisi - 1; ++k) {
                                    secilen_dosyalar[k] = secilen_dosyalar[k + 1];
                                }
                                secilen_dosyalar = (char**)realloc(secilen_dosyalar, (secilen_dosya_sayisi - 1) * sizeof(char*));
                                secilen_dosya_sayisi--;
                                break;
                            }
                        }
                    }
                }

                // Dosya listede varsa seçim işlemi yap
                if (dosya_var_mi) {
                    // Dosyanın zaten seçili olup olmadığını kontrol et
                    if (secilen_dosyalar_index[secilen_dosya_index] == 0) {
                        // Dosya zaten seçili değilse seç ve işaretle
                        secilen_dosyalar_index[secilen_dosya_index] = 1;
                        // Önceki seçimi iptal et
                        for (int i = 0; i < secilen_dosya_sayisi; ++i) {
                            free(secilen_dosyalar[i]);
                        }
                        free(secilen_dosyalar);
                        secilen_dosyalar = (char**)malloc(sizeof(char*));
                        secilen_dosyalar[0] = strdup(dosya_adi[secilen_dosya_index]);
                        secilen_dosya_sayisi = 1;
                    }
                }

                break;
            }


        }
    }
}

void dosya_sil(char *dosya_adi) {
    if (remove(dosya_adi) == 0) {
        clear();
        printw("%s dosyasi basariyla silindi.\n", dosya_adi);
        char message[100];
        sprintf(message, "%s dosyasi basariyla silindi.\n", dosya_adi);
        log_massage(message);
        printw("\n\npress a key...");
        refresh();
        getch();
    } else {
        clear();
        fprintf(stderr, "Hata: %s dosyasi silinemedi.\n", dosya_adi);
        printw("\n\npress a key...");
        refresh();
        getch();
    }
}

void dosya_sec_ve_sil() {
    char** secilen_dosyalar = dosya_sec();
    if (secilen_dosyalar != NULL) {
        clear();
        printw("Seçilen dosyalar:\n");
        for (int i = 0; secilen_dosyalar[i] != NULL; ++i) {
            printf("%s\n", secilen_dosyalar[i]);
            dosya_sil(secilen_dosyalar[i]); // Dosyayı sil
            free(secilen_dosyalar[i]); // Bellek sızıntısını önlemek için belleği serbest bırak
        }
        printw("\n\npress a key...");
        refresh();
        getch();
        free(secilen_dosyalar);
    } else {
        clear();
        printf("Dosya seçimi iptal edildi.\n");
        printw("\n\npress a key...");
        refresh();
        getch();
    }
}

void dosya_duzenle(char *dosya_adi) {
    // Editör olarak kullanılacak komutun oluşturulması
    const char *editor = getenv("EDITOR");
    if (editor == NULL || strlen(editor) == 0) {
        editor = "nano"; // Varsayılan olarak nano kullan
    }
    
    // Dosyanın düzenlenmesi için komut oluşturma
    char komut[1024];
    snprintf(komut, sizeof(komut), "%s %s", editor, dosya_adi);

    // Dosyanın düzenlenmesi için editörü açma
    int sonuc = system(komut);
    if (sonuc == -1) {
        printw("Hata: Editör açilamai.\n");
        printw("\n\npress a key...");
        refresh();
        getch();

    } else {
        printw("%s \ndosyasi basariyla düzenlendi.\n", dosya_adi);
        char message[100];
        sprintf(message, "%s adli dosya düzenlendi\n", dosya_adi);
        log_massage(message);
    }
}

void dosya_sec_ve_duzenle() {
    char** secilen_dosyalar = dosya_sec();
    if (secilen_dosyalar != NULL) {
        clear();
        printw("Seçilen dosyalar:\n");
        for (int i = 0; secilen_dosyalar[i] != NULL; ++i) {
            printw("%s\n", secilen_dosyalar[i]);
            dosya_duzenle(secilen_dosyalar[i]); // Dosyayı düzenle
            free(secilen_dosyalar[i]); // Bellek sızıntısını önlemek için belleği serbest bırak
        }
        printw("\n\npress a key...");
        refresh();
        getch();
        free(secilen_dosyalar);
    } else {
        clear();
        printw("Dosya seçimi iptal edildi.\n");
        printw("\n\npress a key...");
        refresh();
        getch();
    }
}
// Dosya türünü (uzantısını) değiştirip dosya adını güncelleyen fonksiyon
void dosya_turu_degistir(char *yeni_tur) {
    // Kullanıcıdan yeni dosya türünü al
    char** dosyalar = dosya_sec();
    clear();
    system("clear");
    printf("Dosya türünü giriniz orn '.txt' : ");
    scanf("%s", yeni_tur);

    // Dosya türlerini değiştir ve dosya adını güncelle
    for (int i = 0; dosyalar[i] != NULL; ++i) {
        char* nokta = strrchr(dosyalar[i], '.'); // Dosya adındaki son noktayı bul
        if (nokta != NULL) {
            // Yeni dosya adını oluştur
            char yeni_dosya_adi[100];
            strncpy(yeni_dosya_adi, dosyalar[i], nokta - dosyalar[i]);
            yeni_dosya_adi[nokta - dosyalar[i]] = '\0';
            strcat(yeni_dosya_adi, yeni_tur);

            // Dosya adını değiştir
            if (rename(dosyalar[i], yeni_dosya_adi) == 0) {
                printw("\nDosya türü degistirildi ve dosya adi güncellendi: %s\n", yeni_dosya_adi);
                char message[250];
                sprintf(message, "%s Dosya türü degistirildi ve dosya adi güncellendi: %s\n", dosyalar[i],yeni_dosya_adi);
                log_massage(message);

            } else {
                printw("\nDosya türü degistirildi ancak dosya adi güncellenemedi: %s\n", dosyalar[i]);

            }
        } else {
            printw("\nDosya uzantisi bulunamadi: %s\n", dosyalar[i]);

        }
        free(dosyalar[i]); // Bellek temizle

    }
    printw("\n\npress a key...");
    refresh();
    getch();
    free(dosyalar);
}

// Dosya erişim izinlerini güncelleyen fonksiyon
void dosya_izinleri_ayarla(char *izinler) {
    char** dosya_adlari = dosya_sec();
    // Dosyaların erişim izinlerini güncelle
    clear();
    for (int i = 0; dosya_adlari[i] != NULL; ++i) {
        if (chmod(dosya_adlari[i], strtol(izinler, 0, 8)) == 0) {
            printw("Dosya erisim izinleri güncellendi: %s\n", dosya_adlari[i]);
            char message[255];
            sprintf(message, "%s -> Dosya erisim izinleri güncellendi:\n", dosya_adlari[i]);
            log_massage(message);
        } else {
            printw("Dosya erisim izinleri güncellenemedi: %s\n", dosya_adlari[i]);
        }
        free(dosya_adlari[i]); // Bellek temizle
    }
    printw("\n\npress a key...");
    refresh();
    getch();
    free(dosya_adlari);
}

void dosya_tasi() {
    clear();
    printw("Kaynak Dosya seçmek için bir tusa bas\n");
    refresh();
    getch();
    // Kaynak dosyayı seç
    char** kaynak_dosya = tek_dosya_sec();
    
    if (kaynak_dosya == NULL) {
        printw("Kaynak dosya seçilmedi.\n");
        printw("\n\npress a key...");
        refresh();
        getch();
        return;
    }
    clear();
    // Hedef dosyayı seç
    printw("Kaynak Dosya seçmek için bir tusa bas\n");
    refresh();
    getch();
    char** hedef_dosya = tek_dosya_sec();
    clear();
    if (hedef_dosya == NULL) {
        printw("Hedef dosya seçilmedi.\n");
        printw("\n\npress a key...");
        refresh();
        getch();
        free(kaynak_dosya); // Belleği temizle
        return;
    }

    // Hedef dosyanın türünü kontrol et
    struct stat hedef_stat;
    if (stat(*hedef_dosya, &hedef_stat) == 0 && S_ISDIR(hedef_stat.st_mode)) {
        // Hedef dosya bir klasörse, kaynak dosyayı hedef klasörün içine taşı
        char* hedef_klasor = *hedef_dosya;
        char* hedef_klasor_slash = "/";
        char* hedef_dosyanin_adi = *kaynak_dosya;

        // Hedef klasörün sonuna / ekleyerek dosyanın tam yolunu oluştur
        char* hedef_dosyanin_yolu = malloc(strlen(hedef_klasor) + strlen(hedef_klasor_slash) + strlen(hedef_dosyanin_adi) + 1);
        strcpy(hedef_dosyanin_yolu, hedef_klasor);
        strcat(hedef_dosyanin_yolu, hedef_klasor_slash);
        strcat(hedef_dosyanin_yolu, hedef_dosyanin_adi);

        // Dosyayı taşı
        if (rename(*kaynak_dosya, hedef_dosyanin_yolu) == 0) {
            printw("Dosya tasindi: %s -> %s\n", *kaynak_dosya, hedef_dosyanin_yolu);
            printw("\n\npress a key...");
            char message[255];
            sprintf(message, "Dosya tasindi: %s -> %s\n", *kaynak_dosya, *hedef_dosya);
            log_massage(message);
            refresh();
            getch();
        } else {
            printw("Dosya tasima basarisiz: %s -> %s\n", *kaynak_dosya, hedef_dosyanin_yolu);
            printw("\n\npress a key...");
            refresh();
            getch();
        }

        // Belleği temizle
        free(hedef_dosyanin_yolu);
    } else {
        // Hedef dosya bir klasör değilse, orijinal kodu kullanarak devam et
        if (rename(*kaynak_dosya, *hedef_dosya) == 0) {
            printw("Dosya tasindi: %s -> %s\n", *kaynak_dosya, *hedef_dosya);
            printw("\n\npress a key...");
            char message[255];
            sprintf(message, "Dosya tasindi: %s -> %s\n", *kaynak_dosya, *hedef_dosya);
            log_massage(message);
            refresh();
            getch();
        } else {
            printw("Dosya tasima basarisiz: %s -> %s\n", *kaynak_dosya, *hedef_dosya);
            printw("\n\npress a key...");
            refresh();
            getch();
        }
    }

    // Belleği temizle
    free(*kaynak_dosya);
    free(*hedef_dosya);
    free(kaynak_dosya);
    free(hedef_dosya);
}

void dosya_kopyala() {
    // Kaynak dosyayı seç
    clear();
    printw("Kaynak Dosya seçmek için bir tusa bas\n");
    refresh();
    getch();
    char** kaynak_dosya = tek_dosya_sec();
    if (kaynak_dosya == NULL) {
        clear();
        printw("Kaynak dosya seçilmedi.\n");
        printw("\n\npress a key...");
        refresh();
        getch();
        return;
    }

    // Hedef dosyayı seç
    clear();
    printw("Hedef Dosya seçmek için bir tusa bas\n");
    refresh();
    getch();
    char** hedef_dosya = tek_dosya_sec();
    clear();
    if (hedef_dosya == NULL) {
        clear();
        printw("Hedef dosya seçilmedi.\n");
        printw("\n\npress a key...");
        refresh();
        getch();
        free(kaynak_dosya); // Belleği temizle
        return;
    }

    
    FILE *kaynak, *hedef;
    char karakter;

    // Kaynak dosyayı okuma modunda aç
    kaynak = fopen(kaynak_dosya[0], "r");
    if (kaynak == NULL) {
        printw("Kaynak dosya açilamadi");
        printw("\n\npress a key...");
        refresh();
        getch();
        return;
    }

    // Hedef dosyayı yazma modunda aç
    hedef = fopen(hedef_dosya[0], "w");
    if (hedef == NULL) {
        printw("Hedef dosya açilamadi");
        fclose(kaynak); // Açılan kaynak dosyayı kapat
        printw("\n\npress a key...");
        refresh();
        getch();
        return;
    }

    // Kaynak dosyanın sonuna kadar oku ve hedef dosyaya yaz
    while ((karakter = fgetc(kaynak)) != EOF) {
        fputc(karakter, hedef);
    }

    // Dosyaları kapat
    fclose(kaynak);
    fclose(hedef);

    char message[255];
    sprintf(message, "Dosya kopyalandi: %s -> %s\n", kaynak_dosya[0], hedef_dosya[0]);
    log_massage(message);

    printw("Dosya kopyalandi: %s -> %s\n", kaynak_dosya[0], hedef_dosya[0]);
    printw("\n\npress a key...");
    refresh();
    getch();
    // Belleği temizle
    free(kaynak_dosya[0]);
    free(hedef_dosya[0]);
    free(kaynak_dosya);
    free(hedef_dosya);
}


void dosya_yaz() {
    char *dosya_adi = *tek_dosya_sec();
    // Dosyayı okumak için bir dosya işaretçisi oluşturuluyor
    clear();
    FILE *dosya = fopen(dosya_adi, "a");
    if (dosya == NULL) {
        printw("Dosya bulunamadi veya açilamadi: %s\n", dosya_adi);
        printw("\n\npress a key...");
        refresh();
        getch();
        free(dosya);
        return;
    }
    char message[255];
    sprintf(message, "%s adli dosya yazma isteği gönderildi\n", dosya_adi);
    log_massage(message);
    fclose(dosya);

    // Dosya uzantısını kontrol etmek için son noktaya kadar olan kısmı al
    char *uzanti = strrchr(dosya_adi, '.');
    if ((uzanti != NULL && strcmp(uzanti, ".") )== 0) {
                // Dosya bir klasörse, klasör içeriğini listele
        clear();
        DIR *dir;
        struct dirent *entry;
        dir = opendir(dosya_adi);
        while ((entry = readdir(dir)) != NULL)
        {
            printw("%s\n", entry->d_name);
        }
        closedir(dir);
        printw("\n\npress a key...");
        refresh();
        getch();
    } else {
        // Dosya metin dosyasıysa metin editöründe aç
        char komut[100];
        sprintf(komut, "gedit %s", dosya_adi);
        system(komut);

    }

}


void dosya_ac() {
    char *dosya_adi = *tek_dosya_sec();
    // Dosyayı okumak için bir dosya işaretçisi oluşturuluyor
    clear();
    FILE *dosya = fopen(dosya_adi, "r");
    if (dosya == NULL) {
        printw("Dosya bulunamadi veya açilamadi: %s\n", dosya_adi);
        printw("\n\npress a key...");
        refresh();
        getch();
        free(dosya);
        return;
    }
    char message[255];
    sprintf(message, "%s adli dosyaya açma isteği gönderildi\n", dosya_adi);
    log_massage(message);
    fclose(dosya);

    // Dosya uzantısını kontrol etmek için son noktaya kadar olan kısmı al
    char *uzanti = strrchr(dosya_adi, '.');
    if ((uzanti != NULL && strcmp(uzanti, ".") )== 0) {
                // Dosya bir klasörse, klasör içeriğini listele
        clear();
        DIR *dir;
        struct dirent *entry;
        dir = opendir(dosya_adi);
        while ((entry = readdir(dir)) != NULL)
        {
            printw("%s\n", entry->d_name);
        }
        closedir(dir);
        printw("\n\npress a key...");
        refresh();
        getch();
    } else {
        // Dosya metin dosyasıysa metin editöründe aç
        char komut[100];
        sprintf(komut, "gedit %s", dosya_adi);
        system(komut);

    }
}

void temelIslemler(){

    char *menu[] = {"Dosya Ac", "Dosya Yaz", "Dosya Kopyala", "Dosya Tasimak", "Dosya Sil","Help","Geri"};
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
                        dosya_ac();
                        
                        break;
                    case 1:
                        // Dosya Yaz fonksiyonu çağır
                        dosya_yaz();
                        break;
                    case 2:
                        // Dosya Kopyala fonksiyonu çağır
                        dosya_kopyala();
                        break;
                    case 3:
                        // Dosya Tasima fonksiyonu çağır
                        dosya_tasi();
                        break;
                    case 4:
                        dosya_sec_ve_sil();
                        break;
                    case 5:
                        clear();
                        printw("Dosya Aç: Sectiginiz dosyayi acmanizi saglar (Yalnizca 1 secim)\n");
                        printw("\nDosya Yaz: Sectiginiz dosyayinin icerigini degistiribilirsiniz ya da \n'f' tusuna basip doysayi aratirsiniz eger dosya yoksa girdiginiz isimde\ndosya olusur (Yalnizca 1 secim)\n");
                        printw("\nDosya Kopyala: Sectiginiz kaynak dosyaynin icerigini hedef dosyaya kopyalar\n");
                        printw("\nDosya Tasi: Sectiginiz kaynak dosyaynin icerigini hedef dosyaya kopyala\nDaha sonra kaynak dosyayi silerr\n");
                        printw("\nDosya Sil: Sectiginiz dosyalari silmenizi saglar (1'den fazla secim mümkün)\n");
                        printw("\n\n\nGeri dönmek için bir tusa basin");
                        refresh();
                        getch();
                        break; 
                    case 6:
                        dosyaIslemleriMenu();
                        break;  
                }
                break;
        }
    }
}


void gelismisIslemler(){
    
    char *menu[] = {"Dosya Duzenle", "Dosya Turu Degistir", "Dosya Izinleri Ayarla", "Help", "Geri"};
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
                        // Dosya Düzenle fonksiyonu çağır
                        dosya_sec_ve_duzenle();
                        break;
                    case 1:
                        // Dosya Türü Deüiştir fonksiyonu çağır
                        char yeni_tur[50];
                        dosya_turu_degistir(yeni_tur);
                        refresh();
                        break;
                    case 2:
                        //Dosya izinleri Ayarka Fonksiyonu Çapır
                        clear();
                        system("clear");
                        char str[50];

                        printw("Dosya erişim izinlerini belirleyin:\n");
                        printw("1. Sadece okuma izni (444)\n");
                        printw("2. Okuma ve yazma izni (644)\n");
                        printw("3. Tam yetki (777)\n");
                        printw("Seçiminizi yapin (1/2/3): ");
                        echo();
                        getstr(str);
                        int secim;
                        secim = atoi(str);

                        char izinler[5];
                        switch (secim) {
                            case 1:
                                strcpy(izinler, "0444");
                                break;
                            case 2:
                                strcpy(izinler, "0644");
                                break;
                            case 3:
                                strcpy(izinler, "0777");
                                break;
                            default:
                                printf("Geçersiz seçim.\n");
                                break ;
                        }

                        // Dosya erişim izinlerini güncelle
                        dosya_izinleri_ayarla(izinler);
                        break;
                    case 3:
                        // Help
                        clear();
                        printw("Dosya Duzenle: Sectiginiz dosylarin icerigibi duzenlemenizi saglar\n(1'den fazla secim mümkün)\n");
                        printw("\nDosya Turu Degistir: Sectiginiz dosylarin turunu degistirmeyi saglar\n(1'den fazla secim mümkün)\n");
                        printw("\nDosya Izinleri Ayarla: Sectiginiz dosylarin izinlerini degistirmeyi saglar\n(1'den fazla secim mümkün)\n");
                        printw("\n\n\nGeri dönmek için bir tusa basin");
                        refresh();
                        getch();
                        break;
                    case 4:
                        // Geri
                        dosyaIslemleriMenu();
                        break;
                }
                break;
        }
    }
}

