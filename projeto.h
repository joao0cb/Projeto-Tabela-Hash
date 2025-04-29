#define MAX_STRING 1000
#define MAX_TAM 100

typedef struct Livros{
    char isbn[MAX_STRING];
    char titulo[MAX_STRING];
    char autor[MAX_STRING];
    int ano;
    char editora[MAX_STRING];
    int num_copias;
    struct Livro* prox;
} Livros;

typedef struct Usuarios{
    int id;
    char nome[MAX_STRING];
    char email[MAX_STRING];
    int telefone;
} Usuario;

Livros* tabelaLivros[MAX_TAM];
Usuarios* tabelaUsuarios[MAX_TAM];