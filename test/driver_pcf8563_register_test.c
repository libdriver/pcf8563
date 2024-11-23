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
 * @file      driver_pcf8563_register_test.c
 * @brief     driver pcf8563 register test source file
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
#include <stdlib.h>

static pcf8563_handle_t gs_handle;        /**< pcf8563 handle */

/**
 * @brief  register test
 * @return status code
 *         - 0 success
 *         - 1 test failed
 * @note   none
 */
uint8_t pcf8563_register_test(void)
{
    uint8_t res;
    uint8_t reg;
    uint8_t reg_check;
    pcf8563_info_t info;
    pcf8563_bool_t enable;
    pcf8563_time_t time_in;
    pcf8563_time_t time_out;
    pcf8563_clock_out_t clk;
    pcf8563_timer_freq_t freq;
    pcf8563_interrupt_mode_t mode;
    
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
    
    /* start register test */
    pcf8563_interface_debug_print("pcf8563: start register test.\n");
    
    /* init pcf8563 */
    res = pcf8563_init(&gs_handle);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: init failed.\n");
        
        return 1;
    }
    
    /* pcf8563_set_rtc_stop/pcf8563_get_rtc_stop test */
    pcf8563_interface_debug_print("pcf8563: pcf8563_set_rtc_stop/pcf8563_get_rtc_stop test.\n");
    
    /* enable rtc stop */
    res = pcf8563_set_rtc_stop(&gs_handle, PCF8563_BOOL_TRUE);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set rtc stop failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: enable rtc stop.\n");
    res = pcf8563_get_rtc_stop(&gs_handle, &enable);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: get rtc stop failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: check rtc stop %s.\n", (enable == PCF8563_BOOL_TRUE) ? "ok" : "error");
    
    /* disable rtc stop */
    res = pcf8563_set_rtc_stop(&gs_handle, PCF8563_BOOL_FALSE);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set rtc stop failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: disable rtc stop.\n");
    res = pcf8563_get_rtc_stop(&gs_handle, &enable);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: get rtc stop failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: check rtc stop %s.\n", (enable == PCF8563_BOOL_FALSE) ? "ok" : "error");
    
    /* pcf8563_set_test_mode/pcf8563_get_test_mode test */
    pcf8563_interface_debug_print("pcf8563: pcf8563_set_test_mode/pcf8563_get_test_mode test.\n");
    
    /* enable test mode */
    res = pcf8563_set_test_mode(&gs_handle, PCF8563_BOOL_TRUE);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set test mode failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: enable test mode.\n");
    res = pcf8563_get_test_mode(&gs_handle, &enable);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: get test mode failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: check test mode %s.\n", (enable == PCF8563_BOOL_TRUE) ? "ok" : "error");
    
    /* disable test mode */
    res = pcf8563_set_test_mode(&gs_handle, PCF8563_BOOL_FALSE);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set test mode failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: disable test mode.\n");
    res = pcf8563_get_test_mode(&gs_handle, &enable);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: get test mode failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: check test mode %s.\n", (enable == PCF8563_BOOL_FALSE) ? "ok" : "error");
    
    /* pcf8563_set_power_on_reset/pcf8563_get_power_on_reset test */
    pcf8563_interface_debug_print("pcf8563: pcf8563_set_power_on_reset/pcf8563_get_power_on_reset test.\n");
    
    /* enable power on reset */
    res = pcf8563_set_power_on_reset(&gs_handle, PCF8563_BOOL_TRUE);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set power on reset failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: enable power on reset.\n");
    res = pcf8563_get_power_on_reset(&gs_handle, &enable);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: get power on reset failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: check power on reset %s.\n", (enable == PCF8563_BOOL_TRUE) ? "ok" : "error");
    
    /* disable power on reset */
    res = pcf8563_set_power_on_reset(&gs_handle, PCF8563_BOOL_FALSE);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set power on reset failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: disable power on reset.\n");
    res = pcf8563_get_power_on_reset(&gs_handle, &enable);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: get power on reset failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: check power on reset %s.\n", (enable == PCF8563_BOOL_FALSE) ? "ok" : "error");
    
    /* pcf8563_set_time/pcf8563_get_time test */
    pcf8563_interface_debug_print("pcf8563: pcf8563_set_time/pcf8563_get_time test.\n");
    
    /* set time */
    time_in.year = rand() % 100 + 2000;
    time_in.month = rand() % 12 + 1;
    time_in.date = rand() % 20 + 1;
    time_in.week = rand() % 7;
    time_in.hour = rand() % 11 + 1;
    time_in.minute = rand() % 60;
    time_in.second = rand() % 60;
    res = pcf8563_set_time(&gs_handle, &time_in);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set time failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: set time %04d-%02d-%02d %02d:%02d:%02d %d.\n",
                                  time_in.year, time_in.month, time_in.date,
                                  time_in.hour, time_in.minute, time_in.second, time_in.week
                                 );
    res = pcf8563_get_time(&gs_handle, &time_out);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: get time failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: check time %s.\n", (memcmp((uint8_t *)&time_in, (uint8_t *)&time_out, sizeof(pcf8563_time_t)) == 0) ? "ok" : "error");
    
    /* pcf8563_set_minute_alarm_enable/pcf8563_get_minute_alarm_enable test */
    pcf8563_interface_debug_print("pcf8563: pcf8563_set_minute_alarm_enable/pcf8563_get_minute_alarm_enable test.\n");
    
    /* enable minute alarm */
    res = pcf8563_set_minute_alarm_enable(&gs_handle, PCF8563_BOOL_TRUE);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set minute alarm enable failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: enable minute alarm.\n");
    res = pcf8563_get_minute_alarm_enable(&gs_handle, &enable);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: get minute alarm enable failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: check minute alarm enable %s.\n", (enable == PCF8563_BOOL_TRUE) ? "ok" : "error");
    
    /* disable minute alarm */
    res = pcf8563_set_minute_alarm_enable(&gs_handle, PCF8563_BOOL_FALSE);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set minute alarm enable failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: disable minute alarm.\n");
    res = pcf8563_get_minute_alarm_enable(&gs_handle, &enable);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: get minute alarm enable failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: check minute alarm enable %s.\n", (enable == PCF8563_BOOL_FALSE) ? "ok" : "error");
    
    /* pcf8563_set_minute_alarm/pcf8563_get_minute_alarm test */
    pcf8563_interface_debug_print("pcf8563: pcf8563_set_minute_alarm/pcf8563_get_minute_alarm test.\n");
    
    reg = rand() % 60 + 1;
    res = pcf8563_set_minute_alarm(&gs_handle, reg);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set minute alarm failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: set minute alarm %d.\n", reg);
    res = pcf8563_get_minute_alarm(&gs_handle, &reg_check);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: get minute alarm failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: check minute alarm %s.\n", (reg == reg_check) ? "ok" : "error");
    
    /* pcf8563_set_hour_alarm_enable/pcf8563_get_hour_alarm_enable test */
    pcf8563_interface_debug_print("pcf8563: pcf8563_set_hour_alarm_enable/pcf8563_get_hour_alarm_enable test.\n");
    
    /* enable hour alarm */
    res = pcf8563_set_hour_alarm_enable(&gs_handle, PCF8563_BOOL_TRUE);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set hour alarm enable failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: enable hour alarm.\n");
    res = pcf8563_get_hour_alarm_enable(&gs_handle, &enable);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: get hour alarm enable failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: check hour alarm enable %s.\n", (enable == PCF8563_BOOL_TRUE) ? "ok" : "error");
    
    /* disable hour alarm */
    res = pcf8563_set_hour_alarm_enable(&gs_handle, PCF8563_BOOL_FALSE);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set hour alarm enable failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: disable hour alarm.\n");
    res = pcf8563_get_hour_alarm_enable(&gs_handle, &enable);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: get hour alarm enable failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: check hour alarm enable %s.\n", (enable == PCF8563_BOOL_FALSE) ? "ok" : "error");
    
    /* pcf8563_set_hour_alarm/pcf8563_get_hour_alarm test */
    pcf8563_interface_debug_print("pcf8563: pcf8563_set_hour_alarm/pcf8563_get_hour_alarm test.\n");
    
    reg = rand() % 23 + 1;
    res = pcf8563_set_hour_alarm(&gs_handle, reg);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set hour alarm failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: set hour alarm %d.\n", reg);
    res = pcf8563_get_hour_alarm(&gs_handle, &reg_check);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: get hour alarm failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: check hour alarm %s.\n", (reg == reg_check) ? "ok" : "error");
    
    /* pcf8563_set_day_alarm_enable/pcf8563_get_day_alarm_enable test */
    pcf8563_interface_debug_print("pcf8563: pcf8563_set_day_alarm_enable/pcf8563_get_day_alarm_enable test.\n");
    
    /* enable day alarm */
    res = pcf8563_set_day_alarm_enable(&gs_handle, PCF8563_BOOL_TRUE);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set day alarm enable failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: enable day alarm.\n");
    res = pcf8563_get_day_alarm_enable(&gs_handle, &enable);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: get day alarm enable failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: check day alarm %s.\n", (enable == PCF8563_BOOL_TRUE) ? "ok" : "error");
    
    /* disable day alarm */
    res = pcf8563_set_day_alarm_enable(&gs_handle, PCF8563_BOOL_FALSE);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set day alarm enable failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: disable day alarm.\n");
    res = pcf8563_get_day_alarm_enable(&gs_handle, &enable);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: get day alarm enable failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: check day alarm %s.\n", (enable == PCF8563_BOOL_FALSE) ? "ok" : "error");
    
    /* pcf8563_set_day_alarm/pcf8563_get_day_alarm test */
    pcf8563_interface_debug_print("pcf8563: pcf8563_set_day_alarm/pcf8563_get_day_alarm test.\n");
    
    reg = rand() % 30 + 1;
    res = pcf8563_set_day_alarm(&gs_handle, reg);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set day alarm failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: set day alarm %d.\n", reg);
    res = pcf8563_get_day_alarm(&gs_handle, &reg_check);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: get day alarm failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: check day alarm %s.\n", (reg == reg_check) ? "ok" : "error");
    
    /* pcf8563_set_week_alarm_enable/pcf8563_get_week_alarm_enable test */
    pcf8563_interface_debug_print("pcf8563: pcf8563_set_week_alarm_enable/pcf8563_get_week_alarm_enable test.\n");
    
    /* enable week alarm */
    res = pcf8563_set_week_alarm_enable(&gs_handle, PCF8563_BOOL_TRUE);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set week alarm enable failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: enable week alarm.\n");
    res = pcf8563_get_week_alarm_enable(&gs_handle, &enable);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: get week alarm enable failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: check week alarm %s.\n", (enable == PCF8563_BOOL_TRUE) ? "ok" : "error");
    
    /* disable week alarm */
    res = pcf8563_set_week_alarm_enable(&gs_handle, PCF8563_BOOL_FALSE);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set week alarm enable failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: disable week alarm.\n");
    res = pcf8563_get_week_alarm_enable(&gs_handle, &enable);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: get week alarm enable failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: check week alarm %s.\n", (enable == PCF8563_BOOL_FALSE) ? "ok" : "error");
    
    /* pcf8563_set_week_alarm/pcf8563_get_week_alarm test */
    pcf8563_interface_debug_print("pcf8563: pcf8563_set_week_alarm/pcf8563_get_week_alarm test.\n");
    
    reg = rand() % 7;
    res = pcf8563_set_week_alarm(&gs_handle, reg);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set week alarm failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: set week alarm %d.\n", reg);
    res = pcf8563_get_week_alarm(&gs_handle, &reg_check);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: get week alarm failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: check week alarm %s.\n", (reg == reg_check) ? "ok" : "error");
    
    /* pcf8563_set_clock_out_enable/pcf8563_get_clock_out_enable test */
    pcf8563_interface_debug_print("pcf8563: pcf8563_set_clock_out_enable/pcf8563_get_clock_out_enable test.\n");
    
    /* enable clock out */
    res = pcf8563_set_clock_out_enable(&gs_handle, PCF8563_BOOL_TRUE);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set clock out enable failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: enable clock out.\n");
    res = pcf8563_get_clock_out_enable(&gs_handle, &enable);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: get clock out enable failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: check clock out enable %s.\n", (enable == PCF8563_BOOL_TRUE) ? "ok" : "error");
    
    /* disable clock out */
    res = pcf8563_set_clock_out_enable(&gs_handle, PCF8563_BOOL_FALSE);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set clock out enable failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: disable clock out.\n");
    res = pcf8563_get_clock_out_enable(&gs_handle, &enable);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: get clock out enable failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: check clock out enable %s.\n", (enable == PCF8563_BOOL_FALSE) ? "ok" : "error");
    
    /* pcf8563_set_clock_out/pcf8563_get_clock_out test */
    pcf8563_interface_debug_print("pcf8563: pcf8563_set_clock_out/pcf8563_get_clock_out test.\n");
    
    /* set clock out 32.768khz */
    res = pcf8563_set_clock_out(&gs_handle, PCF8563_CLOCK_OUT_32P768KHZ);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set clock out failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: set clock out 32.768khz.\n");
    res = pcf8563_get_clock_out(&gs_handle, &clk);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: get clock out failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: check clock out %s.\n", (clk == PCF8563_CLOCK_OUT_32P768KHZ) ? "ok" : "error");
    
    /* set clock out 1.024khz */
    res = pcf8563_set_clock_out(&gs_handle, PCF8563_CLOCK_OUT_1P024KHZ);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set clock out failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: set clock out 1.024khz.\n");
    res = pcf8563_get_clock_out(&gs_handle, &clk);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: get clock out failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: check clock out %s.\n", (clk == PCF8563_CLOCK_OUT_1P024KHZ) ? "ok" : "error");
    
    /* set clock out 32hz */
    res = pcf8563_set_clock_out(&gs_handle, PCF8563_CLOCK_OUT_32HZ);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set clock out failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: set clock out 32hz.\n");
    res = pcf8563_get_clock_out(&gs_handle, &clk);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: get clock out failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: check clock out %s.\n", (clk == PCF8563_CLOCK_OUT_32HZ) ? "ok" : "error");
    
    /* set clock out 1hz */
    res = pcf8563_set_clock_out(&gs_handle, PCF8563_CLOCK_OUT_1HZ);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set clock out failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: set clock out 1hz.\n");
    res = pcf8563_get_clock_out(&gs_handle, &clk);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: get clock out failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: check clock out %s.\n", (clk == PCF8563_CLOCK_OUT_1HZ) ? "ok" : "error");
    
    /* pcf8563_set_timer_enable/pcf8563_get_timer_enable test */
    pcf8563_interface_debug_print("pcf8563: pcf8563_set_timer_enable/pcf8563_get_timer_enable test.\n");
    
    /* enable timer */
    res = pcf8563_set_timer_enable(&gs_handle, PCF8563_BOOL_TRUE);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set timer enable failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: enable timer.\n");
    res = pcf8563_get_timer_enable(&gs_handle, &enable);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: get timer enable failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: check timer enable %s.\n", (enable == PCF8563_BOOL_TRUE) ? "ok" : "error");
    
    /* disable timer */
    res = pcf8563_set_timer_enable(&gs_handle, PCF8563_BOOL_FALSE);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set timer enable failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: disable timer.\n");
    res = pcf8563_get_timer_enable(&gs_handle, &enable);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: get timer enable failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: check timer enable %s.\n", (enable == PCF8563_BOOL_FALSE) ? "ok" : "error");
    
    /* pcf8563_set_timer_freq/pcf8563_get_timer_freq test */
    pcf8563_interface_debug_print("pcf8563: pcf8563_set_timer_freq/pcf8563_get_timer_freq test.\n");
    
    /* timer freq 4.096khz */
    res = pcf8563_set_timer_freq(&gs_handle, PCF8563_TIMER_FREQ_4P096KHZ);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set timer freq failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: set timer freq 4.096khz.\n");
    res = pcf8563_get_timer_freq(&gs_handle, &freq);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: get timer freq failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: check timer freq %s.\n", (freq == PCF8563_TIMER_FREQ_4P096KHZ) ? "ok" : "error");
    
    /* timer freq 64hz */
    res = pcf8563_set_timer_freq(&gs_handle, PCF8563_TIMER_FREQ_64HZ);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set timer freq failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: set timer freq 64hz.\n");
    res = pcf8563_get_timer_freq(&gs_handle, &freq);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: get timer freq failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: check timer freq %s.\n", (freq == PCF8563_TIMER_FREQ_64HZ) ? "ok" : "error");
    
    /* timer freq 1hz */
    res = pcf8563_set_timer_freq(&gs_handle, PCF8563_TIMER_FREQ_1HZ);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set timer freq failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: set timer freq 1hz.\n");
    res = pcf8563_get_timer_freq(&gs_handle, &freq);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: get timer freq failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: check timer freq %s.\n", (freq == PCF8563_TIMER_FREQ_1HZ) ? "ok" : "error");
    
    /* timer freq 1/60hz */
    res = pcf8563_set_timer_freq(&gs_handle, PCF8563_TIMER_FREQ_1_DIV_60HZ);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set timer freq failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: set timer freq 1/60hz.\n");
    res = pcf8563_get_timer_freq(&gs_handle, &freq);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: get timer freq failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: check timer freq %s.\n", (freq == PCF8563_TIMER_FREQ_1_DIV_60HZ) ? "ok" : "error");
    
    /* pcf8563_set_timer_value/pcf8563_get_timer_value test */
    pcf8563_interface_debug_print("pcf8563: pcf8563_set_timer_value/pcf8563_get_timer_value test.\n");
    
    reg = rand() % 256;
    res = pcf8563_set_timer_value(&gs_handle, reg);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set timer value failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: set timer value %d.\n", reg);
    res = pcf8563_get_timer_value(&gs_handle, &reg_check);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: get timer value failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: check timer value %s.\n", (reg == reg_check) ? "ok" : "error");
    
    /* pcf8563_set_timer_interrupt/pcf8563_get_timer_interrupt test */
    pcf8563_interface_debug_print("pcf8563: pcf8563_set_timer_interrupt/pcf8563_get_timer_interrupt test.\n");
    
    /* enable timer interrupt */
    res = pcf8563_set_timer_interrupt(&gs_handle, PCF8563_BOOL_TRUE);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set timer interrupt failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: enable timer interrupt.\n");
    res = pcf8563_get_timer_interrupt(&gs_handle, &enable);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: get timer interrupt failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: check timer interrupt %s.\n", (enable == PCF8563_BOOL_TRUE) ? "ok" : "error");
    
    /* disable timer interrupt */
    res = pcf8563_set_timer_interrupt(&gs_handle, PCF8563_BOOL_FALSE);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set timer interrupt failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: disable timer interrupt.\n");
    res = pcf8563_get_timer_interrupt(&gs_handle, &enable);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: get timer interrupt failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: check timer interrupt %s.\n", (enable == PCF8563_BOOL_FALSE) ? "ok" : "error");
    
    /* pcf8563_set_alarm_interrupt/pcf8563_get_alarm_interrupt test */
    pcf8563_interface_debug_print("pcf8563: pcf8563_set_alarm_interrupt/pcf8563_get_alarm_interrupt test.\n");
    
    /* enable alarm interrupt */
    res = pcf8563_set_alarm_interrupt(&gs_handle, PCF8563_BOOL_TRUE);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set alarm interrupt failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: enable alarm interrupt.\n");
    res = pcf8563_get_alarm_interrupt(&gs_handle, &enable);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: get alarm interrupt failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: check alarm interrupt %s.\n", (enable == PCF8563_BOOL_TRUE) ? "ok" : "error");
    
    /* disable alarm interrupt */
    res = pcf8563_set_alarm_interrupt(&gs_handle, PCF8563_BOOL_FALSE);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set alarm interrupt failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: disable alarm interrupt.\n");
    res = pcf8563_get_alarm_interrupt(&gs_handle, &enable);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: get alarm interrupt failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: check alarm interrupt %s.\n", (enable == PCF8563_BOOL_FALSE) ? "ok" : "error");
    
    /* pcf8563_set_interrupt_mode/pcf8563_get_interrupt_mode test */
    pcf8563_interface_debug_print("pcf8563: pcf8563_set_interrupt_mode/pcf8563_get_interrupt_mode test.\n");
    
    /* set interrupt mode event */
    res = pcf8563_set_interrupt_mode(&gs_handle, PCF8563_INTERRUPT_MODE_LEVEL);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set interrupt mode failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: set interrupt mode level.\n");
    res = pcf8563_get_interrupt_mode(&gs_handle, &mode);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: get interrupt mode failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: check interrupt mode %s.\n", (mode == PCF8563_INTERRUPT_MODE_LEVEL) ? "ok" : "error");
    
    /* set interrupt mode pulse */
    res = pcf8563_set_interrupt_mode(&gs_handle, PCF8563_INTERRUPT_MODE_PULSE);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: set interrupt mode failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: set interrupt mode pulse.\n");
    res = pcf8563_get_interrupt_mode(&gs_handle, &mode);
    if (res != 0)
    {
        pcf8563_interface_debug_print("pcf8563: get interrupt mode failed.\n");
        (void)pcf8563_deinit(&gs_handle);
        
        return 1;
    }
    pcf8563_interface_debug_print("pcf8563: check interrupt mode %s.\n", (mode == PCF8563_INTERRUPT_MODE_PULSE) ? "ok" : "error");
    
    /* finish register test */
    pcf8563_interface_debug_print("pcf8563: finish register test.\n");
    (void)pcf8563_deinit(&gs_handle);
    
    return 0;
}
