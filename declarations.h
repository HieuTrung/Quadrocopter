signed int ACCEL_XOUT = 0;
signed int ACCEL_YOUT = 0;
signed int ACCEL_ZOUT = 0;
float GYRO_XRATE = 0;
float GYRO_YRATE = 0;
float GYRO_ZRATE = 0;
int GYRO_XRATERAW = 0;
int GYRO_YRATERAW = 0;
int GYRO_ZRATERAW = 0;

unsigned char GYRO_XOUT_L = ' ';
unsigned char GYRO_XOUT_H = ' ';
unsigned char GYRO_YOUT_L = ' ';
unsigned char GYRO_YOUT_H = ' ';
unsigned char GYRO_ZOUT_L = ' ';
unsigned char GYRO_ZOUT_H = ' ';
signed int GYRO_XOUT = 0;
signed int GYRO_YOUT = 0;
signed int GYRO_ZOUT = 0;	

unsigned char ACCEL_XOUT_L = ' ';
unsigned char ACCEL_XOUT_H = ' ';
unsigned char ACCEL_YOUT_L = ' ';
unsigned char ACCEL_YOUT_H = ' ';
unsigned char ACCEL_ZOUT_L = ' ';
unsigned char ACCEL_ZOUT_H = ' ';

signed long GYRO_XOUT_OFFSET_1000SUM = 0;
signed long GYRO_YOUT_OFFSET_1000SUM = 0;
signed long GYRO_ZOUT_OFFSET_1000SUM = 0;

float GYRO_XANGLE = 0;
float GYRO_YANGLE = 0;
float GYRO_ZANGLE = 0;
long GYRO_XANGLERAW = 0;
long GYRO_YANGLERAW = 0;
long GYRO_ZANGLERAW = 0;
float ACCEL_XANGLE = 0;
float ACCEL_YANGLE = 0;
float ACCEL_ZANGLE = 0;

float KALMAN_XANGLE = 0;
float KALMAN_YANGLE = 0;
float KALMAN_ZANGLE = 0;

signed int GYRO_XOUT_OFFSET = 0;
signed int GYRO_YOUT_OFFSET = 0;
signed int GYRO_ZOUT_OFFSET = 0;
float dt = 0;

float COMPLEMENTARY_XANGLE = 0;
float COMPLEMENTARY_YANGLE = 0;

float TARGET_XANGLE = 0;
float TARGET_YANGLE = 0;
float TARGET_ZANGLE = 0;

float PID_XOUTPUT = 0;
float PID_YOUTPUT = 0;
float PID_ZOUTPUT = 0;

float KP = 35.0;
float KI = 85.0; //42
float KD = 30.0; //2.625

float ZKP = 40.0;
float ZKD = 25.0;

float XERROR = 0;
float YERROR = 0;
float ZERROR = 0;

float throttle = 0;

float OC1_output = 0.0;
float OC2_output = 0.0;
float OC3_output = 0.0;
float OC4_output = 0.0;

int count = 0;

float XINTEGRAL = 0;
float YINTEGRAL = 0;

int throttle_input = 0;
int yaw_input = 0;
int pitch_input = 0;
int roll_input = 0;

