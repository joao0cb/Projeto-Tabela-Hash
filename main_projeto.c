#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "projeto.h"

int main() {
    inicializarTabelas();
    descarregarArquivos();
    Livros* Livro;
    Usuarios* Usuario;
    int opc;
    char isbn[20];
    int id;
    char tempString[MAX_STRING];
    do {
    printf("---------------MENU-----------------\n");
    printf("1-Cadastrar Livro\n");
    printf("2-Consultar Livro\n");
    printf("3-Cadastrar Usuario\n");
    printf("4-Consultar Usuario\n");
    printf("5-Atualizar Dado\n");   
    printf("6-Emprestimo de Livros\n");   
    printf("7-Devolutiva de Livros\n");
    printf("8-Ativar Usuario\n");
    printf("9-Consultar Livros mais emprestados\n");
    printf("10-Desativar Usuario\n");
    printf("0-Encerrar\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &opc);
    lerStr(tempString, MAX_STRING);
    switch(opc){
        case 1:
            printf("\nCADASTRO LIVRO\n");
            printf("Insira o ISBN do livro: ");
            lerStr(isbn, 20);
            int ISBNhash = hashISBN(isbn);
            Livro = ConsultarISBN(ISBNhash,isbn);
            if(Livro == NULL){
                cadastrarLivro(isbn);
                printf("Livro cadastrado com sucesso!\n\n");
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
            lerStr(tempString, MAX_STRING);
            int IDhash = hashID(id);
            Usuario = ConsultarID(IDhash, id);
            if(Usuario != NULL){
                exibirUsuario(Usuario);
            }else{
                printf("O Usuario nao existe!\n");
            }
            break;
        case 5:
            atualizarDado();
            break;
        case 6:
            emprestarLivro();
            break;
        case 7:
            devolutivaLivros();
            break;
        case 8:
            ativarUsuario();
            break;
        case 9:
            Livros_Mais_Emprestados();
            break;
        case 10:
            desativarUsuario();
            break;
        case 0:
            break;
        default:
            printf("Opcao invalida!\n");
            break;
                }
    } while(opc !=0);
    return 0;
}
