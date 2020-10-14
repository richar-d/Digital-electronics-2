#ifndef GPIO_H
#define GPIO_H

/***********************************************************************
 * 
 * GPIO library for AVR-GCC.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2019-2020 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 *
 **********************************************************************/

/**
 * @file  gpio.h
 * @brief GPIO library for AVR-GCC.
 *
 * @details
 * The library contains functions for controlling AVRs' gpio pin(s).
 *
 * @note
 * Based on AVR Libc Reference Manual. Tested on ATmega328P (Arduino Uno),
 * 16 MHz, AVR 8-bit Toolchain 3.6.2.
 * 
 * @copyright (c) 2019-2020 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 */

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>

/* Function prototypes -----------------------------------------------*/
/**
 * @brief Configure one output pin in Data Direction Register.
 * @param reg_name - Address of Data Direction Register, such as &DDRA,
 *                   &DDRB, ...
 * @param pin_num - Pin designation in the interval 0 to 7
 */

//Configure one output pin in Data Direction Register
void GPIO_config_output(volatile uint8_t *reg_name, uint8_t pin_num); 

//Configure one input pin in DDR without pull-up resistor
void GPIO_config_input_nopull(volatile uint8_t *reg_name, uint8_t pin_num);

//Configure one input pin in DDR and enable pull-up resistor
void GPIO_config_input_pullup(volatile uint8_t *reg_name, uint8_t pin_num);

//Set one output pin in PORT register to low
void GPIO_write_low(volatile uint8_t *reg_name, uint8_t pin_num);

//Set one output pin in PORT register to high
void GPIO_write_high(volatile uint8_t *reg_name, uint8_t pin_num);

//Toggle one output pin value in PORT register
void GPIO_toggle(volatile uint8_t *reg_name, uint8_t pin_num);

//Get input pin value from PIN register
uint8_t GPIO_read(volatile uint8_t *reg_name, uint8_t pin_num);

#endif