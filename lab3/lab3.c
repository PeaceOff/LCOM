#include <minix/drivers.h>
#include "test3.h"

static int proc_args(int argc, char *argv[]);
static unsigned long parse_ulong(char *str, int base);
static long parse_long(char *str, int base);
static void print_usage(char *argv[]);

int main(int argc, char **argv) {

	sef_startup();
	sys_enable_iop(SELF);
	printf("Lab3: KeyboardInput.\n");

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
					"\t service run %s -args \"testscan <decimal number>\" \n"
					"\t service run %s -args \"testleds <decimal number> <decimal number>\"\n"
					"\t service run %s -args \"testtimed <decimal number>\" \n",
			argv[0], argv[0], argv[0]);
}

unsigned long pow10(unsigned long n){
	int value=10;
	int i=0;
	for (i; i <n; i++) {
		value *=10;
	}
	return value;
}

static int proc_args(int argc, char *argv[]) {

	unsigned long val1,val2;

	//kbd_test_scan()
	if (strncmp(argv[1], "testscan", strlen("testscan")) == 0) {
		if (argc != 3) {
			printf("test3: wrong no of arguments for test of kbd_test_scan() \n");
			return 1;
		}
		if ((val1 = parse_ulong(argv[2], 10)) == ULONG_MAX)
			return 1;

		printf("test3:: kbd_test_scan( %d )\n", val1);
		if(kbd_test_scan(val1)!=0){
			printf("test3:: kbd_test_scan Error!\n");
			return 1;
		}
		return 0;

	//testleds()
	} else if (strncmp(argv[1], "testleds", strlen("testleds")) == 0) {
		if (argc < 4) {
			printf("test3: wrong no of arguments for test of kbd_test_leds() \n");
			return 1;
		}
		if ((val1 = parse_ulong(argv[2], 10)) == ULONG_MAX)
			return 1;
		if ((val2 = parse_ulong(argv[3], 10)) == ULONG_MAX)
					return 1;

		printf("test3:: kbd_test_leds( %d , [", val1);


		unsigned short arrayB[val1];
		int var;
		for ( var= 0; var < val1; var++) {
			if ((arrayB[var] = parse_ulong(argv[3+var], 10)) == ULONG_MAX)
								return 1;
			printf("%d", arrayB[var]);
			if(var!= val1-1)
				printf(",");
		}
		printf("]\n");
		unsigned short temp = val2;
		if(kbd_test_leds(val1, arrayB)!=0){
			printf("test3::kbd_test_leds Error!\n");
			return 1;
		}

		return 0;

	//timer_test_int()
	} else if (strncmp(argv[1], "testtimed", strlen("testtimed")) == 0) {
		if (argc != 3) {
			printf("test3: wrong no of arguments for test of kbd_test_timed_scan() \n");
			return 1;
		}
		if ((val1 = parse_ulong(argv[2], 10)) == ULONG_MAX)
			return 1;

		printf("test3:: kbd_test_timed_scan(%d)\n",val1);
		if(kbd_test_timed_scan(val1) != 0){
			printf("test3::kbd_test_timed_scan Error!\n");
			return 1;
		}
		return 0;
	} else {
		printf("timer: non valid function \"%s\" to test\n", argv[1]);
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
