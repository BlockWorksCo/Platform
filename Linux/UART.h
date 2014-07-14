

#ifndef __UART_H__
#define __UART_H__


#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>



//
//
//
template <  typename TxQueueType, 
            typename RxQueueType,
            uint32_t BaudRate>
class UART
{

public:

    //
    // Initialise the queue.
    //
    UART(char* portName, RxQueueType& _rxQueue, TxQueueType& _txQueue) :
        txQueue(_txQueue),
        rxQueue(_rxQueue),
        queueTooSmallFlag(false)
    {
        struct termios  options;

        printf("hello world\n");

        fd = open(portName, O_RDWR | O_NOCTTY | O_NDELAY);
        if(fd == -1)
        {
            perror("open_port: Unable to open:");
        }
        else
        {
            fcntl(fd, F_SETFL, 0);
            printf("hi again\n");
        }

        tcgetattr(fd, &options);

        cfsetispeed(&options, B115200);
        cfsetospeed(&options, B115200);
        options.c_cflag |= (CLOCAL | CREAD);
        options.c_cflag &= ~PARENB;
        options.c_cflag &= ~CSTOPB;
        options.c_cflag &= ~CSIZE;
        options.c_cflag |= CS8;
        options.c_cflag &= ~( ICANON | ECHO | ECHOE |ISIG );
        options.c_iflag &= ~(IXON | IXOFF | IXANY );
        options.c_oflag &= ~OPOST;

        tcsetattr(fd, TCSANOW, &options);
    }



    //
    // Process the master command queue.
    //
    bool ProcessQueue()
    {
        bool        dataAvailableFlag   = false;
        uint8_t     data    = txQueue.Get(dataAvailableFlag);

        if(dataAvailableFlag == true)
        {
            //
            // Let the ISR empty the queue.
            //
            write(fd, &data,1);
            printf("%c",data);

            return true;
        }

        return false;
    }


    //
    //
    //
    void ISR(void)
    {
        // Rx buffer not empty.
        //if( USART_GetITStatus(usart, USART_IT_RXNE) )
        {
            char            buffer[10];
            int             bytes;
            char            c;
            int             x;
            char*           bufptr;
            
            fcntl(fd, F_SETFL, FNDELAY);
            bytes = read(fd, bufptr, sizeof(buffer));
            printf("number of bytes read is %d\n", bytes);
            perror ("read error:");

            for (x = 0; x < 10 ; x++)
            {
                c = buffer[x];
                printf("%d  ",c);
            }
            close(fd);

            //
            // Data waiting in the UART Rx buffer, get it and put it in the queue
            // for the AppLevel to process.
            //
            char    receivedByte = 0;
            rxQueue.Put(receivedByte, queueTooSmallFlag);
        }
    }

private:    

    //
    //
    //
    bool            queueTooSmallFlag;
    TxQueueType&    txQueue;
    RxQueueType&    rxQueue;
    int             fd;

};



#endif







