#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "projeto.h"

Livros* tabelaLivros[MAX_TAM];
Usuarios* tabelaUsuarios[MAX_TAM];

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

void exibirLivro(Livros* Livro){
    printf("\n----------Informacoes do Livro----------\n");
    printf("ISBN: %s\n", Livro->isbn);
    printf("Titulo: %s\n", Livro->titulo);
    printf("Autor: %s\n", Livro->autor);
    printf("Editora: %s\n", Livro->editora);
    printf("Ano: %d\n", Livro->ano);
    printf("Copias: %d\n", Livro->numCopias);
    printf("Emprestimos: %d\n\n",Livro->numEmprestimos);
}

void exibirUsuario(Usuarios* Usuario){
    printf("----------Informacoes do Usuario----------\n");
    printf("ID: %d\n", Usuario->id);
    printf("Nome: %s\n", Usuario->nome);
    printf("Email: %s\n", Usuario->email);
    printf("Telefone: %s\n\n", Usuario->telefone);
    if(Usuario->ativo == 0){
        printf("USUARIO DESATIVADO\n\n");
    }
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
    char tempString[MAX_STRING];
    if (Livro == NULL) {
        printf("Erro de alocacao!\n");
        return;
    }
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
    int ISBNhash = hashISBN(Livro->isbn);
    Livro->prox = tabelaLivros[ISBNhash];
    tabelaLivros[ISBNhash] = Livro;
}

void cadastrarUsuario() {
    Usuarios* Usuario = malloc(sizeof(Usuarios));
    char tempString[MAX_STRING];
    if(Usuario == NULL) {
        printf("Erro de alocacao\n");
        return;
    }
    printf("\nCADASTRO USUARIO\n");
    printf("Digite o nome: ");
    lerStr(Usuario->nome, MAX_STRING);
    printf("Digite o ID: ");
    scanf("%d", &Usuario->id);
    lerStr(tempString, MAX_STRING);     // limpa o buffer
    printf("Digite o e-mail: ");
    lerStr(Usuario->email, MAX_STRING);
    printf("Digite o telefone: ");
    lerStr(Usuario->telefone, MAX_STRING);
    Usuario->ativo = 1;
    Usuario->prox = NULL;
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
    int IDhash = hashID(Usuario->id);
    Usuario->prox = tabelaUsuarios[IDhash];
    tabelaUsuarios[IDhash] = Usuario;
}

void atualizarNumCopias(char* isbn){
    char tempString[MAX_STRING];

    int ISBNhash = hashISBN(isbn);
    Livros* atual = tabelaLivros[ISBNhash];

    while(atual != NULL){
        if(strcmp(atual->isbn, isbn) == 0){
            int x;
            printf("Digite quantas copias voce quer adicionar: ");
            scanf("%d",&x);
            lerStr(tempString, MAX_STRING);
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

void atualizarDado() {
    int opc;
    int opcLivro;
    int opcUsuario;
    int id;
    int novoAno;
    int novoID;
    int IDhash;
    int ISBNhash;
    char isbn[MAX_STRING];
    char tempString[MAX_STRING];
    char novoISBN[MAX_STRING];
    char novoTitulo[MAX_STRING];
    char novoAutor[MAX_STRING];
    char novoEditora[MAX_STRING];
    char novoNome[MAX_STRING];
    char novoEmail[MAX_STRING];
    Usuarios* atual = NULL;
    Livros* atualB = NULL;
    printf("---------------ATUALIZAR-----------------\n");
    printf("1-Livro\n");
    printf("2-Usuario\n");
    printf("3-Voltar\n");
    printf("Escolha uma opcao para atualizar: ");
    scanf("%d", &opc);
    lerStr(tempString, MAX_STRING);
    switch (opc) {
        case 1:
            printf("---------------MENU-----------------\n");
            printf("1-ISBN\n");
            printf("2-Titulo\n");
            printf("3-Autor\n");
            printf("4-Ano\n");
            printf("5-Editora\n");
            printf("6-Numero de Copias\n");
            printf("7-Voltar\n");
            printf("Escolha um dado para atualiza-lo: ");
            scanf("%d", &opcLivro);
            lerStr(tempString, MAX_STRING);
            switch (opcLivro) {
                case 1:  // isbn
                    ; // NAO REMOVA ESSE ; POR FAVOR .
                    char isbnAntigo[MAX_STRING];
                    char isbnNovo[MAX_STRING];
                    int hashAntigo, hashNovo;
                    Livros *livroAntigo = NULL, *livroNovo = NULL, *anterior = NULL;

                    printf("Digite o ISBN atual do livro: ");
                    lerStr(isbnAntigo, MAX_STRING);
                    hashAntigo = hashISBN(isbnAntigo);
                    livroAntigo = tabelaLivros[hashAntigo];

                    while (livroAntigo && strcmp(livroAntigo->isbn, isbnAntigo) != 0) {
                        anterior = livroAntigo;
                        livroAntigo = livroAntigo->prox;
                    }

                    if (!livroAntigo) {
                        printf("Livro com ISBN atual nao encontrado.\n");
                        return;
                    }

                    printf("Digite o novo ISBN para o livro: ");
                    lerStr(isbnNovo, MAX_STRING);
                    hashNovo = hashISBN(isbnNovo);
                    livroNovo = tabelaLivros[hashNovo];

                    while (livroNovo && strcmp(livroNovo->isbn, isbnNovo) != 0) {
                        livroNovo = livroNovo->prox;
                    }

                    if (livroNovo) {
                        livroNovo->numCopias += livroAntigo->numCopias;

                        FILE *arq = fopen("ArquivoLivros.dat", "rb+");
                        if (!arq) {
                            printf("Erro ao abrir arquivo.\n");
                            return;
                        }

                        Livros temp;
                        while (fread(&temp, sizeof(Livros), 1, arq)) {
                            if (strcmp(temp.isbn, isbnNovo) == 0) {
                                temp.numCopias = livroNovo->numCopias;
                                fseek(arq, -sizeof(Livros), SEEK_CUR);
                                fwrite(&temp, sizeof(Livros), 1, arq);
                                break;
                            }
                        }
                        fclose(arq);

                        if (anterior) {
                            anterior->prox = livroAntigo->prox;
                        } else {
                            tabelaLivros[hashAntigo] = livroAntigo->prox;
                        }
                        free(livroAntigo);

                        FILE *arqOrig = fopen("ArquivoLivros.dat", "rb");
                        FILE *arqTemp = fopen("TempLivros.dat", "wb");
                        if (!arqOrig || !arqTemp) {
                            printf("Erro ao abrir arquivos temporarios.\n");
                            return;
                        }

                        while (fread(&temp, sizeof(Livros), 1, arqOrig)) {
                            if (strcmp(temp.isbn, isbnAntigo) != 0) {
                                fwrite(&temp, sizeof(Livros), 1, arqTemp);
                            }
                        }

                        fclose(arqOrig);
                        fclose(arqTemp);
                        remove("ArquivoLivros.dat");
                        rename("TempLivros.dat", "ArquivoLivros.dat");

                        printf("ISBN ja existia. Copias somadas e ISBN antigo removido.\n");

                    } else {
                        if (anterior) {
                            anterior->prox = livroAntigo->prox;
                        } else {
                            tabelaLivros[hashAntigo] = livroAntigo->prox;
                        }

                        strcpy(livroAntigo->isbn, isbnNovo);
                        livroAntigo->prox = tabelaLivros[hashNovo];
                        tabelaLivros[hashNovo] = livroAntigo;

                        FILE *arq = fopen("ArquivoLivros.dat", "rb+");
                        if (!arq) {
                            printf("Erro ao abrir arquivo.\n");
                            return;
                        }

                        Livros temp;
                        while (fread(&temp, sizeof(Livros), 1, arq)) {
                            if (strcmp(temp.isbn, isbnAntigo) == 0) {
                                strcpy(temp.isbn, isbnNovo);
                                fseek(arq, -sizeof(Livros), SEEK_CUR);
                                fwrite(&temp, sizeof(Livros), 1, arq);
                                break;
                            }
                        }
                        fclose(arq);
                        printf("ISBN atualizado com sucesso.\n");
                    }

                case 2:  // título
                    printf("Digite o ISBN do livro: ");
                    lerStr(isbn, MAX_STRING);
                    ISBNhash = hashISBN(isbn);
                    atualB = tabelaLivros[ISBNhash];
                    while (atualB != NULL) {
                        if (strcmp(atualB->isbn, isbn) == 0) {
                            printf("Digite o novo Titulo do livro: ");
                            lerStr(novoTitulo, MAX_STRING);
                            strcpy(atualB->titulo, novoTitulo);
                            printf("Titulo atualizado para: %s\n", novoTitulo);
                            atualizarArquivoLivro();
                            return;
                        }
                        atualB = atualB->prox;
                    }
                    printf("Livro nao encontrado.\n\n");
                    break;

                case 3:  // autor
                    printf("Digite o ISBN do livro: ");
                    lerStr(isbn, MAX_STRING);
                    ISBNhash = hashISBN(isbn);
                    atualB = tabelaLivros[ISBNhash];
                    while (atualB != NULL) {
                        if (strcmp(atualB->isbn, isbn) == 0) {
                            printf("Digite o novo Autor do livro: ");
                            lerStr(novoAutor, MAX_STRING);
                            strcpy(atualB->autor, novoAutor);
                            printf("Autor atualizado para: %s\n", novoAutor);
                            atualizarArquivoLivro();
                            return;
                        }
                        atualB = atualB->prox;
                    }
                    printf("Livro nao encontrado.\n\n");
                    break;

                case 4:  // ano
                    printf("Digite o ISBN do livro: ");
                    lerStr(isbn, MAX_STRING);
                    ISBNhash = hashISBN(isbn);
                    atualB = tabelaLivros[ISBNhash];
                    while (atualB != NULL) {
                        if (strcmp(atualB->isbn, isbn) == 0) {
                            printf("Digite o novo Ano do livro: ");
                            scanf("%d", &novoAno);
                            lerStr(tempString, MAX_STRING);
                            atualB->ano = novoAno;
                            printf("Ano atualizado para: %d\n", novoAno);
                            atualizarArquivoLivro();
                            return;
                        }
                        atualB = atualB->prox;
                    }
                    printf("Livro nao encontrado.\n\n");
                    break;

                case 5:  // editora
                    printf("Digite o ISBN do livro: ");
                    lerStr(isbn, MAX_STRING);
                    ISBNhash = hashISBN(isbn);
                    atualB = tabelaLivros[ISBNhash];
                    while (atualB != NULL) {
                        if (strcmp(atualB->isbn, isbn) == 0) {
                            printf("Digite a nova Editora do livro: ");
                            lerStr(novoEditora, MAX_STRING);
                            strcpy(atualB->editora, novoEditora);
                            printf("Editora atualizada para: %s\n", novoEditora);
                            atualizarArquivoLivro();
                            return;
                        }
                        atualB = atualB->prox;
                    }
                    printf("Livro nao encontrado.\n\n");
                    break;

                case 6:
                    printf("Digite o ISBN do livro: ");
                    lerStr(isbn, MAX_STRING);   
                    atualizarNumCopias(isbn);
                    break;
                case 7:
                    printf("Voltando ao menu anterior...\n");
                    break; 
                default:
                    printf("Opcao invalida!\n");
                    break;
            }
            break;

        case 2:
            printf("---------------MENU-----------------\n");
            printf("1-ID\n");
            printf("2-Nome\n");
            printf("3-E-mail\n");
            printf("4-Telefone\n");
            printf("5-Voltar\n");
            printf("Escolha um dado para atualiza-lo: ");
            scanf("%d", &opcUsuario);
            lerStr(tempString, MAX_STRING);
            switch (opcUsuario) {
                case 1:  // id
                    ;
                    int idAntigo, idNovo;
                    int hashAntigo, hashNovo;
                    Usuarios *usuarioAntigo = NULL, *usuarioNovo = NULL, *anterior = NULL;

                    printf("Digite o ID atual do usuario: ");
                    scanf("%d", &idAntigo);
                    lerStr(tempString, MAX_STRING);
                    hashAntigo = hashID(idAntigo);
                    usuarioAntigo = tabelaUsuarios[hashAntigo];
                    if(usuarioAntigo->ativo == 0){
                        printf("Usuario inativo.\n\n");
                        return;
                    }else{
                        while (usuarioAntigo && usuarioAntigo->id != idAntigo) {
                        anterior = usuarioAntigo;
                        usuarioAntigo = usuarioAntigo->prox;
                    }

                    if (!usuarioAntigo) {
                        printf("Usuario com ID atual nao encontrado.\n");
                        return;
                    }

                    printf("Digite o novo ID para o usuario: ");
                    scanf("%d", &idNovo);
                    lerStr(tempString, MAX_STRING);
                    hashNovo = hashID(idNovo);
                    usuarioNovo = tabelaUsuarios[hashNovo];

                    while (usuarioNovo && usuarioNovo->id != idNovo) {
                        usuarioNovo = usuarioNovo->prox;
                    }

                    if (usuarioNovo) {
                        printf("ID novo ja existe. Nao eh possivel atualizar.\n");
                        return;
                    } else {
                        usuarioAntigo->id = idNovo;

                        if (anterior) {
                            anterior->prox = usuarioAntigo->prox;
                        } else {
                            tabelaUsuarios[hashAntigo] = usuarioAntigo->prox;
                        }

                        usuarioAntigo->prox = tabelaUsuarios[hashNovo];
                        tabelaUsuarios[hashNovo] = usuarioAntigo;

                        FILE *arqOrig = fopen("ArquivoUsuarios.dat", "rb");
                        FILE *arqTemp = fopen("TempUsuarios.dat", "wb");
                        if (!arqOrig || !arqTemp) {
                            printf("Erro ao abrir arquivos temporarios.\n");
                            return;
                        }

                        Usuarios temp;
                        while (fread(&temp, sizeof(Usuarios), 1, arqOrig)) {
                            if (temp.id == idAntigo) {
                                temp.id = idNovo;
                            }
                            fwrite(&temp, sizeof(Usuarios), 1, arqTemp);
                        }

                        fclose(arqOrig);
                        fclose(arqTemp);
                        remove("ArquivoUsuarios.dat");
                        rename("TempUsuarios.dat", "ArquivoUsuarios.dat");

                        printf("ID atualizado com sucesso.\n");
                    }
                    break;
                    }
                case 2:  // nome
                    printf("Digite o ID do usuario: ");
                    scanf("%d", &id);
                    lerStr(tempString, MAX_STRING);
                    IDhash = hashID(id);
                    atual = tabelaUsuarios[IDhash];
                    while (atual != NULL) {
                        if (atual->id == id && atual->ativo == 1) {
                            printf("Digite o novo nome do usuario: ");
                            lerStr(novoNome, MAX_STRING);
                            strcpy(atual->nome, novoNome);
                            printf("Nome atualizado para: %s\n", novoNome);
                            atualizarArquivoUsuario();
                            return;
                        }
                        atual = atual->prox;
                    }
                    printf("Usuario nao encontrado ou esta inativo.\n\n");
                    break;

                case 3:  // e-mail
                    printf("Digite o ID do usuario: ");
                    scanf("%d", &id);
                    lerStr(tempString, MAX_STRING);
                    IDhash = hashID(id);
                    atual = tabelaUsuarios[IDhash];
                    while (atual != NULL) {
                        if (atual->id == id && atual->ativo == 1) {
                            printf("Digite o novo e-mail: ");
                            lerStr(novoEmail, MAX_STRING);
                            strcpy(atual->email, novoEmail);
                            printf("E-mail atualizado para: %s\n", novoEmail);
                            atualizarArquivoUsuario();
                            return;
                        }
                        atual = atual->prox;
                    }
                    printf("Usuario nao encontrado ou esta inativo.\n\n");
                    break;

                case 4:  // telefone
                    printf("Digite o ID do usuario: ");
                    scanf("%d", &id);
                    lerStr(tempString, MAX_STRING);
                    IDhash = hashID(id);
                    atual = tabelaUsuarios[IDhash];
                    while (atual != NULL) {
                        if (atual->id == id && atual->ativo == 1) {
                            printf("Digite o novo telefone do usuario (tudo junto): ");
                            lerStr(atual->telefone, MAX_STRING);
                            printf("Telefone atualizado!\n");
                            atualizarArquivoUsuario();
                            return;
                        }
                        atual = atual->prox;
                    }
                    printf("Usuario nao encontrado ou esta inativo.\n\n");
                    break;

                case 5:
                    printf("Voltando ao menu anterior...\n");
                    break;
                default:
                    printf("Opcao invalida!\n");
                    break;
            }
            break;

        case 3:
            printf("Voltando ao menu principal...\n");
            break;
    }
}

void atualizarArquivoLivro() {
    FILE *arqLivros = fopen("ArquivoLivros.dat", "wb");
    if (arqLivros == NULL) {
        printf("Erro ao abrir arquivo de livros para atualizar.\n");
        return;
    }

    for (int i = 0; i < MAX_TAM; i++) {
        Livros* atual = tabelaLivros[i];
        while (atual != NULL) {
            if (atual->isbn[0] != '\0') {
                fwrite(atual, sizeof(Livros), 1, arqLivros);
            }
            atual = atual->prox;
        }
    }
    fclose(arqLivros);
}


void atualizarArquivoUsuario() {
    FILE *arqUsuarios = fopen("ArquivoUsuarios.dat", "wb");
    if (arqUsuarios == NULL) {
        printf("Erro ao abrir arquivo de usuarios para atualizar.\n");
        return;
    }

    for (int i = 0; i < MAX_TAM; i++) {
        Usuarios* atual = tabelaUsuarios[i];
        while (atual != NULL) {
            fwrite(atual, sizeof(Usuarios), 1, arqUsuarios);
            atual = atual->prox;
        }
    }
    fclose(arqUsuarios);
}

void emprestarLivro(){  
    char isbn[20];
    int id;
    char tempString[MAX_STRING];

    printf("\n\n------EMPRESTIMO DE LIVRO------\n");
    printf("Digite o ISBN do livro: ");
    lerStr(isbn, MAX_STRING);
    int hashL = hashISBN(isbn);
    Livros* livro = ConsultarISBN(hashL, isbn);

    if (livro == NULL) {
        printf("Livro nao encontrado.\n");
        return;
    } else if (livro->numCopias <= 0) {
        printf("Nao ha copias disponiveis.\n");
        return;
    }
    printf("Digite o ID do usuario: ");
    scanf("%d", &id);
    lerStr(tempString, MAX_STRING);
    int hashU = hashID(id);
    Usuarios* user = ConsultarID(hashU, id);

    if (user == NULL || user->ativo == 0) {
        printf("Usuario nao encontrado ou inativo.\n");
        return;
    }

    livro->numCopias--;
    FILE* arqLivros = fopen("ArquivoLivros.dat", "rb+");
    if (arqLivros == NULL) {
        printf("Erro ao abrir o arquivo de livros.\n");
        return;
    }
    Livros temp;
    while (fread(&temp, sizeof(Livros), 1, arqLivros)) {
        if (strcmp(temp.isbn, isbn) == 0) {
            temp.numCopias = livro->numCopias;
            fseek(arqLivros, -sizeof(Livros), SEEK_CUR);
            fwrite(&temp, sizeof(Livros), 1, arqLivros);
            break;
        }
    }
    fclose(arqLivros);

    FILE* emprestimos = fopen("Emprestimos.txt", "a");
    if (emprestimos == NULL) {
        printf("Erro ao abrir arquivo de emprestimos.\n");
        return;
    }
    fprintf(emprestimos, "Usuario com ID %d pegou o livro ISBN %s emprestado\n", id, isbn);
    fclose(emprestimos);
    printf("Emprestimo realizado com sucesso!\n");
}

void descarregarArquivos() {
    FILE *arqLivros = fopen("ArquivoLivros.dat", "rb");
    FILE *arqUsuarios = fopen("ArquivoUsuarios.dat", "rb");
    if (arqLivros == NULL || arqUsuarios == NULL) {
        return;
    }
    Livros temp;
    while (fread(&temp, sizeof(Livros), 1, arqLivros) == 1) {
        Livros* novo = (Livros*)malloc(sizeof(Livros));
        if (novo == NULL) {
            printf("Erro de alocação de livro.\n");
            break;
        }
        int ISBNhash = hashISBN(temp.isbn);
        *novo = temp;
        novo->prox = tabelaLivros[ISBNhash];
        tabelaLivros[ISBNhash] = novo;
    }

    Usuarios temp2;
    while (fread(&temp2, sizeof(Usuarios), 1, arqUsuarios) == 1) {
        Usuarios* novo2 = (Usuarios*)malloc(sizeof(Usuarios));
        if (novo2 == NULL) {
            printf("Erro de alocação de usuário.\n");
            break;
        }
        int IDhash = hashID(temp2.id);
        *novo2 = temp2;
        novo2->prox = tabelaUsuarios[IDhash];
        tabelaUsuarios[IDhash] = novo2;
    }
    fclose(arqLivros);
    fclose(arqUsuarios);
}


void devolutivaLivros() {
    char isbn[20];
    int id;
    char tempString[MAX_STRING];
    int diasAtraso = 0;
    float multa = 0.0;
    printf("\n------DEVOLUCAO DE LIVRO------\n");
    printf("Digite o ISBN do livro: ");
    lerStr(isbn, MAX_STRING);
    int hashL = hashISBN(isbn);
    Livros* livro = ConsultarISBN(hashL, isbn);
    if (livro == NULL) {
        printf("Livro nao encontrado.\n\n");
        return;
    }
    
    printf("Digite o ID do usuario: ");
    scanf("%d", &id);
    lerStr(tempString, MAX_STRING);
    int hashU = hashID(id);
    Usuarios* user = ConsultarID(hashU, id);
    if (user == NULL || user->ativo == 0) {
        printf("Usuario nao encontrado ou inativo.\n\n");
        return;
    }

    printf("Digite o numero de dias de atraso (0 se nenhum): ");
    scanf("%d", &diasAtraso);
    lerStr(tempString, MAX_STRING);
    if (diasAtraso > 0) {
        multa = diasAtraso * 0.50; 
        printf("Multa calculada: R$%.2f\n", multa);
    }
    livro->numCopias++;
    
    FILE* arqLivros = fopen("ArquivoLivros.dat", "rb+");
    if (arqLivros == NULL) {
        printf("Erro ao abrir o arquivo de livros.\n\n");
        return;
    }
    Livros tempLivro;
    while (fread(&tempLivro, sizeof(Livros), 1, arqLivros)) {
        if (strcmp(tempLivro.isbn, isbn) == 0) {
            tempLivro.numCopias = livro->numCopias;
            fseek(arqLivros, -sizeof(Livros), SEEK_CUR);
            fwrite(&tempLivro, sizeof(Livros), 1, arqLivros);
            break;
        }
    }
    fclose(arqLivros);
    FILE* transacoes = fopen("Transacoes.txt", "a");
    if (transacoes == NULL) {
        printf("Erro ao abrir arquivo de transacoes.\n\n");
        return;
    }
    fprintf(transacoes, "DEVOLUCAO - Livro ISBN: %s - Usuario ID: %d", isbn, id);
    if (diasAtraso > 0) {
        fprintf(transacoes, " - Dias de atraso: %d - Multa: R$%.2f", diasAtraso, multa);
    }
    fprintf(transacoes, "\n");
    fclose(transacoes);
    printf("Devolucao registrada com sucesso!\n");
    printf("Numero atual de copias disponiveis: %d\n", livro->numCopias);
    if (multa > 0) {
        printf("Multa a ser paga: R$%.2f\n", multa);
    }
}

void desativarUsuario(){
    int id;
    char tempString[MAX_STRING];
    printf("Digite o ID do usuario: ");
    scanf("%d", &id);
    lerStr(tempString, MAX_STRING);
    
    int IDhash = hashID(id);
    Usuarios* atual = tabelaUsuarios[IDhash];

    while (atual != NULL) {
        if (atual->id == id && atual->ativo == 1) {
                atual->ativo = 0;
                printf("Usuario foi inativado.\n");
                atualizarArquivoUsuario();
                    return;
                }
                atual = atual->prox;
    }
    printf("Usuario nao encontrado ou inativo.\n\n");
}   

void ativarUsuario(){
    int id;
    char tempString[MAX_STRING];
    printf("Digite o ID do usuario: ");
    scanf("%d", &id);
    lerStr(tempString, MAX_STRING);
    
    int IDhash = hashID(id);
    Usuarios* atual = tabelaUsuarios[IDhash];

    while (atual != NULL) {
        if (atual->id == id && atual->ativo == 0) {
                atual->ativo = 1;
                printf("Usuario foi ativado.\n");
                atualizarArquivoUsuario();
                    return;
                }
                atual = atual->prox;
    }
    printf("Usuario nao encontrado ou ja esta ativado.\n\n");
}

// gcc main_projeto.c projeto.c -o hash.exe
// ./hash
