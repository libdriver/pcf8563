[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver PCF8563

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/pcf8563/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE) 

Der PCF8563 ist eine CMOS1-Echtzeituhr (RTC) und ein Kalender, die auf geringen Stromverbrauch optimiert sind. Ein programmierbarer Taktausgang, ein Interrupt-Ausgang und ein Unterspannungsdetektor sind ebenfalls vorhanden. Alle Adressen und Daten werden seriell über einen bidirektionalen I2C-Bus mit zwei Leitungen übertragen. Die maximale Busgeschwindigkeit beträgt 400 kbit/s. Die Registeradresse wird nach jedem geschriebenen oder gelesenen Datenbyte automatisch erhöht.

LibDriver PCF8563 ist ein voll funktionsfähiger Treiber von pcf8563, der von LibDriver eingeführt wurde. Er bietet Zeitablesung, Weckerauslösung, Timer-Interruptauslösung, Rechteckwellenausgabe und andere Funktionen. LibDriver ist MISRA-kompatibel.

### Inhaltsverzeichnis

  - [Anweisung](#Anweisung)
  - [Installieren](#Installieren)
  - [Nutzung](#Nutzung)
    - [example basic](#example-basic)
    - [example alarm](#example-alarm)
    - [example timer](#example-timer)
    - [example output](#example-output)
  - [Dokument](#Dokument)
  - [Beitrag](#Beitrag)
  - [Lizenz](#Lizenz)
  - [Kontaktieren Sie uns](#Kontaktieren-Sie-uns)

### Anweisung

/src enthält LibDriver PCF8563-Quelldateien.

/interface enthält die plattformunabhängige Vorlage LibDriver PCF8563 IIC.

/test enthält den Testcode des LibDriver PCF8563-Treibers und dieser Code kann die erforderliche Funktion des Chips einfach testen.

/example enthält LibDriver PCF8563-Beispielcode.

/doc enthält das LibDriver PCF8563-Offlinedokument.

/Datenblatt enthält PCF8563-Datenblatt.

/project enthält den allgemeinen Beispielcode für Linux- und MCU-Entwicklungsboards. Alle Projekte verwenden das Shell-Skript, um den Treiber zu debuggen, und die detaillierten Anweisungen finden Sie in der README.md jedes Projekts.

/misra enthält die Ergebnisse des LibDriver MISRA Code Scans.

### Installieren

Verweisen Sie auf eine plattformunabhängige IIC-Schnittstellenvorlage und stellen Sie Ihren Plattform-IIC-Treiber fertig.

Fügen Sie das Verzeichnis /src, den Schnittstellentreiber für Ihre Plattform und Ihre eigenen Treiber zu Ihrem Projekt hinzu. Wenn Sie die Standardbeispieltreiber verwenden möchten, fügen Sie das Verzeichnis /example zu Ihrem Projekt hinzu.

### Nutzung

Sie können auf die Beispiele im Verzeichnis /example zurückgreifen, um Ihren eigenen Treiber zu vervollständigen. Wenn Sie die Standardprogrammierbeispiele verwenden möchten, erfahren Sie hier, wie Sie diese verwenden.

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

### Dokument

Online-Dokumente: [https://www.libdriver.com/docs/pcf8563/index.html](https://www.libdriver.com/docs/pcf8563/index.html).

Offline-Dokumente: /doc/html/index.html.

### Beitrag

Bitte beachten Sie CONTRIBUTING.md.

### Lizenz

Urheberrechte © (c) 2015 - Gegenwart LibDriver Alle Rechte vorbehalten



Die MIT-Lizenz (MIT)



Hiermit wird jeder Person kostenlos die Erlaubnis erteilt, eine Kopie zu erhalten

dieser Software und zugehörigen Dokumentationsdateien (die „Software“) zu behandeln

in der Software ohne Einschränkung, einschließlich, aber nicht beschränkt auf die Rechte

zu verwenden, zu kopieren, zu modifizieren, zusammenzuführen, zu veröffentlichen, zu verteilen, unterzulizenzieren und/oder zu verkaufen

Kopien der Software und Personen, denen die Software gehört, zu gestatten

dazu eingerichtet werden, unter folgenden Bedingungen:



Der obige Urheberrechtshinweis und dieser Genehmigungshinweis müssen in allen enthalten sein

Kopien oder wesentliche Teile der Software.



DIE SOFTWARE WIRD "WIE BESEHEN" BEREITGESTELLT, OHNE JEGLICHE GEWÄHRLEISTUNG, AUSDRÜCKLICH ODER

STILLSCHWEIGEND, EINSCHLIESSLICH, ABER NICHT BESCHRÄNKT AUF DIE GEWÄHRLEISTUNG DER MARKTGÄNGIGKEIT,

EIGNUNG FÜR EINEN BESTIMMTEN ZWECK UND NICHTVERLETZUNG VON RECHTEN DRITTER. IN KEINEM FALL DARF DAS

AUTOREN ODER URHEBERRECHTSINHABER HAFTEN FÜR JEGLICHE ANSPRÜCHE, SCHÄDEN ODER ANDERE

HAFTUNG, OB AUS VERTRAG, DELIKT ODER ANDERWEITIG, ENTSTEHEND AUS,

AUS ODER IM ZUSAMMENHANG MIT DER SOFTWARE ODER DER VERWENDUNG ODER ANDEREN HANDLUNGEN MIT DER

SOFTWARE.

### Kontaktieren Sie uns

Bitte senden Sie eine E-Mail an lishifenging@outlook.com.