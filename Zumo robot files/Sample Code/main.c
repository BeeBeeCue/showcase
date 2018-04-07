//LINE FOLLOWER
//libraries for robot functions
#include <project.h>
#include <stdio.h>
#include "Motor.h"
#include "Ultra.h"
#include "Nunchuk.h"
#include "Reflectance.h"
#include "I2C_made.h"
#include "Gyro.h"
#include "Accel_magnet.h"
#include "IR.h"
#include "Ambient.h"
#include "Beep.h"
//Analog values for IR sensors
//Black for IF/Else
#define BLACKL3 9000
#define BLACKL1 16000
#define BLACKR1 16000
#define BLACKR3 9000

#define WHITEL3 7000
#define WHITEL1 7000
#define WHITER1 7000
#define WHITER3 7000

//Stopping Black value
#define BLACKL32 8000
#define BLACKL12 8000
#define BLACKR12 8000
#define BLACKR32 8000
#define HIGHBLACKVALUE 19000
//Speed and delay settings
#define STARTSPEED 100
#define MAXSPEED 200
#define TURNSPEED 255
#define TANKTURNSPEED 255
#define OTHERTURNSPEED 0
#define MOVETIME 1
#define HARD_TURN_TIME 55

int rread(void);


//tank turn left, so robot can spin
void tankturnleft(uint8 speed,uint32 delay)
{
    MotorDirLeft_Write(1);      // set LeftMotor backward mode
    MotorDirRight_Write(0);     // set RightMotor backward mode
    PWM_WriteCompare1(speed); 
    PWM_WriteCompare2(speed); 
    CyDelay(delay);
}
//tank turn right, so robot can spin
void tankturnright(uint8 speed,uint32 delay)
{
    MotorDirLeft_Write(0);      // set LeftMotor backward mode
    MotorDirRight_Write(1);     // set RightMotor backward mode
    PWM_WriteCompare1(speed); 
    PWM_WriteCompare2(speed); 
    CyDelay(delay);
}
//IF ELSE CODE!!!
int main()
{
    //Important stuff for sensors, IR, batteries etc
    struct sensors_ ref;
    CyGlobalIntEnable; 
    UART_1_Start();
    sensor_isr_StartEx(sensor_isr_handler);
    reflectance_start();
    int IR_val;
    IR_led_Write(1);

    //Start of the race, robot is placed down and wait for its first 
    //IR signal to drive to the first black line 
    //Press a IR source to start loop
    do
    {
        IR_val = get_IR();
    }
    while(!IR_val);
    //Motor start
    motor_start(); 
    //Forward loop until the first black line
    for(;;)
    {
        //Analog sensor read
        reflectance_read(&ref);
        //If all the sensors see black then we break this loop
        if (ref.l3 > BLACKL3 && ref.l1 > BLACKL1 && ref.r1 > BLACKR1 && ref.r3 > BLACKR3)
        {
            break;
        }        
        //Left turn if robot is drifting to the right
        else if (ref.l1 > BLACKL1 && ref.r1 < BLACKR1)
        {
            motor_turn(OTHERTURNSPEED,TURNSPEED,MOVETIME);
        }               
        //Right turn if robot is drifting to the left
        else if (ref.l1 < BLACKL1 && ref.r1 > BLACKR1)
        {
            motor_turn(TURNSPEED,OTHERTURNSPEED,MOVETIME);
        }           
        //Slowly forward to the starting line
        else motor_forward(STARTSPEED,MOVETIME);       
    }
    
    //Stop the motor at the first black line
    motor_stop();
    //Wait for the IR signel to start the 2nd time
    do
    {
        IR_val = get_IR();
    }
    while(!IR_val);
    //Start the motor again
    motor_start();                   
    //Go over the first black line
    motor_forward(MAXSPEED,200);

    //Main loop for line tracking
    for(;;)
    {
        //Analog sensor read
        reflectance_read(&ref);
        //If all the sensors see black then we break this loop
        if (ref.l3 > BLACKL32 && ref.l1 > BLACKL12 && ref.r1 > BLACKR12 && ref.r3 > BLACKR32)
        {
            break;
        }
        //Mega hard left if the left side is only on the black line
        else if (ref.l3 > BLACKL3 && ref.l1 < WHITEL1 && ref.r1 < WHITER1 && ref.r3 < WHITER3)
        {
            tankturnleft(TANKTURNSPEED,HARD_TURN_TIME);
        }
        //Mega hard right if the right side is only on the black line
        else if (ref.l3 < WHITEL3 && ref.l1 < WHITEL1 && ref.r1 < WHITER1 && ref.r3 > BLACKR3)
        {
            tankturnright(TANKTURNSPEED,HARD_TURN_TIME);
        }
        //Hard left if there is a sharp left turn and the 3 left sensors pick up black and right one white
        else if (ref.l3 > BLACKL3 && ref.l1 > BLACKL1 && ref.r1 > BLACKR1 && ref.r3 < WHITER3)
        {
            tankturnleft(TANKTURNSPEED,MOVETIME);
        }
        //Hard right if there is a sharp right turn and the 3 right sensors pick up black and left one white
        else if (ref.l3 < WHITEL3 && ref.l1 > BLACKL1 && ref.r1 > BLACKR1 && ref.r3 > BLACKR3)
        {
            tankturnright(TANKTURNSPEED,MOVETIME);
        }          
        //Left turn if robot is drifting to the right
        else if (ref.l1 > BLACKL1 && ref.r1 < BLACKR1)
        {
            motor_turn(OTHERTURNSPEED,TURNSPEED,MOVETIME);
        }                   
        //Right turn if robot is drifting to the left
        else if (ref.l1 < BLACKL1 && ref.r1 > BLACKR1)
        {
            motor_turn(TURNSPEED,OTHERTURNSPEED,MOVETIME);
        }
        //If middle sensors are on the black line then full throttle
        else if (ref.l1 > HIGHBLACKVALUE && ref.r1 > HIGHBLACKVALUE)
        {
            motor_forward (MAXSPEED,MOVETIME);
        }
        //Forward if other conditions apply
        else motor_forward(MAXSPEED,MOVETIME);
    }
    //Forward to cross the line so sensors won't read it again
    motor_forward(MAXSPEED,50);
   
    //Final stop loop
    //Loop to go to the last black line and stop
    for(;;)
    {
        //Analog reading
        reflectance_read(&ref);
        //If all the sensors see black then we stop the motors, we have won
        if (ref.l3 > BLACKL32 && ref.l1 > BLACKL12 && ref.r1 > BLACKR12 && ref.r3 > BLACKR32)
        {
            motor_stop();
        }
        //Hard left if there is a sharp left turn and the 3 left sensors pick up black and right one white
        else if (ref.l3 > BLACKL3 && ref.l1 > BLACKL1 && ref.r1 > BLACKR1 && ref.r3 < WHITER3)
        {
            motor_turn(0,100,MOVETIME);
        }        
        //Hard right if there is a sharp right turn and the 3 right sensors pick up black and left one white
        else if (ref.l3 < WHITEL3 && ref.l1 > BLACKL1 && ref.r1 > BLACKR1 && ref.r3 > BLACKR3)
        {
            motor_turn(100,0,MOVETIME);
        }        
        //Left turn if robot is drifting to the right
        else if (ref.l1 > BLACKL1 && ref.r1 < BLACKR1)
        {
            motor_turn(0,100,MOVETIME);
        }                
        //Right turn if robot is drifting to the left
        else if (ref.l1 < BLACKL1 && ref.r1 > BLACKR1)
        {
            motor_turn(100,0,MOVETIME);
        }
        //Forward if robot is on the black line          
        else if (ref.l1 > BLACKL1 && ref.r1 > BLACKR3)
        {
            motor_forward (MAXSPEED,MOVETIME);
        }
        //Forward if other contidions apply
        else motor_forward(MAXSPEED,MOVETIME);
    } 
} 
   

//IMPORTANT FILES
#if 0
int rread(void)
{
    SC0_SetDriveMode(PIN_DM_STRONG);
    SC0_Write(1);
    CyDelayUs(10);
    SC0_SetDriveMode(PIN_DM_DIG_HIZ);
    Timer_1_Start();
    uint16_t start = Timer_1_ReadCounter();
    uint16_t end = 0;
    while(!(Timer_1_ReadStatusRegister() & Timer_1_STATUS_TC)) {
        if(SC0_Read() == 0 && end == 0) {
            end = Timer_1_ReadCounter();
        }
    }
    Timer_1_Stop();
    
    return (start - end);
}
#endif

/* Don't remove the functions below */
int _write(int file, char *ptr, int len)
{
    (void)file; /* Parameter is not used, suppress unused argument warning */
	int n;
	for(n = 0; n < len; n++) {
        if(*ptr == '\n') UART_1_PutChar('\r');
		UART_1_PutChar(*ptr++);
	}
	return len;
}

int _read (int file, char *ptr, int count)
{
    int chs = 0;
    char ch;
 
    (void)file; /* Parameter is not used, suppress unused argument warning */
    while(count > 0) {
        ch = UART_1_GetChar();
        if(ch != 0) {
            UART_1_PutChar(ch);
            chs++;
            if(ch == '\r') {
                ch = '\n';
                UART_1_PutChar(ch);
            }
            *ptr++ = ch;
            count--;
            if(ch == '\n') break;
        }
    }
    return chs;
}
/* [] END OF FILE */
