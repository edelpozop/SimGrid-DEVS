#include "./simulation.h"


void test_all(char *file)
{
	int argc;
	char str[50];
	msg_process_t p;

	MSG_create_environment(file);
	MSG_function_register("redLight", redLight);
	MSG_function_register("greenLight", greenLight);
	MSG_function_register("yellowLight", yellowLight);
	MSG_function_register("controller", controller);



	sprintf(str, "redLight");

	printf("Creando    %s    \n", str);

	argc = 0;
	char **argvc0 = xbt_new(char *, 1);

	argvc0[0] = NULL;

	p = MSG_process_create_with_arguments(str, redLight, NULL, MSG_get_host_by_name(str), argc, argvc0);
	if(p == NULL)
	{
		printf("Error en ......... %s", str);
		printf("\n");
		exit(0);
	}


	sprintf(str, "greenLight");

	printf("Creando    %s    \n", str);

	argc = 0;
	char **argvc1 = xbt_new(char *, 1);

	argvc1[0] = NULL;

	p = MSG_process_create_with_arguments(str, greenLight, NULL, MSG_get_host_by_name(str), argc, argvc1);
	if(p == NULL)
	{
		printf("Error en ......... %s", str);
		printf("\n");
		exit(0);
	}


	sprintf(str, "yellowLight");

	printf("Creando    %s    \n", str);

	argc = 0;
	char **argvc2 = xbt_new(char *, 1);

	argvc2[0] = NULL;

	p = MSG_process_create_with_arguments(str, yellowLight, NULL, MSG_get_host_by_name(str), argc, argvc2);
	if(p == NULL)
	{
		printf("Error en ......... %s", str);
		printf("\n");
		exit(0);
	}


	//Controller

	sprintf(str, "cont-0");

	printf("Creando    %s    \n", str);

	argc = 0;
	char **argvc3 = xbt_new(char *, 1);

	
	argvc3[0] = NULL;

	p = MSG_process_create_with_arguments(str, controller, NULL, MSG_get_host_by_name(str), argc, argvc3);

	if(p == NULL)
	{
		printf("Error en ......... cont-0");
		printf("\n");
		exit(0);
	}

	return;
}




int main(int argc, char *argv[])
{
	msg_error_t res = MSG_OK;

	if (argc < 2)
	{
		printf("Usage: %s platform_file", argv[0]);
		printf("\n");
		exit(1);
	}

	MSG_init(&argc, argv);


	test_all(argv[1]);
	res = MSG_main();


	if (res == MSG_OK) return 0;
	else return 1;
}
