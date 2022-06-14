#include "./simulation_devs.h"


void test_all(char *file)
{
	int argc = 0;
	char str[50];
	msg_process_t p;

	MSG_create_environment(file);
	MSG_function_register("controller", controller);
	MSG_function_register("externalTransition", externalTransition);
	MSG_function_register("manual", manual);
	MSG_function_register("toAutomatic", toAutomatic);
	MSG_function_register("greenLight", greenLight);
	MSG_function_register("yellowLight", yellowLight);
	MSG_function_register("redLight", redLight);
	MSG_function_register("toManual", toManual);



	/*manual*/

	sprintf(str, "manual");
	printf("Creating %s\n", str);
	char **argvc0 = xbt_new(char *, 1);
	argvc0[0] = NULL;
	p = MSG_process_create_with_arguments(str, manual, NULL, MSG_get_host_by_name(str), argc, argvc0);

	if( p == NULL )
	{
		printf("Error en %s\n", str);
		exit(0);
	}


	/*toAutomatic*/

	sprintf(str, "toAutomatic");
	printf("Creating %s\n", str);
	char **argvc1 = xbt_new(char *, 1);
	argvc1[0] = NULL;
	p = MSG_process_create_with_arguments(str, toAutomatic, NULL, MSG_get_host_by_name(str), argc, argvc1);

	if( p == NULL )
	{
		printf("Error en %s\n", str);
		exit(0);
	}


	/*greenLight*/

	sprintf(str, "greenLight");
	printf("Creating %s\n", str);
	char **argvc2 = xbt_new(char *, 1);
	argvc2[0] = NULL;
	p = MSG_process_create_with_arguments(str, greenLight, NULL, MSG_get_host_by_name(str), argc, argvc2);

	if( p == NULL )
	{
		printf("Error en %s\n", str);
		exit(0);
	}


	/*yellowLight*/

	sprintf(str, "yellowLight");
	printf("Creating %s\n", str);
	char **argvc3 = xbt_new(char *, 1);
	argvc3[0] = NULL;
	p = MSG_process_create_with_arguments(str, yellowLight, NULL, MSG_get_host_by_name(str), argc, argvc3);

	if( p == NULL )
	{
		printf("Error en %s\n", str);
		exit(0);
	}


	/*redLight*/

	sprintf(str, "redLight");
	printf("Creating %s\n", str);
	char **argvc4 = xbt_new(char *, 1);
	argvc4[0] = NULL;
	p = MSG_process_create_with_arguments(str, redLight, NULL, MSG_get_host_by_name(str), argc, argvc4);

	if( p == NULL )
	{
		printf("Error en %s\n", str);
		exit(0);
	}


	/*toManual*/

	sprintf(str, "toManual");
	printf("Creating %s\n", str);
	char **argvc5 = xbt_new(char *, 1);
	argvc5[0] = NULL;
	p = MSG_process_create_with_arguments(str, toManual, NULL, MSG_get_host_by_name(str), argc, argvc5);

	if( p == NULL )
	{
		printf("Error en %s\n", str);
		exit(0);
	}


	/* Controller */

	sprintf(str, "cont-0");
	printf("Creating %s\n", str);
	char **argvc6 = xbt_new(char *, 1);
	argvc6[0] = NULL;
	p = MSG_process_create_with_arguments(str, controller, NULL, MSG_get_host_by_name(str), argc, argvc6);

	if( p == NULL )
	{
		printf("Error en %s\n", str);
		exit(0);
	}

	
	/* externalTransition */

	sprintf(str, "externalTransition");
	printf("Creating %s\n", str);
	char **argvc7 = xbt_new(char *, 1);
	argvc7[0] = NULL;
	p = MSG_process_create_with_arguments(str, externalTransition, NULL, MSG_get_host_by_name(str), argc, argvc7);

	if( p == NULL )
	{
		printf("Error en %s\n", str);
		exit(0);
	}

	return;

}

int main(int argc, char *argv[])
{
	msg_error_t res = MSG_OK;

	if (argc < 2)
	{
		printf("Usage: %s platform_devs.xml", argv[0]);
		exit(1);
	}
	MSG_init(&argc, argv);

	test_all(argv[1]);
	res = MSG_main();
	if (res == MSG_OK) return 0;
	else return 1; 
}
