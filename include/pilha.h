#ifndef PILHA_H
#define PILHA_H

#include "structs.h"

PilhaExecucao *criaPilha(int maxTam);
void destroiPilha(PilhaExecucao *pilha);
void PilhaPush(PilhaExecucao *pilha, ItemPilha item);
void PilhaPop(PilhaExecucao *pilha);
void PilhaTopo(PilhaExecucao *pilha, ItemPilha *item);
#endif //PILHA_H