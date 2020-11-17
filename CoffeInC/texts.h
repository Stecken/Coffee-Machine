/*
 * texts.h
 *
 * Created: 12/11/2020 22:34:23
 *  Author: Stecken
 */ 


#ifndef TEXTS_H_
#define TEXTS_H_
#include "LCD.h"

PROGMEM const char nadaAnimation[] = "   "; // 7 caracteres
PROGMEM const char msgInicio[] = "Bem-vindo(a)";
PROGMEM const char msgInicio2[] = "Vai um cafe?";
PROGMEM const char msgInicio3[] = "Opa, so bora!";

PROGMEM const char msgPresence[] = "Aguardando a "; //11 caracteres
PROGMEM const char msgPresenceStatus[] = "Copo "; //5 caracteres
PROGMEM const char msgPresenceStatusEnt[] = " colocado"; //6 caracteres
PROGMEM const char msgPresenceStatusRet[] = " retirado"; //6 caracteres
PROGMEM const char msgPresenceEnt[] = "entrada"; //6 caracteres
PROGMEM const char msgPresenceRet[] = "retirada"; //11 caracteres
PROGMEM const char msgPresenceCopText[] = "do copo "; //11 caracteres


PROGMEM const char msgConfigQuantCopo[] = "Sr gostaria de";
PROGMEM const char msgConfigQuantCopo1[] = "quantos copos:";

PROGMEM const char msgConfigTypeCopo[] = "Sr. selecione";
PROGMEM const char msgConfigTypeCopo1[] = "o tipo do copo";

PROGMEM const char msgConfigTypeCop2[] = "Selecione:";
PROGMEM const char msgConfigTypeCop[] = " Peq  Med  Gra ";

PROGMEM const char msgConfigQuantCoffe[] = "Coloque cafe";
PROGMEM const char msgConfigQuantCoffe2[] = "na maquina:";

PROGMEM const char msgConfigSucefful[] = "Cafe colocado";
PROGMEM const char msgConfigSucefful2[] = "no copo";

PROGMEM const char msgDebbugCoffe[] = "Consumo: ";
PROGMEM const char msgDebbugCoffe2[] = "Disponivel: ";
PROGMEM const char msgConfigLeite[] = "Deseja leite C"; //13 caracteres
PROGMEM const char msgConfigAcucar[] = "Deseja acucar C"; //14 caracteres
PROGMEM const char msgConfigChocolate[] = "Deseja choco. C"; //14 caracteres
PROGMEM const char msgResp[] = " Sim    Nao"; // SIM -> 0xC0 NAO-> 0xC7

PROGMEM const char msgActivity[] = "Colocando "; // 9 caracteres
PROGMEM const char msgActivityCafe[] = "cafe"; // 13
PROGMEM const char msgActivityLeie[] = "leite";
PROGMEM const char msgActivityAcucar[] = "acucar";
PROGMEM const char msgActivityChocolate[] = "chocolate";
PROGMEM const char msgActivityText[] = "no copo "; // 7 caracteres

PROGMEM const char msgAllReady[] = "Seu(s) cafe(s)";
PROGMEM const char msgAllReady2[] = "esta(o) pronto(s)";

PROGMEM const char msgFInalizado[] = "Obrigado(a) por";
PROGMEM const char msgFInalizado1[] = "nos escolher ";

void configQuantSystem(unsigned int lim, int *pointVar, unsigned char position);
void conversorNumeros(int number, unsigned char position);
void conversorNumeros2(int number, unsigned char position);
void selectType(int number, unsigned char position);
void respYesOrNo(int number, unsigned char position);

const unsigned char carac1[] PROGMEM = {
	0b00000,//coração
	0b01010,
	0b11111,
	0b11111,
	0b11111,
	0b01110,
	0b00100
};

/*somar o conversor(48) para unit e deze*/
void conversorNumeros(int number, unsigned char position){
	unsigned int unit;
	unsigned int deze = 0;
	if (number > 9){
		unit = number % 10;
		deze = number / 10;
		unit += 48;
		deze += 48;
		cmd_LCD(position, 0);
		cmd_LCD((char) deze, 1);
		cmd_LCD((char) unit, 1);
	}
	else{
		conversorNumeros2(number, position);
		cmd_LCD(position + 1, 0);
		cmd_LCD(' ', 1);
	}
}

void conversorNumeros2(int number, unsigned char position){
	number += 48;
	cmd_LCD(position, 0);
	cmd_LCD((char) number, 1);
}

void selectType(int number, unsigned char position){
	if (number == 1){
		cmd_LCD(position, 0);
		cmd_LCD('>', 1);
		cmd_LCD(position + 5, 0);
		cmd_LCD(' ', 1);
		cmd_LCD(position + 10, 0);
		cmd_LCD(' ', 1);
	}
	else if(number == 2){
		cmd_LCD(position + 5, 0);
		cmd_LCD('>', 1);
		cmd_LCD(position, 0);
		cmd_LCD(' ', 1);
		cmd_LCD(position + 10, 0);
		cmd_LCD(' ', 1);
	}
	else if(number == 3){
		cmd_LCD(position + 10, 0);
		cmd_LCD('>', 1);
		cmd_LCD(position, 0);
		cmd_LCD(' ', 1);
		cmd_LCD(position + 5, 0);
		cmd_LCD(' ', 1);
	}
}

void respYesOrNo(int number, unsigned char position){
	if (number == 0){
		cmd_LCD(position, 0);
		cmd_LCD('>', 1);
		cmd_LCD(position + 7, 0);
		cmd_LCD(' ', 1);
	}
	else if(number == 1){
		cmd_LCD(position + 7, 0);
		cmd_LCD('>', 1);
		cmd_LCD(position, 0);
		cmd_LCD(' ', 1);
	}
}

/*
void configQuantSystem(unsigned int lim, int *pointVar, unsigned char position){
	conversorNumeros2(*pointVar, position);
	while(tst_bit(PINC, ENTER)){
		if (!tst_bit(PINC, BMAIS)){
			if (*pointVar < lim){
				*pointVar = *pointVar + 1;
				conversorNumeros2(*pointVar, position);
			}
			_delay_ms(150);
		}
		else if(!tst_bit(PINC, BMENOS)){
			if(*pointVar != 1){
				*pointVar = *pointVar - 1;
				conversorNumeros2(*pointVar, position);
			}
			_delay_ms(150);
		}
	}
	conversorNumeros2(*pointVar, position);
}
*/
#endif /* TEXTS_H_ */