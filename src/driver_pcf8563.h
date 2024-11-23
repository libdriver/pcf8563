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
 * @file      driver_pcf8563.h
 * @brief     driver pcf8563 header file
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

#ifndef DRIVER_PCF8563_H
#define DRIVER_PCF8563_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup pcf8563_driver pcf8563 driver function
 * @brief    pcf8563 driver modules
 * @{
 */

/**
 * @addtogroup pcf8563_base_driver
 * @{
 */

/**
 * @brief pcf8563 bool enumeration definition
 */
typedef enum
{
    PCF8563_BOOL_FALSE = 0x00,        /**< disable */
    PCF8563_BOOL_TRUE  = 0x01,        /**< enable */
} pcf8563_bool_t;

/**
 * @brief pcf8563 clock out enumeration definition
 */
typedef enum
{
    PCF8563_CLOCK_OUT_32P768KHZ = 0x00,        /**< 32.768khz */
    PCF8563_CLOCK_OUT_1P024KHZ  = 0x01,        /**< 1.024khz */
    PCF8563_CLOCK_OUT_32HZ      = 0x02,        /**< 32hz */
    PCF8563_CLOCK_OUT_1HZ       = 0x03,        /**< 1hz */
} pcf8563_clock_out_t;

/**
 * @brief pcf8563 timer freq enumeration definition
 */
typedef enum
{
    PCF8563_TIMER_FREQ_4P096KHZ   = 0x00,        /**< 4.096khz */
    PCF8563_TIMER_FREQ_64HZ       = 0x01,        /**< 64hz */
    PCF8563_TIMER_FREQ_1HZ        = 0x02,        /**< 1hz */
    PCF8563_TIMER_FREQ_1_DIV_60HZ = 0x03,        /**< 1/60hz */
} pcf8563_timer_freq_t;

/**
 * @brief pcf8563 interrupt mode enumeration definition
 */
typedef enum
{
    PCF8563_INTERRUPT_MODE_LEVEL = 0x00,        /**< level mode */
    PCF8563_INTERRUPT_MODE_PULSE = 0x01,        /**< pulse mode */
} pcf8563_interrupt_mode_t;

/**
 * @brief pcf8563 interrupt event enumeration definition
 */
typedef enum
{
    PCF8563_INTERRUPT_EVENT_TIMER = 0x00,        /**< timer */
    PCF8563_INTERRUPT_EVENT_ALARM = 0x01,        /**< alarm */
} pcf8563_interrupt_event_t;

/**
 * @brief pcf8563 time structure definition
 */
typedef struct pcf8563_time_s
{
    uint16_t year;         /**< year */
    uint8_t month;         /**< month */
    uint8_t week;          /**< week */
    uint8_t date;          /**< date */
    uint8_t hour;          /**< hour */
    uint8_t minute;        /**< minute */
    uint8_t second;        /**< second */
} pcf8563_time_t;

/**
 * @brief pcf8563 handle structure definition
 */
typedef struct pcf8563_handle_s
{
    uint8_t (*iic_init)(void);                                                          /**< point to an iic_init function address */
    uint8_t (*iic_deinit)(void);                                                        /**< point to an iic_deinit function address */
    uint8_t (*iic_write)(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);        /**< point to an iic_write function address */
    uint8_t (*iic_read)(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);         /**< point to an iic_read function address */
    void (*debug_print)(const char *const fmt, ...);                                    /**< point to a debug_print function address */
    void (*delay_ms)(uint32_t ms);                                                      /**< point to a delay_ms function address */
    void (*receive_callback)(uint8_t type);                                             /**< point to a receive_callback function address */
    uint8_t inited;                                                                     /**< inited flag */
} pcf8563_handle_t;

/**
 * @brief pcf8563 information structure definition
 */
typedef struct pcf8563_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} pcf8563_info_t;

/**
 * @}
 */

/**
 * @defgroup pcf8563_link_driver pcf8563 link driver function
 * @brief    pcf8563 link driver modules
 * @ingroup  pcf8563_driver
 * @{
 */

/**
 * @brief     initialize pcf8563_handle_t structure
 * @param[in] HANDLE points to a pcf8563 handle structure
 * @param[in] STRUCTURE is pcf8563_handle_t
 * @note      none
 */
#define DRIVER_PCF8563_LINK_INIT(HANDLE, STRUCTURE)         memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link iic_init function
 * @param[in] HANDLE points to a pcf8563 handle structure
 * @param[in] FUC points to an iic_init function address
 * @note      none
 */
#define DRIVER_PCF8563_LINK_IIC_INIT(HANDLE, FUC)          (HANDLE)->iic_init = FUC

/**
 * @brief     link iic_deinit function
 * @param[in] HANDLE points to a pcf8563 handle structure
 * @param[in] FUC points to an iic_deinit function address
 * @note      none
 */
#define DRIVER_PCF8563_LINK_IIC_DEINIT(HANDLE, FUC)        (HANDLE)->iic_deinit = FUC

/**
 * @brief     link iic_read function
 * @param[in] HANDLE points to a pcf8563 handle structure
 * @param[in] FUC points to an iic_read function address
 * @note      none
 */
#define DRIVER_PCF8563_LINK_IIC_READ(HANDLE, FUC)          (HANDLE)->iic_read = FUC

/**
 * @brief     link iic_write function
 * @param[in] HANDLE points to a pcf8563 handle structure
 * @param[in] FUC points to an iic_write function address
 * @note      none
 */
#define DRIVER_PCF8563_LINK_IIC_WRITE(HANDLE, FUC)         (HANDLE)->iic_write = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE points to a pcf8563 handle structure
 * @param[in] FUC points to a delay_ms function address
 * @note      none
 */
#define DRIVER_PCF8563_LINK_DELAY_MS(HANDLE, FUC)          (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE points to a pcf8563 handle structure
 * @param[in] FUC points to a debug_print function address
 * @note      none
 */
#define DRIVER_PCF8563_LINK_DEBUG_PRINT(HANDLE, FUC)       (HANDLE)->debug_print = FUC

/**
 * @brief     link receive_callback function
 * @param[in] HANDLE points to a pcf8563 handle structure
 * @param[in] FUC points to a receive_callback function address
 * @note      none
 */
#define DRIVER_PCF8563_LINK_RECEIVE_CALLBACK(HANDLE, FUC)  (HANDLE)->receive_callback = FUC

/**
 * @}
 */

/**
 * @defgroup pcf8563_base_driver pcf8563 base driver function
 * @brief    pcf8563 base driver modules
 * @ingroup  pcf8563_driver
 * @{
 */

/**
 * @brief      get chip's information
 * @param[out] *info points to a pcf8563 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t pcf8563_info(pcf8563_info_t *info);

/**
 * @brief     irq handler
 * @param[in] *handle points to a pcf8563 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t pcf8563_irq_handler(pcf8563_handle_t *handle);

/**
 * @brief     initialize the chip
 * @param[in] *handle points to a pcf8563 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 * @note      none
 */
uint8_t pcf8563_init(pcf8563_handle_t *handle);

/**
 * @brief     close the chip
 * @param[in] *handle points to a pcf8563 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t pcf8563_deinit(pcf8563_handle_t *handle);

/**
 * @brief     set the current time
 * @param[in] *handle points to a pcf8563 handle structure
 * @param[in] *t points to a time structure
 * @return    status code
 *            - 0 success
 *            - 1 set time failed
 *            - 2 handle or time is NULL
 *            - 3 handle is not initialized
 *            - 4 time is invalid
 * @note      sunday is 0 not 7
 */
uint8_t pcf8563_set_time(pcf8563_handle_t *handle, pcf8563_time_t *t);

/**
 * @brief      get the current time
 * @param[in]  *handle points to a pcf8563 handle structure
 * @param[out] *t points to a time structure
 * @return     status code
 *             - 0 success
 *             - 1 get time failed
 *             - 2 handle or time is NULL
 *             - 3 handle is not initialized
 *             - 4 clock integrity is not guaranteed
 * @note       sunday is 0 not 7
 */
uint8_t pcf8563_get_time(pcf8563_handle_t *handle, pcf8563_time_t *t);

/**
 * @brief     enable or disable test mode
 * @param[in] *handle points to a pcf8563 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set test mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t pcf8563_set_test_mode(pcf8563_handle_t *handle, pcf8563_bool_t enable);

/**
 * @brief      get the test mode status
 * @param[in]  *handle points to a pcf8563 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get test mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t pcf8563_get_test_mode(pcf8563_handle_t *handle, pcf8563_bool_t *enable);

/**
 * @brief     enable or disable rtc stop
 * @param[in] *handle points to a pcf8563 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set rtc stop failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t pcf8563_set_rtc_stop(pcf8563_handle_t *handle, pcf8563_bool_t enable);

/**
 * @brief      get the rtc stop status
 * @param[in]  *handle points to a pcf8563 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get rtc stop failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t pcf8563_get_rtc_stop(pcf8563_handle_t *handle, pcf8563_bool_t *enable);

/**
 * @brief     enable or disable power on reset
 * @param[in] *handle points to a pcf8563 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set power on reset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t pcf8563_set_power_on_reset(pcf8563_handle_t *handle, pcf8563_bool_t enable);

/**
 * @brief      get the power on reset status
 * @param[in]  *handle points to a pcf8563 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get the power on reset failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t pcf8563_get_power_on_reset(pcf8563_handle_t *handle, pcf8563_bool_t *enable);

/**
 * @brief     enable or disable minute alarm
 * @param[in] *handle points to a pcf8563 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set minute alarm enable failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t pcf8563_set_minute_alarm_enable(pcf8563_handle_t *handle, pcf8563_bool_t enable);

/**
 * @brief      get the minute alarm status
 * @param[in]  *handle points to a pcf8563 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get minute alarm enable failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t pcf8563_get_minute_alarm_enable(pcf8563_handle_t *handle, pcf8563_bool_t *enable);

/**
 * @brief     set minute alarm
 * @param[in] *handle points to a pcf8563 handle structure
 * @param[in] minute is the set minute
 * @return    status code
 *            - 0 success
 *            - 1 set minute alarm failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 minute can't be over than 59
 * @note      none
 */
uint8_t pcf8563_set_minute_alarm(pcf8563_handle_t *handle, uint8_t minute);

/**
 * @brief      get minute alarm
 * @param[in]  *handle points to a pcf8563 handle structure
 * @param[out] *minute points to a minute buffer
 * @return     status code
 *             - 0 success
 *             - 1 get minute alarm failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t pcf8563_get_minute_alarm(pcf8563_handle_t *handle, uint8_t *minute);

/**
 * @brief     enable or disable hour alarm
 * @param[in] *handle points to a pcf8563 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set hour alarm enable failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t pcf8563_set_hour_alarm_enable(pcf8563_handle_t *handle, pcf8563_bool_t enable);

/**
 * @brief      get the hour alarm status
 * @param[in]  *handle points to a pcf8563 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get hour alarm enable failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t pcf8563_get_hour_alarm_enable(pcf8563_handle_t *handle, pcf8563_bool_t *enable);

/**
 * @brief     set hour alarm
 * @param[in] *handle points to a pcf8563 handle structure
 * @param[in] hour is the set hour
 * @return    status code
 *            - 0 success
 *            - 1 set hour alarm failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 hour can't be over than 23
 * @note      none
 */
uint8_t pcf8563_set_hour_alarm(pcf8563_handle_t *handle, uint8_t hour);

/**
 * @brief      get hour alarm
 * @param[in]  *handle points to a pcf8563 handle structure
 * @param[out] *hour points to an hour buffer
 * @return     status code
 *             - 0 success
 *             - 1 get hour alarm failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t pcf8563_get_hour_alarm(pcf8563_handle_t *handle, uint8_t *hour);

/**
 * @brief     enable or disable day alarm
 * @param[in] *handle points to a pcf8563 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set day alarm enable failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t pcf8563_set_day_alarm_enable(pcf8563_handle_t *handle, pcf8563_bool_t enable);

/**
 * @brief      get the day alarm status
 * @param[in]  *handle points to a pcf8563 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get day alarm enable failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t pcf8563_get_day_alarm_enable(pcf8563_handle_t *handle, pcf8563_bool_t *enable);

/**
 * @brief     set day alarm
 * @param[in] *handle points to a pcf8563 handle structure
 * @param[in] day is the set day
 * @return    status code
 *            - 0 success
 *            - 1 set day alarm failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 day can't be zero or over than 31
 * @note      none
 */
uint8_t pcf8563_set_day_alarm(pcf8563_handle_t *handle, uint8_t day);

/**
 * @brief      get day alarm
 * @param[in]  *handle points to a pcf8563 handle structure
 * @param[out] *day points to a day buffer
 * @return     status code
 *             - 0 success
 *             - 1 get day alarm failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t pcf8563_get_day_alarm(pcf8563_handle_t *handle, uint8_t *day);

/**
 * @brief     enable or disable week alarm
 * @param[in] *handle points to a pcf8563 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set week alarm enable failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t pcf8563_set_week_alarm_enable(pcf8563_handle_t *handle, pcf8563_bool_t enable);

/**
 * @brief      get the week alarm status
 * @param[in]  *handle points to a pcf8563 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get week alarm enable failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t pcf8563_get_week_alarm_enable(pcf8563_handle_t *handle, pcf8563_bool_t *enable);

/**
 * @brief     set week alarm
 * @param[in] *handle points to a pcf8563 handle structure
 * @param[in] week is the set week
 * @return    status code
 *            - 0 success
 *            - 1 set week alarm failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 week can't be over than 6
 * @note      none
 */
uint8_t pcf8563_set_week_alarm(pcf8563_handle_t *handle, uint8_t week);

/**
 * @brief      get week alarm
 * @param[in]  *handle points to a pcf8563 handle structure
 * @param[out] *week points to a week buffer
 * @return     status code
 *             - 0 success
 *             - 1 get week alarm failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t pcf8563_get_week_alarm(pcf8563_handle_t *handle, uint8_t *week);

/**
 * @brief     enable or disable clock out enable
 * @param[in] *handle points to a pcf8563 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set clock out enable failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t pcf8563_set_clock_out_enable(pcf8563_handle_t *handle, pcf8563_bool_t enable);

/**
 * @brief      get clock out enable status
 * @param[in]  *handle points to a pcf8563 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get clock out enable failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t pcf8563_get_clock_out_enable(pcf8563_handle_t *handle, pcf8563_bool_t *enable);

/**
 * @brief     set clock out
 * @param[in] *handle points to a pcf8563 handle structure
 * @param[in] clk is the set clock
 * @return    status code
 *            - 0 success
 *            - 1 set clock out failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t pcf8563_set_clock_out(pcf8563_handle_t *handle, pcf8563_clock_out_t clk);

/**
 * @brief      get clock out
 * @param[in]  *handle points to a pcf8563 handle structure
 * @param[out] *clk points to a clock buffer
 * @return     status code
 *             - 0 success
 *             - 1 get clock out failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t pcf8563_get_clock_out(pcf8563_handle_t *handle, pcf8563_clock_out_t *clk);

/**
 * @brief     enable or disable timer enable
 * @param[in] *handle points to a pcf8563 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set timer enable failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t pcf8563_set_timer_enable(pcf8563_handle_t *handle, pcf8563_bool_t enable);

/**
 * @brief      get timer enable status
 * @param[in]  *handle points to a pcf8563 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get timer enable failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t pcf8563_get_timer_enable(pcf8563_handle_t *handle, pcf8563_bool_t *enable);

/**
 * @brief     set timer freq
 * @param[in] *handle points to a pcf8563 handle structure
 * @param[in] freq is the set freq
 * @return    status code
 *            - 0 success
 *            - 1 set timer freq failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t pcf8563_set_timer_freq(pcf8563_handle_t *handle, pcf8563_timer_freq_t freq);

/**
 * @brief      get timer freq
 * @param[in]  *handle points to a pcf8563 handle structure
 * @param[out] *freq points to a freq buffer
 * @return     status code
 *             - 0 success
 *             - 1 get timer freq failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t pcf8563_get_timer_freq(pcf8563_handle_t *handle, pcf8563_timer_freq_t *freq);

/**
 * @brief     set timer value
 * @param[in] *handle points to a pcf8563 handle structure
 * @param[in] value is the set value
 * @return    status code
 *            - 0 success
 *            - 1 set timer value failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t pcf8563_set_timer_value(pcf8563_handle_t *handle, uint8_t value);

/**
 * @brief      get timer value
 * @param[in]  *handle points to a pcf8563 handle structure
 * @param[out] *value points to a value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get timer value failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t pcf8563_get_timer_value(pcf8563_handle_t *handle, uint8_t *value);

/**
 * @brief     enable or disable timer interrupt
 * @param[in] *handle points to a pcf8563 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set timer interrupt failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t pcf8563_set_timer_interrupt(pcf8563_handle_t *handle, pcf8563_bool_t enable);

/**
 * @brief      get timer interrupt status
 * @param[in]  *handle points to a pcf8563 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get timer interrupt failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t pcf8563_get_timer_interrupt(pcf8563_handle_t *handle, pcf8563_bool_t *enable);

/**
 * @brief     enable or disable alarm interrupt
 * @param[in] *handle points to a pcf8563 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set alarm interrupt failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t pcf8563_set_alarm_interrupt(pcf8563_handle_t *handle, pcf8563_bool_t enable);

/**
 * @brief      get alarm interrupt status
 * @param[in]  *handle points to a pcf8563 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get alarm interrupt failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t pcf8563_get_alarm_interrupt(pcf8563_handle_t *handle, pcf8563_bool_t *enable);

/**
 * @brief     set the interrupt mode
 * @param[in] *handle points to a pcf8563 handle structure
 * @param[in] mode is the set mode
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t pcf8563_set_interrupt_mode(pcf8563_handle_t *handle, pcf8563_interrupt_mode_t mode);

/**
 * @brief      get the interrupt mode
 * @param[in]  *handle points to a pcf8563 handle structure
 * @param[out] *mode points to a mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t pcf8563_get_interrupt_mode(pcf8563_handle_t *handle, pcf8563_interrupt_mode_t *mode);

/**
 * @brief     clear status
 * @param[in] *handle points to a pcf8563 handle structure
 * @param[in] event is the set event
 * @return    status code
 *            - 0 success
 *            - 1 clear status failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t pcf8563_clear_status(pcf8563_handle_t *handle, pcf8563_interrupt_event_t event);

/**
 * @}
 */

/**
 * @defgroup pcf8563_extern_driver pcf8563 extern driver function
 * @brief    pcf8563 extern driver modules
 * @ingroup  pcf8563_driver
 * @{
 */

/**
 * @brief     set the chip register
 * @param[in] *handle points to a pcf8563 handle structure
 * @param[in] reg is the iic register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t pcf8563_set_reg(pcf8563_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @brief      get the chip register
 * @param[in]  *handle points to a pcf8563 handle structure
 * @param[in]  reg is the iic register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data buffer length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t pcf8563_get_reg(pcf8563_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
