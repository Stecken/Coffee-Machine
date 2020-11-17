/*
 * CoffeInC.c
 *
 * Created: 12/11/2020 16:43:05
 * Author : T-Gamer
 */ 

#include "def_principais.h"
#include "LCD.h"
#include "texts.h"
#define START    PINC0
#define BMAIS    PINC1
#define BMENOS   PINC2
#define ENTER    PINC3
#define PRESENCE PINC4
#define __DELAY_BACKWARD_COMPATIBLE__

void mandaString();
void inicializa();
void inicio();
void configQuantSystem2(unsigned int min, unsigned int lim, int *pointVar, unsigned char position, void (*func)(int, unsigned char));
void configQuantCopo();
void configTypeCopo();
void presence(const char *c);
void consumirCafeMaquina();
void testeCoffe(unsigned int sub);
void colocarCafeMaquina();
void leiteConfig();
void acucarConfig();
void chocolateConfig();
void debbug(unsigned int sub);
void sucessoCafe(int copo);
void copoPronto();
void finalizar();
void Activity(const char *c, int i, unsigned char position);
void Animation(unsigned char position, int time, unsigned int repete);
void mandaCaracterNovo(const unsigned char *text);
void Desligar();

int cafeColocar = 1;
int quantCoffe = 20; // quantidade de grãos na cafeteira
int quantCopo = 1;
int typeCopo[] = {1, 1};
int leiteBool[] = {0, 0};
int acucarBool[] = {0, 0};
int chocolateBool[] = {0, 0};
unsigned int estado = 0;
static unsigned int count = 0;
// Tipos de Copo: Pequeno, Médio ou Grande.
/*
	PINC 0  -> Start;
	PINC 1  -> botão +;
	PINC 2  -> botão -;
	PINC 3  -> ;
*/

/*
	Pequeno -> 2 grãos
	Médio -> 3 grãos
	Grande -> 4 grãos
*/

void my_delay_ms(int ms){
	while (0 < ms){
		_delay_ms(1);
		--ms;
	}
}

int main(void) {
	inicializa();
	cmd_LCD(0x01,0); // limpa lcd
	while (1) {
		switch (estado){
			case 0: {
				inicio();
				estado = 1;
				break;
			}
			case 1: {
				configQuantCopo();
				estado = 2;
				break;
			}
			case 2: {
				configTypeCopo();
				estado = 3;
				break;
			}
			case 3: {
				presence(msgPresenceEnt);
				estado = 4;
				break;
			}
			case 4: {
				consumirCafeMaquina();
				estado = 5;
				break;
			}
			case 5: {
				leiteConfig();
				estado = 6;
				break;
			}
			case 6: {
				acucarConfig();
				estado = 7;
				break;
			}
			case 7: {
				chocolateConfig();
				estado = 8;
				break;
			}
			case 8: {
				copoPronto();
				estado = 9;
				break;
			}
			case 9: {
				presence(msgPresenceRet);
				estado = 10;
				break;
			}
			case 10: {
				finalizar();
				estado = 11;
				break;
			}
			case 11: {
				Desligar();
				estado = 0;
				break;
			}
		}	
	}
	return 0;
}

void inicializa(){
	DDRB = 255;
	DDRC = 0;
	DDRD = 255;
	PORTB = 0;
	PORTC = 255;
	PORTD = 0;
	inic_LCD_4bits();
	cmd_LCD(0x01,0);
	mandaCaracterNovo(carac1);
}

void mandaString(char text[16]){
	for(int i = 0; text[i] != 0; i++){
		cmd_LCD(text[i], 1);
	}
}

void inicio(){
	cmd_LCD(0x01,0); // limpa lcd
	cmd_LCD(0x14, 0); // Segunda linha, posição 0
	for(int i = 0; tst_bit(PINC, 0); i++){
		cmd_LCD(0x80, 0); //Primeira linha, posição 0
		escreve_LCD_Flash(msgInicio);
		cmd_LCD(0xC0, 0); // Segunda linha, posição 0
		escreve_LCD_Flash(msgInicio2);
		cmd_LCD(0x00, 1);
		cmd_LCD(0x1C, 0); // move para a direita.
		_delay_ms(150);
	}
	cmd_LCD(0x80, 0); // Primeira linha, posição 0
	cmd_LCD(0x01,0); // limpa lcd
}

void configQuantSystem2(unsigned int min, unsigned int lim, int *pointVar, unsigned char position, void (*func)(int, unsigned char)){
	(*func)(*pointVar, position);
	while(tst_bit(PINC, ENTER)){
		if (!tst_bit(PINC, BMAIS)){
			if(*pointVar == lim){
				*pointVar = min;
				(*func)(*pointVar, position);
			}
			else if (*pointVar < lim){
				*pointVar = *pointVar + 1;
				(*func)(*pointVar, position);
			}
			_delay_ms(250);
		}
		else if(!tst_bit(PINC, BMENOS)){
			if(*pointVar != min){
				*pointVar = *pointVar - 1;
				(*func)(*pointVar, position);
			}
			_delay_ms(250);
		}
	}
	(*func)(*pointVar, position);
}

void configQuantCopo(){
	cmd_LCD(0x01,0); // limpa lcd
	cmd_LCD(0x80, 0);
	escreve_LCD_Flash(msgConfigQuantCopo);
	cmd_LCD(0xC0, 0);
	escreve_LCD_Flash(msgConfigQuantCopo1);
	configQuantSystem2(1, 2, &quantCopo, 0xCF, conversorNumeros2);
}

void configTypeCopo(){
	cmd_LCD(0x01,0); // limpa lcd
	cmd_LCD(0x80, 0);
	escreve_LCD_Flash(msgConfigTypeCopo);
	cmd_LCD(0xC0, 0);
	escreve_LCD_Flash(msgConfigTypeCopo1);
	_delay_ms(1500);
	cmd_LCD(0x01,0); // limpa lcd
	for(int i = 0; i < quantCopo; ++i){
		cmd_LCD(0x80, 0);
		escreve_LCD_Flash(msgConfigTypeCop2);
		cmd_LCD(0xC0, 0);
		escreve_LCD_Flash(msgConfigTypeCop);
		configQuantSystem2(1, 3, &(typeCopo[i]), 0xC0, selectType);
		cmd_LCD(0x01,0); // limpa lcd
		_delay_ms(500); // tempo para n crashar
	}
	if(quantCopo == 1){
		typeCopo[1] = 0;
	}
}

void presence(const char *c){
	for(int i = 0; i < quantCopo; ++i){
		cmd_LCD(0x01,0); // limpa lcd
		cmd_LCD(0x80, 0);
		escreve_LCD_Flash(msgPresence);
		escreve_LCD_Flash(c);
		cmd_LCD(0xC0, 0);
		escreve_LCD_Flash(msgPresenceCopText);
		conversorNumeros2(i + 1, 0xC8);
		while(tst_bit(PINC, PRESENCE)){
			Animation(0xC9, 75, 1);
		}
		cmd_LCD(0x01,0); // limpa lcd
		escreve_LCD_Flash(msgPresenceStatus);
		conversorNumeros2(i + 1, 0x85);
		if(c == msgPresenceEnt){
			escreve_LCD_Flash(msgPresenceStatusEnt);
		}
		else{
			escreve_LCD_Flash(msgPresenceStatusRet);
		}
		_delay_ms(1500); // tempo para n crashar
	}
}

void consumirCafeMaquina(){
	_delay_ms(100);
	for(int i = 0; i < quantCopo; ++i){
		if(typeCopo[i] != 0){
			if(typeCopo[i] == 1){
				quantCoffe -= 2;
				testeCoffe(2);
				Activity(msgActivityCafe, i, 0xC9);
				sucessoCafe(i);
			}
			else if (typeCopo[i] == 2){
				quantCoffe -= 3;
				testeCoffe(3);
				Activity(msgActivityCafe, i, 0xC9);
				sucessoCafe(i);
			}
			else if (typeCopo[i] == 3){
				quantCoffe -= 4;
				testeCoffe(4);
				Activity(msgActivityCafe, i, 0xC9);
				sucessoCafe(i);
			}
		}
		_delay_ms(500);
	}
}

void leiteConfig(){
	cmd_LCD(0x01,0); // limpa lcd
	for(int i = 0; i < quantCopo; ++i){
		cmd_LCD(0x80, 0);
		escreve_LCD_Flash(msgConfigLeite);
		conversorNumeros2(i + 1, 0x8E);
		cmd_LCD(0xC0, 0);
		escreve_LCD_Flash(msgResp);
		configQuantSystem2(0, 1, &(leiteBool[i]), 0xC0, respYesOrNo);
		cmd_LCD(0x01,0); // limpa lcd
		if (leiteBool[i] == 0){
			Activity(msgActivityLeie, i, 0xC9);
		}
		_delay_ms(500); // tempo para n crashar
	}
}

void acucarConfig(){
	cmd_LCD(0x01,0); // limpa lcd
	for(int i = 0; i < quantCopo; ++i){
		cmd_LCD(0x80, 0);
		escreve_LCD_Flash(msgConfigAcucar);
		conversorNumeros2(i + 1, 0x8F);
		cmd_LCD(0xC0, 0);
		escreve_LCD_Flash(msgResp);
		configQuantSystem2(0, 1, &(acucarBool[i]), 0xC0, respYesOrNo);
		cmd_LCD(0x01,0); // limpa lcd
		if (acucarBool[i] == 0){
			Activity(msgActivityAcucar, i, 0xC9);
		}
		_delay_ms(500); // tempo para n crashar
	}
}

void chocolateConfig(){
	cmd_LCD(0x01,0); // limpa lcd
	for(int i = 0; i < quantCopo; ++i){
		cmd_LCD(0x80, 0);
		escreve_LCD_Flash(msgConfigChocolate);
		conversorNumeros2(i + 1, 0x8F);
		cmd_LCD(0xC0, 0);
		escreve_LCD_Flash(msgResp);
		configQuantSystem2(0, 1, &(chocolateBool[i]), 0xC0, respYesOrNo);
		cmd_LCD(0x01,0); // limpa lcd
		if (chocolateBool[i] == 0){
			Activity(msgActivityChocolate, i, 0xC9);
		}
		_delay_ms(500); // tempo para n crashar
	}
}

void testeCoffe(unsigned int sub){
	while(quantCoffe < 0){
		quantCoffe += sub;
		colocarCafeMaquina();
		int total = quantCoffe + cafeColocar - sub;
		if (total >= 0){
			quantCoffe -= sub;
			quantCoffe += cafeColocar;
		}
		else {
			quantCoffe -= sub;
			cafeColocar = 1;
		}
		debbug(sub);
	}
	debbug(sub);
}

void colocarCafeMaquina(){
	cmd_LCD(0x01,0); // limpa lcd
	cmd_LCD(0x80, 0);
	escreve_LCD_Flash(msgConfigQuantCoffe);
	cmd_LCD(0xC0, 0);
	escreve_LCD_Flash(msgConfigQuantCoffe2);
	configQuantSystem2(1, 20, &cafeColocar, 0xCD, conversorNumeros);
}

void debbug(unsigned int sub){
	cmd_LCD(0x01,0); // limpa lcd
	cmd_LCD(0x80, 0);
	escreve_LCD_Flash(msgDebbugCoffe);
	conversorNumeros(sub, 0x88);
	cmd_LCD(0xC0, 0);
	escreve_LCD_Flash(msgDebbugCoffe2);
	conversorNumeros(quantCoffe, 0xCB);
	_delay_ms(2000);
}

void sucessoCafe(int copo){
	cmd_LCD(0x01,0); // limpa lcd
	cmd_LCD(0x80, 0);
	escreve_LCD_Flash(msgConfigSucefful);
	cmd_LCD(0xC0, 0);
	escreve_LCD_Flash(msgConfigSucefful2);
	conversorNumeros2(copo + 1, 0xC8);
	_delay_ms(750);
}

void copoPronto(){
	cmd_LCD(0x01,0); // limpa lcd
	cmd_LCD(0x80, 0);
	escreve_LCD_Flash(msgAllReady);
	cmd_LCD(0xC0, 0);
	escreve_LCD_Flash(msgAllReady2);
	_delay_ms(2000);
}

void finalizar(){
	cmd_LCD(0x01,0); // limpa lcd
	for (int i = 0; i < 50; i++){
		cmd_LCD(0x80, 0);
		escreve_LCD_Flash(msgFInalizado);
		cmd_LCD(0xC0, 0);
		escreve_LCD_Flash(msgFInalizado1);
		cmd_LCD(0x00, 1);
		cmd_LCD(0x1C, 0); // move para a direita.
		_delay_ms(150);
	}
}

void Activity(const char *c, int i, unsigned char position){
	cmd_LCD(0x01,0); // limpa lcd
	cmd_LCD(0x80, 0);
	escreve_LCD_Flash(msgActivity);
	escreve_LCD_Flash(c);
	cmd_LCD(0xC0, 0);
	escreve_LCD_Flash(msgActivityText);
	conversorNumeros2(i + 1, 0xC8);
	Animation(position, 150, 3);
}

void Animation(unsigned char position, int time, unsigned int repete){
	for(int i = 0; i < repete; i++){
		cmd_LCD(position, 0);
		cmd_LCD('.', 1);
		my_delay_ms(time);
		cmd_LCD(position + 1, 0);
		cmd_LCD('.', 1);
		my_delay_ms(time);
		cmd_LCD(position + 2, 0);
		cmd_LCD('.', 1);
		my_delay_ms(time);
		cmd_LCD(position, 0);
		escreve_LCD_Flash(nadaAnimation);
		my_delay_ms(time);
	}
}

void mandaCaracterNovo(const unsigned char *text){
	cmd_LCD(0x40 + (count * 8), 0);				//endereço base para gravar novo segmento 0x40
	for(int k=0; k<7; k++, ++text){
		cmd_LCD(pgm_read_byte(&(*text)), 1);	//grava 8 bytes na DDRAM começando no end. 0x40
	}
	cmd_LCD(0x00,1);
	cmd_LCD(0x80,0);				//endereça a posição para escrita dos caracteres
	count = count + 1;
}
/* By Stecken */

void Desligar(){
	cafeColocar = 1;
	quantCopo = 1;
	for (int i = 0; i < 2; i++){
		typeCopo[i] = 1;
		leiteBool[i] = 0;
		acucarBool[i] = 0;
		chocolateBool[i] = 0;
	}
	count = 0;
}