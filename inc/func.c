// Código de algumas funções utilizadass
#include <stdio.h>
#include <stdlib.h>


//Funçao responsável por ligar o display e a matriz de led
void telaMatrix(char c, ssd1306_t ssd)
{
    switch (c)
    {
    case '0':
        telaChar(&ssd, c);//Função que imprime o charactere no display
        desenhaMatriz(numeros[0], 0, 1);
        break;
    case '1':
        telaChar(&ssd, c);
        desenhaMatriz(numeros[1], 0, 1);
        break;
    case '2':
        telaChar(&ssd, c);
        desenhaMatriz(numeros[2], 0, 1);
        break;
    case '3':
        telaChar(&ssd, c);
        desenhaMatriz(numeros[3], 0, 1);
        break;
    case '4':
        telaChar(&ssd, c);
        desenhaMatriz(numeros[4], 0, 1);
        break;
    case '5':
        telaChar(&ssd, c);
        desenhaMatriz(numeros[5], 0, 1);
        break;
    case '6':
        telaChar(&ssd, c);
        desenhaMatriz(numeros[6], 0, 1);
        break;
    case '7':
        telaChar(&ssd, c);
        desenhaMatriz(numeros[7], 0, 1);
        break;
    case '8':
        telaChar(&ssd, c);
        desenhaMatriz(numeros[8], 0, 1);
        break;
    case '9':
        telaChar(&ssd, c);
        desenhaMatriz(numeros[9], 0, 1);
        break;

    default:
        telaChar(&ssd, c);
        break;
    }
}