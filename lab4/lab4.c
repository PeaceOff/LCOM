#include <minix/drivers.h>
#include "test4.h"

static int proc_args(int argc, char *argv[]);
static unsigned long parse_ulong(char *str, int base);
static long parse_long(char *str, int base);
static void print_usage(char *argv[]);

int main(int argc, char **argv) {

	sef_startup();
	sys_enable_iop(SELF);

	printf("Lab4: MouseInput.\n");

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
					"\t service run %s -args \"testpacket <decimal number>\" \n"
					"\t service run %s -args \"testasync <decimal number>\"\n"
					"\t service run %s -args \"testconfig\" \n"
					"\t service run %s -args \"testgesture <decimal number><decimal number>\" \n",
			argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char *argv[]) {

	unsigned long val1,val2;

	//test_packet()
	if (strncmp(argv[1], "testpacket", strlen("testpacket")) == 0) {
		if (argc != 3) {
			printf("test4: wrong no of arguments for test of test_packet() \n");
			return 1;
		}
		if ((val1 = parse_ulong(argv[2], 10)) == ULONG_MAX)
			return 1;

		printf("test4:: test_packet( %d )\n", val1);
		if(test_packet(val1)!=0){
			printf("test4:: test_packet Error!\n");
			return 1;
		}
		return 0;

	//test_async()
	} else if (strncmp(argv[1], "testasync", strlen("testasync")) == 0) {
		if (argc != 3) {
			printf("test4: wrong no of arguments for test of test_async() \n");
			return 1;
		}
		if ((val1 = parse_ulong(argv[2], 10)) == ULONG_MAX)
			return 1;

		printf("test4:: test_async( %d )\n", val1);

		if(test_async(val1)!=0){
			printf("test4:: test_async Error!\n");
			return 1;
		}

		return 0;

	//test_config()
	} else if (strncmp(argv[1], "testconfig", strlen("testconfig")) == 0) {
		if (argc != 2) {
			printf("test4: wrong no of arguments for test of test_config() \n");
			return 1;
		}

		printf("test4:: test_config()\n");
		if(test_config() != 0){
			printf("test4::test_config Error!\n");
			return 1;
		}
		return 0;

	//test_gesture()
	}else if (strncmp(argv[1], "testgesture", strlen("testgesture")) == 0) {
		if (argc != 4) {
			printf("test4: wrong no of arguments for test of test_gesture() \n");
			return 1;
		}

		if ((val1 = parse_ulong(argv[2], 10)) == ULONG_MAX)
					return 1;
		if ((val2 = parse_ulong(argv[3], 10)) == ULONG_MAX)
					return 1;

		printf("test4:: test_config()\n");
		if(test_gesture(val1, val2) != 0){
			printf("test4::test_gesture Error!\n");
			return 1;
		}
		return 0;
	}else {
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
