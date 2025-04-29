#define MAX_STRING 1000

typedef struct Livros {
    char isbn[MAX_STRING];
    char titulo[MAX_STRING];
    char autor[MAX_STRING];
    int ano;
    char editora[MAX_STRING];
    int num_copias;
    struct Livro* prox;
} Livros;

typedef struct Usuario {
    int id;
    char nome[MAX_STRING];
    char email[MAX_STRING];
    int telefone;
} Usuario;