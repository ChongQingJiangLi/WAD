/***********************************************************************
 *
 *      Wheelchair Event Handling Function
 *
 **********************************************************************/

/*
 * Include Header Files
 */
#include "FXAS21002C.h"
#include "FXOS8700CQ.h"
#include "Lora.h"
#include "Sys_Delay.h"
#include "gpio_pins.h"
#include "gpio_imx.h"

/*
 * Define a little delay time to avoid button bounce
 */
#define buttonDEBOUNCE 70

/*
 * Define Wheelchair ID to Detect
 *  Wheelchair ID Form: ID-UserName-Room-Block
 */
const char wheelchairID[] = "WC1_AC\r\n";

/*
 * Accident Detect Function
 */
void EventHandling()
{
    struct Accel accel_Values;
    struct Gyro gyro_Values;

    bool ButtonStt;


    gyro_Values = FXAS21002C_GetGyro();
    accel_Values = FXOS8700CQ_GetAccel();
    //debug_printf("Accel: X: %d  Y: %d  Z: %d\r\n", accel_Values.a_X, accel_Values.a_Y, accel_Values.a_Z);
    //debug_printf("Gyro: X: %d  Y: %d  Z: %d\r\n", gyro_Values.g_X, gyro_Values.g_Y, gyro_Values.g_Z);

    if ( (gyro_Values.g_X >= 4000) || (gyro_Values.g_Y >= 4000) || (accel_Values.a_X >= 4000) || (accel_Values.a_Y >= 4000))
    {
        Lora_Send(wheelchairID);
        //debug_printf("Accel: X: %d  Y: %d  Z: %d\r\n", accel_Values.a_X, accel_Values.a_Y, accel_Values.a_Z);
        //debug_printf("Gyro: X: %d  Y: %d  Z: %d\r\n", gyro_Values.g_X, gyro_Values.g_Y, gyro_Values.g_Z);
        while (1)
        {
            // Be..Beep.. Buzzer
            GPIO_WritePinOutput(GPIO6_BASE_PTR, 14, gpioPinClear);
            ms_delay(500);
            GPIO_WritePinOutput(GPIO6_BASE_PTR, 14, gpioPinSet);
            ms_delay(100);
            // Check Button Status
            ButtonStt = GPIO_IsIntPending(GPIO4_BASE_PTR, 8);
            if (ButtonStt == 1)
            {
                // Send message
                Lora_Send("WC1_OK\r\n");
                // Turn off Buzzer
                GPIO_WritePinOutput(GPIO6_BASE_PTR, 14, gpioPinSet);
                break;
            }
            else
            {
                ;
            }
            // Button De-bounce
            ms_delay(buttonDEBOUNCE);
        }

    }
}
