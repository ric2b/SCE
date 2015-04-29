#include <cyg/kernel/kapi.h>
#include <cyg/error/codes.h>
#include <cyg/io/io.h>
#include <cyg/io/serialio.h>
#include <cyg/io/config_keys.h>
#include <stdio.h>

int main(void)
{
	Cyg_ErrNo err;
	cyg_io_handle_t serH;
	char c;
	char bufr[10];
	char bufw[10]="123456789";
	int n;
	
	err = cyg_io_lookup("/dev/ser0", &serH);
	printf("lookup err=%x\n", err);

	while(1){
		c = getchar();
		if (c=='l') {
			n = 10;
			err = cyg_io_read(serH, bufr, &n);
			printf("io_read err=%x, n=%d buf=%s\n", err, n, bufr);
		}
		if (c=='w') {
			n = 10;
			err = cyg_io_write(serH, bufw, &n);
			printf("io_write err=%x, n=%d buf=%s\n", err, n, bufw);
		}
	}
	return 0;
}