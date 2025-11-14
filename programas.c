#include "programas.h"
#include "ram.h"
#include "maquina.h"

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
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


void programaMultiplica(int * RAM, int multiplicando, int multiplicador) { // RESULTADO = RAM[0]
    int ramLocal = 0; //indica se é um programa isolado ou que faz parte de outro maior (se faz, usa a ram do programa principal)
    if (RAM == NULL) {
        RAM = criaRam_vazia(2);
        ramLocal = 1;
    }

    //Limpa o RAM[0] pros casos de multiplica ser um programa auxiliar
    Instrucao clean[3];

    clean[0].opcode = 4;
    clean[0].endereco1 = 1;
    clean[0].endereco2 = 0;
    
    clean[1].opcode = 2;
    clean[1].endereco1 = 1;
    clean[1].endereco2 = 0;

    clean[2].opcode = -1;

    maquina(RAM, clean);


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

    //[resultado, multiplicando]

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
    printf("MULT: %d x %d = %d\n", multiplicando, multiplicador, move2[1].endereco2);
    


    if (ramLocal) {
        liberaRAM(RAM);
    }
}

void programaDivisao(int *RAM, int dividendo, int divisor) {

    int ramLocal = 0; //indica se é um programa isolado ou que faz parte de outro maior (se faz, usa a ram do programa principal)
    if (RAM == NULL) {
        RAM = criaRam_vazia(4);
        ramLocal = 1;
    }

    //Limpa o RAM[3] pros casos de Divisao ser um programa auxiliar
    Instrucao clean[3];

    clean[0].opcode = 4;
    clean[0].endereco1 = 1;
    clean[0].endereco2 = 0;
    
    clean[1].opcode = 2;
    clean[1].endereco1 = 1;
    clean[1].endereco2 = 3;

    clean[2].opcode = -1;

    maquina(RAM, clean);
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
    printf("DIV: Resultado da divisao: %d\n", move2[1].endereco2);
    if (ramLocal)
        liberaRAM(RAM);
}

void programaRaizQuad(int* RAM, int radicando) { //RESULTADO == RAM[3]
    int ramLocal = 0; //indica se é um programa isolado ou que faz parte de outro maior (se faz, usa a ram do programa principal)
    if (RAM == NULL) {
        RAM = criaRam_vazia(7);
        ramLocal = 1;
    }

    //Esquema final RAM -> [programaMultiplica, programaMultiplica, radicando, RESULTADO FINAL, result, counter, 1]



    //Move o radicando e '1' pros regs -> depois dos regs pra ram
    Instrucao move[5];
    move[0].opcode = 4;
    move[0].endereco1 = 1;
    move[0].endereco2 = radicando;

    move[1].opcode = 2;
    move[1].endereco1 = 1;
    move[1].endereco2 = 2;

    move[2].opcode = 4;
    move[2].endereco1 = 2;
    move[2].endereco2 = 1;

    move[3].opcode = 2;
    move[3].endereco1 = 2;
    move[3].endereco2 = 6;

    move[4].opcode = -1;

    maquina(RAM, move);

    //ram -> [0, 0, radicando, 0, 0, 0, 1]
    int result = INT_MAX;
    while (result >= 0) {

        Instrucao copia[3];

        //faz a copia do contador (ultimo valor válido) pro resultado final
        copia[0].opcode  = 3;
        copia[0].endereco1 = 1;
        copia[0].endereco2 = 5;

        copia[1].opcode = 2;
        copia[1].endereco1 = 1;
        copia[1].endereco2 = 3;

        copia[2].opcode = -1;

        maquina(RAM, copia);
        //RAM -> [X, X, radicando, counter, X, counter, 1]


        Instrucao sum[2];

        //Incrementa o contador p testar o proximo quadrado
        sum[0].opcode = 0;
        sum[0].endereco1 = 5;
        sum[0].endereco2 = 6;
        sum[0].endereco3 = 5;

        sum[1].opcode = -1;

        maquina(RAM, sum);
        //RAM -> [0, 0, radicando, counter, 0, counter + 1, result]


        Instrucao calcula[3];

        calcula[0].opcode = 3;
        calcula[0].endereco1 = 1;
        calcula[0].endereco2 = 5;

        calcula[1].opcode = 5;
        calcula[1].endereco1 = 1;
        calcula[1].endereco2 = -1;

        calcula[2].opcode = -1;
        
        maquina(RAM, calcula);
        int y = calcula[1].endereco2;    
        programaMultiplica(RAM, y, y);
        //RAM -> [y^2, y, radicando, counter, 0, counter + 1, result]


        Instrucao sub[2];

        sub[0].opcode = 1;
        sub[0].endereco1 = 2;
        sub[0].endereco2 = 0;
        sub[0].endereco3 = 4;

        sub[1].opcode = -1;

        maquina(RAM, sub);
        //RAM -> [y^2, y, radicando, counter, result, counter + 1, result]

        Instrucao att[3];

        att[0].opcode = 3;
        att[0].endereco1 = 1;
        att[0].endereco2 = 4;

        att[1].opcode = 5;
        att[1].endereco1 = 1;
        att[1].endereco2 = -1;

        att[2].opcode  = -1;

        maquina(RAM, att);
        result = att[1].endereco2;

        

    }

    Instrucao extract[3];

    extract[0].opcode = 3;
    extract[0].endereco1 = 1;
    extract[0].endereco2 = 3;

    extract[1].opcode = 5;
    extract[1].endereco1 = 1;
    extract[1].endereco2 = -1;

    maquina(RAM, extract);
    printf("SQRT(%d) = %d\n", radicando, extract[1].endereco2);


    if (ramLocal)
        liberaRAM(RAM);
}

