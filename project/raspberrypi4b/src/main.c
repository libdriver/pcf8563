/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      main.c
 * @brief     main source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2024-11-30
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2024/11/30  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_pcf8563_register_test.h"
#include "driver_pcf8563_alarm_test.h"
#include "driver_pcf8563_timer_test.h"
#include "driver_pcf8563_output_test.h"
#include "driver_pcf8563_basic.h"
#include "driver_pcf8563_alarm.h"
#include "driver_pcf8563_timer.h"
#include "driver_pcf8563_output.h"
#include "gpio.h"
#include <getopt.h>
#include <stdlib.h>
#include <math.h>

volatile uint8_t g_alarm_flag;             /**< alarm flag */
volatile uint8_t g_timer_flag;             /**< timer flag */
uint8_t (*g_gpio_irq)(void) = NULL;        /**< gpio irq function address */

/**
 * @brief     interface receive callback
 * @param[in] type is the event type
 * @note      none
 */
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

/**
 * @brief     interface receive callback
 * @param[in] type is the event type
 * @note      none
 */
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

/**
 * @brief     pcf8563 full function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 5 param is invalid
 * @note      none
 */
uint8_t pcf8563(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    const char short_options[] = "hipe:t:";
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"freq", required_argument, NULL, 1},
        {"minute", required_argument, NULL, 2},
        {"second", required_argument, NULL, 3},
        {"times", required_argument, NULL, 4},
        {"timestamp", required_argument, NULL, 5},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";
    uint32_t times = 3;
    uint32_t t = 0;
    uint8_t time_flag = 0;
    uint8_t minute = 0;
    uint8_t second = 10;
    pcf8563_clock_out_t clk = PCF8563_CLOCK_OUT_1HZ;

    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }

    /* init 0 */
    optind = 0;

    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);

        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "h");

                break;
            }

            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "i");

                break;
            }

            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "p");

                break;
            }

            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "e_%s", optarg);

                break;
            }

            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "t_%s", optarg);

                break;
            }

            /* freq */
            case 1 :
            {
                /* set the freq */
                if (strcmp("1HZ", optarg) == 0)
                {
                    clk = PCF8563_CLOCK_OUT_1HZ;
                }
                else if (strcmp("32HZ", optarg) == 0)
                {
                    clk = PCF8563_CLOCK_OUT_32HZ;
                }
                else if (strcmp("1024HZ", optarg) == 0)
                {
                    clk = PCF8563_CLOCK_OUT_1P024KHZ;
                }
                else if (strcmp("32768HZ", optarg) == 0)
                {
                    clk = PCF8563_CLOCK_OUT_32P768KHZ;
                }
                else
                {
                    return 5;
                }

                break;
            }

            /* minute */
            case 2 :
            {
                /* set the minute */
                minute = (uint8_t)atoi(optarg);

                break;
            }

            /* second */
            case 3 :
            {
                /* set the second */
                second = (uint8_t)atoi(optarg);

                break;
            }

            /* running times */
            case 4 :
            {
                /* set the times */
                times = atol(optarg);

                break;
            }

            /* timestamp */
            case 5 :
            {
                /* set the timestamp */
                t = atoi(optarg);
                time_flag = 1;

                break;
            }

            /* the end */
            case -1 :
            {
                break;
            }

            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);

    /* run the function */
    if (strcmp("t_reg", type) == 0)
    {
        /* run reg test */
        if (pcf8563_register_test() != 0)
        {
            return 1;
        }

        return 0;
    }
    else if (strcmp("t_alarm", type) == 0)
    {
        /* gpio interrupt init */
        gpio_interrupt_init();
        g_gpio_irq = pcf8563_alarm_test_irq_handler;

        /* run alarm test */
        if (pcf8563_alarm_test() != 0)
        {
            g_gpio_irq = 0;
            gpio_interrupt_deinit();

            return 1;
        }

        g_gpio_irq = 0;
        gpio_interrupt_deinit();

        return 0;
    }
    else if (strcmp("t_timer", type) == 0)
    {
        /* gpio interrupt init */
        gpio_interrupt_init();
        g_gpio_irq = pcf8563_timer_test_irq_handler;

        /* run timer test */
        if (pcf8563_timer_test() != 0)
        {
            g_gpio_irq = 0;
            gpio_interrupt_deinit();

            return 1;
        }

        g_gpio_irq = 0;
        gpio_interrupt_deinit();

        return 0;
    }
    else if (strcmp("t_output", type) == 0)
    {
        /* run output test */
        if (pcf8563_output_test(times) != 0)
        {
            return 1;
        }

        return 0;
    }
    else if (strcmp("e_basic-set-time", type) == 0)
    {
        uint8_t res;

        /* check the flag */
        if (time_flag != 1)
        {
            return 5;
        }

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

        /* set time */
        res = pcf8563_basic_set_timestamp((time_t)t);
        if (res != 0)
        {
            (void)pcf8563_basic_deinit();

            return 1;
        }

        /* output */
        pcf8563_interface_debug_print("pcf8563: set timestamp %d.\n", (time_t)t);

        /* basic deinit */
        (void)pcf8563_basic_deinit();

        return 0;
    }
    else if (strcmp("e_basic-get-time", type) == 0)
    {
        uint8_t res;
        pcf8563_time_t time;

        /* basic init */
        res = pcf8563_basic_init();
        if (res != 0)
        {
            return 1;
        }

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

        /* basic deinit */
        (void)pcf8563_basic_deinit();

        return 0;
    }
    else if (strcmp("e_alarm-set-time", type) == 0)
    {
        uint8_t res;

        /* check the flag */
        if (time_flag != 1)
        {
            return 5;
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

        /* set time */
        res = pcf8563_alarm_set_timestamp((time_t)t);
        if (res != 0)
        {
            (void)pcf8563_alarm_deinit();

            return 1;
        }

        /* output */
        pcf8563_interface_debug_print("pcf8563: set timestamp %d.\n", (time_t)t);

        /* alarm deinit */
        (void)pcf8563_alarm_deinit();

        return 0;
    }
    else if (strcmp("e_alarm-get-time", type) == 0)
    {
        uint8_t res;
        pcf8563_time_t time;

        /* alarm init */
        res = pcf8563_alarm_init(a_alarm_callback);
        if (res != 0)
        {
            return 1;
        }

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

        /* alarm deinit */
        (void)pcf8563_alarm_deinit();

        return 0;
    }
    else if (strcmp("e_alarm-run", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        pcf8563_alarm_param_t param;

        /* gpio interrupt init */
        g_gpio_irq = pcf8563_alarm_irq_handler;
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }

        /* alarm init */
        res = pcf8563_alarm_init(a_alarm_callback);
        if (res != 0)
        {
            g_gpio_irq = 0;
            gpio_interrupt_deinit();

            return 1;
        }

        g_alarm_flag = 0;

        /* enable alarm */
        param.minute = minute;
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

        /* disable alarm */
        (void)pcf8563_alarm_disable();

        /* alarm deinit */
        (void)pcf8563_alarm_deinit();

        return 0;
    }
    else if (strcmp("e_timer-set-time", type) == 0)
    {
        uint8_t res;

        /* check the flag */
        if (time_flag != 1)
        {
            return 5;
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

        /* set time */
        res = pcf8563_timer_set_timestamp((time_t)t);
        if (res != 0)
        {
            (void)pcf8563_timer_deinit();

            return 1;
        }

        /* output */
        pcf8563_interface_debug_print("pcf8563: set timestamp %d.\n", (time_t)t);

        /* timer deinit */
        (void)pcf8563_timer_deinit();

        return 0;
    }
    else if (strcmp("e_timer-get-time", type) == 0)
    {
        uint8_t res;
        pcf8563_time_t time;

        /* timer init */
        res = pcf8563_timer_init(a_alarm_callback);
        if (res != 0)
        {
            return 1;
        }

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

        /* timer deinit */
        (void)pcf8563_timer_deinit();

        return 0;
    }
    else if (strcmp("e_timer-run", type) == 0)
    {
        uint8_t res;
        uint32_t i;

        /* gpio interrupt init */
        g_gpio_irq = pcf8563_timer_irq_handler;
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }

        /* timer init */
        res = pcf8563_timer_init(a_timer_callback);
        if (res != 0)
        {
            g_gpio_irq = 0;
            gpio_interrupt_deinit();

            return 1;
        }

        g_timer_flag = 0;

        /* enable timer */
        res = pcf8563_timer_enable(PCF8563_TIMER_FREQ_1HZ, second, PCF8563_INTERRUPT_MODE_PULSE);
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

        /* disable timer */
        (void)pcf8563_timer_disable();

        /* timer deinit */
        (void)pcf8563_timer_deinit();

        return 0;
    }
    else if (strcmp("e_output-set-time", type) == 0)
    {
        uint8_t res;

        /* check the flag */
        if (time_flag != 1)
        {
            return 5;
        }

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

        /* set time */
        res = pcf8563_output_set_timestamp((time_t)t);
        if (res != 0)
        {
            (void)pcf8563_output_deinit();

            return 1;
        }

        /* output */
        pcf8563_interface_debug_print("pcf8563: set timestamp %d.\n", (time_t)t);

        /* output deinit */
        (void)pcf8563_output_deinit();

        return 0;
    }
    else if (strcmp("e_output-get-time", type) == 0)
    {
        uint8_t res;
        pcf8563_time_t time;

        /* output init */
        res = pcf8563_output_init();
        if (res != 0)
        {
            return 1;
        }

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

        /* output deinit */
        (void)pcf8563_output_deinit();

        return 0;
    }
    else if (strcmp("e_output-enable", type) == 0)
    {
        uint8_t res;

        /* output init */
        res = pcf8563_output_init();
        if (res != 0)
        {
            return 1;
        }

        /* output enable */
        res = pcf8563_output_enable(clk);
        if (res != 0)
        {
            (void)pcf8563_output_deinit();

            return 1;
        }

        /* output */
        if (clk == PCF8563_CLOCK_OUT_32P768KHZ)
        {
            pcf8563_interface_debug_print("pcf8563: enable clock 32.768khz.\n");
        }
        else if (clk == PCF8563_CLOCK_OUT_1P024KHZ)
        {
            pcf8563_interface_debug_print("pcf8563: enable clock 1.024khz.\n");
        }
        else if (clk == PCF8563_CLOCK_OUT_32HZ)
        {
            pcf8563_interface_debug_print("pcf8563: enable clock 32hz.\n");
        }
        else
        {
            pcf8563_interface_debug_print("pcf8563: enable clock 1hz.\n");
        }

        /* output deinit */
        (void)pcf8563_output_deinit();

        return 0;
    }
    else if (strcmp("e_output-disable", type) == 0)
    {
        uint8_t res;

        /* output init */
        res = pcf8563_output_init();
        if (res != 0)
        {
            return 1;
        }

        /* output disable */
        res = pcf8563_output_disable();
        if (res != 0)
        {
            (void)pcf8563_output_deinit();

            return 1;
        }

        pcf8563_interface_debug_print("pcf8563: disable clock output.\n");

        /* output deinit */
        (void)pcf8563_output_deinit();

        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        pcf8563_interface_debug_print("Usage:\n");
        pcf8563_interface_debug_print("  pcf8563 (-i | --information)\n");
        pcf8563_interface_debug_print("  pcf8563 (-h | --help)\n");
        pcf8563_interface_debug_print("  pcf8563 (-p | --port)\n");
        pcf8563_interface_debug_print("  pcf8563 (-t reg | --test=reg)\n");
        pcf8563_interface_debug_print("  pcf8563 (-t alarm | --test=alarm)\n");
        pcf8563_interface_debug_print("  pcf8563 (-t timer | --test=timer)\n");
        pcf8563_interface_debug_print("  pcf8563 (-t output | --test=output) [--times=<num>]\n");
        pcf8563_interface_debug_print("  pcf8563 (-e basic-set-time | --example=basic-set-time) --timestamp=<time>\n");
        pcf8563_interface_debug_print("  pcf8563 (-e basic-get-time | --example=basic-get-time)\n");
        pcf8563_interface_debug_print("  pcf8563 (-e alarm-set-time | --example=alarm-set-time) --timestamp=<time>\n");
        pcf8563_interface_debug_print("  pcf8563 (-e alarm-get-time | --example=alarm-get-time)\n");
        pcf8563_interface_debug_print("  pcf8563 (-e alarm-run | --example=alarm-run) --minute=<minute>\n");
        pcf8563_interface_debug_print("  pcf8563 (-e timer-set-time | --example=timer-set-time) --timestamp=<time>\n");
        pcf8563_interface_debug_print("  pcf8563 (-e timer-get-time | --example=timer-get-time)\n");
        pcf8563_interface_debug_print("  pcf8563 (-e timer-run | --example=timer-run) --second=<second>\n");
        pcf8563_interface_debug_print("  pcf8563 (-e output-set-time | --example=output-set-time) --timestamp=<time>\n");
        pcf8563_interface_debug_print("  pcf8563 (-e output-get-time | --example=output-get-time)\n");
        pcf8563_interface_debug_print("  pcf8563 (-e output-enable | --example=output-enable) --freq=<1HZ | 32HZ | 1024HZ | 32768HZ>\n");
        pcf8563_interface_debug_print("  pcf8563 (-e output-disable | --example=output-disable)\n");
        pcf8563_interface_debug_print("\n");
        pcf8563_interface_debug_print(" Options:\n");
        pcf8563_interface_debug_print("   -e <basic-set-time | basic-get-time | alarm-set-time| alarm-get-time | alarm-run |\n");
        pcf8563_interface_debug_print("       timer-set-time | timer-get-time | timer-run | output-set-time | output-get-time |\n");
        pcf8563_interface_debug_print("       output-enable | output-disable>\n");
        pcf8563_interface_debug_print("                                   Run the driver example.\n");
        pcf8563_interface_debug_print("       --freq=<1HZ | 32HZ | 1024HZ | 32768HZ>\n");
        pcf8563_interface_debug_print("                                   Set output frequency.([default: 1HZ])\n");
        pcf8563_interface_debug_print("   -h, --help                      Show the help.\n");
        pcf8563_interface_debug_print("   -i, --information               Show the chip information.\n");
        pcf8563_interface_debug_print("       --minute=<minute>           Set the alarm minute.([default: 0])\n");
        pcf8563_interface_debug_print("   -p, --port                      Display the pin connections of the current board.\n");
        pcf8563_interface_debug_print("       --second=<second>           Set the alarm second.([default: 10])\n");
        pcf8563_interface_debug_print("   -t <reg | alarm | timer | output>, --test=<reg | alarm | timer | output>\n");
        pcf8563_interface_debug_print("                                   Run the driver test.\n");
        pcf8563_interface_debug_print("       --times=<num>               Set the running times.([default: 3])\n");
        pcf8563_interface_debug_print("       --timestamp=<time>          Set the the unix timestamp.\n");

        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        pcf8563_info_t info;

        /* print pcf8563 info */
        pcf8563_info(&info);
        pcf8563_interface_debug_print("pcf8563: chip is %s.\n", info.chip_name);
        pcf8563_interface_debug_print("pcf8563: manufacturer is %s.\n", info.manufacturer_name);
        pcf8563_interface_debug_print("pcf8563: interface is %s.\n", info.interface);
        pcf8563_interface_debug_print("pcf8563: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        pcf8563_interface_debug_print("pcf8563: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        pcf8563_interface_debug_print("pcf8563: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        pcf8563_interface_debug_print("pcf8563: max current is %0.2fmA.\n", info.max_current_ma);
        pcf8563_interface_debug_print("pcf8563: max temperature is %0.1fC.\n", info.temperature_max);
        pcf8563_interface_debug_print("pcf8563: min temperature is %0.1fC.\n", info.temperature_min);

        return 0;
    }
    else if (strcmp("p", type) == 0)
    {
        /* print pin connection */
        pcf8563_interface_debug_print("pcf8563: SCL connected to GPIO3(BCM).\n");
        pcf8563_interface_debug_print("pcf8563: SDA connected to GPIO2(BCM).\n");

        return 0;
    }
    else
    {
        return 5;
    }
}

/**
 * @brief     main function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *             - 0 success
 * @note      none
 */
int main(uint8_t argc, char **argv)
{
    uint8_t res;

    res = pcf8563(argc, argv);
    if (res == 0)
    {
        /* run success */
    }
    else if (res == 1)
    {
        pcf8563_interface_debug_print("pcf8563: run failed.\n");
    }
    else if (res == 5)
    {
        pcf8563_interface_debug_print("pcf8563: param is invalid.\n");
    }
    else
    {
        pcf8563_interface_debug_print("pcf8563: unknown status code.\n");
    }

    return 0;
}
