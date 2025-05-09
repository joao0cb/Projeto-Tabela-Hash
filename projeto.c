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

void exibirLivro(Livros* livro){
    printf("----------Informacoes do Livro----------\n");
    printf("ISBN: %s\n",livro->isbn);
    printf("Titulo: %s\n",livro->titulo);
    printf("Autor: %s\n",livro->autor);
    printf("Editora: %s\n",livro->editora);
    printf("Ano: %d\n",livro->ano);
    printf("Copias: %d\n\n",livro->numCopias);
}

void exibirUsuario(Usuarios* user){
    printf("----------Informacoes do Usuario----------\n");
    printf("ID: %d\n",user->id);
    printf("Nome: %s\n",user->nome);
    printf("Email: %s\n\n",user->email);
}

Livros* ConsultarISBN(int x, char*  isbn){
    Livros* atual = tabelaLivros[x];
    while(atual!=NULL){
        if(strcmp(atual->isbn,isbn)==0){
            return atual;
        }
        atual = atual->prox;
    }
    return NULL;
}

Usuarios* ConsultarID(int x, int  id){
    Usuarios* atual = tabelaUsuarios[x];
    while(atual!=NULL){
        if(atual->id == id){
            return atual;
        }
        atual = atual->prox;
    }
    return NULL;
}

void cadastrarLivro(char* isbn){
    Livros* Livro = malloc(sizeof(Livros));
    if (Livro == NULL) {
        printf("Erro de alocacao!\n");
        return;
    }
    char tempString[MAX_STRING];
    printf("\nCADASTRO LIVRO\n");
    printf("Digite o titulo: ");
    lerStr(Livro->titulo, MAX_STRING);
    printf("Digite o(a) autor(a): ");
    lerStr(Livro->autor, MAX_STRING);
    printf("Digite o ano: ");
    scanf("%d", &Livro->ano);
    lerStr(tempString, MAX_STRING);        // limpa o buffer
    strcpy(Livro->isbn, isbn);
    printf("Digite o numero de copias: ");
    scanf("%d", &Livro->numCopias);
    lerStr(tempString, MAX_STRING);
    printf("Digite a editora: ");
    lerStr(Livro->editora, MAX_STRING);
    Livro->prox = NULL;

    int ISBNhash = hashISBN(Livro->isbn);
    if(tabelaLivros[ISBNhash] != NULL) {
        Livros* temp2 = tabelaLivros[ISBNhash];
        while(temp2->prox != NULL) {
            temp2 = temp2->prox;
        }
        temp2->prox = Livro;
    } else {
        tabelaLivros[ISBNhash] = Livro;
    }
    FILE *arqLivros = fopen("ArquivoLivros.dat", "rb+");
    if(arqLivros == NULL) {
        arqLivros = fopen("ArquivoLivros.dat", "w+b");
        if(arqLivros == NULL) {
            printf("Erro ao criar o arquivo!\n");
            return;
        }
    }
    fseek(arqLivros, 0, SEEK_END);
    if (fwrite(Livro, sizeof(Livros), 1, arqLivros) != 1) {
        printf("Erro ao escrever no arquivo.\n");
    }
    fclose(arqLivros);
}

void cadastrarUsuario() {
    Usuarios* Usuario = malloc(sizeof(Usuarios));
    if(Usuario == NULL) {
        printf("Erro de alocacao\n");
        return;
    }
    char tempString[MAX_STRING];
    printf("\nCADASTRO USUARIO\n");
    printf("Digite o nome: ");
    lerStr(Usuario->nome, MAX_STRING);
    printf("Digite o ID: ");
    scanf("%d", &Usuario->id);
    lerStr(tempString, MAX_STRING);     // limpa o buffer
    printf("Digite o e-mail: ");
    lerStr(Usuario->email, MAX_STRING);
    printf("Digite o telefone: ");
    scanf("%d", &Usuario->telefone);
    lerStr(tempString, MAX_STRING);
    Usuario->ativo = 1;
    Usuario->prox = NULL;
    int IDhash = hashID(Usuario->id);
    if(tabelaUsuarios[IDhash] != NULL) {
        Usuarios* atual = tabelaUsuarios[IDhash];
        while(atual->prox != NULL) {
            atual = atual->prox;
        }
        atual->prox = Usuario;
    } else {
        tabelaUsuarios[IDhash] = Usuario;
    }
    FILE *arqUsuarios = fopen("ArquivoUsuarios.dat", "rb+");
    if(arqUsuarios == NULL) {
        arqUsuarios = fopen("ArquivoUsuarios.dat", "w+b");
        if(arqUsuarios == NULL) {
            printf("Erro ao criar o arquivo!\n");
            return;
        }
    }
    fseek(arqUsuarios, 0, SEEK_END);
    if (fwrite(Usuario, sizeof(Usuarios), 1, arqUsuarios) != 1) {
        printf("Erro ao escrever no arquivo.\n");
    }
    fclose(arqUsuarios);
}

void atualizarNumCopias(){
    char isbn[20];
    char tempString[MAX_STRING];

    printf("Digite o ISBN do livro: ");
    lerStr(isbn, MAX_STRING);
    lerStr(tempString, MAX_STRING);        // limpa o buffer

    int ISBNhash = hashISBN(isbn);
    Livros* atual = tabelaLivros[ISBNhash];

    while(atual != NULL){
        if(strcmp(atual->isbn, isbn) == 0){
            int x;
            printf("Digite quantas copias voce quer adicionar: ");
            scanf("%d",&x);
            atual->numCopias += x;
            printf("Copias adicionadas! Total de copias: %d\n", atual->numCopias);

            FILE *arqLivros = fopen("ArquivoLivros.dat", "rb+");
            if(arqLivros == NULL){
                printf("Erro ao criar o arquivo!\n");
                return;
            }

            Livros tempLivro;
            while(fread(&tempLivro, sizeof(Livros), 1, arqLivros)){
                if(strcmp(tempLivro.isbn, isbn) == 0){
                    tempLivro.numCopias = atual->numCopias;
                    fseek(arqLivros, -sizeof(Livros), SEEK_CUR);
                    if(fwrite(&tempLivro, sizeof(Livros), 1, arqLivros) != 1){
                        printf("Erro ao atualizar o arquivo.");
                    }else{
                        printf("Copias adicionadas no arquivo! Total de copias: %d\n", atual->numCopias);
                    }
                    fclose(arqLivros);
                    return;
                }
            }
            fclose(arqLivros);
            printf("Livro nao encontrado no arquivo.\n");
            return;
        }
        atual = atual->prox;
    }
}

/*
void atualizarEmail(){
    int id;
    char novoEmail[MAX_STRING];
    printf("Digite o ID do usuario: ");
    scanf("%d", &id);
    getchar();

    int IDhash = hashID(id);
    Usuarios* atual = tabelaUsuarios[IDhash];

    while(atual != NULL){
        if(atual->id == id && atual->ativo == 1){
            printf("Digite o novo e-mail: ");
            lerStr(novoEmail, MAX_STRING);
            strcpy(atual->email, novoEmail);

            FILE *arqUsuarios = fopen("ArquivoUsuarios.dat", "rb+");
            if(arqUsuarios == NULL){
                printf("Erro ao abrir o arquivo");
                return;
            }

            Usuarios tempUser;
            while(fread(&tempUser, sizeof(Usuarios), 1, arqUsuarios)){
                if(tempUser.id == id && tempUser.ativo == 1){
                    strcpy(tempUser.email, novoEmail);
                    fseek(arqUsuarios, -sizeof(Usuarios), SEEK_CUR);
                    if(fwrite(&tempUser, sizeof(Usuarios), 1, arqUsuarios) != 1){
                        printf("Erro ao atualizar o arquivo.");
                    }else{
                        printf("E-mail atualizado com sucesso.\n");
                    }
                    fclose(arqUsuarios);
                    return;
                }
            }
            fclose(arqUsuarios);
            printf("Usuario nao encontrado no arquivo.\n");
            return;
        }
        atual = atual->prox;
    }
    printf("Usuario nao encontrado ou esta inativo.\n");
}
*/

/*
Livros* ConsultarISBN(int x, char  isbn){
    Livros* atual = tabelaLivros[x];

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

void exibirLivro(Livros* livro){
    printf("----------Informações do Livro----------\n");
    printf("ISBN: %s\n",livro->isbn);
    printf("Título: %s\n",livro->titulo);
    printf("Autor: %s\n",livro->autor);
    printf("Editora: %s\n",livro->editora);
    printf("Ano: %d\n",livro->ano);
    printf("Cópias: %d\n\n",livro->numCopias);
}
*/

// gcc main_projeto.c projeto.c -o hash.exe
// ./hash