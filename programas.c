#include "programas.h"
#include "ram.h"
#include "maquina.h"

#include <stdlib.h>
#include <stdio.h>

void programaAleatorio(int qtdeInstrucoes) {
    int tamRAM = 1000;
    int *RAM = criaRandomRam(tamRAM);
    Instrucao* programa = (Instrucao*) malloc(qtdeInstrucoes*sizeof(Instrucao));

    for (int i = 0; i < qtdeInstrucoes - 1; i++) {
        programa[i].opcode = rand() % 2;
        programa[i].endereco1 = rand() % (tamRAM - 1);
        programa[i].endereco2 = rand() % (tamRAM - 1);
        programa[i].endereco3 = rand() % (tamRAM - 1);
    }
    programa[qtdeInstrucoes-1].opcode = -1;

    maquina(RAM, programa);
    liberaRAM(RAM);
    free(programa);
}

// -1 - Fim do programa
// 0 - soma                                   [OP, end1, end2, endSalvar]
// 1 - subtracao                              [OP, end1, end2, endSalvar]
// 2 - reg -> ram                             [OP, regFonte, posRam, -1]
// 3 - ram -> reg                             [OP, regDestino, posRam, -1]
// 4 - copia conteudo do exterior pro reg     [OP, regDestino, conteudo, -1]
// 5 - copia conteudo do reg p/ exterior      [OP, regFonte, destino, -1]


void programaMultiplica(int * RAM, int multiplicando, int multiplicador) {


    int ramLocal = 0; //indica se é um programa isolado ou que faz parte de outro maior (se faz, usa a ram do programa principal)
    if (RAM == NULL) {
        RAM = criaRam_vazia(2);
        ramLocal = 1;
    }
    // Move multiplicando pro reg -> depois manda do reg pra ram
    Instrucao move[3];
    move[0].opcode = 4;
    move[0].endereco1 = 1;
    move[0].endereco2 = multiplicando;

    move[1].opcode = 2;
    move[1].endereco1 = 1;
    move[1].endereco2 = 1;

    move[2].opcode = -1;

    maquina(RAM, move);

    // RAM = [0, multiplicando]

    //soma continua da posicao 0 com a posicao 1 da ram
    for (int i = 0; i < multiplicador; i++) {
        Instrucao mult[2];

        mult[0].opcode = 0;
        mult[0].endereco1 = 0;
        mult[0].endereco2 = 1;
        mult[0].endereco3 = 0;

        mult[1].opcode = -1;
        maquina(RAM, mult);
    }
    
    //Move da ram pro reg1 -> depois do reg pro endereco2 da instrucao 2
    Instrucao move2[3];

    move2[0].opcode = 3;
    move2[0].endereco1 = 1;
    move2[0].endereco2 = 0;

    move2[1].opcode = 5;
    move2[1].endereco1 = 1;
    move2[1].endereco2 = -1;

    move2[2].opcode = -1;
    
    maquina(RAM, move2);
    printf("%d x %d = %d\n", multiplicando, multiplicador, move2[1].endereco2);

    if (ramLocal)
        liberaRAM(RAM);

}

void programaDivisao(int *RAM, int dividendo, int divisor) {
    int ramLocal = 0; //indica se é um programa isolado ou que faz parte de outro maior (se faz, usa a ram do programa principal)
    if (RAM == NULL) {
        RAM = criaRam_vazia(4);
        ramLocal = 1;
    }

    //Manda divisor e dividendo para o registrador -> depois manda do registrador pra ram
    Instrucao move[5];

    move[0].opcode = 4;
    move[0].endereco1 = 1;
    move[0].endereco2 = dividendo;

    move[1].opcode = 4;
    move[1].endereco1 = 2;
    move[1].endereco2 = divisor;

    move[2].opcode = 2;
    move[2].endereco1 = 1;
    move[2].endereco2 = 0;
    
    move[3].opcode = 2;
    move[3].endereco1 = 2;
    move[3].endereco2 = 1;

    move[4].opcode = -1;

    maquina(RAM, move);

    //RAM = [dividendo, divisor, 0, 0]

    // Salva '1' na ram para atuar como somador do contador
    Instrucao addCounter[3];

    addCounter[0].opcode = 4;
    addCounter[0].endereco1 = 1;
    addCounter[0].endereco2 = 1;

    addCounter[1].opcode = 2;
    addCounter[1].endereco1 = 1;
    addCounter[1].endereco2 = 2;

    addCounter[2].opcode = -1;

    maquina(RAM, addCounter);
    //RAM = [dividendo, divisor, 1, 0]

    //Subtrai o divisor do dividendo, incrementa 1 no contador e atualiza o dividendo
    while (dividendo >= divisor) {
        Instrucao div[5];

        div[0].opcode = 1;
        div[0].endereco1 = 0;
        div[0].endereco2 = 1;
        div[0].endereco3 = 0;

        div[1].opcode = 0;
        div[1].endereco1 = 2;
        div[1].endereco2 = 3;
        div[1].endereco3 = 3;

        div[2].opcode = 3;
        div[2].endereco1 = 1;
        div[2].endereco2 = 0;

        div[3].opcode = 5;
        div[3].endereco1 = 1;
        div[3].endereco2 = -1;

        div[4].opcode = -1;

        maquina(RAM, div);
        dividendo = div[3].endereco2;
    }

    //Manda o resultado final da ram pro reg -> depois do reg pro exterior
    Instrucao move2[3];

    move2[0].opcode = 3;
    move2[0].endereco1 = 1;
    move2[0].endereco2 = 3;

    move2[1].opcode = 5;
    move2[1].endereco1 = 1;
    move2[1].endereco2 = -1;

    move2[2].opcode = -1;

    maquina(RAM,move2);
    printf("Resultado da divisao: %d\n", move2[1].endereco2);
    if (ramLocal)
        liberaRAM(RAM);
}