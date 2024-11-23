[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver PCF8563

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/pcf8563/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

PCF8563是一款CMOS即時時鐘（RTC）和行事曆，最適合低功耗應用。 還提供可程式設計時鐘輸出、中斷輸出和電壓過低檢測器。 通過兩線雙向I²C匯流排串列傳送所有地址和數據。 最大匯流排速度為400 kbit/s。 寄存器地址會在每次寫入或讀取數據位元組後自動遞增。

LibDriver PCF8563是LibDriver推出的PCF8563全功能驅動，該驅動提供時間讀取、鬧鐘觸發、計时器觸發和方波輸出等功能並且它符合MISRA標準。

### 目錄

  - [說明](#說明)
  - [安裝](#安裝)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example alarm](#example-alarm)
    - [example timer](#example-timer)
    - [example output](#example-output)
  - [文檔](#文檔)
  - [貢獻](#貢獻)
  - [版權](#版權)
  - [聯繫我們](#聯繫我們)

### 說明

/src目錄包含了LibDriver PCF8563的源文件。

/interface目錄包含了LibDriver PCF8563與平台無關的IIC總線模板。

/test目錄包含了LibDriver PCF8563驅動測試程序，該程序可以簡單的測試芯片必要功能。

/example目錄包含了LibDriver PCF8563編程範例。

/doc目錄包含了LibDriver PCF8563離線文檔。

/datasheet目錄包含了PCF8563數據手冊。

/project目錄包含了常用Linux與單片機開發板的工程樣例。所有工程均採用shell腳本作為調試方法，詳細內容可參考每個工程裡面的README.md。

/misra目錄包含了LibDriver MISRA程式碼掃描結果。

### 安裝

參考/interface目錄下與平台無關的IIC總線模板，完成指定平台的IIC總線驅動。

將/src目錄，您使用平臺的介面驅動和您開發的驅動加入工程，如果您想要使用默認的範例驅動，可以將/example目錄加入您的工程。

### 使用

您可以參考/example目錄下的程式設計範例完成適合您的驅動，如果您想要使用默認的程式設計範例，以下是它們的使用方法。

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

### 文檔

在線文檔: [https://www.libdriver.com/docs/pcf8563/index.html](https://www.libdriver.com/docs/pcf8563/index.html)。

離線文檔: /doc/html/index.html。

### 貢獻

請參攷CONTRIBUTING.md。

### 版權

版權 (c) 2015 - 現在 LibDriver 版權所有

MIT 許可證（MIT）

特此免費授予任何獲得本軟件副本和相關文檔文件（下稱“軟件”）的人不受限制地處置該軟件的權利，包括不受限制地使用、複製、修改、合併、發布、分發、轉授許可和/或出售該軟件副本，以及再授權被配發了本軟件的人如上的權利，須在下列條件下：

上述版權聲明和本許可聲明應包含在該軟件的所有副本或實質成分中。

本軟件是“如此”提供的，沒有任何形式的明示或暗示的保證，包括但不限於對適銷性、特定用途的適用性和不侵權的保證。在任何情況下，作者或版權持有人都不對任何索賠、損害或其他責任負責，無論這些追責來自合同、侵權或其它行為中，還是產生於、源於或有關於本軟件以及本軟件的使用或其它處置。

### 聯繫我們

請聯繫lishifenging@outlook.com。
