[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver PCF8563

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/pcf8563/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

PCF8563은 저전력 소모에 최적화된 CMOS1 실시간 클록(RTC) 및 캘린더입니다. 프로그래밍 가능한 클록 출력, 인터럽트 출력 및 전압 낮음 감지기도 제공됩니다. 모든 주소와 데이터는 2라인 양방향 I2C 버스를 통해 직렬로 전송됩니다. 최대 버스 속도는 400kbit/s입니다. 레지스터 주소는 각 쓰기 또는 읽기 데이터 바이트 후에 자동으로 증가합니다.

LibDriver PCF8563은 LibDriver가 출시한 pcf8563의 전체 기능 드라이버입니다. 시간 판독, 알람 시계 트리거링, 타이머 인터럽트 트리거링, 사각파 출력 및 기타 기능을 제공합니다. LibDriver는 MISRA를 준수합니다.

### 콘텐츠

  - [설명](#설명)
  - [설치](#설치)
  - [사용](#사용)
    - [example basic](#example-basic)
    - [example alarm](#example-alarm)
    - [example timer](#example-timer)
    - [example output](#example-output)
  - [문서](#문서)
  - [기고](#기고)
  - [저작권](#저작권)
  - [문의하기](#문의하기)

### 설명

/src 디렉토리에는 LibDriver PCF8563의 소스 파일이 포함되어 있습니다.

/interface 디렉토리에는 LibDriver PCF8563용 플랫폼 독립적인 IIC버스 템플릿이 포함되어 있습니다.

/test 디렉토리에는 LibDriver PCF8563드라이버 테스트 프로그램이 포함되어 있어 칩의 필요한 기능을 간단히 테스트할 수 있습니다.

/example 디렉토리에는 LibDriver PCF8563프로그래밍 예제가 포함되어 있습니다.

/doc 디렉토리에는 LibDriver PCF8563오프라인 문서가 포함되어 있습니다.

/datasheet 디렉토리에는 PCF8563데이터시트가 있습니다.

/project 디렉토리에는 일반적으로 사용되는 Linux 및 마이크로컨트롤러 개발 보드의 프로젝트 샘플이 포함되어 있습니다. 모든 프로젝트는 디버깅 방법으로 셸 스크립트를 사용하며, 자세한 내용은 각 프로젝트의 README.md를 참조하십시오.

/misra 에는 LibDriver misra 코드 검색 결과가 포함됩니다.

### 설치

/interface 디렉토리에서 플랫폼 독립적인 IIC버스 템플릿을 참조하여 지정된 플랫폼에 대한 IIC버스 드라이버를 완성하십시오.

/src 디렉터리, 플랫폼용 인터페이스 드라이버 및 자체 드라이버를 프로젝트에 추가합니다. 기본 예제 드라이버를 사용하려면 /example 디렉터리를 프로젝트에 추가합니다.

### 사용

/example 디렉터리의 예제를 참조하여 자신만의 드라이버를 완성할 수 있습니다. 기본 프로그래밍 예제를 사용하려는 경우 사용 방법은 다음과 같습니다.

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

### 문서

온라인 문서: [https://www.libdriver.com/docs/pcf8563/index.html](https://www.libdriver.com/docs/pcf8563/index.html).

오프라인 문서: /doc/html/index.html.

### 기고

CONTRIBUTING.md 를 참조하십시오.

### 저작권

저작권 (c) 2015 - 지금 LibDriver 판권 소유

MIT 라이선스(MIT)

이 소프트웨어 및 관련 문서 파일("소프트웨어")의 사본을 얻은 모든 사람은 이에 따라 무제한 사용, 복제, 수정, 통합, 출판, 배포, 2차 라이선스를 포함하여 소프트웨어를 처분할 수 있는 권리가 부여됩니다. 소프트웨어의 사본에 대한 라이선스 및/또는 판매, 그리고 소프트웨어가 위와 같이 배포된 사람의 권리에 대한 2차 라이선스는 다음 조건에 따릅니다.

위의 저작권 표시 및 이 허가 표시는 이 소프트웨어의 모든 사본 또는 내용에 포함됩니다.

이 소프트웨어는 상품성, 특정 목적에의 적합성 및 비침해에 대한 보증을 포함하되 이에 국한되지 않는 어떠한 종류의 명시적 또는 묵시적 보증 없이 "있는 그대로" 제공됩니다. 어떤 경우에도 저자 또는 저작권 소유자는 계약, 불법 행위 또는 기타 방식에 관계없이 소프트웨어 및 기타 소프트웨어 사용으로 인해 발생하거나 이와 관련하여 발생하는 청구, 손해 또는 기타 책임에 대해 책임을 지지 않습니다.

### 문의하기

연락주세요lishifenging@outlook.com.