#ifndef DOSYA_ISLEMLERI_H
#define DOSYA_ISLEMLERI_H

//Kütüphaneler
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ncurses.h>
#include <sys/stat.h>
#include <time.h>
#include <math.h>
//Fonksiyon prototiplerinin tanımı
char** dosya_adlarini_al(int* dosya_sayisi);

void dosya_secimi_goster(char** dosya_adi, int dosya_sayisi, int secilen_index, int* secilen_dosyalar_index);

char** dosya_sec();

char** tek_dosya_sec();

void dosya_sil(char *dosya_adi);

void dosya_sec_ve_sil();

void dosya_duzenle(char *dosya_adi);

void dosya_sec_ve_duzenle();

void dosya_turu_degistir(char *yeni_tur);

void dosya_izinleri_ayarla(char *izinler);

void dosya_tasi();

void dosya_kopyala();

void dosya_yaz();

void dosya_ac();

void temelIslemler();

void gelismisIslemler();

void dosyaIslemleriMenu();

void log_massage(const char *format, ...);
#endif
