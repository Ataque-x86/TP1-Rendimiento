#include <Arduino.h>

extern "C" {
#include <user_interface.h>
}

volatile uint32_t sink_u32 = 0;
volatile float sink_f32 = 0.0f;

const uint32_t N_INT = 90000000UL;
const uint32_t N_FLOAT = 90000000UL;
const uint8_t RUNS = 5;

uint32_t run_int_loop() {
  uint32_t t0 = micros();
  uint32_t acc = 0;

  for (uint32_t i = 0; i < N_INT; ++i) {
    acc += (i & 0x7U);
    if ((i & 0xFFFFFU) == 0) {
      ESP.wdtFeed();
    }
  }

  sink_u32 = acc;
  return micros() - t0;
}

uint32_t run_float_loop() {
  uint32_t t0 = micros();
  float acc = 0.0f;

  for (uint32_t i = 0; i < N_FLOAT; ++i) {
    acc += 0.1f;
    if ((i & 0xFFFFFU) == 0) {
      ESP.wdtFeed();
    }
  }

  sink_f32 = acc;
  return micros() - t0;
}

void set_cpu_freq_mhz(uint8_t mhz) {
  if (mhz == 160U) {
    system_update_cpu_freq(SYS_CPU_160MHZ);
  } else {
    system_update_cpu_freq(SYS_CPU_80MHZ);
  }
}

void run_once(uint8_t mhz, uint8_t run_id) {
  set_cpu_freq_mhz(mhz);
  delay(100);

  uint32_t t_int_us = run_int_loop();
  uint32_t t_float_us = run_float_loop();

  Serial.printf(
      "run=%u cpu=%u MHz int=%.3f s float=%.3f s\n",
      run_id,
      (unsigned int)system_get_cpu_freq(),
      (double)t_int_us / 1000000.0,
      (double)t_float_us / 1000000.0);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("ESP8266 frequency scaling benchmark");
  Serial.printf("N_INT=%lu N_FLOAT=%lu RUNS=%u\n",
                (unsigned long)N_INT,
                (unsigned long)N_FLOAT,
                (unsigned int)RUNS);

  for (uint8_t r = 1; r <= RUNS; ++r) {
    run_once(80U, r);
  }
  for (uint8_t r = 1; r <= RUNS; ++r) {
    run_once(160U, r);
  }

  Serial.println("done");
}

void loop() {}
