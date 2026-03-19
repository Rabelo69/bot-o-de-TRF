#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#define TRF_3_botao       0        
#define TRF_3_ledCarros   2   
#define TRF_3_ledPedestre 4 


#define TRF_3_tempoVerdeCarros   13   
#define TRF_3_tempoVerdePedestre 8    
#define TRF_3_tempoCooldown      10
#define TRF_3_reducaoBotao       1

volatile int TRF_3_cooldown = 0;


static void IRAM_ATTR TRF_3_isr_handler(void* arg) {
if (TRF_3_cooldown == 0) {
TRF_3_cooldown = TRF_3_tempoCooldown; 
}
}

void app_main(void) {

gpio_reset_pin(TRF_3_botao);
gpio_set_direction(TRF_3_botao, GPIO_MODE_INPUT);
gpio_set_pull_mode(TRF_3_botao, GPIO_PULLDOWN_ONLY);
gpio_set_intr_type(TRF_3_botao, GPIO_INTR_POSEDGE);
gpio_install_isr_service(0);
gpio_isr_handler_add(TRF_3_botao, TRF_3_isr_handler, NULL);


gpio_reset_pin(TRF_3_ledCarros);
gpio_set_direction(TRF_3_ledCarros, GPIO_MODE_OUTPUT);

gpio_reset_pin(TRF_3_ledPedestre);
gpio_set_direction(TRF_3_ledPedestre, GPIO_MODE_OUTPUT);

int TRF_3_contador;

while (1) {

gpio_set_level(TRF_3_ledCarros, 1);
gpio_set_level(TRF_3_ledPedestre, 0);

TRF_3_contador = TRF_3_tempoVerdeCarros;
while (TRF_3_contador > 0) {
vTaskDelay(1000 / portTICK_PERIOD_MS);
TRF_3_contador--;

if (TRF_3_cooldown > 0) {
TRF_3_cooldown--;
if (TRF_3_contador > 2) {
TRF_3_contador -= TRF_3_reducaoBotao; 
}
}
}

    
gpio_set_level(TRF_3_ledCarros, 0);
gpio_set_level(TRF_3_ledPedestre, 1);

vTaskDelay(TRF_3_tempoVerdePedestre * 1000 / portTICK_PERIOD_MS);
}
}