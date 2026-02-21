#include <stdio.h>
#include "disco.h"


void carregaHd(LinhaCache *cache)
{

}
void salvaHd(LinhaCache *cache, int endHd)

{
    FILE *file = fopen("armazenaHd.bin", "wb+");
    if(!file)
        return;
    fseek(file, endHd * sizeof(LinhaCache), SEEK_SET);
    fwrite(cache, sizeof(LinhaCache), 1 , file);
    


}