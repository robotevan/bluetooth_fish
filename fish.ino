#include "BluetoothA2DPSink.h"

/* macros for bluetooth */ 
#define BLUETOOTH_DEVICE_NAME "fish"
/* macros for audio pins */
#define I2S_DOUT_PIN          21
#define I2S_BCK_PIN           26
#define I2S_WS_PIN            25
/* macros for motor control pins */
#define MOUTH_PIN             23
#define TAIL_PIN              4
/* macros for time */
#define MOUTH_MOVE_DURATION   800 // ms
#define TAIL_MOVE_DURATION    800 // ms
#define MOUTH_MOVE_PROB       76  // %
#define TAIL_MOVE_PROB        40  // %

/* create global vars */
BluetoothA2DPSink a2dp_sink;
i2s_pin_config_t a2dp_config = {
  .bck_io_num  = I2S_BCK_PIN,  
  .ws_io_num = I2S_WS_PIN,
  .data_out_num = I2S_DOUT_PIN,
  .data_in_num = I2S_PIN_NO_CHANGE,
};

void setup() {
  /* setup Serial */
  Serial.begin(9600);

  /* initialize a2dp */
  a2dp_sink.set_pin_config(a2dp_config);
  a2dp_sink.start(BLUETOOTH_DEVICE_NAME);

  /* initialize motor pins */
  pinMode(MOUTH_PIN, OUTPUT);
  pinMode(TAIL_PIN, OUTPUT);

}

void loop() {
  /* if not playing, skip motor movements */
  delay(250);
  esp_a2d_audio_state_t state = a2dp_sink.get_audio_state();
  if (state == ESP_A2D_AUDIO_STATE_STOPPED ||
      state == ESP_A2D_AUDIO_STATE_REMOTE_SUSPEND) {
    /* paused, not connected, close mouth and do nothing */
    digitalWrite(MOUTH_PIN, LOW);
    digitalWrite(TAIL_PIN, LOW);
    return;
  }
  open_mouth();
  move_tail();
}


void open_mouth() {
  /* if mouth move furation ellapsed */
  if (digitalRead(MOUTH_PIN)) {
    digitalWrite(MOUTH_PIN, LOW);
    return;
  }

  /* if rand % in min probability, open mouth, set last open time */
  if (random(0, 100) <= MOUTH_MOVE_PROB) {
    digitalWrite(MOUTH_PIN, HIGH);
  }
}

void move_tail() {
  /* if mouth move furation ellapsed */
  if (digitalRead(TAIL_PIN)) {
    digitalWrite(TAIL_PIN, LOW);
    return;
  }

  /* if rand % in min probability, open mouth, set last open time */
  if (random(0, 100) <= TAIL_MOVE_PROB) {
    digitalWrite(TAIL_PIN, HIGH);
  }
}
