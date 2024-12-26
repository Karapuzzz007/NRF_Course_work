#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/spi.h>
#include <stdio.h>

#include "NRF24_master/RF24.h"

RF24 radio;
uint8_t adress[5] = {0xB4,0xB5,0xB6,0xB7,0xF1};


void setup_clock(){
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOE);
}

void setup_gpio (){
   gpio_mode_setup(GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO9|GPIO10|GPIO11|GPIO12|GPIO13|GPIO14); 
}

void setup_nrf(){

for(volatile uint32_t i=0; i<1000000; i++);

    radio.begin();                          // активировать модуль
    radio.setAutoAck(1);                    // режим подтверждения приёма, 1 вкл 0 выкл
    radio.setRetries(0, 3);                 // (время между попыткой достучаться, число попыток)
    radio.enableAckPayload();               // разрешить отсылку данных в ответ на входящий сигнал
    radio.setPayloadSize(32);               // размер пакета, в байтах
    radio.openWritingPipe(adress);          // открываем канал для передачи данных
    radio.setChannel(0x10);                 // выбираем канал (в котором нет шумов!)

    radio.setPALevel (RF24_PA_MAX);         // уровень мощности передатчика. 
                                            //На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
    radio.setDataRate (RF24_250KBPS);       // скорость обмена. 
                                            //На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS

    radio.powerUp();                        // начать работу
    radio.stopListening();                  // не слушаем радиоэфир, мы передатчик

        gpio_toggle(GPIOE, GPIO9); 
}

void setup(){
    setup_clock();
    setup_gpio();
    setup_nrf();
}

void loop () {
    uint8_t tx_data[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    gpio_toggle(GPIOE, GPIO14);
    radio.write(tx_data, 8);
    for(volatile uint32_t i=0; i<100000; i++);
}

int main () {
    setup();
    
    while (true) {
        loop();
    }   

}

