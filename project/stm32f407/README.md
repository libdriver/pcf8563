### 1. Chip

#### 1.1 Chip Info

Chip Name: STM32F407ZGT6.

Extern Oscillator: 8MHz.

UART Pin: TX/RX PA9/PA10.

IIC Pin: SCL/SDA PB8/PB9.

### 2. Development and Debugging

#### 2.1 Integrated Development Environment

LibDriver provides both Keil and IAR integrated development environment projects.

MDK is the Keil ARM project and your Keil version must be 5 or higher.Keil ARM project needs STMicroelectronics STM32F4 Series Device Family Pack and you can download from https://www.keil.com/dd2/stmicroelectronics/stm32f407zgtx.

EW is the IAR ARM project and your IAR version must be 9 or higher.

#### 2.2 Serial Port Parameter

Baud Rate: 115200.

Data Bits : 8.

Stop Bits: 1.

Parity: None.

Flow Control: None.

#### 2.3 Serial Port Assistant

We use '\n' to wrap lines.If your serial port assistant displays exceptions (e.g. the displayed content does not divide lines), please modify the configuration of your serial port assistant or replace one that supports '\n' parsing.

### 3. PCF8563

#### 3.1 Command Instruction

1. Show pcf8563 chip and driver information.

   ```shell
   pcf8563 (-i | --information)
   ```

2. Show pcf8563 help.

   ```shell
   pcf8563 (-h | --help)
   ```

3. Show pcf8563 pin connections of the current board.

   ```shell
   pcf8563 (-p | --port)
   ```

4. Run pcf8563 register test.

   ```shell
   pcf8563 (-t reg | --test=reg)
   ```

5. Run pcf8563 alarm test.

   ```shell
   pcf8563 (-t alarm | --test=alarm)
   ```
6. Run pcf8563 timer test.

   ```shell
   pcf8563 (-t timer | --test=timer)
   ```
   
7. Run pcf8563 output test, num means test times.

   ```shell
   pcf8563 (-t output | --test=output) [--times=<num>]
   ```

8. Run pcf8563 basic set time function, time is the unix timestamp.

   ```shell
   pcf8563 (-e basic-set-time | --example=basic-set-time) --timestamp=<time>
   ```

9. Run pcf8563 basic get time function.

   ```shell
   pcf8563 (-e basic-get-time | --example=basic-get-time)
   ```

10. Run pcf8563 alarm set time function, time is the unix timestamp.

       ```shell
       pcf8563 (-e alarm-set-time | --example=alarm-set-time) --timestamp=<time>
       ```

11. Run pcf8563 alarm get time function.

       ```shell
       pcf8563 (-e alarm-get-time | --example=alarm-get-time)
       ```

12. Run pcf8563 alarm function, minute is the matched minute.

       ```shell
       pcf8563 (-e alarm-run | --example=alarm-run) --minute=<minute>
       ```

13. Run pcf8563 timer set time function, time is the unix timestamp.

       ```shell
       pcf8563 (-e timer-set-time | --example=timer-set-time) --timestamp=<time>
       ```
14. Run pcf8563 timer get time function.

       ```shell
       pcf8563 (-e timer-get-time | --example=timer-get-time)
       ```

15. Run pcf8563 timer function, second is the matched second.

       ```shell
       pcf8563 (-e timer-run | --example=timer-run) --second=<second>
       ```

16. Run pcf8563 output set time function, time is the unix timestamp.

       ```shell
       pcf8563 (-e output-set-time | --example=output-set-time) --timestamp=<time>
       ```

17. Run pcf8563 output get time function.

      ```shell
      pcf8563 (-e output-get-time | --example=output-get-time)
      ```
18. Run pcf8563 output enable function.

      ```shell
      pcf8563 (-e output-enable | --example=output-enable) --freq=<1HZ | 32HZ | 1024HZ | 32768HZ>
      ```
19. Run pcf8563 output disable function.

      ```shell
      pcf8563 (-e output-disable | --example=output-disable)
      ```
    
#### 3.2 Command Example

```shell
pcf8563 -i

pcf8563: chip is NXP PCF8563.
pcf8563: manufacturer is NXP.
pcf8563: interface is IIC.
pcf8563: driver version is 1.0.
pcf8563: min supply voltage is 1.9V.
pcf8563: max supply voltage is 5.5V.
pcf8563: max current is 0.80mA.
pcf8563: max temperature is 85.0C.
pcf8563: min temperature is -40.0C.
```

```shell
pcf8563 -p

pcf8563: SCL connected to GPIOB PIN8.
pcf8563: SDA connected to GPIOB PIN9.
```

```shell
pcf8563 -t reg

pcf8563: chip is NXP PCF8563.
pcf8563: manufacturer is NXP.
pcf8563: interface is IIC.
pcf8563: driver version is 1.0.
pcf8563: min supply voltage is 1.9V.
pcf8563: max supply voltage is 5.5V.
pcf8563: max current is 0.80mA.
pcf8563: max temperature is 85.0C.
pcf8563: min temperature is -40.0C.
pcf8563: start register test.
pcf8563: pcf8563_set_rtc_stop/pcf8563_get_rtc_stop test.
pcf8563: enable rtc stop.
pcf8563: check rtc stop ok.
pcf8563: disable rtc stop.
pcf8563: check rtc stop ok.
pcf8563: pcf8563_set_test_mode/pcf8563_get_test_mode test.
pcf8563: enable test mode.
pcf8563: check test mode ok.
pcf8563: disable test mode.
pcf8563: check test mode ok.
pcf8563: pcf8563_set_power_on_reset/pcf8563_get_power_on_reset test.
pcf8563: enable power on reset.
pcf8563: check power on reset ok.
pcf8563: disable power on reset.
pcf8563: check power on reset ok.
pcf8563: pcf8563_set_time/pcf8563_get_time test.
pcf8563: set time 2068-04-10 01:41:00 6.
pcf8563: check time ok.
pcf8563: pcf8563_set_minute_alarm_enable/pcf8563_get_minute_alarm_enable test.
pcf8563: enable minute alarm.
pcf8563: check minute alarm enable ok.
pcf8563: disable minute alarm.
pcf8563: check minute alarm enable ok.
pcf8563: pcf8563_set_minute_alarm/pcf8563_get_minute_alarm test.
pcf8563: set minute alarm 28.
pcf8563: check minute alarm ok.
pcf8563: pcf8563_set_hour_alarm_enable/pcf8563_get_hour_alarm_enable test.
pcf8563: enable hour alarm.
pcf8563: check hour alarm enable ok.
pcf8563: disable hour alarm.
pcf8563: check hour alarm enable ok.
pcf8563: pcf8563_set_hour_alarm/pcf8563_get_hour_alarm test.
pcf8563: set hour alarm 2.
pcf8563: check hour alarm ok.
pcf8563: pcf8563_set_day_alarm_enable/pcf8563_get_day_alarm_enable test.
pcf8563: enable day alarm.
pcf8563: check day alarm ok.
pcf8563: disable day alarm.
pcf8563: check day alarm ok.
pcf8563: pcf8563_set_day_alarm/pcf8563_get_day_alarm test.
pcf8563: set day alarm 5.
pcf8563: check day alarm ok.
pcf8563: pcf8563_set_week_alarm_enable/pcf8563_get_week_alarm_enable test.
pcf8563: enable week alarm.
pcf8563: check week alarm ok.
pcf8563: disable week alarm.
pcf8563: check week alarm ok.
pcf8563: pcf8563_set_week_alarm/pcf8563_get_week_alarm test.
pcf8563: set week alarm 3.
pcf8563: check week alarm ok.
pcf8563: pcf8563_set_clock_out_enable/pcf8563_get_clock_out_enable test.
pcf8563: enable clock out.
pcf8563: check clock out enable ok.
pcf8563: disable clock out.
pcf8563: check clock out enable ok.
pcf8563: pcf8563_set_clock_out/pcf8563_get_clock_out test.
pcf8563: set clock out 32.768khz.
pcf8563: check clock out ok.
pcf8563: set clock out 1.024khz.
pcf8563: check clock out ok.
pcf8563: set clock out 32hz.
pcf8563: check clock out ok.
pcf8563: set clock out 1hz.
pcf8563: check clock out ok.
pcf8563: pcf8563_set_timer_enable/pcf8563_get_timer_enable test.
pcf8563: enable timer.
pcf8563: check timer enable ok.
pcf8563: disable timer.
pcf8563: check timer enable ok.
pcf8563: pcf8563_set_timer_freq/pcf8563_get_timer_freq test.
pcf8563: set timer freq 4.096khz.
pcf8563: check timer freq ok.
pcf8563: set timer freq 64hz.
pcf8563: check timer freq ok.
pcf8563: set timer freq 1hz.
pcf8563: check timer freq ok.
pcf8563: set timer freq 1/60hz.
pcf8563: check timer freq ok.
pcf8563: pcf8563_set_timer_value/pcf8563_get_timer_value test.
pcf8563: set timer value 43.
pcf8563: check timer value ok.
pcf8563: pcf8563_set_timer_interrupt/pcf8563_get_timer_interrupt test.
pcf8563: enable timer interrupt.
pcf8563: check timer interrupt ok.
pcf8563: disable timer interrupt.
pcf8563: check timer interrupt ok.
pcf8563: pcf8563_set_alarm_interrupt/pcf8563_get_alarm_interrupt test.
pcf8563: enable alarm interrupt.
pcf8563: check alarm interrupt ok.
pcf8563: disable alarm interrupt.
pcf8563: check alarm interrupt ok.
pcf8563: pcf8563_set_interrupt_mode/pcf8563_get_interrupt_mode test.
pcf8563: set interrupt mode level.
pcf8563: check interrupt mode ok.
pcf8563: set interrupt mode pulse.
pcf8563: check interrupt mode ok.
pcf8563: finish register test.
```

```shell
pcf8563 -t alarm

pcf8563: chip is NXP PCF8563.
pcf8563: manufacturer is NXP.
pcf8563: interface is IIC.
pcf8563: driver version is 1.0.
pcf8563: min supply voltage is 1.9V.
pcf8563: max supply voltage is 5.5V.
pcf8563: max current is 0.80mA.
pcf8563: max temperature is 85.0C.
pcf8563: min temperature is -40.0C.
pcf8563: start alarm test.
pcf8563: time is 07:00:01.
pcf8563: time is 07:00:02.
pcf8563: time is 07:00:03.
pcf8563: time is 07:00:04.
pcf8563: time is 07:00:05.
pcf8563: time is 07:00:06.
pcf8563: time is 07:00:07.
pcf8563: time is 07:00:08.
pcf8563: time is 07:00:09.
pcf8563: time is 07:00:10.
pcf8563: time is 07:00:11.
pcf8563: time is 07:00:12.
pcf8563: time is 07:00:13.
pcf8563: time is 07:00:14.
pcf8563: time is 07:00:15.
pcf8563: time is 07:00:16.
pcf8563: time is 07:00:17.
pcf8563: time is 07:00:18.
pcf8563: time is 07:00:19.
pcf8563: time is 07:00:20.
pcf8563: time is 07:00:21.
pcf8563: time is 07:00:22.
pcf8563: time is 07:00:23.
pcf8563: time is 07:00:24.
pcf8563: time is 07:00:25.
pcf8563: time is 07:00:26.
pcf8563: time is 07:00:27.
pcf8563: time is 07:00:28.
pcf8563: time is 07:00:29.
pcf8563: time is 07:00:30.
pcf8563: time is 07:00:31.
pcf8563: time is 07:00:32.
pcf8563: time is 07:00:33.
pcf8563: time is 07:00:34.
pcf8563: time is 07:00:35.
pcf8563: time is 07:00:36.
pcf8563: time is 07:00:37.
pcf8563: time is 07:00:38.
pcf8563: time is 07:00:39.
pcf8563: time is 07:00:40.
pcf8563: time is 07:00:41.
pcf8563: time is 07:00:42.
pcf8563: time is 07:00:43.
pcf8563: time is 07:00:44.
pcf8563: time is 07:00:45.
pcf8563: time is 07:00:46.
pcf8563: time is 07:00:47.
pcf8563: time is 07:00:48.
pcf8563: time is 07:00:49.
pcf8563: time is 07:00:50.
pcf8563: time is 07:00:51.
pcf8563: time is 07:00:52.
pcf8563: time is 07:00:53.
pcf8563: time is 07:00:54.
pcf8563: time is 07:00:55.
pcf8563: time is 07:00:56.
pcf8563: time is 07:00:57.
pcf8563: time is 07:00:58.
pcf8563: time is 07:00:59.
pcf8563: irq alarm.
pcf8563: time is 07:01:00.
pcf8563: finish alarm test.
```

```shell
pcf8563 -t timer

pcf8563: chip is NXP PCF8563.
pcf8563: manufacturer is NXP.
pcf8563: interface is IIC.
pcf8563: driver version is 1.0.
pcf8563: min supply voltage is 1.9V.
pcf8563: max supply voltage is 5.5V.
pcf8563: max current is 0.80mA.
pcf8563: max temperature is 85.0C.
pcf8563: min temperature is -40.0C.
pcf8563: start timer test.
pcf8563: 1s.
pcf8563: 2s.
pcf8563: 3s.
pcf8563: 4s.
pcf8563: 5s.
pcf8563: 6s.
pcf8563: 7s.
pcf8563: 8s.
pcf8563: 9s.
pcf8563: irq timer.
pcf8563: 10s.
pcf8563: finish timer test.
```

```shell
pcf8563 -t output --times=5

pcf8563: chip is NXP PCF8563.
pcf8563: manufacturer is NXP.
pcf8563: interface is IIC.
pcf8563: driver version is 1.0.
pcf8563: min supply voltage is 1.9V.
pcf8563: max supply voltage is 5.5V.
pcf8563: max current is 0.80mA.
pcf8563: max temperature is 85.0C.
pcf8563: min temperature is -40.0C.
pcf8563: start output test.
pcf8563: set clock out 32.768khz.
pcf8563: run 1s.
pcf8563: run 2s.
pcf8563: run 3s.
pcf8563: run 4s.
pcf8563: run 5s.
pcf8563: set clock out 1.024khz.
pcf8563: run 1s.
pcf8563: run 2s.
pcf8563: run 3s.
pcf8563: run 4s.
pcf8563: run 5s.
pcf8563: set clock out 32hz.
pcf8563: run 1s.
pcf8563: run 2s.
pcf8563: run 3s.
pcf8563: run 4s.
pcf8563: run 5s.
pcf8563: set clock out 1hz.
pcf8563: run 1s.
pcf8563: run 2s.
pcf8563: run 3s.
pcf8563: run 4s.
pcf8563: run 5s.
pcf8563: finish output test.
```

```shell
pcf8563 -e basic-set-time --timestamp=1612686500

pcf8563: set timestamp 1612686500.
```

```shell
pcf8563 -e basic-get-time

pcf8563: 2021-02-07 16:28:33 0.
```

```shell
pcf8563 -e alarm-set-time --timestamp=1612686500

pcf8563: set timestamp 1612686500.
```

```shell
pcf8563 -e alarm-get-time

pcf8563: 2021-02-07 16:28:31 0.
```

```shell
pcf8563 -e alarm-run --minute=30

pcf8563: 1s.
pcf8563: 2s.
pcf8563: 3s.

...

pcf8563: 75s.
pcf8563: 76s.
pcf8563: 77s.
pcf8563: irq alarm.
pcf8563: 78s.
```

```shell
pcf8563 -e timer-set-time --timestamp=1612686500

pcf8563: set timestamp 1612686500.
```

```shell
pcf8563 -e timer-get-time

pcf8563: 2021-02-07 16:28:31 0.
```

```shell
pcf8563 -e timer-run --second=60

pcf8563: 1s.
pcf8563: 2s.
pcf8563: 3s.
pcf8563: 4s.
pcf8563: 5s.
pcf8563: 6s.
pcf8563: 7s.
pcf8563: 8s.
pcf8563: 9s.
pcf8563: 10s.
pcf8563: 11s.
pcf8563: 12s.
pcf8563: 13s.
pcf8563: 14s.
pcf8563: 15s.
pcf8563: 16s.
pcf8563: 17s.
pcf8563: 18s.
pcf8563: 19s.
pcf8563: 20s.
pcf8563: 21s.
pcf8563: 22s.
pcf8563: 23s.
pcf8563: 24s.
pcf8563: 25s.
pcf8563: 26s.
pcf8563: 27s.
pcf8563: 28s.
pcf8563: 29s.
pcf8563: 30s.
pcf8563: 31s.
pcf8563: 32s.
pcf8563: 33s.
pcf8563: 34s.
pcf8563: 35s.
pcf8563: 36s.
pcf8563: 37s.
pcf8563: 38s.
pcf8563: 39s.
pcf8563: 40s.
pcf8563: 41s.
pcf8563: 42s.
pcf8563: 43s.
pcf8563: 44s.
pcf8563: 45s.
pcf8563: 46s.
pcf8563: 47s.
pcf8563: 48s.
pcf8563: 49s.
pcf8563: 50s.
pcf8563: 51s.
pcf8563: 52s.
pcf8563: 53s.
pcf8563: 54s.
pcf8563: 55s.
pcf8563: 56s.
pcf8563: 57s.
pcf8563: 58s.
pcf8563: 59s.
pcf8563: irq timer.
pcf8563: 60s.
```

```shell
pcf8563 -e output-set-time --timestamp=1612686500

pcf8563: set timestamp 1612686500.
```

```shell
pcf8563 -e output-get-time

pcf8563: 2021-02-07 16:28:32 0.
```

```shell
pcf8563 -e output-enable --freq=1HZ

pcf8563: enable clock 1hz.
```

```shell
pcf8563 -e output-disable

pcf8563: disable clock output.
```

```shell
pcf8563 -h

Usage:
  pcf8563 (-i | --information)
  pcf8563 (-h | --help)
  pcf8563 (-p | --port)
  pcf8563 (-t reg | --test=reg)
  pcf8563 (-t alarm | --test=alarm)
  pcf8563 (-t timer | --test=timer)
  pcf8563 (-t output | --test=output) [--times=<num>]
  pcf8563 (-e basic-set-time | --example=basic-set-time) --timestamp=<time>
  pcf8563 (-e basic-get-time | --example=basic-get-time)
  pcf8563 (-e alarm-set-time | --example=alarm-set-time) --timestamp=<time>
  pcf8563 (-e alarm-get-time | --example=alarm-get-time)
  pcf8563 (-e alarm-run | --example=alarm-run) --minute=<minute>
  pcf8563 (-e timer-set-time | --example=timer-set-time) --timestamp=<time>
  pcf8563 (-e timer-get-time | --example=timer-get-time)
  pcf8563 (-e timer-run | --example=timer-run) --second=<second>
  pcf8563 (-e output-set-time | --example=output-set-time) --timestamp=<time>
  pcf8563 (-e output-get-time | --example=output-get-time)
  pcf8563 (-e output-enable | --example=output-enable) --freq=<1HZ | 32HZ | 1024HZ | 32768HZ>
  pcf8563 (-e output-disable | --example=output-disable)

 Options:
   -e <basic-set-time | basic-get-time | alarm-set-time| alarm-get-time | alarm-run |
       timer-set-time | timer-get-time | timer-run | output-set-time | output-get-time |
       output-enable | output-disable>
                                   Run the driver example.
       --freq=<1HZ | 32HZ | 1024HZ | 32768HZ>
                                   Set output frequency.([default: 1HZ])
   -h, --help                      Show the help.
   -i, --information               Show the chip information.
       --minute=<minute>           Set the alarm minute.([default: 0])
   -p, --port                      Display the pin connections of the current board.
       --second=<second>           Set the alarm second.([default: 10])
   -t <reg | alarm | timer | output>, --test=<reg | alarm | timer | output>
                                   Run the driver test.
       --times=<num>               Set the running times.([default: 3])
       --timestamp=<time>          Set the the unix timestamp.
```
