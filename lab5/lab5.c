#include <minix/drivers.h>
#include "test5.h"
#include "pixmap.h"

static int proc_args(int argc, char *argv[]);
static unsigned long parse_ulong(char *str, int base);
static long parse_long(char *str, int base);
static void print_usage(char *argv[]);

int main(int argc, char **argv) {

	sef_startup();
	sys_enable_iop(SELF);

	printf("Lab5: Video Graphic Mode.\n");

	if (argc == 1) {
		print_usage(argv);
		return 0;
	} else {
		proc_args(argc, argv);
	}

	return 0;
}

static void print_usage(char *argv[]) {
	printf(
			"Usage: one of the following:\n"
					"\t service run %s -args \"test_init <decimal number><decimal number>\" \n"
					"\t service run %s -args \"test_square <decimal number><decimal number><decimal number><decimal number>\"\n"
					"\t service run %s -args \"test_line <decimal number><decimal number><decimal number><decimal number><decimal number>\" \n"
					"\t service run %s -args \"test_xpm <decimal number><decimal number><char array>\" \n",
					"\t service run %s -args \"test_move <decimal number><decimal number><char array><decimal number><decimal number><decimal number>\" \n",
					"\t service run %s -args \"test_controller\" \n",
			argv[0], argv[0], argv[0],argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char *argv[]) {

	unsigned long val1,val2,val3,val4,val5,val6;
	char **temp;

	//test_init()-------------------------
	if (strncmp(argv[1], "test_init", strlen("test_init")) == 0) {
		if (argc != 4) {
			printf("test5: wrong no of arguments for test of test_init() \n");
			return 1;
		}
		if ((val1 = parse_ulong(argv[2], 16)) == ULONG_MAX)
			return 1;
		if ((val2 = parse_ulong(argv[3], 16)) == ULONG_MAX)
					return 1;

		printf("test5:: test_init( %d , %d)\n", val1,val2);
		if(test_init(val1, val2)==NULL){
			printf("test5:: test_init Error!\n");
			return 1;
		}
		return 0;

	//test_square()---------------------------
	} else if (strncmp(argv[1], "test_square", strlen("test_square")) == 0) {
		if (argc != 6) {
			printf("test5: wrong no of arguments for test of test_square() \n");
			return 1;
		}
		if ((val1 = parse_ulong(argv[2], 10)) == ULONG_MAX)
			return 1;
		if ((val2 = parse_ulong(argv[3], 10)) == ULONG_MAX)
			return 1;
		if ((val3 = parse_ulong(argv[4], 10)) == ULONG_MAX)
			return 1;
		if ((val4 = parse_ulong(argv[5], 10)) == ULONG_MAX)
			return 1;


		printf("test5:: test_square( %d , %d , %d , %d )\n", val1,val2,val3,val4);

		if(test_square(val1,val2,val3,val4)!=0){
			printf("test5:: test_square Error!\n");
			return 1;
		}

		return 0;

	//test_line()------------------------
	} else if (strncmp(argv[1], "test_line", strlen("test_line")) == 0) {
		if (argc != 7) {
			printf("test5: wrong no of arguments for test of test_line() \n");
			return 1;
		}

		if ((val1 = parse_ulong(argv[2], 10)) == ULONG_MAX)
			return 1;
		if ((val2 = parse_ulong(argv[3], 10)) == ULONG_MAX)
			return 1;
		if ((val3 = parse_ulong(argv[4], 10)) == ULONG_MAX)
			return 1;
		if ((val4 = parse_ulong(argv[5], 10)) == ULONG_MAX)
			return 1;
		if ((val5 = parse_ulong(argv[6], 10)) == ULONG_MAX)
			return 1;

		printf("test5:: test_line( %d , %d , %d , %d , %d )\n", val1,val2,val3,val4,val5);
		if(test_line(val1, val2, val3, val4, val5) != 0){
			printf("test5::test_line Error!\n");
			return 1;
		}
		return 0;

	//test_xpm() -------------------------------------

	}else if (strncmp(argv[1], "test_xpm", strlen("test_xpm")) == 0) {
		if (argc != 5) {
			printf("test5: wrong no of arguments for test of test_xpm() \n");
			return 1;
		}

		if ((val1 = parse_ulong(argv[2], 10)) == ULONG_MAX)
					return 1;
		if ((val2 = parse_ulong(argv[3], 10)) == ULONG_MAX)
					return 1;

		if(strncmp(argv[4], "pic1", strlen("pic1")) == 0){
			temp = pic1;
		}else if(strncmp(argv[4], "pic2", strlen("pic2")) == 0){
			temp = pic2;
		}else if(strncmp(argv[4], "cross", strlen("cross")) == 0){
			temp = cross;
		}else if(strncmp(argv[4], "pic3", strlen("pic3")) == 0){
			temp = pic3;
		}else if(strncmp(argv[4], "penguin", strlen("penguin")) == 0){
			temp = penguin;
		}else{
			printf("\nTest5::test_xpm() ERROR! XPM invalid.\nXPM Available:\npic1 pic2 pic3 cross penguin\n");
			return 1;
		}


		printf("test5:: test_xpm()\n");
		if(test_xpm(val1, val2, temp) != 0){
			printf("test5::test_xpm Error!\n");
			return 1;
		}
		return 0;

		/**/

	}else if (strncmp(argv[1], "test_move", strlen("test_move")) == 0) {
		if (argc != 8) {
			printf("test5: wrong no of arguments for test of test_move() \n");
			return 1;
		}

		if ((val1 = parse_ulong(argv[2], 10)) == ULONG_MAX)
			return 1;
		if ((val2 = parse_ulong(argv[3], 10)) == ULONG_MAX)
			return 1;


		if(strncmp(argv[4], "pic1", strlen("pic1")) == 0){
			temp = pic1;
		}else if(strncmp(argv[4], "pic2", strlen("pic2")) == 0){
			temp = pic2;
		}else if(strncmp(argv[4], "cross", strlen("cross")) == 0){
			temp = cross;
		}else if(strncmp(argv[4], "pic3", strlen("pic3")) == 0){
			temp = pic3;
		}else if(strncmp(argv[4], "penguin", strlen("penguin")) == 0){
			temp = penguin;
		}else{
			printf("\nTest5::test_xpm() ERROR! XPM invalid.\nXPM Available:\npic1 pic2 pic3 cross penguin\n");
			return 1;
		}
		if ((val4 = parse_ulong(argv[5], 10)) == ULONG_MAX)
			return 1;
		if ((val5 = parse_ulong(argv[6], 10)) == ULONG_MAX)
			return 1;
		if ((val6 = parse_ulong(argv[7], 10)) == ULONG_MAX)
			return 1;

		printf("test5:: test_move()\n");
		if(test_move(val1, val2, temp,val4,val5, val6) != 0){
			printf("test5::test_move Error!\n");
			return 1;
		}
		return 0;

		/**/

	}else if (strncmp(argv[1], "test_controller", strlen("test_controller")) == 0) {
		if (argc != 2) {
			printf("test5: wrong no of arguments for test of test_controller() \n");
			return 1;
		}

		printf("test5:: test_controller()\n");
		if(test_controller() != 0){
			printf("test5::test_controller Error!\n");
			return 1;
		}
		return 0;
		/**/
	}else {
		printf("lab5: non valid function \"%s\" to test\n", argv[1]);
		return 1;
	}
}




static unsigned long parse_ulong(char *str, int base) {
	char *endptr;
	unsigned long val;

	val = strtoul(str, &endptr, base);

	if ((errno == ERANGE && val == ULONG_MAX) || (errno != 0 && val == 0)) {
		perror("strtol");
		return ULONG_MAX;
	}

	if (endptr == str) {
		printf("timer: parse_ulong: no digits were found in %s \n", str);
		return ULONG_MAX;
	}

	/* Successful conversion */
	return val;
}

static long parse_long(char *str, int base) {
	char *endptr;
	unsigned long val;

	val = strtol(str, &endptr, base);

	if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
			|| (errno != 0 && val == 0)) {
		perror("strtol");
		return LONG_MAX;
	}

	if (endptr == str) {
		printf("timer: parse_long: no digits were found in %s \n", str);
		return LONG_MAX;
	}

	/* Successful conversion */
	return val;
}
