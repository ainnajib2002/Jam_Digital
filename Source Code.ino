#include <Wire.h>
#include <DS3231.h> 
DS3231  rtc(SDA, SCL); 
Time  t;
unsigned int h,m,s;
byte f;

#include <TM1637Display.h>
#define CLK 7
#define DIO 6
TM1637Display display(CLK, DIO);

#define BUTTON_RESET_PIN 2
#define BUTTON_UP_PIN    3
#define BUZZER_PIN       4 // Tambah definisi pin buzzer
#define BUZZER_DURATION 50 // Durasi bunyi buzzer (ms)

bool buttonResetState = false;
bool buttonUpState = false;
bool settingMode = false; // false untuk mode jam, true untuk mode menit

void setup()
{
  Serial.begin(9600);
  rtc.begin();
  display.setBrightness(7);
  pinMode(BUTTON_RESET_PIN, INPUT_PULLUP);
  pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT); // Inisialisasi pin buzzer
  
  //--------------------------------------------------------------
  //aktifkan baris perintah ini untuk seting RTC
  //lakukan jika waktu tidak sesuai
  //cukup sekali saja, setelah itu di-non-aktifkan lagi perintahnya
  rtc.setDate(8, 8, 2020);  //setting tanggal 8 Agustus 2020
  rtc.setTime(12, 58, 00);   //setting jam 12:58:00
  rtc.setDOW(6);            //setting hari Jumat, 0=Minggu,1=Senin,2=Selasa,3=Rabu,4=Kamis,5=Sabtu
  //--------------------------------------------------------------- 
} 

void loop()
{
  // Baca status tombol
  buttonResetState = digitalRead(BUTTON_RESET_PIN);
  buttonUpState = digitalRead(BUTTON_UP_PIN);

  // Tombol reset untuk mengubah mode pengaturan
  if (buttonResetState == LOW) {
    delay(200); // Delay untuk anti bounce
    settingMode = !settingMode; // Toggle mode pengaturan
    digitalWrite(BUZZER_PIN, HIGH); // Bunyi buzzer
    delay(BUZZER_DURATION); // Tahan bunyi
    digitalWrite(BUZZER_PIN, LOW); // Matikan bunyi
  }
  
  // Jika dalam mode pengaturan jam
  if (!settingMode) {
    // Tambah jam jika tombol up ditekan
    if (buttonUpState == LOW) {
      incrementHour();
      delay(200); // Delay untuk anti bounce
    }
  }
  // Jika dalam mode pengaturan menit
  else {
    // Tambah menit jika tombol up ditekan
    if (buttonUpState == LOW) {
      incrementMinute();
      delay(200); // Delay untuk anti bounce
    }
  }
  
  // Tampilkan waktu pada TM1637
  displayTime();
}

// Fungsi untuk menambah jam
void incrementHour() {
  t = rtc.getTime();
  h = t.hour;
  m = t.min;
  s = t.sec;
  
  h++;
  if (h > 23) {
    h = 0;
  }
  
  rtc.setTime(h, m, s);
}

// Fungsi untuk menambah menit
void incrementMinute() {
  t = rtc.getTime();
  h = t.hour;
  m = t.min;
  s = t.sec;
  
  m++;
  if (m > 59) {
    m = 0;
  }
  
  rtc.setTime(h, m, s);
}

// Fungsi untuk menampilkan waktu pada TM1637
void displayTime() {
  t = rtc.getTime();
  h = t.hour;
  m = t.min;
  s = t.sec;
  
  h = h * 100 + m;
  
  if (f == 0) {
    display.showNumberDecEx(h, 0b01000000); // Tampilkan jam dengan titik di depan
    f = 1;
  } else {
    display.showNumberDecEx(h, 0); // Tampilkan menit tanpa titik
    f = 0;
  }
  
  delay(1000);
}
