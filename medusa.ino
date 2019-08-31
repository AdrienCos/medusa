#include <Keyboard.h>

#define NUM_BUTTONS 1
#define DEBOUNCE_DELAY_MS 200

// Define pins
#define INTERRUPT_PIN 2
#define DATA_SELECT_0 3
#define DATA_SELECT_1 3
#define DATA_SELECT_2 4
#define DATA_SELECT_3 5
#define DATA_SELECT_4 6
#define DATA_SELECT_5 7
#define DATA_SELECT_6 8
#define DATA_IN 10

// Define structs
typedef struct button {
    unsigned long last_pressed;
    bool state;
    char key;
} button_t;

// Global variables
int counter;
button_t* buttons;

void setup()
{
    int i;
    Serial.begin(9600);
    Keyboard.begin();
    counter = 0;
    // Initialize the pins
    pinMode(INTERRUPT_PIN, INPUT);
    pinMode(DATA_SELECT_0, OUTPUT);
    pinMode(DATA_SELECT_1, OUTPUT);
    pinMode(DATA_SELECT_2, OUTPUT);
    pinMode(DATA_SELECT_3, OUTPUT);
    pinMode(DATA_SELECT_4, OUTPUT);
    pinMode(DATA_SELECT_5, OUTPUT);
    pinMode(DATA_SELECT_6, OUTPUT);
    pinMode(DATA_IN, INPUT);
    // Initialize buttons
    buttons
        = (button_t*)malloc(sizeof(button_t) * NUM_BUTTONS);
    for (i = 0; i < NUM_BUTTONS; i++) {
        buttons[i].key = 'i';
        buttons[i].last_pressed = 0;
        buttons[i].state = 0;
    }
    // Declare the interrupt
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), scan_buttons, CHANGE);
}

bool scan_button(int i)
{
    bool out;
    // TODO: implement button reading from GPIO
    // Set the data select address
    digitalWrite(DATA_SELECT_0, (i >> 0) % 2);
    digitalWrite(DATA_SELECT_1, (i >> 1) % 2);
    digitalWrite(DATA_SELECT_2, (i >> 2) % 2);
    digitalWrite(DATA_SELECT_3, (i >> 3) % 2);
    digitalWrite(DATA_SELECT_4, (i >> 4) % 2);
    digitalWrite(DATA_SELECT_5, (i >> 5) % 2);
    digitalWrite(DATA_SELECT_6, (i >> 6) % 2);

    // Read the data
    out = digitalRead(DATA_IN);
    return out;
}

void scan_buttons()
{
    int i;
    bool state;
    for (i = 0; i < NUM_BUTTONS; i++) {
        state = scan_button(i);
        if (state != buttons[i].state && buttons[i].last_pressed + DEBOUNCE_DELAY_MS < millis()) {
            buttons[i].last_pressed = millis();
            buttons[i].state = state;
            Serial.println(buttons[i].key);
            Keyboard.print(buttons[i].key);
        }
    }
}

void loop()
{
    // Do nothing, wait for an interrupt
    delay(1);
}
