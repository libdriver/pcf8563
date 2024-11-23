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
 * @file      driver_pcf8563_output.c
 * @brief     driver pcf8563 output source file
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

#include "driver_pcf8563_output.h"

static pcf8563_handle_t gs_handle;       /**< pcf8563 handle */
static int8_t gs_time_zone = 0;          /**< local zone */

/**
 * @brief  output example init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 * @note   none
 */
uint8_t pcf8563_output_init(void)
{
    uint8_t res;

    /* link functions */
    DRIVER_PCF8563_LINK_INIT(&gs_handle, pcf8563_handle_t);
    DRIVER_PCF8563_LINK_IIC_INIT(&gs_handle, pcf8563_interface_iic_init);
    DRIVER_PCF8563_LINK_IIC_DEINIT(&gs_handle, pcf8563_interface_iic_deinit);
    DRIVER_PCF8563_LINK_IIC_READ(&gs_handle, pcf8563_interface_iic_read);
    DRIVER_PCF8563_LINK_IIC_WRITE(&gs_handle, pcf8563_interface_iic_write);
    DRIVER_PCF8563_LINK_DELAY_MS(&gs_handle, pcf8563_interface_delay_ms);
    DRIVER_PCF8563_LINK_DEBUG_PRINT(&gs_handle, pcf8563_interface_debug_print);
    DRIVER_PCF8563_LINK_RECEIVE_CALLBACK(&gs_handle, pcf8563_interface_receive_callback);

    /* init pcf8563 */
    res = pcf8563_init(&gs_handle);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: init failed.\n");

        return 1;
    }

    /* disable rtc stop */
    res = pcf8563_set_rtc_stop(&gs_handle, PCF8563_BOOL_FALSE);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set rtc stop failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable test mode */
    res = pcf8563_set_test_mode(&gs_handle, PCF8563_BOOL_FALSE);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set test mode failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable power on reset */
    res = pcf8563_set_power_on_reset(&gs_handle, PCF8563_BOOL_FALSE);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set power on reset failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable timer */
    res = pcf8563_set_timer_enable(&gs_handle, PCF8563_BOOL_FALSE);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set timer enable failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable minute alarm */
    res = pcf8563_set_minute_alarm_enable(&gs_handle, PCF8563_BOOL_FALSE);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set minute alarm enable failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable hour alarm */
    res = pcf8563_set_hour_alarm_enable(&gs_handle, PCF8563_BOOL_FALSE);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set hour alarm enable failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable day alarm */
    res = pcf8563_set_day_alarm_enable(&gs_handle, PCF8563_BOOL_FALSE);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set day alarm enable failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable week alarm */
    res = pcf8563_set_week_alarm_enable(&gs_handle, PCF8563_BOOL_FALSE);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set week alarm enable failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable timer interrupt */
    res = pcf8563_set_timer_interrupt(&gs_handle, PCF8563_BOOL_FALSE);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set timer interrupt failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable alarm interrupt */
    res = pcf8563_set_alarm_interrupt(&gs_handle, PCF8563_BOOL_FALSE);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set alarm interrupt failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable clock out */
    res = pcf8563_set_clock_out_enable(&gs_handle, PCF8563_BOOL_FALSE);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set clock out enable failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    
    return 0;
}

/**
 * @brief  output example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t pcf8563_output_deinit(void)
{
    if (pcf8563_deinit(&gs_handle) != 0)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief     output example set the time
 * @param[in] *t points to a time structure
 * @return    status code
 *            - 0 success
 *            - 1 set time failed
 * @note      none
 */
uint8_t pcf8563_output_set_time(pcf8563_time_t *t)
{
    /* set time */
    if (pcf8563_set_time(&gs_handle, t) != 0)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief     output example set the time by a unix timestamp
 * @param[in] timestamp is a unix timestamp
 * @return    status code
 *            - 0 success
 *            - 1 set timestamp failed
 * @note      none
 */
uint8_t pcf8563_output_set_timestamp(time_t timestamp)
{
    pcf8563_time_t t;
    struct tm *timeptr;

    /* convert times */
    timestamp += (time_t)(gs_time_zone * 3600);
    timeptr = localtime(&timestamp);
    t.date = (uint8_t)timeptr->tm_mday;
    t.hour = (uint8_t)timeptr->tm_hour;
    t.minute = (uint8_t)timeptr->tm_min;
    t.month = (uint8_t)timeptr->tm_mon + 1;
    t.second = (uint8_t)timeptr->tm_sec;
    t.week = (uint8_t)timeptr->tm_wday;
    t.year = (uint16_t)(timeptr->tm_year + 1900);

    /* set time */
    if (pcf8563_set_time(&gs_handle, &t) != 0)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief     output example set the local time zone
 * @param[in] zone is the local time zone
 * @return    status code
 *            - 0 success
 * @note      none
 */
uint8_t pcf8563_output_set_timestamp_time_zone(int8_t zone)
{
    gs_time_zone = zone;

    return 0;
}

/**
 * @brief      output example get the time
 * @param[out] *t points to a time structure
 * @return     status code
 *             - 0 success
 *             - 1 get time failed
 * @note       none
 */
uint8_t pcf8563_output_get_time(pcf8563_time_t *t)
{
    /* get time */
    if (pcf8563_get_time(&gs_handle, t) != 0)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief      output example get the time in a unix timestamp
 * @param[out] *timestamp points to a unix timestamp buffer
 * @return     status code
 *             - 0 success
 *             - 1 get timestamp failed
 * @note       none
 */
uint8_t pcf8563_output_get_timestamp(time_t *timestamp)
{
    pcf8563_time_t t;
    struct tm timeptr;

    /* get time */
    if (pcf8563_get_time(&gs_handle, &t) != 0)
    {
        return 1;
    }
    timeptr.tm_year = t.year - 1900;
    timeptr.tm_mon = t.month - 1;
    timeptr.tm_wday = t.week;
    timeptr.tm_mday = t.date;
    timeptr.tm_hour = t.hour;
    timeptr.tm_min = t.minute;
    timeptr.tm_sec = t.second;

    /* make time */
    *timestamp = mktime(&timeptr) - gs_time_zone * 3600;

    return 0;
}

/**
 * @brief      output example get the local time zone
 * @param[out] *zone points to a local time zone buffer
 * @return     status code
 *             - 0 success
 * @note       none
 */
uint8_t pcf8563_output_get_timestamp_time_zone(int8_t *zone)
{
    *zone = gs_time_zone;

    return 0;
}

/**
 * @brief      output example get the ascii time
 * @param[out] *buf points to an ascii buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t pcf8563_output_get_ascii_time(char *buf, uint8_t len)
{
    pcf8563_time_t t;

    /* get time */
    if (pcf8563_get_time(&gs_handle, &t) != 0)
    {
        return 1;
    }
    
    (void)snprintf(buf, len, "%04d-%02d-%02d %02d:%02d:%02d %d.\n", t.year, t.month, t.date, t.hour, t.minute, t.second, t.week);

    return 0;
}

/**
 * @brief     output example enable the output
 * @param[in] clk is the set clock
 * @return    status code
 *            - 0 success
 *            - 1 output enable failed
 * @note      none
 */
uint8_t pcf8563_output_enable(pcf8563_clock_out_t clk)
{
    uint8_t res;
    
    /* set the clock out */
    res = pcf8563_set_clock_out(&gs_handle, clk);
    if (res != 0)
    {
        return 1;
    }
    
    /* enable clock out */
    res = pcf8563_set_clock_out_enable(&gs_handle, PCF8563_BOOL_TRUE);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  output example disable the output
 * @return status code
 *         - 0 success
 *         - 1 output disable failed
 * @note   none
 */
uint8_t pcf8563_output_disable(void)
{
    uint8_t res;
    
    /* disable clock out */
    res = pcf8563_set_clock_out_enable(&gs_handle, PCF8563_BOOL_FALSE);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}
