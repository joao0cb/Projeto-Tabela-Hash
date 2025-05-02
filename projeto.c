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

int hashISBN(char* isbn) {
    int soma = 0;
    for (int i = 0; isbn[i] != '\0'; i++) {
        soma += isbn[i];
    }
    return soma % MAX_TAM;
}

int hashID(int id) {
    return id % MAX_TAM;
}

void lerStr(char *str, int tam) {
    fgets(str, tam, stdin);
    int len = strlen(str);
    if (str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

void cadastrarLivros() {
    Livros* Livro = malloc(sizeof(Livros));
    if (Livro == NULL) {
        printf("Erro de alocação!\n");
        return;
    }
    char tempString[MAX_STRING];
    printf("\nCADASTRO LIVRO\n");
    printf("Digite o ISBN: ");
    lerStr(Livro->isbn, MAX_STRING);
    printf("Digite o numero de copias: ");
    scanf("%d", &Livro->numCopias);
    printf("Digite o titulo: ");
    lerStr(Livro->titulo, MAX_STRING);
    printf("Digite o(a) autor(a): ");
    lerStr(Livro->autor, MAX_STRING);
    printf("Digite o ano: ");
    scanf("%d", &Livro->ano);
    lerStr(tempString, MAX_STRING);     // limpa o buffer
    printf("Digite a editora: ");
    lerStr(Livro->editora, MAX_STRING);
    lerStr(tempString, MAX_STRING);
    Livro->prox = NULL;
    int ISBNhash = hashISBN(Livro->isbn);
    if(tabelaLivros[ISBNhash] != NULL) {
        Livros* atual = tabelaLivros[ISBNhash];
        while(atual != NULL) {
            if(strcmp(atual->isbn, Livro->isbn) == 0) {
                atual->numCopias += Livro->numCopias;
                free(Livro);
                printf("Este livro já existe. Numero de copias atualizado.");
                return;
            }
            atual = atual->prox;
        }
    }
    if(tabelaLivros[ISBNhash] != NULL) {
        Livros* temp2 = tabelaLivros[ISBNhash];
        while(temp2->prox != NULL) {
            temp2 = temp2->prox;
        }
        temp2->prox = Livro;
    } else {
        tabelaLivros[ISBNhash] = Livro;
    }
}
Livro* ConsultarISBN(int x, char  isbn){

    Livro* atual = tabelaLivros[x];

    while(atual!=NULL){
        if(strcmp(atual->isbn,isbn)==0){
            return atual;
        }
        atual = atual->prox;
    }
    printf("O livro não existe!\n");
    free(atual);
    return NULL;
}
void exibirLivro(Livro* livro){
    printf("----------Informações do Livro----------\n");
    printf("ISBN: %s\n",livro->isbn);
    printf("Título: %s\n",livro->titulo);
    printf("Autor: %s\n",livro->autor);
    printf("Editora: %s\n",livro->editora);
    printf("Ano: %d\n",livro->ano);
    printf("Cópias: %d\n\n",livro->copias);
}
