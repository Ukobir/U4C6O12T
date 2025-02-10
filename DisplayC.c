/*
 * Por: Wilton Lacerda Silva
 *    Comunicação serial com I2C
 *
 * Uso da interface I2C para comunicação com o Display OLED
 *
 * Estudo da biblioteca ssd1306 com PicoW na Placa BitDogLab.
 *
 * Uso dos demais periféricos para contemplar a atividade desta aula.
 * 
 * Por: Leonardo Romão
 * DATA: 09/02/25
 */

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include "inc/ws2812.c"
#include "pico/bootrom.h"
#include "inc/func.c"

// variaveis globais
const uint reset_pin = 22;
bool reset = 0;
const uint ledRGB[3] = {11, 12, 13}; // Red=13, Blue=12, Green=11
const uint botaoA_pin = 5;           // Botão A = 5, Botão B = 6 , BotãoJoy = 22
const uint botaoB_pin = 6;           // Botão A = 5, Botão B = 6 , BotãoJoy = 22
// variaveis de controle
static volatile uint32_t last_time = 0;
bool ligaledA = true;
bool ligaledB = true;
uint control = 0;

// função IRQ
static void gpio_irq_handler(uint gpio, uint32_t events);

ssd1306_t ssd; // Inicializa a estrutura do display

int main()
{
  stdio_init_all();
  initButton(reset_pin);  // Função que configura o botão Joystick
  initButton(botaoA_pin); // Função que configura o botão A
  initButton(botaoB_pin); // Função que configura o botão A
  initLEDs(ledRGB);       // Função que configura os leds

  gpio_set_irq_enabled_with_callback(reset_pin, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
  gpio_set_irq_enabled_with_callback(botaoA_pin, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
  gpio_set_irq_enabled_with_callback(botaoB_pin, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

  ssd1306_t ssd; // Inicializa a estrutura do display

  initDisplay(&ssd); // Inicialização do Display ssd1306

  // Inicializa matriz de LEDs NeoPixel.
  npInit(LED_PIN);
  desenhaMatriz(desliga_LED, 0, 0.8); // Função para desligar todos os LEDs ws2812

  bool cor = true;
  while (true)
  {
    cor = !cor;
    // Atualiza o conteúdo do display com textos
    telaInicial(&ssd, cor);


    if (control == 1 && ligaledA == false)
    {
      printf("Ligou o LED Verde\n");
      ligouVerde(&ssd, cor); // Função de desenho da tela
      control = 0;
      sleep_ms(3e3); // Tempo para o usuário ler o display ssd1306
    }
    else if(control == 1 && ligaledA == true){
      printf("Desligou o LED Verde\n");
      desligouVerde(&ssd, cor); // Função de desenho da tela
      control = 0;
      sleep_ms(3e3); // Tempo para o usuário ler o display ssd1306
    }
    else if (control == 2 && ligaledB == false)
    {
      printf("Ligou o LED Azul\n");
      ligouAzul(&ssd, cor);
      control = 0;
      sleep_ms(3e3); // Tempo para o usuário ler o display ssd1306
    }
    else if(control == 2 && ligaledB == true){
      printf("Desigou o LED Azul\n");
      desligouAzul(&ssd, cor);
      control = 0;
      sleep_ms(3e3); // Tempo para o usuário ler o display ssd1306
    }

    if (reset)
    {
      // Limpa o display. O display inicia com todos os pixels apagados.
      ssd1306_fill(&ssd, false);
      ssd1306_send_data(&ssd);
      printf("HABILITANDO O MODO GRAVAÇÃO");
      reset_usb_boot(0, 0);
    }
    else if (stdio_usb_connected())
    { // Certifica-se de que o USB está conectado
      char c;
      printf("Digite um Caractere ou Numero:\n");
      if (scanf("%c", &c) == 1)
      {
        // Exibe o caractere recebido pela serial e pelo display caso seja um número exibido tambem pela matriz de led
        printf("Caractere recebido: %c\n", c);
        telaMatrix(c, ssd); //Funçao responsável por ligar o display e a matriz de led
        sleep_ms(3e3);
      }
    }

    sleep_ms(5e2);
  }
}

// Interrupção para os botões A, B e reset
void gpio_irq_handler(uint gpio, uint32_t events)
{
  uint32_t current_time = to_ms_since_boot(get_absolute_time()); // Espera 200 ms para contornar o debouncing effect
  if (gpio == botaoA_pin && current_time - last_time > 2e2)      // Ao apertar o Botão A o led verde liga
  {
    
    gpio_put(ledRGB[0], ligaledA);
    ligaledA = !ligaledA;
    control = 1;

    last_time = current_time;
  }
  else if (gpio == botaoB_pin && current_time - last_time > 2e2) // Ao apertar o Botão B o led azul liga
  {
    gpio_put(ledRGB[1], ligaledB);
    ligaledB = !ligaledB;
    control = 2;

    last_time = current_time;
  }
  if (gpio == reset_pin) // Ao apertar o butão do joystick Entra no modo Bootloader
  {
    reset = 1;
  }
}
