# uartproject1
Bu proje, bir mikrodenetleyici tabanlı sistemde UART (Evrensel Asenkron Alıcı Verici) ve timer kesmeleri kullanarak LED kontrolü sağlamaktadır. UART üzerinden gönderilen komutlar yardımıyla kırmızı, mavi ve yeşil LED'lerin yakılması ve zamanlama işlemleri gerçekleştirilmektedir.

Kullanılan Kütüphaneler

Standart Kütüphaneler: stdio.h, stdint.h, stdbool.h, string.h, stdlib.h, math.h, time.h

TivaWare Sürücü Kütüphaneleri: Mikrodenetleyicinin donanımını kontrol etmek için aşağıdaki kütüphaneler kullanılmıştır:

GPIO (Genel Amaçlı Giriş/Çıkış Kontrolü)

UART (Seri Haberleşme)

Timer (Zamanlayıcı Kesme Mekanizması)

Global Değişkenler

uartBuffer[50]: UART üzerinden alınan verilerin depolandığı tampon bellek.

uartCnt: UART'tan gelen veri sayısını takip eden sayaç.

tmrCnt: Timer kesmesinde kullanılan sayaç.

LED Yapısı:

Cnt: LED'in yanma zamanını takip eden sayaç.

Flag: LED'in aktif olup olmadığını belirten bayrak.

LED Nesneleri: red, blue, green (kırmızı, mavi ve yeşil LED kontrolü için).

Fonksiyonlar

1. timerInterrupt()

Bu fonksiyon, timer kesmesi tetiklendiğinde çalışır ve LED sayaçlarının değerlerini günceller.

Timer kesme bayrağı temizlenir.

Aktif LED'in (red, blue, green) bayrağı kontrol edilir ve sayaç artırılır.

Hiçbir LED aktif değilse, sayaçlar sıfırlanır.

2. uartinterrupt()

Bu fonksiyon, UART kesmesi tetiklendiğinde çalışır ve UART üzerinden alınan verileri işler.

Gelen veriler uartBuffer tampon belleğine kaydedilir.

Gelen komutlar kontrol edilir:

-A*: Kırmızı LED aktif edilir.

-B*: Mavi LED aktif edilir.

-C*: Yeşil LED aktif edilir.

Uygun olmayan komutlarda tampon bellek sıfırlanır ve tüm LED'ler kapatılır.

3. main()

Projenin ana fonksiyonudur ve aşağıdaki adımları içerir:

a. Sistem Saatinin Ayarlanması

Sistem saati 40 MHz olarak ayarlanır.

b. Timer Yapılandırması

TIMER0, 1 saniyelik zamanlama ile periyodik modda ayarlanır.

Timer kesmesi, timerInterrupt() fonksiyonuna bağlanır.

c. GPIO Yapılandırması

PortF kullanılır ve 3 LED (ışık) pinleri (ışık pinleri: 1, 2, 3) çıkış modunda ayarlanır.

d. UART Yapılandırması

UART1 ve GPIOB aktif edilir.

UART ayarları:

Baud hızı: 115200.

Veri uzunluğu: 8 bit.

Stop biti: 1.

Parite biti: Yok.

UART kesmesi, uartinterrupt() fonksiyonuna bağlanır.

e. Ana Döngü (Infinite Loop)

Kırmızı, mavi ve yeşil LED'lerin bayrak ve sayaç değerlerine göre LED'ler yakılıp söndürülür.

Sayaç değerleri:

Kırmızı LED: 3 saniye.

Mavi LED: 6 saniye.

Yeşil LED: 9 saniye.

UART Komut Formatı

Her komut 3 karakterden oluşmalıdır: -[X]*

X: LED rengi.

A: Kırmızı LED.

B: Mavi LED.

C: Yeşil LED.

Örnek Komutlar

-A*: Kırmızı LED'i aktif eder.

-B*: Mavi LED'i aktif eder.

-C*: Yeşil LED'i aktif eder.

Test ve Gözlemler

UART üzerinden komutlar gönderildiğinde, belirtilen LED belirtilen sürelerde yanıp söner.

Uygun olmayan komutlarda tüm LED'ler kapatılır ve sistem yeniden başlatılır.

Sistem Gereksinimleri

Donanım: Tiva C Series mikrodenetleyici (TM4C123G).

Geliştirme Ortamı: Code Composer Studio (CCS) veya IAR Embedded Workbench.

Bağlantı: UART haberleşmesi için PB0 (RX) ve PB1 (TX).
