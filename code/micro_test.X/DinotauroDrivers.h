/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _DINOTAURODRIVERS_H    /* Guard against multiple inclusion */
#define _DINOTAURODRIVERS_H

#include <p32xxxx.h>
#define _SUPPRESS_PLIB_WARNING 1
#include <stdlib.h>
#include <plib.h>
#include <stdarg.h>

//#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#define _int_(v)  __attribute__((vector(v))) __attribute__((interrupt))
#define _ramf_    __attribute__((longcall,section (".ramfunc")))

#define DisableInterrupts()  asm volatile("di")
#define EnableInterrupts()   asm volatile("ei")
#define NOP()                asm volatile("nop")


#define	PI		3.14159265358979323846	/* pi */

/** \brief sensor ids 
 * \anchor sensor_ids
 */
#define OBST_SENSOR_RIGHT	0
#define OBST_SENSOR_FRONT	1
#define OBST_SENSOR_LEFT	2
//#define AN6					3
//#define AN7					4
//#define BATTERY				5

#define waitTick40ms() {while(!tick40ms); tick40ms = 0;}
#define waitTick20ms() {while(!tick20ms); tick20ms = 0;}

/** \brief get start button status 
 * \return the start button status 
 */
#define startButton() (!PORTBbits.RB3)
#define stopButton() (!PORTBbits.RB4) //*! get stop button status

//#define enableObstSens() {LATBbits.LATB10=1;}
//#define disableObstSens() {LATBbits.LATB10=0;}

//#define enableGroundSens() {LATEbits.LATE5=1;}
//#define disableGroundSens() {LATEbits.LATE5=0;}

//#define readBeaconSens() (PORTBbits.RB9)
#define readBeaconSens() (PORTEbits.RE5)

// ****************************************************************************
// Global variables
typedef union
{
	struct 
	{
		int obstSensRight; ///< right obstacle sensor
		int obstSensFront; ///< frontal obstacle sensor
		int obstSensLeft;
		//int an6;
		//int an7;
		//int batteryVoltage;
	};
	int array[3];
} MR32_analogSensors;

extern MR32_analogSensors analogSensors;

extern int SERVO_WIDTH_MIN;
extern int SERVO_WIDTH_MAX;

extern volatile bool tick10ms;
extern volatile bool tick20ms;
extern volatile bool tick40ms;
extern volatile bool tick80ms;
extern volatile bool tick160ms;

// ****************************************************************************
// Function prototypes
/** \brief Init the Robot's software
 * \details ...
 * \ingroup general
 */
void initPIC32(void);


/** \brief Read all analog sensors.
 *  \details The sensors read are the obstacle sensors and the battery status.
 *      The measured values are stored in an internal memory.
 *  \ingroup sensors
 */
void readAnalogSensors(void);


/** \brief Read the frontal line sensors
 *  \details The integration time of the line sensors can be controlled 
 *      through a gain
 *  \ingroup sensors
 *  \param gain A value controlling the integration time of the line sensor:
 *      if 0, a default value is used; otherwise the given value is used.
 *  \return A 5-bit value representing the current measurements of the sensors. 
 */
unsigned int readLineSensors();	// for default value of gain, pass 0


/** \brief turn on/off leds
 * \ingroup actuators
 * \param state 4-bit value representing the required on/off state of each led
 */
void leds(int state);


/** \brief turn on/off given led
 * \ingroup actuators
 * \param ledNr number of led to turn on/off
 * \param 1-bit value representing the required on/off state of the led
 */
void led(int ledNr, int value);


/** \brief Set beacon sensor servo position
 * \ingroup actuators
 * \param pos The servo position in range [-15,+15], clockwise 
 */
void setServoPos(int pos);


/** \brief Get dead reckoning robot position/pose
 * \param [out] xp Pointer to variable where to store the X coord
 * \param [out] yp Pointer to variable where to store the Y coord
 * \param [out] hp Pointer to variable where to store the heading
 */
void getRobotPos(double *xx, double *yy, double *tt);


/** \brief suspend execution during a given period
 * \ingroup times
 * \param time time in tenths of milliseconds
 */
void delay(unsigned int time);


/** \brief suspend execution during a given period
 * \ingroup times
 * \param time time in tenths of seconds
 */
void wait(unsigned int time);


/** \brief get the last reading of the given obstacle sensor
 * \ingroup sensors
 * \param sensorId sensor id
 * \sa sensor_ids
 * \return the last value read
 */
unsigned int obstacleSensor(unsigned int sensorId);


/** \brief get the last reading of the battery status sensor
 * \ingroup sensors
 * \return the last value read
 */
unsigned int batteryVoltage(void);

/** \brief set control mode
 * \ingroup actuators
 * \param flag set/reset control mode (true set closed mode)
 */
void closedLoopControl(bool flag);


/** \brief Apply the given velocities to the wheel motors
 * \details velocities range from -100 to 100, where 100 roughly represents 0.7 m/s
 * \ingroup actuators
 * \param velLeft setpoint for left motor, in range [-100, 100]
 * \param velRight setpoint for right motor, in range [-100, 100]
 */
void setVel2(int velLeft, int velRight);


/** \brief normalize an angle to range ]-180,+180]
 * \ingroup math
 * \param angle the angle to normalize
 * \return the normalized angle
 */
double normalizeAngle(double angle);

void _mon_putc(char c);

int _mon_getc(int canblock);
#endif /* _DINOTAURODRIVERS_H */

/* *****************************************************************************
 End of File
 */
