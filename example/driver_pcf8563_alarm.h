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
 * @file      driver_pcf8563_alarm.h
 * @brief     driver pcf8563 alarm header file
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
  
#ifndef DRIVER_PCF8563_ALARM_H
#define DRIVER_PCF8563_ALARM_H

#include "driver_pcf8563_interface.h"
#include <time.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup pcf8563_example_driver pcf8563 example driver function
 * @brief    pcf8563 example driver modules
 * @ingroup  pcf8563_driver
 * @{
 */

/**
 * @brief pcf8563 alarm param structure definition
 */
typedef struct pcf8563_alarm_param_s
{
    pcf8563_bool_t minute_enable;                  /**< minute enable */
    uint8_t minute;                                /**< minute */
    pcf8563_bool_t hour_enable;                    /**< hour enable */
    uint8_t hour;                                  /**< hour */
    pcf8563_bool_t day_enable;                     /**< day enable */
    uint8_t day;                                   /**< day */
    pcf8563_bool_t week_enable;                    /**< week enable */
    uint8_t week;                                  /**< week */
    pcf8563_interrupt_mode_t mode;                 /**< interrupt mode */
} pcf8563_alarm_param_t;

/**
 * @brief  alarm example irq
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t pcf8563_alarm_irq_handler(void);

/**
 * @brief     alarm example init
 * @param[in] *callback point to a receive callback function address
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t pcf8563_alarm_init(void (*callback)(uint8_t type));

/**
 * @brief  alarm example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t pcf8563_alarm_deinit(void);

/**
 * @brief     alarm example enable the alarm
 * @param[in] *param points to a pcf8563_alarm_param_t structure
 * @return    status code
 *            - 0 success
 *            - 1 alarm enable failed
 * @note      none
 */
uint8_t pcf8563_alarm_enable(pcf8563_alarm_param_t *param);

/**
 * @brief  alarm example disable the alarm
 * @return status code
 *         - 0 success
 *         - 1 alarm disable failed
 * @note   none
 */
uint8_t pcf8563_alarm_disable(void);

/**
 * @brief     alarm example set the time
 * @param[in] *t points to a time structure
 * @return    status code
 *            - 0 success
 *            - 1 set time failed
 * @note      none
 */
uint8_t pcf8563_alarm_set_time(pcf8563_time_t *t);

/**
 * @brief     alarm example set the time by a unix timestamp
 * @param[in] timestamp is a unix timestamp
 * @return    status code
 *            - 0 success
 *            - 1 set timestamp failed
 * @note      none
 */
uint8_t pcf8563_alarm_set_timestamp(time_t timestamp);

/**
 * @brief     alarm example set the local time zone
 * @param[in] zone is the local time zone
 * @return    status code
 *            - 0 success
 * @note      none
 */
uint8_t pcf8563_alarm_set_timestamp_time_zone(int8_t zone);

/**
 * @brief      alarm example get the time
 * @param[out] *t points to a time structure
 * @return     status code
 *             - 0 success
 *             - 1 get time failed
 * @note       none
 */
uint8_t pcf8563_alarm_get_time(pcf8563_time_t *t);

/**
 * @brief      alarm example get the time in a unix timestamp
 * @param[out] *timestamp points to a unix timestamp buffer
 * @return     status code
 *             - 0 success
 *             - 1 get timestamp failed
 * @note       none
 */
uint8_t pcf8563_alarm_get_timestamp(time_t *timestamp);

/**
 * @brief      alarm example get the local time zone
 * @param[out] *zone points to a local time zone buffer
 * @return     status code
 *             - 0 success
 * @note       none
 */
uint8_t pcf8563_alarm_get_timestamp_time_zone(int8_t *zone);

/**
 * @brief      alarm example get the ascii time
 * @param[out] *buf points to an ascii buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t pcf8563_alarm_get_ascii_time(char *buf, uint8_t len);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
