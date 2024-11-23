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
 * @file      driver_pcf8563.c
 * @brief     driver pcf8563 source file
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

#include "driver_pcf8563.h"

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "NXP PCF8563"        /**< chip name */
#define MANUFACTURER_NAME         "NXP"                /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        1.9f                 /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        5.5f                 /**< chip max supply voltage */
#define MAX_CURRENT               0.80f                /**< chip max current */
#define TEMPERATURE_MIN           -40.0f               /**< chip min operating temperature */
#define TEMPERATURE_MAX           85.0f                /**< chip max operating temperature */
#define DRIVER_VERSION            1000                 /**< driver version */

/**
 * @brief chip register definition
 */
#define PCF8563_REG_CONTROL_STATUS1      0x00        /**< control status1 register */
#define PCF8563_REG_CONTROL_STATUS2      0x01        /**< control status2 register */
#define PCF8563_REG_SECOND               0x02        /**< second register */
#define PCF8563_REG_MINUTE               0x03        /**< minute register */
#define PCF8563_REG_HOUR                 0x04        /**< hour register */
#define PCF8563_REG_DAY                  0x05        /**< day register */
#define PCF8563_REG_WEEK                 0x06        /**< week register */
#define PCF8563_REG_MONTH                0x07        /**< month register */
#define PCF8563_REG_YEAR                 0x08        /**< year register */
#define PCF8563_REG_MINUTE_ALARM         0x09        /**< minute alarm register */
#define PCF8563_REG_HOUR_ALARM           0x0A        /**< hour alarm register */
#define PCF8563_REG_DAY_ALARM            0x0B        /**< day alarm register */
#define PCF8563_REG_WEEK_ALARM           0x0C        /**< week alarm register */
#define PCF8563_REG_CLKOUT_CONTROL       0x0D        /**< clkout control register */
#define PCF8563_REG_TIMER_CONTROL        0x0E        /**< timer control register */
#define PCF8563_REG_TIMER                0x0F        /**< timer register */

/**
 * @brief chip address definition
 */
#define PCF8563_ADDRESS        0xA2        /**< iic device address */ 

/**
 * @brief     write one byte
 * @param[in] *handle points to a pcf8563 handle structure
 * @param[in] reg is the iic register address
 * @param[in] data is the write data
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
static uint8_t a_pcf8563_iic_write(pcf8563_handle_t *handle, uint8_t reg, uint8_t data)
{
    if (handle->iic_write(PCF8563_ADDRESS, reg, &data, 1) != 0)        /* write data */
    {
        return 1;                                                      /* return error */
    }
    
    return 0;                                                          /* success return 0 */
}

/**
 * @brief      read multiple bytes
 * @param[in]  *handle points to a pcf8563 handle structure
 * @param[in]  reg is the iic register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data buffer length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
static uint8_t a_pcf8563_iic_multiple_read(pcf8563_handle_t *handle, uint8_t reg, uint8_t *buf, uint8_t len)
{
    if (handle->iic_read(PCF8563_ADDRESS, reg, buf, len) != 0)       /* read data */
    {
        return 1;                                                    /* return error */
    }
    
    return 0;                                                        /* success return 0 */
}

/**
 * @brief     hex to bcd
 * @param[in] val is the hex data
 * @return    bcd data
 * @note      none
 */
static uint8_t a_pcf8563_hex2bcd(uint8_t val)
{
    uint8_t i, j, k;
    
    i = val / 10;            /* get tens place */
    j = val % 10;            /* get ones place */
    k = j + (i << 4);        /* set bcd */
    
    return k;                /* return bcd */
}

/**
 * @brief     bcd to hex
 * @param[in] val is the bcd data
 * @return    hex data
 * @note      none
 */
static uint8_t a_pcf8563_bcd2hex(uint8_t val)
{
    uint8_t temp;
    
    temp = val & 0x0F;              /* get ones place */
    val = (val >> 4) & 0x0F;        /* get tens place */
    val = val * 10;                 /* set tens place */
    temp = temp + val;              /* get hex */
    
    return temp;                    /* return hex */
}

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
uint8_t pcf8563_set_time(pcf8563_handle_t *handle, pcf8563_time_t *t)
{
    uint8_t res;
    uint16_t year;
    
    if (handle == NULL)                                                                                      /* check handle */
    {
        return 2;                                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                                 /* check handle initialization */
    {
        return 3;                                                                                            /* return error */
    }
    if (t == NULL)                                                                                           /* check time */
    {
        handle->debug_print("pcf8563: time is null.\n");                                                     /* time is null */
        
        return 2;                                                                                            /* return error */
    }

    if ((t->year < 2000) || (t->year > 2199))                                                                /* check year */
    {
        handle->debug_print("pcf8563: year can't be over 2199 or less than 2000.\n");                        /* year can't be over 2199 or less than 2000 */
        
        return 4;                                                                                            /* return error */
    }
    if ((t->month == 0) || (t->month > 12))                                                                  /* check month */
    {
        handle->debug_print("pcf8563: month can't be zero or over than 12.\n");                              /* month can't be zero or over than 12 */
        
        return 4;                                                                                            /* return error */
    }
    if (t->week > 6)                                                                                         /* check week */
    {
        handle->debug_print("pcf8563: week can't be over than 6.\n");                                        /* week can't be over than 6 */
        
        return 4;                                                                                            /* return error */
    }
    if ((t->date == 0) || (t->date > 31))                                                                    /* check data */
    {
        handle->debug_print("pcf8563: date can't be zero or over than 31.\n");                               /* date can't be zero or over than 31 */
        
        return 4;                                                                                            /* return error */
    }
    if (t->hour > 23)                                                                                        /* check hour */
    {
        handle->debug_print("pcf8563: hour can't be over than 23.\n");                                       /* hour can't be over than 23 */
        
        return 4;                                                                                            /* return error */
    }
    if (t->minute > 59)                                                                                      /* check minute */
    {
        handle->debug_print("pcf8563: minute can't be over than 59.\n");                                     /* minute can't be over than 59 */
        
        return 4;                                                                                            /* return error */
    }
    if (t->second > 59)                                                                                      /* check second */
    {
        handle->debug_print("pcf8563: second can't be over than 59.\n");                                     /* second can't be over than 59 */
        
        return 4;                                                                                            /* return error */
    }
    res = a_pcf8563_iic_write(handle, PCF8563_REG_SECOND, a_pcf8563_hex2bcd(t->second));                     /* write second */
    if (res != 0)                                                                                            /* check result */
    {
        handle->debug_print("pcf8563: write second failed.\n");                                              /* write second failed */
        
        return 1;                                                                                            /* return error */
    }
    res = a_pcf8563_iic_write(handle, PCF8563_REG_MINUTE, a_pcf8563_hex2bcd(t->minute));                     /* write minute */
    if (res != 0)                                                                                            /* check result */
    {
        handle->debug_print("pcf8563: write minute failed.\n");                                              /* write minute failed */
        
        return 1;                                                                                            /* return error */
    }
    res = a_pcf8563_iic_write(handle, PCF8563_REG_HOUR, a_pcf8563_hex2bcd(t->hour));                         /* write hour */
    if (res != 0)                                                                                            /* check result */
    {
        handle->debug_print("pcf8563: write hour failed.\n");                                                /* write hour failed */
        
        return 1;                                                                                            /* return error */
    }
    res = a_pcf8563_iic_write(handle, PCF8563_REG_WEEK, a_pcf8563_hex2bcd(t->week));                         /* write week */
    if (res != 0)                                                                                            /* check result */
    {
        handle->debug_print("pcf8563: write week failed.\n");                                                /* write week failed */
        
        return 1;                                                                                            /* return error */
    }
    res = a_pcf8563_iic_write(handle, PCF8563_REG_DAY, a_pcf8563_hex2bcd(t->date));                          /* write day */
    if (res != 0)                                                                                            /* check result */
    {
        handle->debug_print("pcf8563: write day failed.\n");                                                 /* write day failed */
        
        return 1;                                                                                            /* return error */
    }
    year = t->year - 2000;                                                                                   /* convert year */
    if (year >= 100)                                                                                         /* check year */
    {
        res = a_pcf8563_iic_write(handle, PCF8563_REG_MONTH, a_pcf8563_hex2bcd(t->month) | (1 << 7));        /* write month and century */
        if (res != 0)                                                                                        /* check result */
        {
            handle->debug_print("pcf8563: write century and month failed.\n");                               /* write century and month failed */
            
            return 1;                                                                                        /* return error */
        }
        year = year - 100;                                                                                   /* year - 100 */
        res = a_pcf8563_iic_write(handle, PCF8563_REG_YEAR, a_pcf8563_hex2bcd((uint8_t)year));               /* write year */
        if (res != 0)                                                                                        /* check result */
        {
            handle->debug_print("pcf8563: write year failed.\n");                                            /* write year failed */
            
            return 1;                                                                                        /* return error */
        }
    }
    else
    {
        res = a_pcf8563_iic_write(handle, PCF8563_REG_MONTH, a_pcf8563_hex2bcd(t->month));                   /* write month and century */
        if (res != 0)                                                                                        /* check result */
        {
            handle->debug_print("pcf8563: write century and month failed.\n");                               /* write century and month failed */
            
            return 1;                                                                                        /* return error */
        }
        res = a_pcf8563_iic_write(handle, PCF8563_REG_YEAR, a_pcf8563_hex2bcd((uint8_t)year));               /* write year */
        if (res != 0)                                                                                        /* check result */
        {
            handle->debug_print("pcf8563: write year failed.\n");                                            /* write year failed */
            
            return 1;                                                                                        /* return error */
        }
    }
    
    return 0;                                                                                                /* success return 0 */
}

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
uint8_t pcf8563_get_time(pcf8563_handle_t *handle, pcf8563_time_t *t)
{
    uint8_t res;
    uint8_t buf[7];
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    if (t == NULL)                                                                        /* check time */
    {
        handle->debug_print("pcf8563: time is null.\n");                                  /* time is null */
        
        return 2;                                                                         /* return error */
    }
    
    memset(buf, 0, sizeof(uint8_t) * 7);                                                  /* clear the buffer */
    res = a_pcf8563_iic_multiple_read(handle, PCF8563_REG_SECOND, (uint8_t *)buf, 7);     /* multiple_read */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("pcf8563: multiple read failed.\n");                          /* multiple read failed */
        
        return 1;                                                                         /* return error */
    }
    if (((buf[0] >> 7) & 0x01) != 0)                                                      /* check bit */
    {
        handle->debug_print("pcf8563: clock integrity is not guaranteed.\n");             /* clock integrity is not guaranteed */
        
        return 4;                                                                         /* return error */
    }
    t->year = a_pcf8563_bcd2hex(buf[6]) + 2000;                                           /* get year */
    if (((buf[5] >> 7) & 0x01) != 0)                                                      /* check century */
    {
        t->year += 100;                                                                   /* add 100 */
        buf[5] &= ~(1 << 7);                                                              /* clear bit */
    }
    t->month = a_pcf8563_bcd2hex(buf[5] & 0x1F);                                          /* get month */
    t->week = a_pcf8563_bcd2hex(buf[4] & 0x7);                                            /* get week */
    t->date = a_pcf8563_bcd2hex(buf[3] & 0x3F);                                           /* get date */
    t->hour = a_pcf8563_bcd2hex(buf[2] & 0x3F);                                           /* get hour */
    t->minute = a_pcf8563_bcd2hex(buf[1] & 0x7F);                                         /* get minute */
    t->second = a_pcf8563_bcd2hex(buf[0] & 0x7F);                                         /* get second */
    
    return 0;                                                                             /* success return 0 */
}

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
uint8_t pcf8563_set_test_mode(pcf8563_handle_t *handle, pcf8563_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    res = a_pcf8563_iic_multiple_read(handle, PCF8563_REG_CONTROL_STATUS1, &prev, 1);        /* read control status1 */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("pcf8563: read control status1 failed.\n");                      /* read control status1 failed */
        
        return 1;                                                                            /* return error */
    }
    prev &= ~(1 << 7);                                                                       /* clear config */
    prev |= enable << 7;                                                                     /* set bool */
    res = a_pcf8563_iic_write(handle, PCF8563_REG_CONTROL_STATUS1, prev);                    /* write control status1 */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("pcf8563: write control status1 failed.\n");                     /* write control status1 failed */
        
        return 1;                                                                            /* return error */
    }
    
    return 0;                                                                                /* success return 0 */
}

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
uint8_t pcf8563_get_test_mode(pcf8563_handle_t *handle, pcf8563_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    res = a_pcf8563_iic_multiple_read(handle, PCF8563_REG_CONTROL_STATUS1, &prev, 1);        /* read control status1 */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("pcf8563: read control status1 failed.\n");                      /* read control status1 failed */
        
        return 1;                                                                            /* return error */
    }
    *enable = (pcf8563_bool_t)(((prev >> 7) & 0x01));                                        /* get enable */
    
    return 0;                                                                                /* success return 0 */
}

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
uint8_t pcf8563_set_rtc_stop(pcf8563_handle_t *handle, pcf8563_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    res = a_pcf8563_iic_multiple_read(handle, PCF8563_REG_CONTROL_STATUS1, &prev, 1);        /* read control status1 */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("pcf8563: read control status1 failed.\n");                      /* read control status1 failed */
        
        return 1;                                                                            /* return error */
    }
    prev &= ~(1 << 5);                                                                       /* clear config */
    prev |= enable << 5;                                                                     /* set bool */
    res = a_pcf8563_iic_write(handle, PCF8563_REG_CONTROL_STATUS1, prev);                    /* write control status1 */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("pcf8563: write control status1 failed.\n");                     /* write control status1 failed */
        
        return 1;                                                                            /* return error */
    }
    
    return 0;                                                                                /* success return 0 */
}

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
uint8_t pcf8563_get_rtc_stop(pcf8563_handle_t *handle, pcf8563_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    res = a_pcf8563_iic_multiple_read(handle, PCF8563_REG_CONTROL_STATUS1, &prev, 1);        /* read control status1 */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("pcf8563: read control status1 failed.\n");                      /* read control status1 failed */
        
        return 1;                                                                            /* return error */
    }
    *enable = (pcf8563_bool_t)(((prev >> 5) & 0x01));                                        /* get enable */
    
    return 0;                                                                                /* success return 0 */
}

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
uint8_t pcf8563_set_power_on_reset(pcf8563_handle_t *handle, pcf8563_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    res = a_pcf8563_iic_multiple_read(handle, PCF8563_REG_CONTROL_STATUS1, &prev, 1);        /* read control status1 */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("pcf8563: read control status1 failed.\n");                      /* read control status1 failed */
        
        return 1;                                                                            /* return error */
    }
    prev &= ~(1 << 3);                                                                       /* clear config */
    prev |= enable << 3;                                                                     /* set bool */
    res = a_pcf8563_iic_write(handle, PCF8563_REG_CONTROL_STATUS1, prev);                    /* write control status1 */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("pcf8563: write control status1 failed.\n");                     /* write control status1 failed */
        
        return 1;                                                                            /* return error */
    }
    
    return 0;                                                                                /* success return 0 */
}

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
uint8_t pcf8563_get_power_on_reset(pcf8563_handle_t *handle, pcf8563_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    res = a_pcf8563_iic_multiple_read(handle, PCF8563_REG_CONTROL_STATUS1, &prev, 1);        /* read control status1 */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("pcf8563: read control status1 failed.\n");                      /* read control status1 failed */
        
        return 1;                                                                            /* return error */
    }
    *enable = (pcf8563_bool_t)(((prev >> 3) & 0x01));                                        /* get enable */
    
    return 0;                                                                                /* success return 0 */
}

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
uint8_t pcf8563_set_minute_alarm_enable(pcf8563_handle_t *handle, pcf8563_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_pcf8563_iic_multiple_read(handle, PCF8563_REG_MINUTE_ALARM, &prev, 1);        /* read minute alarm */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("pcf8563: read minute alarm failed.\n");                      /* read minute alarm failed */
        
        return 1;                                                                         /* return error */
    }
    prev &= ~(1 << 7);                                                                    /* clear config */
    prev |= (!enable) << 7;                                                               /* set bool */
    res = a_pcf8563_iic_write(handle, PCF8563_REG_MINUTE_ALARM, prev);                    /* write minute alarm */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("pcf8563: write minute alarm failed.\n");                     /* write minute alarm failed */
        
        return 1;                                                                         /* return error */
    }
    
    return 0;                                                                             /* success return 0 */
}

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
uint8_t pcf8563_get_minute_alarm_enable(pcf8563_handle_t *handle, pcf8563_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_pcf8563_iic_multiple_read(handle, PCF8563_REG_MINUTE_ALARM, &prev, 1);        /* read minute alarm */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("pcf8563: read minute alarm failed.\n");                      /* read minute alarm failed */
        
        return 1;                                                                         /* return error */
    }
    *enable = (pcf8563_bool_t)(!((prev >> 7) & 0x01));                                    /* get bool */
    
    return 0;                                                                             /* success return 0 */
}

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
uint8_t pcf8563_set_minute_alarm(pcf8563_handle_t *handle, uint8_t minute)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    if (minute > 59)                                                                      /* check minute */
    {
        handle->debug_print("pcf8563: minute can't be over than 59.\n");                  /* minute can't be over than 59 */
        
        return 4;                                                                         /* return error */
    }
    
    res = a_pcf8563_iic_multiple_read(handle, PCF8563_REG_MINUTE_ALARM, &prev, 1);        /* read minute alarm */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("pcf8563: read minute alarm failed.\n");                      /* read minute alarm failed */
        
        return 1;                                                                         /* return error */
    }
    prev &= ~(0x7F << 0);                                                                 /* clear config */
    prev |= a_pcf8563_hex2bcd(minute);                                                    /* set config */
    res = a_pcf8563_iic_write(handle, PCF8563_REG_MINUTE_ALARM, prev);                    /* write minute alarm */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("pcf8563: write minute alarm failed.\n");                     /* write minute alarm failed */
        
        return 1;                                                                         /* return error */
    }
    
    return 0;                                                                             /* success return 0 */
}

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
uint8_t pcf8563_get_minute_alarm(pcf8563_handle_t *handle, uint8_t *minute)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_pcf8563_iic_multiple_read(handle, PCF8563_REG_MINUTE_ALARM, &prev, 1);        /* read minute alarm */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("pcf8563: read minute alarm failed.\n");                      /* read minute alarm failed */
        
        return 1;                                                                         /* return error */
    }
    *minute = a_pcf8563_bcd2hex(prev & 0x7F);                                             /* convert */
    
    return 0;                                                                             /* success return 0 */
}

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
uint8_t pcf8563_set_hour_alarm_enable(pcf8563_handle_t *handle, pcf8563_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_pcf8563_iic_multiple_read(handle, PCF8563_REG_HOUR_ALARM, &prev, 1);        /* read hour alarm */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("pcf8563: read hour alarm failed.\n");                      /* read hour alarm failed */
        
        return 1;                                                                       /* return error */
    }
    prev &= ~(1 << 7);                                                                  /* clear config */
    prev |= (!enable) << 7;                                                             /* set bool */
    res = a_pcf8563_iic_write(handle, PCF8563_REG_HOUR_ALARM, prev);                    /* write hour alarm */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("pcf8563: write hour alarm failed.\n");                     /* write hour alarm failed */
        
        return 1;                                                                       /* return error */
    }
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t pcf8563_get_hour_alarm_enable(pcf8563_handle_t *handle, pcf8563_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_pcf8563_iic_multiple_read(handle, PCF8563_REG_HOUR_ALARM, &prev, 1);        /* read hour alarm */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("pcf8563: read hour alarm failed.\n");                      /* read hour alarm failed */
        
        return 1;                                                                       /* return error */
    }
    *enable = (pcf8563_bool_t)(!((prev >> 7) & 0x01));                                  /* get bool */
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t pcf8563_set_hour_alarm(pcf8563_handle_t *handle, uint8_t hour)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    if (hour > 23)                                                                      /* check hour */
    {
        handle->debug_print("pcf8563: hour can't be over than 23.\n");                  /* hour can't be over than 23 */
        
        return 4;                                                                       /* return error */
    }
    
    res = a_pcf8563_iic_multiple_read(handle, PCF8563_REG_HOUR_ALARM, &prev, 1);        /* read hour alarm */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("pcf8563: read hour alarm failed.\n");                      /* read hour alarm failed */
        
        return 1;                                                                       /* return error */
    }
    prev &= ~(0x3F << 0);                                                               /* clear config */
    prev |= a_pcf8563_hex2bcd(hour);                                                    /* set config */
    res = a_pcf8563_iic_write(handle, PCF8563_REG_HOUR_ALARM, prev);                    /* write hour alarm */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("pcf8563: write hour alarm failed.\n");                     /* write hour alarm failed */
        
        return 1;                                                                       /* return error */
    }
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t pcf8563_get_hour_alarm(pcf8563_handle_t *handle, uint8_t *hour)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_pcf8563_iic_multiple_read(handle, PCF8563_REG_HOUR_ALARM, &prev, 1);        /* read hour alarm */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("pcf8563: read hour alarm failed.\n");                      /* read hour alarm failed */
        
        return 1;                                                                       /* return error */
    }
    *hour = a_pcf8563_bcd2hex(prev & 0x3F);                                             /* convert */
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t pcf8563_set_day_alarm_enable(pcf8563_handle_t *handle, pcf8563_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }
    
    res = a_pcf8563_iic_multiple_read(handle, PCF8563_REG_DAY_ALARM, &prev, 1);        /* read day alarm */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("pcf8563: read day alarm failed.\n");                      /* read day alarm failed */
        
        return 1;                                                                      /* return error */
    }
    prev &= ~(1 << 7);                                                                 /* clear config */
    prev |= (!enable) << 7;                                                            /* set bool */
    res = a_pcf8563_iic_write(handle, PCF8563_REG_DAY_ALARM, prev);                    /* write day alarm */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("pcf8563: write day alarm failed.\n");                     /* write day alarm failed */
        
        return 1;                                                                      /* return error */
    }
    
    return 0;                                                                          /* success return 0 */
}

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
uint8_t pcf8563_get_day_alarm_enable(pcf8563_handle_t *handle, pcf8563_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }
    
    res = a_pcf8563_iic_multiple_read(handle, PCF8563_REG_DAY_ALARM, &prev, 1);        /* read day alarm */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("pcf8563: read day alarm failed.\n");                      /* read day alarm failed */
        
        return 1;                                                                      /* return error */
    }
    *enable = (pcf8563_bool_t)(!((prev >> 7) & 0x01));                                 /* get bool */
    
    return 0;                                                                          /* success return 0 */
}

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
uint8_t pcf8563_set_day_alarm(pcf8563_handle_t *handle, uint8_t day)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }
    if ((day == 0) || (day > 31))                                                      /* check day */
    {
        handle->debug_print("pcf8563: day can't be zero or over than 31.\n");          /* day can't be zero or over than 31 */
        
        return 4;                                                                      /* return error */
    }
    
    res = a_pcf8563_iic_multiple_read(handle, PCF8563_REG_DAY_ALARM, &prev, 1);        /* read day alarm */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("pcf8563: read day alarm failed.\n");                      /* read day alarm failed */
        
        return 1;                                                                      /* return error */
    }
    prev &= ~(0x3F << 0);                                                              /* clear config */
    prev |= a_pcf8563_hex2bcd(day);                                                    /* set config */
    res = a_pcf8563_iic_write(handle, PCF8563_REG_DAY_ALARM, prev);                    /* write day alarm */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("pcf8563: write day alarm failed.\n");                     /* write day alarm failed */
        
        return 1;                                                                      /* return error */
    }
    
    return 0;                                                                          /* success return 0 */
}

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
uint8_t pcf8563_get_day_alarm(pcf8563_handle_t *handle, uint8_t *day)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }
    
    res = a_pcf8563_iic_multiple_read(handle, PCF8563_REG_DAY_ALARM, &prev, 1);        /* read day alarm */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("pcf8563: read day alarm failed.\n");                      /* read day alarm failed */
        
        return 1;                                                                      /* return error */
    }
    *day = a_pcf8563_bcd2hex(prev & 0x3F);                                             /* convert */
    
    return 0;                                                                          /* success return 0 */
}

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
uint8_t pcf8563_set_week_alarm_enable(pcf8563_handle_t *handle, pcf8563_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }
    
    res = a_pcf8563_iic_multiple_read(handle, PCF8563_REG_WEEK_ALARM, &prev, 1);       /* read week alarm */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("pcf8563: read week alarm failed.\n");                     /* read week alarm failed */
        
        return 1;                                                                      /* return error */
    }
    prev &= ~(1 << 7);                                                                 /* clear config */
    prev |= (!enable) << 7;                                                            /* set bool */
    res = a_pcf8563_iic_write(handle, PCF8563_REG_WEEK_ALARM, prev);                   /* write week alarm */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("pcf8563: write week alarm failed.\n");                    /* write week alarm failed */
        
        return 1;                                                                      /* return error */
    }
    
    return 0;                                                                          /* success return 0 */
}

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
uint8_t pcf8563_get_week_alarm_enable(pcf8563_handle_t *handle, pcf8563_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }
    
    res = a_pcf8563_iic_multiple_read(handle, PCF8563_REG_WEEK_ALARM, &prev, 1);       /* read week alarm */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("pcf8563: read week alarm failed.\n");                     /* read week alarm failed */
        
        return 1;                                                                      /* return error */
    }
    *enable = (pcf8563_bool_t)(!((prev >> 7) & 0x01));                                 /* get bool */
    
    return 0;                                                                          /* success return 0 */
}

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
uint8_t pcf8563_set_week_alarm(pcf8563_handle_t *handle, uint8_t week)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    if (week > 6)                                                                       /* check week */
    {
        handle->debug_print("pcf8563: week can't be over than 6.\n");                   /* week can't be over than 6 */
        
        return 4;                                                                       /* return error */
    }
    
    res = a_pcf8563_iic_multiple_read(handle, PCF8563_REG_WEEK_ALARM, &prev, 1);        /* read week alarm */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("pcf8563: read week alarm failed.\n");                      /* read week alarm failed */
        
        return 1;                                                                       /* return error */
    }
    prev &= ~(0x7 << 0);                                                                /* clear config */
    prev |= a_pcf8563_hex2bcd(week);                                                    /* set config */
    res = a_pcf8563_iic_write(handle, PCF8563_REG_WEEK_ALARM, prev);                    /* write week alarm */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("pcf8563: write week alarm failed.\n");                     /* write week alarm failed */
        
        return 1;                                                                       /* return error */
    }
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t pcf8563_get_week_alarm(pcf8563_handle_t *handle, uint8_t *week)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_pcf8563_iic_multiple_read(handle, PCF8563_REG_WEEK_ALARM, &prev, 1);        /* read week alarm */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("pcf8563: read week alarm failed.\n");                      /* read week alarm failed */
        
        return 1;                                                                       /* return error */
    }
    *week = a_pcf8563_bcd2hex(prev & 0x07);                                             /* convert */
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t pcf8563_set_clock_out_enable(pcf8563_handle_t *handle, pcf8563_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                    /* check handle */
    {
        return 2;                                                                          /* return error */
    }
    if (handle->inited != 1)                                                               /* check handle initialization */
    {
        return 3;                                                                          /* return error */
    }
    
    res = a_pcf8563_iic_multiple_read(handle, PCF8563_REG_CLKOUT_CONTROL, &prev, 1);       /* read clkout control */
    if (res != 0)                                                                          /* check result */
    {
        handle->debug_print("pcf8563: read clkout control failed.\n");                     /* read clkout control failed */
        
        return 1;                                                                          /* return error */
    }
    prev &= ~(1 << 7);                                                                     /* clear config */
    prev |= enable << 7;                                                                   /* set bool */
    res = a_pcf8563_iic_write(handle, PCF8563_REG_CLKOUT_CONTROL, prev);                   /* write clkout control */
    if (res != 0)                                                                          /* check result */
    {
        handle->debug_print("pcf8563: write clkout control failed.\n");                    /* write clkout control failed */
        
        return 1;                                                                          /* return error */
    }
    
    return 0;                                                                              /* success return 0 */
}

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
uint8_t pcf8563_get_clock_out_enable(pcf8563_handle_t *handle, pcf8563_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                    /* check handle */
    {
        return 2;                                                                          /* return error */
    }
    if (handle->inited != 1)                                                               /* check handle initialization */
    {
        return 3;                                                                          /* return error */
    }
    
    res = a_pcf8563_iic_multiple_read(handle, PCF8563_REG_CLKOUT_CONTROL, &prev, 1);       /* read clkout control */
    if (res != 0)                                                                          /* check result */
    {
        handle->debug_print("pcf8563: read clkout control failed.\n");                     /* read clkout control failed */
        
        return 1;                                                                          /* return error */
    }
    *enable = (pcf8563_bool_t)((prev >> 7) & 0x01);                                        /* get bool */
    
    return 0;                                                                              /* success return 0 */
}

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
uint8_t pcf8563_set_clock_out(pcf8563_handle_t *handle, pcf8563_clock_out_t clk)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                    /* check handle */
    {
        return 2;                                                                          /* return error */
    }
    if (handle->inited != 1)                                                               /* check handle initialization */
    {
        return 3;                                                                          /* return error */
    }
    
    res = a_pcf8563_iic_multiple_read(handle, PCF8563_REG_CLKOUT_CONTROL, &prev, 1);       /* read clkout control */
    if (res != 0)                                                                          /* check result */
    {
        handle->debug_print("pcf8563: read clkout control failed.\n");                     /* read clkout control failed */
        
        return 1;                                                                          /* return error */
    }
    prev &= ~(3 << 0);                                                                     /* clear config */
    prev |= clk << 0;                                                                      /* set config */
    res = a_pcf8563_iic_write(handle, PCF8563_REG_CLKOUT_CONTROL, prev);                   /* write clkout control */
    if (res != 0)                                                                          /* check result */
    {
        handle->debug_print("pcf8563: write clkout control failed.\n");                    /* write clkout control failed */
        
        return 1;                                                                          /* return error */
    }
    
    return 0;                                                                              /* success return 0 */
}

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
uint8_t pcf8563_get_clock_out(pcf8563_handle_t *handle, pcf8563_clock_out_t *clk)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                    /* check handle */
    {
        return 2;                                                                          /* return error */
    }
    if (handle->inited != 1)                                                               /* check handle initialization */
    {
        return 3;                                                                          /* return error */
    }
    
    res = a_pcf8563_iic_multiple_read(handle, PCF8563_REG_CLKOUT_CONTROL, &prev, 1);       /* read clkout control */
    if (res != 0)                                                                          /* check result */
    {
        handle->debug_print("pcf8563: read clkout control failed.\n");                     /* read clkout control failed */
        
        return 1;                                                                          /* return error */
    }
    *clk = (pcf8563_clock_out_t)(prev & 0x03);                                             /* get clock */
    
    return 0;                                                                              /* success return 0 */
}

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
uint8_t pcf8563_set_timer_enable(pcf8563_handle_t *handle, pcf8563_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_pcf8563_iic_multiple_read(handle, PCF8563_REG_TIMER_CONTROL, &prev, 1);       /* read timer control */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("pcf8563: read timer control failed.\n");                     /* read timer control failed */
        
        return 1;                                                                         /* return error */
    }
    prev &= ~(1 << 7);                                                                    /* clear config */
    prev |= enable << 7;                                                                  /* set bool */
    res = a_pcf8563_iic_write(handle, PCF8563_REG_TIMER_CONTROL, prev);                   /* write timer control */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("pcf8563: write timer control failed.\n");                    /* write timer control failed */
        
        return 1;                                                                         /* return error */
    }
    
    return 0;                                                                             /* success return 0 */
}

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
uint8_t pcf8563_get_timer_enable(pcf8563_handle_t *handle, pcf8563_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_pcf8563_iic_multiple_read(handle, PCF8563_REG_TIMER_CONTROL, &prev, 1);       /* read timer control */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("pcf8563: read timer control failed.\n");                     /* read timer control failed */
        
        return 1;                                                                         /* return error */
    }
    *enable = (pcf8563_bool_t)((prev >> 7) & 0x01);                                       /* get bool */
    
    return 0;                                                                             /* success return 0 */
}

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
uint8_t pcf8563_set_timer_freq(pcf8563_handle_t *handle, pcf8563_timer_freq_t freq)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_pcf8563_iic_multiple_read(handle, PCF8563_REG_TIMER_CONTROL, &prev, 1);       /* read timer control */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("pcf8563: read timer control failed.\n");                     /* read timer control failed */
        
        return 1;                                                                         /* return error */
    }
    prev &= ~(3 << 0);                                                                    /* clear config */
    prev |= freq << 0;                                                                    /* set freq */
    res = a_pcf8563_iic_write(handle, PCF8563_REG_TIMER_CONTROL, prev);                   /* write timer control */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("pcf8563: write timer control failed.\n");                    /* write timer control failed */
        
        return 1;                                                                         /* return error */
    }
    
    return 0;                                                                             /* success return 0 */
}

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
uint8_t pcf8563_get_timer_freq(pcf8563_handle_t *handle, pcf8563_timer_freq_t *freq)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_pcf8563_iic_multiple_read(handle, PCF8563_REG_TIMER_CONTROL, &prev, 1);       /* read timer control */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("pcf8563: read timer control failed.\n");                     /* read timer control failed */
        
        return 1;                                                                         /* return error */
    }
    *freq = (pcf8563_timer_freq_t)(prev & 0x03);                                          /* get freq */
    
    return 0;                                                                             /* success return 0 */
}

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
uint8_t pcf8563_set_timer_value(pcf8563_handle_t *handle, uint8_t value)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                /* check handle */
    {
        return 2;                                                      /* return error */
    }
    if (handle->inited != 1)                                           /* check handle initialization */
    {
        return 3;                                                      /* return error */
    }
    
    prev = value;                                                      /* set value */
    res = a_pcf8563_iic_write(handle, PCF8563_REG_TIMER, prev);        /* write timer */
    if (res != 0)                                                      /* check result */
    {
        handle->debug_print("pcf8563: write timer failed.\n");         /* write timer failed */
        
        return 1;                                                      /* return error */
    }
    
    return 0;                                                          /* success return 0 */
}

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
uint8_t pcf8563_get_timer_value(pcf8563_handle_t *handle, uint8_t *value)
{
    uint8_t res;
    
    if (handle == NULL)                                                           /* check handle */
    {
        return 2;                                                                 /* return error */
    }
    if (handle->inited != 1)                                                      /* check handle initialization */
    {
        return 3;                                                                 /* return error */
    }
    
    res = a_pcf8563_iic_multiple_read(handle, PCF8563_REG_TIMER, value, 1);       /* read timer */
    if (res != 0)                                                                 /* check result */
    {
        handle->debug_print("pcf8563: read timer failed.\n");                     /* read timer failed */
        
        return 1;                                                                 /* return error */
    }
    
    return 0;                                                                     /* success return 0 */
}

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
uint8_t pcf8563_set_timer_interrupt(pcf8563_handle_t *handle, pcf8563_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    res = a_pcf8563_iic_multiple_read(handle, PCF8563_REG_CONTROL_STATUS2, &prev, 1);        /* read control status2 */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("pcf8563: read control status2 failed.\n");                      /* read control status2 failed */
        
        return 1;                                                                            /* return error */
    }
    prev &= ~(1 << 0);                                                                       /* clear config */
    prev |= enable << 0;                                                                     /* set bool */
    res = a_pcf8563_iic_write(handle, PCF8563_REG_CONTROL_STATUS2, prev);                    /* write control status2 */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("pcf8563: write control status2 failed.\n");                     /* write control status2 failed */
        
        return 1;                                                                            /* return error */
    }
    
    return 0;                                                                                /* success return 0 */
}

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
uint8_t pcf8563_get_timer_interrupt(pcf8563_handle_t *handle, pcf8563_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    res = a_pcf8563_iic_multiple_read(handle, PCF8563_REG_CONTROL_STATUS2, &prev, 1);        /* read control status2 */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("pcf8563: read control status2 failed.\n");                      /* read control status2 failed */
        
        return 1;                                                                            /* return error */
    }
    *enable = (pcf8563_bool_t)((prev >> 0) & 0x01);                                          /* get bool */
    
    return 0;                                                                                /* success return 0 */
}

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
uint8_t pcf8563_set_alarm_interrupt(pcf8563_handle_t *handle, pcf8563_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    res = a_pcf8563_iic_multiple_read(handle, PCF8563_REG_CONTROL_STATUS2, &prev, 1);        /* read control status2 */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("pcf8563: read control status2 failed.\n");                      /* read control status2 failed */
        
        return 1;                                                                            /* return error */
    }
    prev &= ~(1 << 1);                                                                       /* clear config */
    prev |= enable << 1;                                                                     /* set bool */
    res = a_pcf8563_iic_write(handle, PCF8563_REG_CONTROL_STATUS2, prev);                    /* write control status2 */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("pcf8563: write control status2 failed.\n");                     /* write control status2 failed */
        
        return 1;                                                                            /* return error */
    }
    
    return 0;                                                                                /* success return 0 */
}

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
uint8_t pcf8563_get_alarm_interrupt(pcf8563_handle_t *handle, pcf8563_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    res = a_pcf8563_iic_multiple_read(handle, PCF8563_REG_CONTROL_STATUS2, &prev, 1);        /* read control status2 */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("pcf8563: read control status2 failed.\n");                      /* read control status2 failed */
        
        return 1;                                                                            /* return error */
    }
    *enable = (pcf8563_bool_t)((prev >> 1) & 0x01);                                          /* get bool */
    
    return 0;                                                                                /* success return 0 */
}

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
uint8_t pcf8563_set_interrupt_mode(pcf8563_handle_t *handle, pcf8563_interrupt_mode_t mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    res = a_pcf8563_iic_multiple_read(handle, PCF8563_REG_CONTROL_STATUS2, &prev, 1);        /* read control status2 */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("pcf8563: read control status2 failed.\n");                      /* read control status2 failed */
        
        return 1;                                                                            /* return error */
    }
    prev &= ~(1 << 4);                                                                       /* clear config */
    prev |= mode << 4;                                                                       /* set mode */
    res = a_pcf8563_iic_write(handle, PCF8563_REG_CONTROL_STATUS2, prev);                    /* write control status2 */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("pcf8563: write control status2 failed.\n");                     /* write control status2 failed */
        
        return 1;                                                                            /* return error */
    }
    
    return 0;                                                                                /* success return 0 */
}

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
uint8_t pcf8563_get_interrupt_mode(pcf8563_handle_t *handle, pcf8563_interrupt_mode_t *mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    res = a_pcf8563_iic_multiple_read(handle, PCF8563_REG_CONTROL_STATUS2, &prev, 1);        /* read control status2 */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("pcf8563: read control status2 failed.\n");                      /* read control status2 failed */
        
        return 1;                                                                            /* return error */
    }
    *mode = (pcf8563_interrupt_mode_t)((prev >> 4) & 0x1);                                   /* get mode */
    
    return 0;                                                                                /* success return 0 */
}

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
uint8_t pcf8563_irq_handler(pcf8563_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    uint8_t prev2;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    res = a_pcf8563_iic_multiple_read(handle, PCF8563_REG_CONTROL_STATUS2, &prev, 1);        /* read control status2 */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("pcf8563: read control status2 failed.\n");                      /* read control status2 failed */
        
        return 1;                                                                            /* return error */
    }
    prev2 = prev;                                                                            /* save config */
    prev2 &= ~(3 << 2);                                                                      /* clear settings */
    if (((prev >> 2) & 0x1) != 0)                                                            /* check timer */
    {
        if (handle->receive_callback != NULL)                                                /* if not null */
        {
            handle->receive_callback(PCF8563_INTERRUPT_EVENT_TIMER);                         /* run the callback */
        }
    }
    if (((prev >> 3) & 0x01) != 0)                                                           /* check alarm */
    {
        if (handle->receive_callback != NULL)                                                /* if not null */
        {
            handle->receive_callback(PCF8563_INTERRUPT_EVENT_ALARM);                         /* run the callback */
        }
    }
    res = a_pcf8563_iic_write(handle, PCF8563_REG_CONTROL_STATUS2, prev2);                   /* write control status2 */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("pcf8563: write control status2 failed.\n");                     /* write control status2 failed */
        
        return 1;                                                                            /* return error */
    }
    
    return 0;                                                                                /* success return 0 */
}

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
uint8_t pcf8563_clear_status(pcf8563_handle_t *handle, pcf8563_interrupt_event_t event)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    res = a_pcf8563_iic_multiple_read(handle, PCF8563_REG_CONTROL_STATUS2, &prev, 1);        /* read control status2 */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("pcf8563: read control status2 failed.\n");                      /* read control status2 failed */
        
        return 1;                                                                            /* return error */
    }
    if (event == PCF8563_INTERRUPT_EVENT_TIMER)                                              /* timer */
    {
        prev &= ~(1 << 2);                                                                   /* clear bit */
    }
    else                                                                                     /* alarm */
    {
        prev &= ~(1 << 3);                                                                   /* clear bit */
    }
    res = a_pcf8563_iic_write(handle, PCF8563_REG_CONTROL_STATUS2, prev);                    /* write control status2 */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("pcf8563: write control status2 failed.\n");                     /* write control status2 failed */
        
        return 1;                                                                            /* return error */
    }
    
    return 0;                                                                                /* success return 0 */
}

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
uint8_t pcf8563_init(pcf8563_handle_t *handle)
{
    if (handle == NULL)                                              /* check handle */
    {
        return 2;                                                    /* return error */
    }
    if (handle->debug_print == NULL)                                 /* check debug_print */
    {
        return 3;                                                    /* return error */
    }
    if (handle->iic_init == NULL)                                    /* check iic_init */
    {
        handle->debug_print("pcf8563: iic_init is null.\n");         /* iic_init is null */
       
        return 3;                                                    /* return error */
    }
    if (handle->iic_deinit == NULL)                                  /* check iic_deinit */
    {
        handle->debug_print("pcf8563: iic_deinit is null.\n");       /* iic_deinit is null */
       
        return 3;                                                    /* return error */
    }
    if (handle->iic_write == NULL)                                   /* check iic_write */
    {
        handle->debug_print("pcf8563: iic_write is null.\n");        /* iic_write is null */
       
        return 3;                                                    /* return error */
    }
    if (handle->iic_read == NULL)                                    /* check iic_read */
    {
        handle->debug_print("pcf8563: iic_read is null.\n");         /* iic_read is null */
       
        return 3;                                                    /* return error */
    }
    if (handle->delay_ms == NULL)                                    /* check delay_ms */
    {
        handle->debug_print("pcf8563: delay_ms is null.\n");         /* delay_ms is null */
       
        return 3;                                                    /* return error */
    }
    if (handle->receive_callback == NULL)                            /* check receive_callback */
    {
        handle->debug_print("pcf8563: receive_callback is null.\n"); /* receive_callback is null */
       
        return 3;                                                    /* return error */
    }
    
    if (handle->iic_init() != 0)                                     /* iic init */
    {
        handle->debug_print("pcf8563: iic init failed.\n");          /* iic init failed */
       
        return 1;                                                    /* return error */
    }
    handle->inited = 1;                                              /* flag finish initialization */
    
    return 0;                                                        /* success return 0 */
}

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
uint8_t pcf8563_deinit(pcf8563_handle_t *handle)
{
    if (handle == NULL)                                             /* check handle */
    {
        return 2;                                                   /* return error */
    }
    if (handle->inited != 1)                                        /* check handle initialization */
    {
        return 3;                                                   /* return error */
    }
    
    if (handle->iic_deinit() != 0)                                  /* iic deinit */
    {
        handle->debug_print("pcf8563: iic deinit failed.\n");       /* iic deinit failed */
       
        return 1;                                                   /* return error */
    }
    handle->inited = 0;                                             /* flag close */
    
    return 0;                                                       /* success return 0 */
}

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
uint8_t pcf8563_set_reg(pcf8563_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                               /* check handle */
    {
        return 2;                                                     /* return error */
    }
    if (handle->inited != 1)                                          /* check handle initialization */
    {
        return 3;                                                     /* return error */
    }
    
    if (handle->iic_write(PCF8563_ADDRESS, reg, buf, len) != 0)       /* write data */
    {
        return 1;                                                     /* return error */
    }
    
    return 0;                                                         /* success return 0 */
}

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
uint8_t pcf8563_get_reg(pcf8563_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                              /* check handle */
    {
        return 2;                                                    /* return error */
    }
    if (handle->inited != 1)                                         /* check handle initialization */
    {
        return 3;                                                    /* return error */
    }
    
    if (handle->iic_read(PCF8563_ADDRESS, reg, buf, len) != 0)       /* read data */
    {
        return 1;                                                    /* return error */
    }
    
    return 0;                                                        /* success return 0 */
}

/**
 * @brief      get chip's information
 * @param[out] *info points to a pcf8563 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t pcf8563_info(pcf8563_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(pcf8563_info_t));                        /* initialize pcf8563 info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "IIC", 8);                             /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */
    
    return 0;                                                       /* success return 0 */
}
