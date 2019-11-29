
#ifdef  I2C_GLOBALS_H
  #define I2C_EXT
#else
  #define I2C_EXT extern
#endif

I2C_EXT void I2C_Init( unsigned char byI2CChannel );
I2C_EXT void I2C_Delay( unsigned int wTmp );
I2C_EXT void I2C_Flash( void );
I2C_EXT void I2C_Start( unsigned char byI2CChannel );
I2C_EXT void I2C_Stop( unsigned char byI2CChannel );
I2C_EXT void I2C_WriteX( unsigned char byTmp, unsigned char byI2CChannel );
I2C_EXT unsigned char I2C_ReadX( unsigned char byI2CChannel );
I2C_EXT unsigned char I2C_ACK( unsigned char byI2CChannel );













