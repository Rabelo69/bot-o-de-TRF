#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

#define BOTAO 0        
#define LED_CARROS 2  
#define LED_PEDESTRE 4

#define TEMPO_VERDE_CARROS 10
#define TEMPO_VERDE_PEDESTRE 5
#define TEMPO_COOLDOWN 8   

volatile int cooldown = 0;

void app_main(void) {
gpio_reset_pin(BOTAO);
gpio_set_direction(BOTAO, GPIO_MODE_INPUT);
gpio_set_pull_mode(BOTAO, GPIO_PULLDOWN_ONLY);

gpio_reset_pin(LED_CARROS);
gpio_set_direction(LED_CARROS, GPIO_MODE_OUTPUT);

gpio_reset_pin(LED_PEDESTRE);
gpio_set_direction(LED_PEDESTRE, GPIO_MODE_OUTPUT);

int contador;

while (1) {
        
gpio_set_level(LED_CARROS, 0);
gpio_set_level(LED_PEDESTRE, 1);
printf("Carros liberados (verde/amarelo), pedestres vermelho.\n");

contador = TEMPO_VERDE_CARROS;

while (contador > 0) {
            
if (gpio_get_level(BOTAO) == 1) {
if (cooldown == 0 && contador > 3) {
contador = 3; 
cooldown = TEMPO_COOLDOWN;
printf("Botão pressionado: reduzindo tempo do semáforo!\n");
} else {
printf("Botão em cooldown, aguarde %d segundos...\n", cooldown);
}
}
vTaskDelay(1000 / portTICK_PERIOD_MS);
contador--;           
if (cooldown > 0) {
cooldown--;
}
}        
gpio_set_level(LED_CARROS, 1);
gpio_set_level(LED_PEDESTRE, 0);
printf("Carros vermelho, pedestres verde!\n");

vTaskDelay(TEMPO_VERDE_PEDESTRE * 1000 / portTICK_PERIOD_MS);
}
}
