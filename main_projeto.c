#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "projeto.h"

int main() {
    Livros* Livro;
    Usuarios* Usuario;
    int opc;
    char isbn[20];
    int id;
    char tempString[MAX_STRING];
    inicializarTabelas();
    do {
    printf("---------------MENU-----------------\n");
    printf("1-Cadastrar Livro\n");
    printf("2-Consultar Livro\n");
    printf("3-Cadastrar Usuario\n");
    printf("4-Consultar Usuario\n");
    printf("5-Atualizar o Numero de Copias\n");    
    printf("6-Emprestimo de Livros\n");   
    printf("0-Encerrar\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &opc);
    getchar();
    switch(opc){
        case 1:
            printf("\nCADASTRO LIVRO\n");
            printf("Insira o ISBN do livro: ");
            lerStr(isbn, 20);
            int ISBNhash = hashISBN(isbn);
            Livro = ConsultarISBN(ISBNhash,isbn);
            if(Livro == NULL){
                cadastrarLivro(isbn);
                printf("Livro cadastrado com sucesso!\n");
            }else{
                char resp[3];
                printf("O livro ja existe na biblioteca\nDeseja adicionar uma nova copia? Resposta(S/N): ");
                lerStr(resp, 3);
                if (strcmp(resp, "S") == 0 || strcmp(resp, "s") == 0) {
                    atualizarNumCopias(isbn);
                }
            }
            break;
        case 2:
            printf("Insira o ISBN do livro: ");
            lerStr(isbn, 20);
            ISBNhash = hashISBN(isbn);
            Livro = ConsultarISBN(ISBNhash, isbn);
            if(Livro != NULL){
                exibirLivro(Livro);
            }else{
                printf("O livro nao existe!\n");
            }
            break;
        case 3:
            cadastrarUsuario();
            break;
        case 4:
            printf("Insira o ID do usuario: ");
            scanf("%d", &id);
            int IDhash = hashID(id);
            Usuario = ConsultarID(IDhash, id);
            if(Usuario != NULL){
                exibirUsuario(Usuario);
            }else{
                printf("O Usuario nao existe!\n");
            }
            break;
        case 5:
            printf("Digite o ISBN do livro: ");
            lerStr(isbn, MAX_STRING);   
            atualizarNumCopias(isbn);
            break;
        case 6:
            emprestarLivro();
            break;
        default:
            printf("Opcao invalida!\n");
            break;
        }
    } while(opc !=0);
    return 0;
}