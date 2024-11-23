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
 * @file      driver_pcf8563_alarm_test.c
 * @brief     driver pcf8563 alarm test source file
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
 
#include "driver_pcf8563_alarm_test.h"
#include <stdlib.h>

static pcf8563_handle_t gs_handle;        /**< pcf8563 handle */
static volatile uint8_t gs_flag;          /**< flag */

/**
 * @brief     interface receive callback
 * @param[in] type is the event type
 * @note      none
 */
static void a_receive_callback(uint8_t type)
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
            gs_flag = 1;
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
 * @brief  alarm test irq
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t pcf8563_alarm_test_irq_handler(void)
{
    if (pcf8563_irq_handler(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  alarm test
 * @return status code
 *         - 0 success
 *         - 1 test failed
 * @note   none
 */
uint8_t pcf8563_alarm_test(void)
{
    uint8_t res;
    uint32_t i;
    pcf8563_info_t info;
    pcf8563_time_t time_in;
    pcf8563_time_t time_out;
    
    /* link functions */
    DRIVER_PCF8563_LINK_INIT(&gs_handle, pcf8563_handle_t);
    DRIVER_PCF8563_LINK_IIC_INIT(&gs_handle, pcf8563_interface_iic_init);
    DRIVER_PCF8563_LINK_IIC_DEINIT(&gs_handle, pcf8563_interface_iic_deinit);
    DRIVER_PCF8563_LINK_IIC_READ(&gs_handle, pcf8563_interface_iic_read);
    DRIVER_PCF8563_LINK_IIC_WRITE(&gs_handle, pcf8563_interface_iic_write);
    DRIVER_PCF8563_LINK_DELAY_MS(&gs_handle, pcf8563_interface_delay_ms);
    DRIVER_PCF8563_LINK_DEBUG_PRINT(&gs_handle, pcf8563_interface_debug_print);
    DRIVER_PCF8563_LINK_RECEIVE_CALLBACK(&gs_handle, a_receive_callback);
    
    /* get pcf8563 info */
    res = pcf8563_info(&info);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print pcf8563 info */
        pcf8563_interface_debug_print("pcf8563: chip is %s.\n", info.chip_name);
        pcf8563_interface_debug_print("pcf8563: manufacturer is %s.\n", info.manufacturer_name);
        pcf8563_interface_debug_print("pcf8563: interface is %s.\n", info.interface);
        pcf8563_interface_debug_print("pcf8563: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        pcf8563_interface_debug_print("pcf8563: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        pcf8563_interface_debug_print("pcf8563: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        pcf8563_interface_debug_print("pcf8563: max current is %0.2fmA.\n", info.max_current_ma);
        pcf8563_interface_debug_print("pcf8563: max temperature is %0.1fC.\n", info.temperature_max);
        pcf8563_interface_debug_print("pcf8563: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start alarm test */
    pcf8563_interface_debug_print("pcf8563: start alarm test.\n");
    
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
    
    /* disable clock out */
    res = pcf8563_set_clock_out_enable(&gs_handle, PCF8563_BOOL_FALSE);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set clock out enable failed.\n");
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
    
    /* set interrupt mode pulse */
    res = pcf8563_set_interrupt_mode(&gs_handle, PCF8563_INTERRUPT_MODE_PULSE);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set interrupt pin mode failed.\n");
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
    
    /* set time */
    time_in.year = rand() % 100 + 2000;
    time_in.month = rand() % 12 + 1;
    time_in.date = rand() % 20 + 1;
    time_in.week = rand() % 7;
    time_in.hour = rand() % 11 + 1;
    time_in.minute = 0;
    time_in.second = 0;
    res = pcf8563_set_time(&gs_handle, &time_in);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set time failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set minute */
    res = pcf8563_set_minute_alarm(&gs_handle, 1);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set minute alarm failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable minute alarm */
    res = pcf8563_set_minute_alarm_enable(&gs_handle, PCF8563_BOOL_TRUE);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set minute alarm enable failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    
    /* clear status */
    res = pcf8563_clear_status(&gs_handle, PCF8563_INTERRUPT_EVENT_ALARM);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: clear status failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable alarm interrupt */
    res = pcf8563_set_alarm_interrupt(&gs_handle, PCF8563_BOOL_TRUE);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set alarm interrupt failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    
    /* wait for trigger */
    gs_flag = 0;
    for (i = 0; i < 180; i++)
    {
        if (gs_flag != 0)
        {
            break;
        }
        pcf8563_interface_delay_ms(1000);
        res = pcf8563_get_time(&gs_handle, &time_out);
        if (res != 0)
        {
            pcf8563_interface_debug_print("pcf8563: get time failed.\n");
            (void)pcf8563_deinit(&gs_handle);
            
            return 1;
        }
        pcf8563_interface_debug_print("pcf8563: time is %02d:%02d:%02d.\n",
                                       time_out.hour, time_out.minute, time_out.second
                                     );
    }
    
    /* check flag */
    if (gs_flag == 0)
    {
        pcf8563_interface_debug_print("pcf8563: not found alarm.\n");
    }
    
    /* disable minute alarm */
    res = pcf8563_set_minute_alarm_enable(&gs_handle, PCF8563_BOOL_FALSE);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set minute alarm enable failed.\n");
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
    
    /* finish alarm test */
    pcf8563_interface_debug_print("pcf8563: finish alarm test.\n");
    (void)pcf8563_deinit(&gs_handle);
    
    return 0;
}
