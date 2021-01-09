#ifndef _INPUT_H_
#define _INPUT_H_

#include <stdio.h>
#include <stdint.h>


const int INPUT_KEY_ENTER = 1;
const int INPUT_KEY_LEFT  = 2;
const int INPUT_KEY_RIGHT = 3;
const int INPUT_KEY_UP    = 4;
const int INPUT_KEY_DOWN  = 5;
const int INPUT_KEY_ESC   = 6;
const int INPUT_KEY_Y     = 7;
const int INPUT_KEY_N     = 8;

typedef struct input_key_struct {
    int key_code;
} input_key_t;

typedef struct input_struct {

} input_t;


#endif
