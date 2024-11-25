// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

#define ___ KC_NO

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(

    KC_A, KC_B, KC_C,
    KC_D, KC_E, KC_F, KC_G

    ),
    [1] = LAYOUT(

    ___, ___, ___,
    ___, ___, ___, ___
    ),
    [2] = LAYOUT(

    ___, ___, ___,
    ___, ___, ___, ___
    ),
    [3] = LAYOUT(

    ___, ___, ___,
    ___, ___, ___, ___
    ),
    [4] = LAYOUT(

    ___, ___, ___,
    ___, ___, ___, ___
    ),
    [5] = LAYOUT(

    ___, ___, ___,
    ___, ___, ___, ___
    )
};

// Encoder update function
bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {  // If you're using one encoder (index is 0)
        if (clockwise) {
            tap_code(KC_2);  // Trigger action for turning right (e.g., Key 2)
        } else {
            tap_code(KC_1);  // Trigger action for turning left (e.g., Key 1)
        }
    }
    return false;
}


#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [0] =   { ENCODER_CCW_CW(KC_2, KC_1) },
    [1] =   { ENCODER_CCW_CW(___, ___) },
    [2] =   { ENCODER_CCW_CW(___, ___) },
    [3] =   { ENCODER_CCW_CW(___, ___) },
    [4] =   { ENCODER_CCW_CW(___, ___) },
    [5] =   { ENCODER_CCW_CW(___, ___) }

};
#endif

// x = 224 / (3 - 1) * COL_POSITION
// y =  64 / (2 - 1) * ROW_POSITION


led_config_t g_led_config = { {
  // Key Matrix to LED Index
  // Mapping of the key matrix (rows and columns) to the LED index
  {  0,  1,  2 },  // Row 0 (top row: LEDs 0, 1, 2)
  {  5,  4,  3 },  // Row 1 (bottom row: LEDs 5, 4, 3)
}, {
  // LED Index to Physical Position
  // X, Y coordinates of the LEDs (these values represent the physical positioning of each LED on the keyboard)
  {  0,  0 },  // LED 0 (top-left)
  { 64,  0 },  // LED 1 (top-center)
  {128,  0 },  // LED 2 (top-right)
  {128, 32 },  // LED 3 (bottom-right)
  { 64, 32 },  // LED 4 (bottom-center)
  {  0, 32 },  // LED 5 (bottom-left)
}, {
  // LED Index to Flag
  // Flags indicating what each LED represents (modifier, underglow, etc.)
  // For simplicity, we'll assume they're all normal keys here.
  4, 4, 4, 4, 4, 4  // All are normal key LEDs
} };

// bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
//     if (get_highest_layer(layer_state) > 0) {
//         uint8_t layer = get_highest_layer(layer_state);

//         for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
//             for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
//                 uint8_t index = g_led_config.matrix_co[row][col];

//                 if (index >= led_min && index < led_max && index != NO_LED &&
//                 keymap_key_to_keycode(layer, (keypos_t){col,row}) > KC_TRNS) {
//                     rgb_matrix_set_color(index, RGB_GREEN);
//                 }
//             }
//         }
//     }
//     return false;
// }




// DYNAMIC LAYER SWITCH INDICATORS =================================================================================================


// Function to check if a keycode is a layer switch code
bool is_layer_switch_code(uint16_t keycode) {
    // Check for layer tap keys (LT)
    if ((keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX) &&
        (((keycode >> 8) & 0xF) == 0xB)) {  // 0xB is the identifier for LT
        return true;
    }

    // Check for other layer switching keys
    switch (keycode) {
        case QK_MOMENTARY ... QK_MOMENTARY_MAX:  // MO(layer)
        case QK_DEF_LAYER ... QK_DEF_LAYER_MAX:  // DF(layer)
        case QK_TOGGLE_LAYER ... QK_TOGGLE_LAYER_MAX:  // TG(layer)
        case QK_ONE_SHOT_LAYER ... QK_ONE_SHOT_LAYER_MAX:  // OSL(layer)
        case QK_LAYER_TAP_TOGGLE ... QK_LAYER_TAP_TOGGLE_MAX:  // TT(layer)
        case QK_TO ... QK_TO_MAX:  // TO(layer)

        //add Tap Dance Layers where numbers signify the layer
        case TD(0):
        case TD(1):
        case TD(2):
        case TD(3):
        case TD(4):
        case TD(5):
        case TD(6):
        case TD(7):
        case TD(8):
        case TD(9):
        case TD(10):
        case TD(11):

            return true;
        default:
            return false;
    }
}

// Function to map matrix position to LED index
uint8_t matrix_to_led_index(uint8_t row, uint8_t col) {
    static const uint8_t led_map[2][3] = {
        {0, 1, 2},  // First row
        {5, 4, 3}   // Second row
    };

    // Check if row and col are within bounds
    if (row < 2 && col < 3) {
        return led_map[row][col];
    } else {
        // Return NO_LED or another suitable value for out-of-bounds access
        return NO_LED;
    }
}


bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    uint8_t layer = get_highest_layer(layer_state);

    // Define colors for each layer (excluding Layer 0, which should retain RGB effects)
    uint8_t layer_colors[6][3] = {
        {0, 0, 0},       // Layer 0: Transparent (will not be applied)
        {255, 0, 0},     // Layer 2: Red (255, 0, 0)
        {0, 255, 0},     // Layer 1: Green (0, 255, 0)
        {0, 255, 255},   // Layer 3: Cyan (0, 255, 0)
        {0, 0, 255},     // Layer 4: Blue (0, 0, 255)
        {255, 255, 255}  // Layer 5: White (255, 255, 255)
    };

      // Define a static brightness level for indicator LEDs (50% of 255)
    const uint8_t static_brightness = 100;  // 50% of 255

    for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
        for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
            uint8_t index = matrix_to_led_index(row, col);

            if (index != NO_LED && index >= led_min && index < led_max) {
                uint16_t keycode = keymap_key_to_keycode(layer, (keypos_t){col, row});

                // Check if this key is used for layer change
                if (is_layer_switch_code(keycode)) {
                    if (layer > 0) {
                        //retain original brightness

                         // Set color based on the current layer with static brightness of 50%
                        uint8_t r = (layer_colors[layer][0] * static_brightness) / 255;
                        uint8_t g = (layer_colors[layer][1] * static_brightness) / 255;
                        uint8_t b = (layer_colors[layer][2] * static_brightness) / 255;

                        rgb_matrix_set_color(index, r, g, b);
                    }
                }
                // For layer 0 or non-layer-switch keys, allow RGB effects to continue (no color is set)
            }
        }
    }

    return false;
}


// ==============================================================================================================================
