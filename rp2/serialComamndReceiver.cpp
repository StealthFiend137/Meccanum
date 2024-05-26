#pragma once

#include <functional>
#include <hardware/uart.h>

class SerialCommandReceiver
{
private:

    uart_inst_t* _uart_instance;

    void uart_rx_isr(uart_inst_t* uart_inst)
    {
        
        while (uart_is_readable(uart_inst)) {
            uint8_t ch = uart_getc(uart_inst);
            // Can we send it back?
            // if (uart_is_writable(uart_inst)) {
            //     // Change it slightly first!
            //     ch++;
            //     uart_putc(uart_inst, ch);
            // }
            // chars_rxed++;
        }
    }

    static void uart0_rx_isr()
    {
        _serialCommandReceiver_instance->uart_rx_isr(uart0);
    };

    static void uart1_rx_isr()
    {
        _serialCommandReceiver_instance->uart_rx_isr(uart1);
    };

public:

    static inline SerialCommandReceiver* _serialCommandReceiver_instance = nullptr;

    SerialCommandReceiver(uart_inst_t* uart_inst, Chassis* chassis) : _uart_instance(uart_inst)
    {
    };

    void command_receiver_init(uint baud_rate, uint tx_pin, uint rx_pin, uint16_t movement_timeout_ms)
    {
        // Set up our UART with a basic baud rate.
        uart_init(_uart_instance, 2400);
        gpio_set_function(tx_pin, GPIO_FUNC_UART);
        gpio_set_function(rx_pin, GPIO_FUNC_UART);

        // Actually, we want a different speed
        // The call will return the actual baud rate selected, which will be as close as possible to that requested
        int __unused actual = uart_set_baudrate(_uart_instance, baud_rate);

        // Set UART flow control CTS/RTS, we don't want these, so turn them off
        uart_set_hw_flow(_uart_instance, false, false);

        #define DATA_BITS 8
        #define STOP_BITS 1
        #define PARITY    UART_PARITY_NONE
        uart_set_format(_uart_instance, DATA_BITS, STOP_BITS, PARITY);

        // Turn off FIFO's - we want to do this character by character
        //TODO: see if we can do this more efficently with the FIFO.
        uart_set_fifo_enabled(_uart_instance, false);

        // Set up the RX handler
        _serialCommandReceiver_instance = this;

        int UART_IRQ = _uart_instance == uart0 ? UART0_IRQ : UART1_IRQ;
        irq_handler_t uart_isr = _uart_instance == uart0 ? uart0_rx_isr : uart1_rx_isr;
        irq_set_exclusive_handler(UART_IRQ, uart_isr);
        irq_set_enabled(UART_IRQ, true);
    };
};
