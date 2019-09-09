#ifndef __I2C_FORCE_SENSOR_CPP__
#define __I2C_FORCE_SENSOR_CPP__

#include "i2c_force_sensor.h"

I2C_FORCE_SENSOR::I2C_FORCE_SENSOR(int bus_ch, int dev_addr) {
	channel = bus_ch;
	address = dev_addr;
	polling_ms = 100;
}

void I2C_FORCE_SENSOR::init(void) {
	// clear error flag
	error = true;
	// set initialized flag
	initialized = false;

	// Reset speed of I2C
	i2c_config_t conf;

	conf.mode = I2C_MODE_MASTER;
	conf.sda_io_num = CHAIN_SDA_GPIO;
	conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
	conf.scl_io_num = CHAIN_SCL_GPIO;
	conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
	conf.master.clk_speed = 1E3;

	i2c_param_config(I2C_NUM_1, &conf);

	// Set new timeout of i2c
	i2c_set_timeout(I2C_NUM_1, 20000);
	
	state = s_detect;
}

int I2C_FORCE_SENSOR::prop_count(void) {
	// not supported
	return 0;
}

bool I2C_FORCE_SENSOR::prop_name(int index, char *name) {
	// not supported
	return false;
}

bool I2C_FORCE_SENSOR::prop_unit(int index, char *unit) {
	// not supported
	return false;
}

bool I2C_FORCE_SENSOR::prop_attr(int index, char *attr) {
	// not supported
	return false;
}

bool I2C_FORCE_SENSOR::prop_read(int index, char *value) {
	// not supported
	return false;
}

bool I2C_FORCE_SENSOR::prop_write(int index, char *value) {
	// not supported
	return false;
}
// --------------------------------------

void I2C_FORCE_SENSOR::process(Driver *drv) {
	I2CDev *i2c = (I2CDev *)drv;
	switch (state) {
		case s_detect:
			// detect i2c device
			if (i2c->detect(channel, address) == ESP_OK) {
				state = s_read1;
			} else {
				state = s_error;
			}
			break;
	
		case s_read1: {
			if (is_tickcnt_elapsed(tickcnt, polling_ms)) {
				tickcnt = get_tickcnt();

				uint8_t reg = 0x00;
				uint8_t data[2];
				
				reg = 0xA1;

				i2c->write(channel, address, &reg, 1);
				vTaskDelay(50 / portTICK_PERIOD_MS);
				if (i2c->read(channel, address, NULL, 0, data, 2) != ESP_OK) {
					state = s_error;
					return;
				}
				
				forceValue[0] = ((data[0]<<8)|data[1])&0x3FF;

				state = s_read1;
			}
		}

		case s_read2: {
			if (is_tickcnt_elapsed(tickcnt, polling_ms)) {
				tickcnt = get_tickcnt();

				uint8_t reg = 0x00;
				uint8_t data[2];
				
				reg = 0xA2;
				if (i2c->read(channel, address, &reg, 1, data, 2) != ESP_OK) {
					state = s_error;
					return;
				}
				
				forceValue[1] = ((data[0]<<8)|data[1])&0x3FF;

				state = s_read1;
			}
		}
		
		case s_wait:
			if (error) {
				// wait polling_ms timeout
				if (is_tickcnt_elapsed(tickcnt, polling_ms)) {
					state = s_detect;
				}
			}
			break;

		case s_error:
			forceValue[0] = 0;
			forceValue[1] = 0;
			
			// set error flag
			error = true;
			// clear initialized flag
			initialized = false;
			// get current tickcnt
			tickcnt = get_tickcnt();
			// goto wait and retry with detect state
			state = s_wait;
			break;

	}
}

// Method
int I2C_FORCE_SENSOR::getRaw(uint8_t ch) {
	if (ch > 1) return 0;
	
	return forceValue[ch];
}


#endif