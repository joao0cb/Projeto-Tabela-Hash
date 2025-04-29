#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "projeto.h"

void inicializarTabelas() {
    for (int i = 0; i < MAX_TAM; i++) {
        tabelaLivros[i] = NULL;
        tabelaUsuarios[i] = NULL;
    }
}