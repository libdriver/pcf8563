[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver PCF8563

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/pcf8563/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

The PCF8563 is a CMOS1 Real-Time Clock (RTC) and calendar optimized for low power consumption. A programmable clock output, interrupt output, and voltage-low detector are also provided. All addresses and data are transferred serially via a two-line bidirectional I2C-bus. Maximum bus speed is 400 kbit/s. The register address is incremented automatically after each written or read data byte.

LibDriver PCF8563 is a full function driver of pcf8563 launched by LibDriver.It provides time reading, alarm clock triggering, timer interrupt triggering, square wave output and other functions. LibDriver is MISRA compliant.

### Table of Contents

  - [Instruction](#Instruction)
  - [Install](#Install)
  - [Usage](#Usage)
    - [example basic](#example-basic)
    - [example alarm](#example-alarm)
    - [example timer](#example-timer)
    - [example output](#example-output)
  - [Document](#Document)
  - [Contributing](#Contributing)
  - [License](#License)
  - [Contact Us](#Contact-Us)

### Instruction

/src includes LibDriver PCF8563 source files.

/interface includes LibDriver PCF8563 IIC platform independent template.

/test includes LibDriver PCF8563 driver test code and this code can test the chip necessary function simply.

/example includes LibDriver PCF8563 sample code.

/doc includes LibDriver PCF8563 offline document.

/datasheet includes PCF8563 datasheet.

/project includes the common Linux and MCU development board sample code. All projects use the shell script to debug the driver and the detail instruction can be found in each project's README.md.

/misra includes the LibDriver MISRA code scanning results.

### Install

Reference /interface IIC platform independent template and finish your platform IIC driver.

Add the /src directory, the interface driver for your platform, and your own drivers to your project, if you want to use the default example drivers, add the /example directory to your project.

### Usage

You can refer to the examples in the /example directory to complete your own driver. If you want to use the default programming examples, here's how to use them.

#### example basic

```C
#include "driver_pcf8563_basic.h"

uint8_t res;
pcf8563_time_t time;

/* basic init */
res = pcf8563_basic_init();
if (res != 0)
{
    return 1;
}

/* set the zone */
res = pcf8563_basic_set_timestamp_time_zone(8);
if (res != 0)
{
    (void)pcf8563_basic_deinit();

    return 1;
}

...
    
/* set time */
res = pcf8563_basic_set_timestamp((time_t)1612686500);
if (res != 0)
{
    (void)pcf8563_basic_deinit();

    return 1;
}

/* output */
pcf8563_interface_debug_print("pcf8563: set timestamp %d.\n", (time_t)1612686500);

...

/* basic get time */
res = pcf8563_basic_get_time(&time);
if (res != 0)
{
    (void)pcf8563_basic_deinit();

    return 1;
}

/* output */
pcf8563_interface_debug_print("pcf8563: %04d-%02d-%02d %02d:%02d:%02d %d.\n",
                              time.year, time.month, time.date,
                              time.hour, time.minute, time.second, time.week
                             );

...
    
/* basic deinit */
(void)pcf8563_basic_deinit();

return 0;
```

#### example alarm

```C
#include "driver_pcf8563_alarm.h"

uint8_t res;
uint8_t g_alarm_flag;
uint32_t i;
pcf8563_alarm_param_t param;
pcf8563_time_t time;

static void a_alarm_callback(uint8_t type)
{
    switch (type)
    {
        case PCF8563_INTERRUPT_EVENT_TIMER :
        {
            pcf8563_interface_debug_print("pcf8563: irq timer.\n");

            break;
        }
        case PCF8563_INTERRUPT_EVENT_ALARM :
        {
            g_alarm_flag = 1;
            pcf8563_interface_debug_print("pcf8563: irq alarm.\n");

            break;
        }
        default :
        {
            pcf8563_interface_debug_print("pcf8563: irq unknown.\n");

            break;
        }
    }
}

/* alarm init */
res = pcf8563_alarm_init(a_alarm_callback);
if (res != 0)
{
    return 1;
}

/* set the zone */
res = pcf8563_alarm_set_timestamp_time_zone(8);
if (res != 0)
{
    (void)pcf8563_alarm_deinit();

    return 1;
}

...
    
/* set time */
res = pcf8563_alarm_set_timestamp((time_t)1612686500);
if (res != 0)
{
    (void)pcf8563_alarm_deinit();

    return 1;
}

/* output */
pcf8563_interface_debug_print("pcf8563: set timestamp %d.\n", (time_t)1612686500);

...

/* alarm get time */
res = pcf8563_alarm_get_time(&time);
if (res != 0)
{
    (void)pcf8563_alarm_deinit();

    return 1;
}

/* output */
pcf8563_interface_debug_print("pcf8563: %04d-%02d-%02d %02d:%02d:%02d %d.\n",
                              time.year, time.month, time.date,
                              time.hour, time.minute, time.second, time.week
                             );

...

/* gpio interrupt init */
g_gpio_irq = pcf8563_alarm_irq_handler;
res = gpio_interrupt_init();
if (res != 0)
{
    (void)pcf8563_alarm_deinit();
    
    return 1;
}

g_alarm_flag = 0;

/* enable alarm */
param.minute = 1;
param.minute_enable = PCF8563_BOOL_TRUE;
param.hour = 1;
param.hour_enable = PCF8563_BOOL_FALSE;
param.day = 1;
param.day_enable = PCF8563_BOOL_FALSE;
param.week = 1;
param.week_enable = PCF8563_BOOL_FALSE;
param.mode = PCF8563_INTERRUPT_MODE_PULSE;
res = pcf8563_alarm_enable(&param);
if (res != 0)
{
    g_gpio_irq = 0;
    gpio_interrupt_deinit();
    (void)pcf8563_alarm_deinit();

    return 1;
}

for (i = 0; i < 180; i++)
{
    if (g_alarm_flag != 0)
    {
        break;
    }
    pcf8563_interface_delay_ms(1000);
    pcf8563_interface_debug_print("pcf8563: %ds.\n", i + 1);
}

/* check flag */
if (g_alarm_flag == 0)
{
    pcf8563_interface_debug_print("pcf8563: not found alarm.\n");
}

/* gpio interrupt deinit */
g_gpio_irq = 0;
gpio_interrupt_deinit();

...

/* disable alarm */
(void)pcf8563_alarm_disable();

/* alarm deinit */
(void)pcf8563_alarm_deinit();

return 0;
```
#### example timer

```C
#include "driver_pcf8563_timer.h"

uint8_t res;
uint8_t g_alarm_flag;
uint32_t i;
pcf8563_time_t time;

static void a_timer_callback(uint8_t type)
{
    switch (type)
    {
        case PCF8563_INTERRUPT_EVENT_TIMER :
        {
            g_timer_flag = 1;
            pcf8563_interface_debug_print("pcf8563: irq timer.\n");

            break;
        }
        case PCF8563_INTERRUPT_EVENT_ALARM :
        {
            pcf8563_interface_debug_print("pcf8563: irq alarm.\n");

            break;
        }
        default :
        {
            pcf8563_interface_debug_print("pcf8563: irq unknown.\n");

            break;
        }
    }
}

/* timer init */
res = pcf8563_timer_init(a_timer_callback);
if (res != 0)
{
    return 1;
}

/* set the zone */
res = pcf8563_timer_set_timestamp_time_zone(8);
if (res != 0)
{
    (void)pcf8563_timer_deinit();

    return 1;
}

...
    
/* set time */
res = pcf8563_timer_set_timestamp((time_t)1612686500);
if (res != 0)
{
    (void)pcf8563_timer_deinit();

    return 1;
}

/* output */
pcf8563_interface_debug_print("pcf8563: set timestamp %d.\n", (time_t)1612686500);

...
    
/* timer get time */
res = pcf8563_timer_get_time(&time);
if (res != 0)
{
    (void)pcf8563_timer_deinit();

    return 1;
}

/* output */
pcf8563_interface_debug_print("pcf8563: %04d-%02d-%02d %02d:%02d:%02d %d.\n",
                              time.year, time.month, time.date,
                              time.hour, time.minute, time.second, time.week
                             );

...
    
/* gpio interrupt init */
g_gpio_irq = pcf8563_timer_irq_handler;
res = gpio_interrupt_init();
if (res != 0)
{
    (void)pcf8563_timer_deinit();
    
    return 1;
}

g_timer_flag = 0;

/* enable timer */
res = pcf8563_timer_enable(PCF8563_TIMER_FREQ_1HZ, 60, PCF8563_INTERRUPT_MODE_PULSE);
if (res != 0)
{
    g_gpio_irq = 0;
    gpio_interrupt_deinit();
    (void)pcf8563_timer_deinit();

    return 1;
}

for (i = 0; i < 180; i++)
{
    if (g_timer_flag != 0)
    {
        break;
    }
    pcf8563_interface_delay_ms(1000);
    pcf8563_interface_debug_print("pcf8563: %ds.\n", i + 1);
}

/* check flag */
if (g_timer_flag == 0)
{
    pcf8563_interface_debug_print("pcf8563: not found timer.\n");
}

/* gpio interrupt deinit */
g_gpio_irq = 0;
gpio_interrupt_deinit();

...
    
/* disable timer */
(void)pcf8563_timer_disable();
    
/* timer deinit */
(void)pcf8563_timer_deinit();

return 0;
```

#### example output

```C
#include "driver_pcf8563_output.h"

uint8_t res;
pcf8563_time_t time;

/* output init */
res = pcf8563_output_init();
if (res != 0)
{
    return 1;
}

/* set the zone */
res = pcf8563_output_set_timestamp_time_zone(8);
if (res != 0)
{
    (void)pcf8563_output_deinit();

    return 1;
}

...
    
/* set time */
res = pcf8563_output_set_timestamp((time_t)1612686500);
if (res != 0)
{
    (void)pcf8563_output_deinit();

    return 1;
}

/* output */
pcf8563_interface_debug_print("pcf8563: set timestamp %d.\n", (time_t)1612686500);

...

/* output get time */
res = pcf8563_output_get_time(&time);
if (res != 0)
{
    (void)pcf8563_output_deinit();

    return 1;
}

/* output */
pcf8563_interface_debug_print("pcf8563: %04d-%02d-%02d %02d:%02d:%02d %d.\n",
                              time.year, time.month, time.date,
                              time.hour, time.minute, time.second, time.week
                             );

...
    
/* output enable */
res = pcf8563_output_enable(PCF8563_CLOCK_OUT_1HZ);
if (res != 0)
{
    (void)pcf8563_output_deinit();

    return 1;
}

...
    
/* output disable */
res = pcf8563_output_disable();
if (res != 0)
{
    (void)pcf8563_output_deinit();

    return 1;
}

...
    
/* output deinit */
(void)pcf8563_output_deinit();

return 0;
```

### Document

Online documents: [https://www.libdriver.com/docs/pcf8563/index.html](https://www.libdriver.com/docs/pcf8563/index.html).

Offline documents: /doc/html/index.html.

### Contributing

Please refer to CONTRIBUTING.md.

### License

Copyright (c) 2015 - present LibDriver All rights reserved



The MIT License (MIT) 



Permission is hereby granted, free of charge, to any person obtaining a copy

of this software and associated documentation files (the "Software"), to deal

in the Software without restriction, including without limitation the rights

to use, copy, modify, merge, publish, distribute, sublicense, and/or sell

copies of the Software, and to permit persons to whom the Software is

furnished to do so, subject to the following conditions: 



The above copyright notice and this permission notice shall be included in all

copies or substantial portions of the Software. 



THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR

IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,

FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE

AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER

LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,

OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE

SOFTWARE. 

### Contact Us

Please send an e-mail to lishifenging@outlook.com.