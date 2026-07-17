#!/usr/bin/env py

import enum


class Command(enum.StrEnum):
    MOVE_RIGHT = '>'
    MOVE_LEFT = '<'
    INCREMENT = '+'
    DECREMENT = '-'
    PUT_CHAR = '.'
    GET_CHAR = ','
    LOOP_START = '['
    LOOP_END = ']'
