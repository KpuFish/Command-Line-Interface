/**
 * Module      : Command Line Interface
 * File Name   : CLI_INTERFACE.c
 * Description : Command Parser
 * Who         : Deckster
 **/
#include "usart.h"
#include "../../Core/Inc/cli_interface.h"

/* CBF		     */
int cbf_test(int argc, char *argv[]);
	
/* CLI Def       */
typedef int (*CBF)(int argc, char *argv[]);
typedef struct 
{
    char *name;
    CBF   func;
} CMD_LIST;

/* -------------------------
 * CLI LIST
 * -------------------------
 * (1) - Register here CallBack Function that you want to be added
 * (2) - Define callback function
 * (3) - Fill in the callback function
 */
const CMD_LIST cmd_list[] =
{
    {"test" , cbf_test},
    {(char*)NULL, (CBF)NULL}
};

/* CLI PARSER */
int parser(char *cmd)
{
    int    argc    = 0;
    char  *argv[NUMBER_OF_DELIMITER_VALUE];
    rUART *s_uart = GetUartStructure();

    if (s_uart == NULL) {
        printf("Uart Buffer Error \n");
        return PARSER_ERROR;
    }

    argv[argc++] = strtok(cmd, D_DELIMITER);
	
	//----------------------------------------
    // SPLIT THE UART RX STRING
    //----------------------------------------
    while (1) {
        argv[argc] = strtok(NULL, D_DELIMITER);
        
        if (argv[argc] == NULL) {
            break;
        }
        argc++;
    }

    //----------------------------------------
    // FIND THE MATCHED STRING
    //----------------------------------------
    for (int cnt = 0; cmd_list[cnt].name != NULL; cnt++) {
        if (strcmp(cmd_list[cnt].name, argv[0]) == CLI_MATCH) {
            return cmd_list[cnt].func(argc, argv);
        }
    }
    return LIST_NOT_FOUND;
}

/**
  * @brief  call back function
  * @retval int
  */
int cbf_test(int argc, char *argv[])
{
	printf("argv : %s \r\n", argv[0]);
	printf("argc : %d , argv : %s \r\n", argc, argv[1]);
	printf("TEST \r\n");
	return 0;
}
