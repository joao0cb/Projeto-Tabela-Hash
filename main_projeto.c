#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "projeto.h"

int main() {
    inicializarTabelas();
    cadastrarLivro();
    cadastrarUsuario();
    devolutivaDeLivros();
    Livro* livro;
    Usuario* user;
    int opc;
    int x;
    char isbn[20];
    int id;
    do{
    printf("---------------MENU-----------------\n");
    printf("1-Cadastrar Livro\n");
    printf("2-Consultar Livro\n");
    printf("3-Cadastrar Usuário\n");
    printf("4-Consultar Usuário\n");
    printf("0-Encerrar\n");
    printf("Escolha uma opção: ");
    scanf("%d",&opc);
    getchar();
    switch(opc){
        case 1:
            printf("Insira o ISBN do livro: ");
            lerSTR(isbn,20);
        
            x = hashISBN(isbn);

            livro = ConsultarISBN(x,isbn);
            if(livro==NULL){
                Livro* novo_livro = cadastrarLivro(isbn); // Vai inserir no início
                novo_livro->prox = tabelaLivros[x];
                tabelaLivros[x] = novo_livro;
                printf("Livro cadastrado com sucesso!\n");
            }else{
                char resp[3];
                printf("O livro já existe na biblioteca\nDeseja adicionar uma nova cópia? Resposta(S/N): ");
                lerSTR(resp,3);
                if (strcmp(resp, "S") == 0 || strcmp(resp, "s") == 0) {
                    int x;
                    printf("Quantas de cópias que você quer adicionar: ");
                    scanf("%d",&x);
                    livro->copias += x;

                    printf("Cópias adicionadas! Total de cópias: %d\n", livro->copias);
                }
            }
            break;
        case 2:
            printf("Insira o ISBN do livro: ");
            lerSTR(isbn,20);
        
            x = hashISBN(isbn);

            livro = ConsultarISBN(x,isbn);
            if(livro!=NULL){
                exibirLivro(livro);
            }else{
                printf("O livro não existe!\n");
            }
            break;
        
        case 4:
            
            printf("Insira o ID do usuário: ");
            scanf("%d",&id);
        
            x = hashID(id);

            user = ConsultarID(x,id);
            if(user!=NULL){
                exibirUsuario(user);
            }else{
                printf("O Usuário não existe!\n");
            }
            break;
        default:
            break;
        }
    } while(opc !=0);
    return 0;
}
