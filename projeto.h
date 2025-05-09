#define MAX_STRING 1000
#define MAX_TAM 100

typedef struct Livros{
    char isbn[MAX_STRING];
    char titulo[MAX_STRING];
    char autor[MAX_STRING];
    int ano;
    char editora[MAX_STRING];
    int numCopias;
    struct Livros* prox;
} Livros;

typedef struct Usuarios{
    int id;
    char nome[MAX_STRING];
    char email[MAX_STRING];
    int telefone;
    int ativo;
    struct Usuarios* prox;
} Usuarios;

// Tabelas Hash
Livros* tabelaLivros[MAX_TAM];
Usuarios* tabelaUsuarios[MAX_TAM];

void inicializarTabelas();
void lerStr(char *str, int tam);
int hashISBN(char* isbn);
int hashID(int id);
void atualizarNumCopias();

Livros* ConsultarISBN(int x, char* isbn);
void cadastrarLivro(char* isbn);
void exibirLivro(Livros* livro);

Usuarios* ConsultarID(int x, int id);
void cadastrarUsuario();
void exibirUsuario(Usuarios* user);

// void devolutivaDeLivros();
