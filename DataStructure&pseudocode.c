typedef struct
{
  uint8_t* u8p_txData;
  uint8_t* u8p_rxData;
  uint8_t  u8_mode;
  uint32_t u32_speedHz;
  uint8_t  u8_bitsPerMsg;
  uint16_t u16_delayUs;
} spiMsg_t;

/*****************************************************************/
//pseudocode for MCU A
void main_MCU_A(void)
{
	Configure modbus();
	Configure spi();	

	while(1)
	{
		if (false != modbus_flag)
		{
			// process data received into predefined modbus ram data structure
			modbus_data = modbus_receive();
			if (FWUPDATE_COMMAND == modbus_data.command) 
			{
				// transmit fw data to mcuB through SPI by passing the pointer as argument
				spi_transmit(&modbus_data.fwdata);
				flash_write();
				bootloader_enter();
				mcu_reset();
			}
			else
			{
				/*Do something else*/
			}
		}
		else
		{
			/*Do something else*/
		}
	}
}

__IRQ modbus_handler()
{
	// host communicate mcuA, hw intr triggered, set flag in ISR
	set modbus_flag;
}
/*****************************************************************/
//pseudocode for MCU B
void main_MCU_B(void)
{
	Configure spi();	

	while(1)
	{
		if (false != spi_flag)
		{
			// process data received into predefined modbus ram data structure
			spiMsg_t spi_data = spi_receive();
			if (FWUPDATE_COMMAND == spi_data->u8p_rxData) 
			{
				flash_write();
				bootloader_enter();
				mcu_reset();
			}
			else
			{
				/*Do something else*/
			}
		}
		else
		{
			/*Do something else*/
		}
	}
}

__IRQ spi_handler()
{
	// mcuA communicate mcuB, hw intr triggered, set flag in ISR
	set spi_flag;
}