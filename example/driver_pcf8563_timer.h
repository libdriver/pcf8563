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
 * @file      driver_pcf8563_timer.h
 * @brief     driver pcf8563 timer header file
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
  
#ifndef DRIVER_PCF8563_TIMER_H
#define DRIVER_PCF8563_TIMER_H

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
 * @brief  timer example irq
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t pcf8563_timer_irq_handler(void);

/**
 * @brief     timer example init
 * @param[in] *callback point to a receive callback function address
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t pcf8563_timer_init(void (*callback)(uint8_t type));

/**
 * @brief  timer example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t pcf8563_timer_deinit(void);

/**
 * @brief     timer example enable the timer
 * @param[in] freq is the set freq
 * @param[in] value is the set value
 * @param[in] mode is the set interrupt mode
 * @return    status code
 *            - 0 success
 *            - 1 timer enable failed
 * @note      none
 */
uint8_t pcf8563_timer_enable(pcf8563_timer_freq_t freq, uint8_t value, pcf8563_interrupt_mode_t mode);

/**
 * @brief  timer example disable the timer
 * @return status code
 *         - 0 success
 *         - 1 timer disable failed
 * @note   none
 */
uint8_t pcf8563_timer_disable(void);

/**
 * @brief     timer example set the time
 * @param[in] *t points to a time structure
 * @return    status code
 *            - 0 success
 *            - 1 set time failed
 * @note      none
 */
uint8_t pcf8563_timer_set_time(pcf8563_time_t *t);

/**
 * @brief     timer example set the time by a unix timestamp
 * @param[in] timestamp is a unix timestamp
 * @return    status code
 *            - 0 success
 *            - 1 set timestamp failed
 * @note      none
 */
uint8_t pcf8563_timer_set_timestamp(time_t timestamp);

/**
 * @brief     timer example set the local time zone
 * @param[in] zone is the local time zone
 * @return    status code
 *            - 0 success
 * @note      none
 */
uint8_t pcf8563_timer_set_timestamp_time_zone(int8_t zone);

/**
 * @brief      timer example get the time
 * @param[out] *t points to a time structure
 * @return     status code
 *             - 0 success
 *             - 1 get time failed
 * @note       none
 */
uint8_t pcf8563_timer_get_time(pcf8563_time_t *t);

/**
 * @brief      timer example get the time in a unix timestamp
 * @param[out] *timestamp points to a unix timestamp buffer
 * @return     status code
 *             - 0 success
 *             - 1 get timestamp failed
 * @note       none
 */
uint8_t pcf8563_timer_get_timestamp(time_t *timestamp);

/**
 * @brief      timer example get the local time zone
 * @param[out] *zone points to a local time zone buffer
 * @return     status code
 *             - 0 success
 * @note       none
 */
uint8_t pcf8563_timer_get_timestamp_time_zone(int8_t *zone);

/**
 * @brief      timer example get the ascii time
 * @param[out] *buf points to an ascii buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t pcf8563_timer_get_ascii_time(char *buf, uint8_t len);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
