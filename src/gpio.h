/**
    @file gpio.h
    
    @section DESCRIPTION    
    
    Robotics library for the Autonomous Robotics Development Platform  
    
    @brief [HEADER] Functions for the Xilinx XPS GPIO cores in userspace with IRQ support.
    
*/

#ifndef __GPIO_H__
#define __GPIO_H__

#include <native/mutex.h>
#include <native/task.h>
#include <native/intr.h>

/** Device memory mappings **/
#define GPIO_MAX_CHANNELS 2 /*! Device max number of channels */
#define GPIO_CHANNEL1 0x00 /*! Offset of channel IO from base */
#define GPIO_CHANNEL2 0x02 /*! Offset of channel IO from base */
#define GPIO_DATA_OFFSET 0     /*! Offset for data */
#define GPIO_TRISTATE_OFFSET 1 /*! Offset from any channel */

#define GPIO_GIE_OFFSET (0x11c>>2) /*! Global Interrupt Enable Register */
#define GPIO_GIE_MASK 0x80000000
/*! Interrupt Enable Register 
   RW
   Offset from base 
   Desc: Individual interrupt enable for each channel
*/
#define GPIO_IER_OFFSET (0x128>>2)
#define GPIO_IER_MASK 0xc0000000
#define GPIO_IER_CHANNEL1_MASK 0x1
#define GPIO_IER_CHANNEL2_MASK 0x2

#define GPIO_ISR_OFFSET (0x120>>2) /*! Interrupt Status Register */
#define GPIO_ISR_MASK GPIO_IER_MASK
#define GPIO_ISR_CHANNEL1_MASK GPIO_IER_CHANNEL1_MASK
#define GPIO_ISR_CHANNEL2_MASK GPIO_IER_CHANNEL2_MASK

#define GPIO_FLAGS_INPUT 0x01	/*! Driver parameters */
#define GPIO_FLAGS_OUTPUT 0x02  /*! Driver parameters */
#define GPIO_IRQ_CHANNEL1 0x04  /*! Driver parameters */
#define GPIO_IRQ_CHANNEL2 0x08  /*! Driver parameters */

typedef struct struct_gpio
{	
	unsigned long base_add; ///< Physical memory address where to map the device  (beginning)
	unsigned long end_add; ///< Physical memory address where to map the device (end)
	volatile int* vadd; ///< virtual address where device is mapped
	char flags; ///< IO/Allow Interrupt
	unsigned num_of_channels; ///< Number of activated channels ( either 1 or 2 )
	unsigned tristate[GPIO_MAX_CHANNELS]; ///< Previously assigned value (for muxed IO)
	unsigned value[GPIO_MAX_CHANNELS]; ///< Previously assigned value (for muxed IO)
	RT_MUTEX mutex; ///< Mutex for accessing IO
	RT_TASK interrupt_task; ///< Task from which the ISR is spawned
	RT_INTR intr_desc; ///< Interrupt pointer
	void (*isr)(void*); ///< ISR for both channels
} GPIO;

int gpio_fast_write(GPIO* gpio, unsigned offset, unsigned value);

int gpio_fast_read(GPIO* gpio, unsigned offset, unsigned* ret);

int gpio_write(GPIO* gpio, unsigned mask, unsigned shift, unsigned offset, unsigned value);

int gpio_read(GPIO* gpio, unsigned mask, unsigned shift, unsigned offset, unsigned* ret);

/* Some quick macros */

/*! Write/Read from the DATA register */

#define gpio_write_data(gpio,mask,shift,value) \
	gpio_write(gpio, mask, shift, GPIO_DATA_OFFSET, value)

/*! Write/Read from the DATA register */

#define gpio_read_data(gpio,mask,shift,ret) \
	gpio_read(gpio, mask, shift, GPIO_DATA_OFFSET, ret)

/*! Write/Read from the TRISTATE register ( for bidirectional GPIOs) */

#define gpio_set_dir(gpio,mask,shift,value) \
	gpio_write(gpio, mask, shift, GPIO_TRISTATE_OFFSET, value)

/*! Write/Read from the TRISTATE register ( for bidirectional GPIOs) */

#define gpio_get_dir(gpio,mask,shift,ret) \
	gpio_read(gpio, mask, shift, GPIO_TRISTATE_OFFSET, ret)

/*! Set mask to 1/0  - Useful for bit toggling */

#define gpio_set1_mask(gpio, mask, offset) \
	gpio_write(gpio, mask, 0, offset, ~0x0)

/*! Set mask to 1/0  - Useful for bit toggling */

#define gpio_set0_mask(gpio, mask, offset) \
	gpio_write(gpio, mask, 0, offset, 0)


/* Quick functions for IRQ handling inside the device */
int gpio_irq_enable_global(GPIO* gpio);

int gpio_irq_disable_global(GPIO* gpio);

/* By default assumes n = 1 */
int gpio_irq_enable_channel(GPIO* gpio,int n);

/* By default assumes n = 1 */
int gpio_irq_disble_channel(GPIO* gpio,int n);

int gpio_irq_isr_checkandtoggle_channel(GPIO* gpio,int n, unsigned* ret );

/* TODO:So far only for 1channel GPIO cores */
int gpio_init(GPIO* gpio, 
	      long unsigned int base_add, 
	      long unsigned int end_add, 
	      int num_of_channels, 
	      char flags, 
	      unsigned irqno,
	      void (*fisr)(void*),
	      int irq_prio);

int gpio_clean(GPIO* gpio);

#endif
