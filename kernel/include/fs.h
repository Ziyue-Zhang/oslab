#include <devices.h>
#include <common.h>
#include <klib.h>

typedef struct ext2{
	  uint32_t size;
	    uint32_t block_num;
		  uint32_t block_size;
		    char block_used[1024];
			  device_t *dev;
			    char fsname[80];
}ext2_t;

#endif
