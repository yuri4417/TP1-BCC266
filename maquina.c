#include "maquina.h"
#include "ram.h"
#include <stdlib.h>
#include <stdio.h>


void maquina(int *ram, Instrucao* programa) {
    int reg1, reg2;
    int PC = 0;
    int opcode = 0;
    while (opcode != -1) {

        Instrucao inst = programa[PC];

        switch (inst.opcode) {
            
            // -1 - Fim do programa
            // 0 - soma
            // 1 - subtracao
            // 2 - reg -> ram
            // 3 - ram -> reg
            // 4 - copia conteudo pro reg
            // 5 - copia conteudo do reg
            case 0: //[OP, nro1, nro2, endSalvar]
                reg1 = load(ram, inst.endereco1);
                reg2 = load(ram, inst.endereco2);
                reg1 += reg2;
                store(ram, inst.endereco3, reg1);
                printf("Resultado %d salvo no endereço %d\n", reg1, inst.endereco3);
                break;

            case 1: //[OP, nro1, nro2, endSalvar]
                reg1 = load(ram, inst.endereco1);
                reg2 = load(ram, inst.endereco2);
                reg1 -= reg2;
                store(ram, inst.endereco3, reg1);
                printf("Resultado %d salvo no endereço %d\n", reg1, inst.endereco3);
                break;

            case 2: //[OP, regFonte, posRam, -1]
                if (inst.endereco1 == 1) 
                    store(ram, inst.endereco2, reg1);
                
                else if (inst.endereco1 == 2)
                    store(ram, inst.endereco2, reg2);
                break;

            case 3: //[OP, regDestino, posRam, -1]
                if (inst.endereco1 == 1)
                    reg1 = load(ram, inst.endereco2);
                else if (inst.endereco2 == 2)
                    reg2 = load(ram, inst.endereco2);
                break;

            case 4: //[OP, regDestino, content, -1]
                if (inst.endereco1 == 1)
                    reg1 = inst.endereco2;
                else if (inst.endereco1 == 2)
                    reg2 = inst.endereco2;
                break;

            case 5: // [OP, regFonte, conteudo, -1]
                if (inst.endereco1 == 1)
                    inst.endereco2 = reg1;
                else if (inst.endereco1 == 2)
                    inst.endereco2 = reg2;
                break;
        }

        PC++;
    }
}