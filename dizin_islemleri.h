#ifndef DIZIN_ISLEMLERI_H
#define DIZIN_ISLEMLERI_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ncurses.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <errno.h>
int alfabetik_sirala(const void *a, const void *b);
int ture_gore_sirala(const void *a, const void *b);
void dosya_sirala();
void dosya_filtrele();
void dosya_ara();
void dizin_tasi();
void klasor_kopyala(const char *kaynak_dizin, const char *hedef_dizin);
void dizin_kopyala();
void alt_dizinleri_listele();
void dizin_degistir();
void dizin_sil();
void dizin_olustur();
void dizinIslemleriMenu();
void dizin_temelIslemler();
void dizin_aramaFonksiyonlari();
void dizinIslemleriMenu();
void log_massage(const char *format, ...);
#endif
