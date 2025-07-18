#include "UltrasonicSensor.h"

UltrasonicSensor::UltrasonicSensor(uint trig, uint echo)
    : trigPin(trig), echoPin(echo) {}

void UltrasonicSensor::init() {
    gpio_init(trigPin);
    gpio_set_dir(trigPin, GPIO_OUT);
    gpio_put(trigPin, 0);

    gpio_init(echoPin);
    gpio_set_dir(echoPin, GPIO_IN);
}

int64_t UltrasonicSensor::measureEchoTime() {
    gpio_put(trigPin, 1);
    sleep_us(10);
    gpio_put(trigPin, 0);

    absolute_time_t timeout = make_timeout_time_us(2000);
    while (!gpio_get(echoPin)) {
        if (absolute_time_diff_us(get_absolute_time(), timeout) <= 0) {
            return -1;
        }
    }

    absolute_time_t start = get_absolute_time();
    timeout = make_timeout_time_us(30000);
    while (gpio_get(echoPin)) {
        if (absolute_time_diff_us(get_absolute_time(), timeout) <= 0) {
            return -1;
        }
    }

    absolute_time_t end = get_absolute_time();
    return absolute_time_diff_us(start, end);
}

std::optional<float> UltrasonicSensor::readDistanceCM() {
    int64_t duration = measureEchoTime();
    if (duration < 0) return std::nullopt;

    float distance = duration * 0.0343f / 2.0f;
    return distance;
}
