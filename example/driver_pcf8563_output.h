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
 * @file      driver_pcf8563_output.h
 * @brief     driver pcf8563 output header file
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
  
#ifndef DRIVER_PCF8563_OUTPUT_H
#define DRIVER_PCF8563_OUTPUT_H

#include "driver_pcf8563_interface.h"
#include <time.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @addtogroup pcf8563_example_driver
 * @{
 */

/**
 * @brief  output example init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 * @note   none
 */
uint8_t pcf8563_output_init(void);

/**
 * @brief  output example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t pcf8563_output_deinit(void);

/**
 * @brief     output example set the time
 * @param[in] *t pointer to a time structure
 * @return    status code
 *            - 0 success
 *            - 1 set time failed
 * @note      none
 */
uint8_t pcf8563_output_set_time(pcf8563_time_t *t);

/**
 * @brief     output example set the time by a unix timestamp
 * @param[in] timestamp unix timestamp
 * @return    status code
 *            - 0 success
 *            - 1 set timestamp failed
 * @note      none
 */
uint8_t pcf8563_output_set_timestamp(time_t timestamp);

/**
 * @brief     output example set the local time zone
 * @param[in] zone local time zone
 * @return    status code
 *            - 0 success
 * @note      none
 */
uint8_t pcf8563_output_set_timestamp_time_zone(int8_t zone);

/**
 * @brief      output example get the time
 * @param[out] *t pointer to a time structure
 * @return     status code
 *             - 0 success
 *             - 1 get time failed
 * @note       none
 */
uint8_t pcf8563_output_get_time(pcf8563_time_t *t);

/**
 * @brief      output example get the time in a unix timestamp
 * @param[out] *timestamp pointer to a unix timestamp buffer
 * @return     status code
 *             - 0 success
 *             - 1 get timestamp failed
 * @note       none
 */
uint8_t pcf8563_output_get_timestamp(time_t *timestamp);

/**
 * @brief      output example get the local time zone
 * @param[out] *zone pointer to a local time zone buffer
 * @return     status code
 *             - 0 success
 * @note       none
 */
uint8_t pcf8563_output_get_timestamp_time_zone(int8_t *zone);

/**
 * @brief      output example get the ascii time
 * @param[out] *buf pointer to an ascii buffer
 * @param[in]  len data length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t pcf8563_output_get_ascii_time(char *buf, uint8_t len);

/**
 * @brief     output example enable the output
 * @param[in] clk set clock
 * @return    status code
 *            - 0 success
 *            - 1 output enable failed
 * @note      none
 */
uint8_t pcf8563_output_enable(pcf8563_clock_out_t clk);

/**
 * @brief  output example disable the output
 * @return status code
 *         - 0 success
 *         - 1 output disable failed
 * @note   none
 */
uint8_t pcf8563_output_disable(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
