#include <stdio.h>
#include <stdint.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <unistd.h> // for close

//Sensor Type Indentifiers
#define SENSORTYPE_AS7261 61
#define SENSORTYPE_AS72651 65

#define AS72XX_SLAVE_TX_VALID 0x02
#define AS72XX_SLAVE_RX_VALID 0x01

#define AS72XX_SLAVE_STATUS_REG 0x00
#define AS72XX_SLAVE_WRITE_REG 0x01
#define AS72XX_SLAVE_READ_REG 0x02

//Register addresses
#define AS726x_DEVICE_TYPE 0x00
#define AS726x_HW_VERSION 0x01
#define AS726x_CONTROL_SETUP 0x04
#define AS726x_INT_T 0x05
#define AS726x_DEVICE_TEMP 0x06
#define AS726x_LED_CONTROL 0x07

//AS7261 Registers
//Raw channel registers
#define AS7261_X 0x08
#define AS7261_Y 0x0A
#define AS7261_Z 0x0C
#define AS7261_NIR 0x0E
#define AS7261_DARK 0x10
#define AS7261_CLEAR 0x12
//Calibrated channel registers
#define AS7261_X_CAL 0x14
#define AS7261_Y_CAL 0x18
#define AS7261_Z_CAL 0x1C
#define AS7261_LUX_CAL 0x3C
#define AS7261_CCT_CAL 0x40

//AS7265X
//Raw channel registers
#define AS7265X_R_G_A     0x08
#define AS7265X_S_H_B     0x0A
#define AS7265X_T_I_C     0x0C
#define AS7265X_U_J_D     0x0E
#define AS7265X_V_K_E     0x10
#define AS7265X_W_L_F     0x12
//Calibrated channel registers
#define AS7265X_R_G_A_CAL   0x14
#define AS7265X_S_H_B_CAL   0x18
#define AS7265X_T_I_C_CAL   0x1C
#define AS7265X_U_J_D_CAL   0x20
#define AS7265X_V_K_E_CAL   0x24
#define AS7265X_W_L_F_CAL   0x28

//AS7265X Device Selection
#define AS7265X_DEV_SELECT_CONTROL  0x4F
#define AS72652_ACTIVE  4 // Bit 4 of AS7265X_DEV_SELECT_CONTROL is set if AS72652 is detected
#define AS72653_ACTIVE  5 // Bit 5 of AS7265X_DEV_SELECT_CONTROL is set if AS72653 is detected
//AS7265X Device Selcetors
#define AS72651_id    	0x00
#define AS72652_id      0x01
#define AS72653_id      0x02

#define POLLING_DELAY 5 //Amount of ms to wait between checking for virtual register changes

struct sensor {
	int8_t address;
  	int8_t  type;
};
typedef struct sensor sensor_list;

uint8_t begin(uint8_t gain, uint8_t measurementMode, int fd);
uint8_t getVersion(int fd); //61 oder 65
void I2C_Scan(sensor_list *const s);
uint8_t scan_AS7262(int fd);
uint8_t scan_AS7263(int fd);
void selectDevice(uint8_t device, int fd);
void setMeasurementMode(uint8_t mode, int fd);
void setGain(uint8_t gain, int fd);
void setIntegrationTime(uint8_t integrationValue, int fd);
void enableInterrupt(int fd);
void disableInterrupt(int fd);
void takeMeasurements(int fd);
void MeasurementFromAdress(int address);
void takeMeasurementsWithBulb( int fd);


//Get RAW AS7261 readings
int getX_CIE(int fd);
int getY_CIE(int fd);
int getZ_CIE(int fd);
int getNIR(int fd);
int getDark(int fd);
int getClear(int fd);

//Get RAW AS72651(NIR) readings
int getR(int fd);
int getS(int fd);
int getT(int fd);
int getU(int fd);
int getV(int fd);
int getW(int fd);

//Get RAW AS72652(color) readings
int getG(int fd);
int getX(int fd);
int getI(int fd);
int getJ(int fd);
int getK(int fd);
int getL(int fd);

//Get the various UV readings
//Get RAW AS72653(UV) readings
int getA(int fd);
int getB(int fd);
int getC(int fd);
int getD(int fd);
int getE(int fd);
int getF(int fd);

int getChannel_AS7265X(int device, uint8_t channelRegister, int fd);
int getChannel(uint8_t channelRegister, int fd);


float getCalibratedX(int fd);
float getCalibratedY(int fd);
float getCalibratedZ(int fd);
float getCalibratedLUX(int fd);
float getCalibratedCCT(int fd);

float getCalibratedViolet(int fd);
float getCalibratedBlue(int fd);
float getCalibratedGreen(int fd);
float getCalibratedYellow(int fd);
float getCalibratedOrange(int fd) ;
float getCalibratedRed(int fd);
float getCalibratedT(int fd);
float getCalibratedS(int fd);
float getCalibratedR(int fd);
float getCalibratedU(int fd);
float getCalibratedV(int fd);
float getCalibratedW(int fd);
float getCalibratedValue(uint8_t calAddress, int fd);

float convertBytesToFloat(uint32_t myLong);
uint8_t dataAvailable(int fd);
void clearDataAvailable(int fd);
void enableIndicator(int fd);
void disableIndicator(int fd);
void setIndicatorCurrent(uint8_t current, int fd);
void enableBulb(int fd);
void disableBulb (int fd);
void setBulbCurrent(uint8_t current , int fd);
uint8_t getTemperature(int fd);
float getTemperatureF(int fd);
void softReset(int fd);
uint8_t virtualReadRegister(uint8_t virtualAddr, int fd);
void virtualWriteRegister(uint8_t virtualAddr, uint8_t dataToWrite, int fd);







