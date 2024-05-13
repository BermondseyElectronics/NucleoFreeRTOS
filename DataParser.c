// Online C compiler to run C program online
#include <stdio.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

// Define maximum buffer size
#define BUFFER_SIZE 20

// Circular buffer for storing incoming data
uint8_t buffer[BUFFER_SIZE];
volatile int head = 0;
volatile int tail = 0;

// UART interrupt handler prototype and UART_ReadDataRegister (platform-specific)
void UART_IRQHandler(void);
uint8_t UART_ReadDataRegister();

// Dispatcher and callback prototypes
void Dispatcher(uint8_t command,uint8_t *pData);
void ResetBuffer(uint8_t *pData);
void PrintRecord(uint8_t *pData);

// Command structure for dispatcher
typedef struct {
    char command;
    void (*function)(uint8_t *pData);
} Command;

Command commands[] = {
    {'r', ResetBuffer},
    {'p', PrintRecord}
};

// State machine states
typedef enum 
{
    WAIT_FOR_PERCENT,
    WAIT_FOR_LENGTH,
    READ_MESSAGE
} State;

volatile State current_state = WAIT_FOR_PERCENT;
volatile int expected_length = 0;
volatile int message_length = 0;
uint8_t message_buffer[BUFFER_SIZE];
uint8_t UartTestByte_U8 = 0;

uint8_t UART_ReadDataRegister() 
{
    return UartTestByte_U8;
}

// UART interrupt handler implementation
void UART_IRQHandler(void) 
{
    uint8_t received_byte = UART_ReadDataRegister(); // Pseudo-function

    // Store byte in circular buffer
    buffer[tail] = received_byte;
    tail = (tail + 1) % BUFFER_SIZE;
}

// Function to process received data
void ProcessData(void) 
{
    while (head != tail) 
    {
        uint8_t data = buffer[head];
        head = (head + 1) % BUFFER_SIZE;

        switch (current_state) 
        {
            case WAIT_FOR_PERCENT:
            {
                if (data == '%') 
                {
                    current_state = WAIT_FOR_LENGTH;
                }
            }
            break;
            case WAIT_FOR_LENGTH:
            {
                expected_length = data;
                message_length = 0;
                current_state = READ_MESSAGE;
            }
            break;
            case READ_MESSAGE:
            {
                message_buffer[message_length++] = data;
                if (message_length == expected_length) 
                {
                    Dispatcher(message_buffer[0],&message_buffer[1]); // Dispatch based on first byte
                    current_state = WAIT_FOR_PERCENT; // Reset state
                }
            }
            break;
        }
    }
}

// Dispatcher function
void Dispatcher(uint8_t command, uint8_t * pData) 
{
    for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) 
    {
        if (commands[i].command == command) 
        {
            commands[i].function(pData);
            return;
        }
    }
    printf("Not existing Function\n");
}

// Callback functions
 void ResetBuffer(uint8_t * pData) 
 {
     printf("Reset Buffer\n");
     head = 0;
     tail = 0;
 }

 void PrintRecord(uint8_t * pData) 
 {
     printf("Record:%s\n",pData);
 }

 int main(void) 
 {
     
     // Initialization code here
     UartTestByte_U8 = 0;
     UART_IRQHandler();
     UartTestByte_U8 = '%';
     UART_IRQHandler();
     UartTestByte_U8 = 1;
     UART_IRQHandler();
     UartTestByte_U8 = 'r';
     UART_IRQHandler();
     
     ProcessData(); // Continuously process data


     // Initialization code here
     UartTestByte_U8 = 0;
     UART_IRQHandler();
     UartTestByte_U8 = '%';
     UART_IRQHandler();
     UartTestByte_U8 = 5;
     UART_IRQHandler();
     UartTestByte_U8 = 'p';
     UART_IRQHandler();
     UartTestByte_U8 = 'T';
     UART_IRQHandler();
     UartTestByte_U8 = 'E';
     UART_IRQHandler();
     UartTestByte_U8 = 'S';
     UART_IRQHandler();
     UartTestByte_U8 = 'T';
     UART_IRQHandler();
     
     ProcessData(); // Continuously process data

     return 0;
 }
