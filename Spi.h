/* 
 * File:   Spi.h
 * Author: andrey
 *
 * Created on 13 ????????? 2013, 13:52
 * library for work with spi
 */

/**
 * @author Kyrylov Andrii
 * @todo ini spi modul (8-bit interface)
 */
void ConfigurateSPI2();

/**
 * @author Kyrylov Andrii
 * @param data_out - output value
 * @todo write over spi
 * @return 1 - success
 */
char spi2_write(unsigned int data_out);

/**
 * @author Kyrylov Andrii
 * @todo read byte over spi module
 * @return byte is received from spi
 */
unsigned char spi2_read(void);