#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define SERVO_PIN 22
#define PWM_FREQUENCY 50  // Frequência de 50Hz (Período de 20ms)

void set_servo_pulse(uint slice_num, uint channel, uint32_t pulse_width_us) {
    uint32_t clock_speed = clock_get_hz(clk_sys);
    uint32_t wrap_value = clock_speed / PWM_FREQUENCY - 1;
    pwm_set_wrap(slice_num, wrap_value);
    uint32_t level = (pulse_width_us * (wrap_value + 1)) / 20000;
    pwm_set_chan_level(slice_num, channel, level);
}

int main() {
    stdio_init_all();
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(SERVO_PIN);
    uint channel = pwm_gpio_to_channel(SERVO_PIN);
    pwm_set_enabled(slice_num, true);

    while (1) {
        set_servo_pulse(slice_num, channel, 2400); // 180°
        sleep_ms(5000);
        set_servo_pulse(slice_num, channel, 1470); // 90°
        sleep_ms(5000);
        set_servo_pulse(slice_num, channel, 500);  // 0°
        sleep_ms(5000);

        for (uint32_t pulse = 500; pulse <= 2400; pulse += 5) {
            set_servo_pulse(slice_num, channel, pulse);
            sleep_ms(10);
        }
        for (uint32_t pulse = 2400; pulse >= 500; pulse -= 5) {
            set_servo_pulse(slice_num, channel, pulse);
            sleep_ms(10);
        }
    }
}
