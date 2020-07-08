#ifndef __CLI_INTERFACE_H__
#define __CLI_INTERFACE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>    // strtok 함수가 선언된 헤더 파일
#include <stdint.h>
//#include <ctype.h>     // isdigit

#define PARSER_ERROR                        2

#define LIST_NOT_FOUND                      1
                    
#define CLI_END                             0
                    
#define CLI_MATCH                           0
                    
#define D_DELIMITER                         " "

#define NUMBER_OF_DELIMITER_VALUE           20


int parser(char *cmd);

#endif /* __CLI_INTERFACE_H__ */
