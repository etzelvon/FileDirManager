# Dosya ve Dizin İşlemleri Projesi

Bu proje, terminal tabanlı bir dosya ve dizin yönetim uygulamasıdır. Uygulama, ncurses kütüphanesi kullanılarak menü tabanlı bir arayüz sağlar ve çeşitli dosya ve dizin işlemlerini gerçekleştirebilir.

## Özellikler

- **Dosya İşlemleri:**
  - Dosya oluşturma, okuma, yazma, silme ve listeleme
  - Dosya içeriğini görüntüleme ve düzenleme

- **Dizin İşlemleri:**
  - Dizin oluşturma, değiştirme, silme ve listeleme
  - Dizin içeriğinde gezinme

- **Loglama:**
  - Yapılan işlemler loglanır ve mevcut klasörde bir log dosyası oluşturulur.

## Kurulum ve Çalıştırma

Projenin derlenmesi ve çalıştırılması için aşağıdaki adımları izleyin:

1. Gerekli dosyaları derleyin:

    ```sh
    gcc -c main.c -o main.o
    gcc -c dizin_islemleri.c -o dizin_islemleri.o
    gcc -c dosya_islemleri.c -o dosya_islemleri.o
    gcc main.o dizin_islemleri.o dosya_islemleri.o -o program -lncurses
    ```

2. Programı çalıştırın:

    ```sh
    ./program
    ```

## Kullanım

- Menüler arasında yukarı ve aşağı ok tuşları ile hareket edebilirsiniz.
- Dosya ve dizin işlemleri menülerden seçilebilir.
- Dizin değiştir fonksiyonunu kullanarak işlem yapılacak dizini değiştirebilirsiniz. Dizinlerde gezinmek için terminalde olduğu gibi `.` ve `/` kullanabilirsiniz.
- Güvenlik amacıyla, dizin silme işlemi alt klasörleri olan dizinler için özel bir işleme tabi tutulmuştur. Önce alt klasöre girip ardından silme işlemi yapılmalıdır.

## Ek Bilgiler

- Bu proje ncurses kütüphanesi kullanılarak geliştirilmiştir. Menüler arasında rahatça gezinebilmek için ok tuşları kullanılabilir.
- Dizin değiştirme fonksiyonu ile dizin değiştirildiğinde, tüm fonksiyonlar yeni dizin içinde kullanılabilir.

Her türlü geri bildirim ve katkı için lütfen iletişime geçin!

