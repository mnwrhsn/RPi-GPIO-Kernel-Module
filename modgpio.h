#ifndef __RPI_GPIO_H__
#define __RPI_GPIO_H__
//magical IOCTL number
#define GPIO_IOC_MAGIC 'k'

typedef enum {MODE_INPUT=0, MODE_OUTPUT} PIN_MODE_t;

struct gpio_data_write {
	int pin;
	char data;
};

struct gpio_data_mode {
	int pin;
	PIN_MODE_t data;
};

#define GPIO_READ			_IOR(GPIO_IOC_MAGIC, 0x90, int) //in: pin to read				//out: value 			//the value read the value
#define GPIO_WRITE			_IOW(GPIO_IOC_MAGIC, 0x91, struct gpio_data_write) //in: struct(pin, data)		//out: NONE

#define GPIO_REQUEST		_IOR(GPIO_IOC_MAGIC, 0x92, int) //in: pin to reque			//out: success/fail 	// request exclusive write privalages
#define GPIO_FREE			_IOR(GPIO_IOC_MAGIC, 0x93, int) //in: pin to free 			//out: success/fail?

#define GPIO_TOGGLE			_IOW(GPIO_IOC_MAGIC, 0x94, int) //in: pin to toggle			//out: NONE
#define GPIO_MODE			_IOW(GPIO_IOC_MAGIC, 0x95, struct gpio_data_mode) //in: struct (pin, mode[i/o])




//The header file <asm/ioctl.h>, which is included by <linux/ioctl.h>, defines
//macros that help set up the command numbers as follows:
// _IO(type,nr) (for a command that has no argument),
// _IOR(type,nr,datatype) (for reading data from the driver),
// _IOW(type,nr,datatype) (for writing data to the driver),
// _IOWR(type,nr,datatype) (for bidirectional transfers). The type and number
// 		fields are passed as arguments, and the size field is derived by applying sizeof to the datatype argument.

//https://github.com/raspberrypi/linux/blob/rpi-3.6.y/Documentation/ioctl/ioctl-number.txt


#endif //__RPI_GPIO_H__