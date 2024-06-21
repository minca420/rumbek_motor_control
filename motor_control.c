// Program do sterowania silnikami za pomocą pada PS2
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>

#define PIN_M1A 7 // GPIO pin
#define PIN_M1B 0 // GPIO pin
#define PIN_M2A 2 // GPIO pin
#define PIN_M2B 3 // GPIO pin

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input device>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    // Initialize WiringPi
    if (wiringPiSetup() == -1) {
        fprintf(stderr, "WiringPi setup failed\n");
        exit(1);
    }

    // Set pin mode to output
    pinMode(PIN_M1A, OUTPUT);
    pinMode(PIN_M1B, OUTPUT);
    pinMode(PIN_M2A, OUTPUT);
    pinMode(PIN_M2B, OUTPUT);

    void wheel_forward_left() {
        digitalWrite(PIN_M1A, HIGH);
        digitalWrite(PIN_M1B, LOW);
    }
    void wheel_backward_left() {
        digitalWrite(PIN_M1A, LOW);
        digitalWrite(PIN_M1B, HIGH);
    }
    void wheel_forward_right() {
        digitalWrite(PIN_M2A, HIGH);
        digitalWrite(PIN_M2B, LOW);
    }
    void wheel_backward_right() {
        digitalWrite(PIN_M2A, LOW);
        digitalWrite(PIN_M2B, HIGH);
    }
    void wheel_stop_left() {
	digitalWrite(PIN_M1A, LOW);
	digitalWrite(PIN_M1B, LOW);
    }
    void wheel_stop_right() {
	digitalWrite(PIN_M2A, LOW);
	digitalWrite(PIN_M2B, LOW);
    }
    void wheels_stop() {
	wheel_stop_left();
	wheel_stop_right();
    }
    void wheels_forward() {
        printf("wheels_forward\n");
	wheel_forward_left();
	wheel_forward_right();
    }
    void wheels_backward() {
	wheel_backward_left();
	wheel_backward_right();
    }
    void wheels_turn_left() {
	wheel_forward_right();
	wheel_backward_left();
    }
    void wheels_turn_right() {
	wheel_forward_left();
	wheel_backward_right();
    }

    const char *device = argv[1];
    int fd = open(device, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open the input device");
        exit(EXIT_FAILURE);
    }

    struct input_event ev;
    while (1) {
        ssize_t n = read(fd, &ev, sizeof(ev));
        if (n == (ssize_t)-1) {
            perror("Failed to read input event");
            close(fd);
            exit(EXIT_FAILURE);
        } else if (n != sizeof(ev)) {
            fprintf(stderr, "Unexpected read size\n");
            close(fd);
            exit(EXIT_FAILURE);
        }

        // Process the input event
        printf("Event type: %d, code: %d, value: %d\n", ev.type, ev.code, ev.value);
	if (ev.type == 1 && ev.code == 288) {
		if (ev.value == 1) wheels_forward();
		else wheels_stop();
	} else if (ev.type == 1 && ev.code == 290) {
		if (ev.value == 1) wheels_backward();
		else wheels_stop();
	}
    }

    close(fd);
    return 0;
}

