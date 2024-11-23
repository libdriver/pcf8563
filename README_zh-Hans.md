[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver PCF8563

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/pcf8563/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

PCF8563是一款CMOS实时时钟(RTC)和日历，最适合低功耗应用。还提供可编程时钟输出、中断输出和电压过低检测器。通过两线双向I²C总线串行传送所有地址和数据。最大总线速度为400 kbit/s。寄存器地址会在每次写入或读取数据字节后自动递增。

LibDriver PCF8563是LibDriver推出的PCF8563全功能驱动，该驱动提供时间读取、闹钟触发、定时器触发和方波输出等功能并且它符合MISRA标准。

### 目录

  - [说明](#说明)
  - [安装](#安装)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example alarm](#example-alarm)
    - [example timer](#example-timer)
    - [example output](#example-output)
  - [文档](#文档)
  - [贡献](#贡献)
  - [版权](#版权)
  - [联系我们](#联系我们)

### 说明

/src目录包含了LibDriver PCF8563的源文件。

/interface目录包含了LibDriver PCF8563与平台无关的IIC总线模板。

/test目录包含了LibDriver PCF8563驱动测试程序，该程序可以简单的测试芯片必要功能。

/example目录包含了LibDriver PCF8563编程范例。

/doc目录包含了LibDriver PCF8563离线文档。

/datasheet目录包含了PCF8563数据手册。

/project目录包含了常用Linux与单片机开发板的工程样例。所有工程均采用shell脚本作为调试方法，详细内容可参考每个工程里面的README.md。

/misra目录包含了LibDriver MISRA代码扫描结果。

### 安装

参考/interface目录下与平台无关的IIC总线模板，完成指定平台的IIC总线驱动。

将/src目录，您使用平台的接口驱动和您开发的驱动加入工程，如果您想要使用默认的范例驱动，可以将/example目录加入您的工程。

### 使用

您可以参考/example目录下的编程范例完成适合您的驱动，如果您想要使用默认的编程范例，以下是它们的使用方法。

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

### 文档

在线文档: [https://www.libdriver.com/docs/pcf8563/index.html](https://www.libdriver.com/docs/pcf8563/index.html)。

离线文档: /doc/html/index.html。

### 贡献

请参考CONTRIBUTING.md。

### 版权

版权 (c) 2015 - 现在 LibDriver 版权所有

MIT 许可证（MIT）

特此免费授予任何获得本软件副本和相关文档文件（下称“软件”）的人不受限制地处置该软件的权利，包括不受限制地使用、复制、修改、合并、发布、分发、转授许可和/或出售该软件副本，以及再授权被配发了本软件的人如上的权利，须在下列条件下：

上述版权声明和本许可声明应包含在该软件的所有副本或实质成分中。

本软件是“如此”提供的，没有任何形式的明示或暗示的保证，包括但不限于对适销性、特定用途的适用性和不侵权的保证。在任何情况下，作者或版权持有人都不对任何索赔、损害或其他责任负责，无论这些追责来自合同、侵权或其它行为中，还是产生于、源于或有关于本软件以及本软件的使用或其它处置。

### 联系我们

请联系lishifenging@outlook.com。