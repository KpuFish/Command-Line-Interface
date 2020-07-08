/**
 * Module      : APP_MAIN
 * File Name   : APP_MAIN.C
 * Description : APP
 * Who         : Deckster
 **/

#include "string.h"
#include "usart.h"
#include "../../Core/Inc/cli_interface.h"
#include "../../Utill/common.h"


/**
  * @brief  The app main entry point.
  * @retval None
  */
void app_main(void)
{
	rUART *s_uart = GetUartStructure();

	if (s_uart == NULL) {
		printf("Failed to Setting UART \n");
	}

	HAL_UART_Receive_IT(&huart3, (uint8_t *)&s_uart->isr_rx_data, 1);

	memset((uint8_t*)&s_uart->rx_data[0], RESET, sizeof(s_uart));

	printf("System Start... \r\n");

	while (1)
	{
		if (IsRxDone() == SET) {
			ResetRxDone();

			if (parser((char*)&s_uart->rx_data[0]) == LIST_NOT_FOUND) {
				printf("List Not Found \r\n");
			}
			memset((rUART*)s_uart, RESET, sizeof(rUART));
		}
	}
}
