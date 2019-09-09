#ifndef __I2C_FORCE_SENSOR_H__
#define __I2C_FORCE_SENSOR_H__

#include "driver.h"
#include "device.h"
#include "i2c-dev.h"
#include "kidbright32.h"

class I2C_FORCE_SENSOR : public Device {
	private:		
		enum {
			s_detect,
			s_read1,
			s_read2,
			s_wait,
			s_error
		} state;
		TickType_t tickcnt, polling_tickcnt;
		
		int forceValue[2];

	public:
		// constructor
		I2C_FORCE_SENSOR(int bus_ch, int dev_addr) ;
		
		// override
		void init(void);
		void process(Driver *drv);
		int prop_count(void);
		bool prop_name(int index, char *name);
		bool prop_unit(int index, char *unit);
		bool prop_attr(int index, char *attr);
		bool prop_read(int index, char *value);
		bool prop_write(int index, char *value);
		
		// method
		int getRaw(uint8_t ch) ;
		
		
};

#endif