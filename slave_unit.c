/*
 * slave_unit.c
 *
 * Created: 2/23/2015 12:13:31
 *  Author: Jimmy Chau
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include "atMega64m1/Drivers/spi.h"
#include "atMega64m1/Drivers/MPU6000.h"
//#include "atMega64m1/can.h"
//#include "atMega64m1/can_drv.h"
#include "atMega64m1/Drivers/can_lib.h"

//SPI INTERRUPT HANDLER
#define  F_CPU  8000000UL
typedef union{
	uint8_t u8[8];
	uint16_t u16;
	}acc_data_t;

int main(void)
{
	//INIT HARDWARE
	can_init(mainClockFreq_8MHZ, CAN_BPS_1000k);
	spi_init(spi_mode_3, spi_prescaler_2);
	
	
	struct mpu6000Settings mpuSettings = {
		.gyroRange = gyroRange_250DPS,
		.accRange = accRange_4g,
		.sampleRateDivider  = 1,
		.lpfBw = ACC10HZ_GYRO10HZ
	};
	mpu6000_init(&mpuSettings);
	
	uint8_t data_tx[8] = {0,0,0,0,0,0,0,0,0,0};
	uint8_t data_rx = 0x11;
	st_cmd_t msg_tx;
	msg_tx.ctrl.ide = 0;
	msg_tx.ctrl.rtr = 0;
	msg_tx.id.std = 0x41;
	msg_tx.dlc = 8;
	msg_tx.cmd = CMD_TX_DATA;
	msg_tx.pt_data = data_tx;
	/*
	st_cmd_t msg_rx;
	msg_rx.ctrl.ide = 0;
	msg_rx.id.std = 0x20;
	msg_rx.mask.std = 0xf0;
	msg_rx.ctrl.rtr = 0;
	msg_rx.dlc = 2;
	msg_rx.cmd = CMD_RX_DATA_MASKED;
	msg_rx.pt_data = &data_rx;
	*/
	
	//while(can_cmd(&msg_rx) != CAN_CMD_ACCEPTED);
	/*while(1){
		DDRD |= (1<<3);
		
		
	}*/
	//initialize hardware
	
	

	//while(1);
	//can_free_single_mob(&msg_rx.handle);
	can_cmd(&msg_tx);	
    while(1)
    {
		/*volatile uint8_t temp = can_get_status(&msg_rx);
		while(temp != CAN_STATUS_COMPLETED){
			temp = can_get_status(&msg_rx);
		}*/
		//_delay_ms(1000);
		can_free_single_mob(&msg_tx.handle);
		uint8_t a[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
		//mpu6000_burstReadRegister(MPUREG_ACCEL_XOUT_H,a,14);
		
		acc_data_t data;
		data.u8[0] = mpu6000_readRegister(MPUREG_ACCEL_XOUT_H);
		data.u8[1] = mpu6000_readRegister(MPUREG_ACCEL_XOUT_L);
		msg_tx.pt_data->i16[0] = data.u16;
		_delay_ms(75);
		can_cmd(&msg_tx);	
		//while(can_cmd(&msg_rx) != CAN_CMD_ACCEPTED);
		
    }
}