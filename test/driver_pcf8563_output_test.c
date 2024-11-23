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
 * @file      driver_pcf8563_output_test.c
 * @brief     driver pcf8563 output test source file
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
 
#include "driver_pcf8563_output_test.h"
#include <stdlib.h>

static pcf8563_handle_t gs_handle;        /**< pcf8563 handle */

/**
 * @brief     output test
 * @param[in] times is the test times
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t pcf8563_output_test(uint32_t times)
{
    uint8_t res;
    uint32_t i;
    pcf8563_info_t info;
    
    /* link functions */
    DRIVER_PCF8563_LINK_INIT(&gs_handle, pcf8563_handle_t);
    DRIVER_PCF8563_LINK_IIC_INIT(&gs_handle, pcf8563_interface_iic_init);
    DRIVER_PCF8563_LINK_IIC_DEINIT(&gs_handle, pcf8563_interface_iic_deinit);
    DRIVER_PCF8563_LINK_IIC_READ(&gs_handle, pcf8563_interface_iic_read);
    DRIVER_PCF8563_LINK_IIC_WRITE(&gs_handle, pcf8563_interface_iic_write);
    DRIVER_PCF8563_LINK_DELAY_MS(&gs_handle, pcf8563_interface_delay_ms);
    DRIVER_PCF8563_LINK_DEBUG_PRINT(&gs_handle, pcf8563_interface_debug_print);
    DRIVER_PCF8563_LINK_RECEIVE_CALLBACK(&gs_handle, pcf8563_interface_receive_callback);
    
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
    
    /* start output test */
    pcf8563_interface_debug_print("pcf8563: start output test.\n");
    
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
    
    /* set interrupt mode pulse */
    res = pcf8563_set_interrupt_mode(&gs_handle, PCF8563_INTERRUPT_MODE_PULSE);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set interrupt mode failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable clock out */
    res = pcf8563_set_clock_out_enable(&gs_handle, PCF8563_BOOL_TRUE);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set clock out enable failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set clock out 32.768khz */
    res = pcf8563_set_clock_out(&gs_handle, PCF8563_CLOCK_OUT_32P768KHZ);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set clock out failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    pcf8563_interface_debug_print("pcf8563: set clock out 32.768khz.\n");
    
    /* loop */
    for (i = 0; i < times; i++)
    {
        /* delay 1000ms */
        pcf8563_interface_delay_ms(1000);
        
        /* output */
        pcf8563_interface_debug_print("pcf8563: run %ds.\n", i + 1);
    }
    
    /* set clock out 1.024khz */
    res = pcf8563_set_clock_out(&gs_handle, PCF8563_CLOCK_OUT_1P024KHZ);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set clock out failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    pcf8563_interface_debug_print("pcf8563: set clock out 1.024khz.\n");
    
    /* loop */
    for (i = 0; i < times; i++)
    {
        /* delay 1000ms */
        pcf8563_interface_delay_ms(1000);
        
        /* output */
        pcf8563_interface_debug_print("pcf8563: run %ds.\n", i + 1);
    }
    
    /* set clock out 32hz */
    res = pcf8563_set_clock_out(&gs_handle, PCF8563_CLOCK_OUT_32HZ);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set clock out failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    pcf8563_interface_debug_print("pcf8563: set clock out 32hz.\n");
    
    /* loop */
    for (i = 0; i < times; i++)
    {
        /* delay 1000ms */
        pcf8563_interface_delay_ms(1000);
        
        /* output */
        pcf8563_interface_debug_print("pcf8563: run %ds.\n", i + 1);
    }
    
    /* set clock out 1hz */
    res = pcf8563_set_clock_out(&gs_handle, PCF8563_CLOCK_OUT_1HZ);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set clock out failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    pcf8563_interface_debug_print("pcf8563: set clock out 1hz.\n");
    
    /* loop */
    for (i = 0; i < times; i++)
    {
        /* delay 1000ms */
        pcf8563_interface_delay_ms(1000);
        
        /* output */
        pcf8563_interface_debug_print("pcf8563: run %ds.\n", i + 1);
    }
    
    /* disable clock out */
    res = pcf8563_set_clock_out_enable(&gs_handle, PCF8563_BOOL_FALSE);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set clock out enable failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    
    /* finish output test */
    pcf8563_interface_debug_print("pcf8563: finish output test.\n");
    (void)pcf8563_deinit(&gs_handle);
    
    return 0;
}
