#include "dizin_islemleri.h"
#define PATH_MAX 4096

// Karşılaştırma fonksiyonları
int alfabetik_sirala(const void *a, const void *b);
int ture_gore_sirala(const void *a, const void *b);

// Dosya sıralama fonksiyonu
void dosya_sirala() {
    clear();
    endwin();
    struct dirent **dosyalar;
    int dosya_sayisi;
    int i,secim;
    system("clear");
    printf("Dosya siralamak icin secim yapin:\n");
    printf("1. Alfabetik siralama\n");
    printf("2. Ture gore siralama\n");
    printf("Seciminiz: ");
    scanf("%d", &secim);
    // Dizindeki dosyaları oku
    dosya_sayisi = scandir(".", &dosyalar, NULL, NULL);
    if (dosya_sayisi >= 0) {
        // Seçime göre sırala
        switch (secim) {
            case 1:
                // Alfabetik sıralama
                qsort(dosyalar, dosya_sayisi, sizeof(struct dirent *), alfabetik_sirala);
                printw("Alfabetik siralama:\n\n");
                break;
            case 2:
                // Dosya türüne göre sıralama
                qsort(dosyalar, dosya_sayisi, sizeof(struct dirent *), ture_gore_sirala);
                printw("Ture gore siralama:\n\n");
                break;
            default:
                printw("Gecersiz secim.\n");
                printw("\n\nPress a key...");
                refresh();
                getch();
                return;
        }

        // Sıralı dosya listesini yazdır
        for (i = 0; i < dosya_sayisi; ++i) {
            printw("%s\n", dosyalar[i]->d_name);
            free(dosyalar[i]); // Belleği serbest bırak
        }
        free(dosyalar);
    } else {
        printw("Dosya okuma hatasi.\n");
    }
        printw("\n\nPress a key...");
        refresh();
        getch();
        system("clear");
}

// Dosya adına göre alfabetik sıralama fonksiyonu
int alfabetik_sirala(const void *a, const void *b) {
    struct dirent *dosya1 = *(struct dirent **)a;
    struct dirent *dosya2 = *(struct dirent **)b;
    return strcmp(dosya1->d_name, dosya2->d_name);
}

// Dosya türüne göre sıralama fonksiyonu
int ture_gore_sirala(const void *a, const void *b) {
    struct dirent *dosya1 = *(struct dirent **)a;
    struct dirent *dosya2 = *(struct dirent **)b;

    // Dosya uzantılarını al
    const char *uzanti1 = strrchr(dosya1->d_name, '.');
    const char *uzanti2 = strrchr(dosya2->d_name, '.');

    // Uzantılar yoksa dosyaları alfabetik olarak sırala
    if (!uzanti1 && !uzanti2) {
        return strcmp(dosya1->d_name, dosya2->d_name);
    } else if (!uzanti1) {
        return -1; // dosya1, dosya2'den önce gelsin
    } else if (!uzanti2) {
        return 1; // dosya2, dosya1'den önce gelsin
    }

    // Dosya uzantılarına göre sırala
    return strcmp(uzanti1, uzanti2);
}




// Dosya filtreleme fonksiyonu
void dosya_filtrele() {
    DIR *dizin;
    struct dirent *dosya;

    clear();
    endwin();
    char filtre_kriteri[512];

    system("clear");
    printf("Kriter girin orn '.txt' : ");
    scanf("%s", filtre_kriteri);
    // Mevcut dizini aç
    dizin = opendir(".");
    if (dizin) {
        
        while ((dosya = readdir(dizin)) != NULL) {
            // Dosya ismi "." veya ".." ise geç
            if (strcmp(dosya->d_name, ".") == 0 || strcmp(dosya->d_name, "..") == 0) {
                continue;
            }

            // Dosya adı ve uzantısını al
            char *dosya_adi = dosya->d_name;
            char *dosya_uzantisi = strrchr(dosya_adi, '.'); // Dosya uzantısının bulunduğu kısmı al

            // Dosya uzantısı var mı ve kriterle eşleşiyor mu kontrol et
            if (dosya_uzantisi != NULL && strcmp(dosya_uzantisi, filtre_kriteri) == 0) {
                printw("%s\n", dosya_adi);
            }
        }
        // Dizini kapat
        closedir(dizin);
        printw("\n\nPress a key...");
        refresh();
        getch();
    }
}
// Dosya arama fonksiyonu
void dosya_ara() {
    DIR *dizin;
    struct dirent *dosya;
    FILE *dosya_ptr;
    char satir[512];
    char dosya_adi[256];
    clear();
    endwin();
    char aranacak_metin[512];
    system("clear");
    printf("Aranacak meetni girin girin: ");
    scanf("%s", aranacak_metin);
    // Mevcut dizini aç
    dizin = opendir(".");
    if (dizin) {
        
        while ((dosya = readdir(dizin)) != NULL) {
            // Dosya ismi "." veya ".." ise geç
            if (strcmp(dosya->d_name, ".") == 0 || strcmp(dosya->d_name, "..") == 0) {
                continue;
            }

            // Dosya adını kopyala
            strcpy(dosya_adi, dosya->d_name);

            // Dosyayı aç
            dosya_ptr = fopen(dosya_adi, "r");
            if (dosya_ptr) {
                // Dosyanın her satırını oku
                while (fgets(satir, sizeof(satir), dosya_ptr)) {
                    // Metni içeriyor mu kontrol et
                    if (strstr(satir, aranacak_metin) != NULL || strstr(dosya_adi, aranacak_metin) != NULL) {
                        printw("'%s' dosyasında '%s' metni bulundu.\n", dosya_adi, aranacak_metin);
                        break;
                    }
                }
                // Dosyayı kapat
                fclose(dosya_ptr);
            }
        }
        // Dizini kapat
        closedir(dizin);
        printw("\n\nPress a key...");
        refresh();
        getch();
    }
}

// Dizin taşıma fonksiyonu
void dizin_tasi() {
    clear();
    endwin();
    char kaynak_dizin[100];
    char hedef_dizin[100];
    system("clear");
    printf("Tasinacak kaynak dizinini girin: ");
    scanf("%s", kaynak_dizin);

    system("clear");
    printf("Hedef dizinini girin: ");
    scanf("%s", hedef_dizin);

    // Taşıma işlemi için sistem çağrıları
    char komut[512]; // Komutu tutmak için bir karakter dizisi oluşturuyoruz
    sprintf(komut, "mv %s %s", kaynak_dizin, hedef_dizin); // mv komutu ile kaynak dizini hedef dizine taşıyoruz
    system(komut); // Komutu çalıştırıyoruz
    char message[255];
    sprintf(message, "Dizin tasime isteği (Kaynak) %s -> (Hedef) %s tasindi\n",kaynak_dizin ,hedef_dizin);
    log_massage(message);
    // Kaynak dizini silme işlemi
    sprintf(komut, "rm -rf %s", kaynak_dizin); // rm -rf komutu ile kaynak dizini siliyoruz (force delete)
    system(komut); // Komutu çalıştırıyoruz

    printw("Dizin tasima tamamlandi.\n");
    printw("\n\nPress a key...");
    refresh();
    getch();
}


void klasor_kopyala(const char *kaynak_dizin, const char *hedef_dizin) {
    DIR *dp;
    struct dirent *entry;
    char kaynak_path[PATH_MAX];
    char hedef_path[PATH_MAX];

    dp = opendir(kaynak_dizin);
    if (dp == NULL) {
        perror("klasor_kopyala opendir");
        return;
    }

    while ((entry = readdir(dp)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(kaynak_path, PATH_MAX, "%s/%s", kaynak_dizin, entry->d_name);
        snprintf(hedef_path, PATH_MAX, "%s/%s", hedef_dizin, entry->d_name);

        if (entry->d_type == DT_DIR) {
            mkdir(hedef_path, 0777); // Alt klasörü oluştur
            klasor_kopyala(kaynak_path, hedef_path); // Alt klasörü kopyala
        } else {
            FILE *kaynak_file = fopen(kaynak_path, "r");
            FILE *hedef_file = fopen(hedef_path, "w");
            if (kaynak_file && hedef_file) {
                char buffer[4096];
                size_t bytesRead;
                while ((bytesRead = fread(buffer, 1, sizeof(buffer), kaynak_file)) > 0) {
                    fwrite(buffer, 1, bytesRead, hedef_file);
                }
                fclose(kaynak_file);
                fclose(hedef_file);
            } else {
                perror("klasor_kopyala fopen");
            }
        }
    }
    char message[255];
    sprintf(message, "Dizin kopyalama isteği (Kaynak) %s -> (Hedef) %s kopyalandi\n",kaynak_dizin ,hedef_dizin);
    log_massage(message);

    closedir(dp);
}

void dizin_kopyala() {
    clear();
    endwin();
    char kaynak_dizin[100];
    char hedef_dizin[100];
    system("clear");
    printf("Kopyalanacak kaynak dizinini girin: ");
    scanf("%s", kaynak_dizin);

    system("clear");
    printf("Hedef dizinini girin: ");
    scanf("%s", hedef_dizin);

    int status = system("clear"); // Temizleme komutunu çalıştır

    if (status == -1) {
        perror("clear komutu çalistirilamadi");
        return;
    }

    klasor_kopyala(kaynak_dizin, hedef_dizin); // Klasörü kopyala

    printw("Kopyalama tamamlandi.\n");
    printw("\n\nPress a key...");
    refresh();
    getch();
}


void alt_dizinleri_listele() {
    clear();
    endwin();
    char dizin_adi[256];
    system("clear");
    printw("Listelemek istediginiz dizinin tam yolunu girin:");
  
    getnstr(dizin_adi, sizeof(dizin_adi));
    // Satır sonu karakterini kaldır
    dizin_adi[strcspn(dizin_adi, "\n")] = '\0';

    DIR *dizin = opendir(dizin_adi);
    if (dizin == NULL) {
        perror("Hata");
        exit(EXIT_FAILURE);
    }

    printw("Dizin: %s\n", dizin_adi);
    printw("\n");
    struct dirent *dosya;
    while ((dosya = readdir(dizin)) != NULL) {
        // Dosya/dizin adını al
        char dosya_adi[256];
        strcpy(dosya_adi, dizin_adi);
        strcat(dosya_adi, "/");
        strcat(dosya_adi, dosya->d_name);

        // Dosya/dizin bilgilerini al
        struct stat bilgi;
        if (stat(dosya_adi, &bilgi) == -1) {
            perror("Hata");
            exit(EXIT_FAILURE);
        }

        // Dosya/dizin türünü belirle
        const char *tur = NULL;
        if (S_ISREG(bilgi.st_mode)) {
            tur = "Dosya";
        } else if (S_ISDIR(bilgi.st_mode)) {
            tur = "Dizin";
        } else {
            tur = "Bilinmeyen";
        }

        // Dosya/dizin boyutunu al
        off_t boyut = bilgi.st_size;

        // Dosya/dizin değiştirilme tarihini al
        char zaman[20];
        strftime(zaman, sizeof(zaman), "%Y-%m-%d %H:%M:%S", localtime(&bilgi.st_mtime));

        // Bilgileri ekrana yazdır
        printw("  Ad: %-20s  Tür: %-10s  Boyut: %-10lld  Değişim Tarihi: %s\n", dosya->d_name, tur, (long long)boyut, zaman);
    }
    printw("\n\nPress a key...");
    refresh();
    getch();
    endwin();
    closedir(dizin);
}

void dizin_degistir() {
    clear();
    endwin(); // ncurses penceresini kapat
    system("clear");
    printf("Gecmek istediginiz dizinin tam yolunu girin: ");
    char yeni_dizin[100];
    if (fgets(yeni_dizin, sizeof(yeni_dizin), stdin) == NULL) {
        perror("fgets hatasi");
        exit(EXIT_FAILURE);
    }

    // Satır sonu karakterini kaldır
    yeni_dizin[strcspn(yeni_dizin, "\n")] = '\0';

    if (chdir(yeni_dizin) == -1) {
        if (errno == ENOENT) {
            printw("Hata: '%s' dizini bulunamadi.\n", yeni_dizin);
        } else {
            printw("Hata: '%s' dizinine gecilemedi: %s\n", yeni_dizin, strerror(errno));
        }
        printw("\n\nPress a key...");
        refresh();
        getch();
        exit(EXIT_FAILURE);
    } else {
        printw("'%s' dizinine basariyla gecildi.\n", yeni_dizin);
    }

    // Program sonlandığında çalışma dizinini kontrol et
    char simdiki_dizin[100];
    if (getcwd(simdiki_dizin, sizeof(simdiki_dizin)) == NULL) {
        perror("getcwd hatasi");
        exit(EXIT_FAILURE);
    }
    printw("'%s' dizininde bulunuyorsunuz.\n", simdiki_dizin);
    printw("\n\nPress a key...");
    refresh();
    getch();
}

void dizin_sil() {
    clear();
    printw("Silmek istediginiz dizinin adini girin: ");
    echo();

    char dizin_adi[100];
    getstr(dizin_adi);

    DIR *dizin = opendir(dizin_adi);
    if (dizin == NULL) {
        printw("Hata: '%s' adinda bir dizin bulunamadi.\n", dizin_adi);
        refresh();
        getch();
        return;
    }

    struct dirent *dosya;
    while ((dosya = readdir(dizin)) != NULL) {
        char dosya_adi[256];
        dosya_adi[0] = '\0'; // dosya_adi dizisini boşalt
        strcat(dosya_adi, dizin_adi);
        strcat(dosya_adi, "/");
        strcat(dosya_adi, dosya->d_name);
        if (remove(dosya_adi) == -1) {
            printw("Hata: '%s' dosyasi silinemedi.\n", dosya_adi);
        }
    }
    closedir(dizin);

    if (rmdir(dizin_adi) == -1) {
        printw("Hata: '%s' dizini silinemedi.\n", dizin_adi);
    } else {
        printw("'%s' adindaki dizin ve icerigi basariyla silindi.\n", dizin_adi);
        char message[255];
        sprintf(message, "'%s' adindaki dizin ve icerigi basariyla silindi.\n", dizin_adi);
        log_massage(message);
        
    }

    refresh();
    getch();
}

void dizin_olustur() {
    clear();
    printw("Yeni bir dizin olusturmak icin adini girin: ");
    refresh();

    char dizin_adi[100];
    getstr(dizin_adi);

    if (mkdir(dizin_adi, 0777) == -1) {
        if (errno == EEXIST) {
            printw("Hata: '%s' adinda bir dizin zaten var.\n", dizin_adi);
        } else {
            perror("Hata");
        }
    } else {
        printw("'%s' adinda yeni bir dizin olusturuldu.\n", dizin_adi);
        char message[255];
        sprintf(message, "'%s' adinda yeni bir dizin olusturuldu.\n", dizin_adi);
        log_massage(message);
    }

    refresh();
    getch();
}

void dizin_temelIslemler(){

    char *menu[] = {"Dizin Olustur", "Dizin Sil", "Dizin Degistir","Alt dizinleri Listele","Dizin Kopyala","Dizin Tasi","Help","Geri"};
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
                        dizin_olustur();
                        break;
                    case 1:
                        dizin_sil();
                        break;
                    case 2:
                        dizin_degistir();
                        break;
                    case 3:
                        alt_dizinleri_listele();
                        break;
                    case 4:
                        dizin_kopyala();
                        break; 
                    case 5:
                        dizin_tasi();
                        break;
                    case 6:
                        clear();
                        printw("Dizin Olustur: Yeni bir dizin olusturur.\n");
                        printw("\nDizin Sil: Belirtilen dizini ve içerigini siler. Dizin bos degilse,\niçerikle birlikte siler.\n");
                        printw("\nDizin Degistir: Geçerli çalisma dizinini, parametre olarak verilen dizinle degistirir.\n");
                        printw("\nAlt Dizin Listeli: Belirtilen dizindeki tüm alt dizin/dosyalari listeler\n");
                        printw("\nDizin Kopyala: Kaynak dizini içerigiyle, hedef dizine kopyalar\n");
                        printw("\nDizin Tasi: Kaynak dizini içerigiyle, hedef dizine kopyalar.\nDaha sonra kaynak dizini siler.\n");

                        printw("\n\nPress a key...");
                        refresh();
                        getch();
                        break;
                    case 7:
                        dizinIslemleriMenu();
                        break;
                }
                break;
        }
    }
}

void dizin_aramaFonksiyonlari(){
    char *menu[] = {"Dosya Ara", "Dosya Filtrele", "Dosya Siralama", "Help", "Geri"};
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
                        dosya_ara();
                        break;
                    case 1:
                        dosya_filtrele();
                        break;
                    case 2:
                        dosya_sirala();
                        break;
                    case 3:
                        // Help
                        clear();
                        printw("Dosya Ara: Mevcut dizindeki tüm dosyalari tarar ve aranan metini\ndosya adlarinde ve içeriklerinde arar.)\n");
                        printw("\nDosya Filtrele: Belirtilen uzantiya gore filtreler ve listeler\n");
                        printw("\nDosya Siralama: Alfabetik ve Turune olmak uzere dosylari siralar.\n");
                        printw("\n\nGeri dönmek için bir tusa basin");
                        refresh();
                        getch();
                        break;
                    case 4:
                        // Geri
                        dizinIslemleriMenu();
                        break;
                }
                break;
        }
    }
}