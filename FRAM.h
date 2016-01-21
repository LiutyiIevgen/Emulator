/* 
 * File:   FRAM.h
 * Author: andrey
 *
 * Created on 13 ????????? 2013, 14:15
 */
#define WRITE_ENABLE      0b00000110
#define WRITE_MEMORY_DATA 0b00000010
#define READ_MEMORY_DATA  0b00000011
#define READ_STATUS_REGISTER 0b00000101

/**
 * @author Kyrylov Andrii
 * @todo init fram
 */
void FramInitialization();

/**
 * @author Kyrylov Andrii
 * @todo set write mode
 */
void FramWriteEnable();

/**
 * @author Kyrylov Andrii
 * @param address - address of memory cell (max 8192)
 * @param *data - pointer to data
 * @param length - byte count
 * @todo write array to fram
 */
void FramWrite(unsigned int address,char *data, int length);

/**
 * @author Kyrylov Andrii
 * @param address - address of memory cell (max 8192)
 * @param *data - pointer to data
 * @param length - byte count
 * @todo read data from fram
 */
void FramRead(unsigned int address,char *data, int length);

/**
 * @author Kyrylov Andrii
 * @todo read current status of fram
 * @return current status of fram
 */
unsigned char FramReadStatus();

/**
 * @author Kyrylov Andrii
 * @todo read saved position counter
 * @return value of position counter
 */
long FramReadPositionCounter();