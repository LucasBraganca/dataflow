//
// Created by lucas on 08/02/2020.
//

#ifndef MAIN_DEFS_H
#define MAIN_DEFS_H

typedef enum {
    OP_BASIC,
    OP_IMMEDIATE,
    OP_IN,
    OP_OUT
} op_type_t;

typedef enum {
    PORT_A = 0,
    PORT_B = 1,
    PORT_BRANCH = 2
} PORT;

#endif //MAIN_DEFS_H
