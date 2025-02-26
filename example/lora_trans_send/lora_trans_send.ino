#include <Arduino.h>
#include <vector>
#include "e220900t22s_jp.h"

CLoRa lora;

// LoRa設定値
struct LoRaConfigItem_t config = {
  0xFFFF,   // own_address
  0b011,    // baud_rate 9600 bps
  0b10000,  // air_data_rate SF:9 BW:125
  0b11,     // subpacket_size 32
  0b1,      // rssi_ambient_noise_flag 有効
  0b0,      // transmission_pause_flag 有効
  0b01,     // transmitting_power 13 dBm
  0x00,     // own_channel 0
  0b1,      // rssi_byte_flag 有効
  TRANSPARENT_MODE,      // transmission_method_type トランスペアレントモード
  0b0,      // lbt_flag 有効
  0b011,    // wor_cycle 2000 ms
  0x0000,   // encryption_key 0
  0x0000,   // target_address 0
  0x00      // target_channel 0
};

void setup() {
  // put your setup code here, to run once:
  pinMode(LED0, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  /* Set serial baudrate. */
  Serial.begin(115200);

  /* Wait HW initialization done. */
  sleep(1);
  Serial.printf("program start\n");

   // LoRaライブラリの初期設定
  lora.begin();

  // E220-900T22S(JP)へのLoRa初期設定
  if (lora.InitLoRaModule(config)) {
    SerialMon.printf("init error\n");
    return;
  } else {
    Serial.printf("init ok\n");
  }

  // ノーマルモード(M0=0,M1=0)へ移行する
  SerialMon.printf("switch to normal mode\n");
  lora.SwitchToNormalMode();
}

/* Send data size */
#define MAX_SEND_DATA_SIZE (20) 

void loop() {
  char msg[MAX_SEND_DATA_SIZE] = { 0 };
  static char dmy_data = '0';
  static int send_data_size = 10;

  for (int i = 0; i < send_data_size; i++) msg[i] = dmy_data + (i % 40);
  dmy_data > 'S' ? dmy_data = '0' : dmy_data++;
  (send_data_size >= MAX_SEND_DATA_SIZE) ? send_data_size = 10 : send_data_size++;

  if (lora.SendFrame((uint8_t *)msg, send_data_size) == 0) {
    SerialMon.printf("send succeeded.\n");
    SerialMon.printf("\n");
  } else {
    SerialMon.printf("send failed.\n");
    SerialMon.printf("\n");
  }
  SerialMon.flush();

  sleep(10);
}
