#define MAX_STRING 1000
#define MAX_TAM 100

typedef struct Livros{
    char isbn[MAX_STRING];
    char titulo[MAX_STRING];
    char autor[MAX_STRING];
    int ano;
    char editora[MAX_STRING];
    int numCopias;
    int numEmprestimos;
    struct Livros* prox;
} Livros;   

typedef struct Usuarios{
    int id;
    char nome[MAX_STRING];
    char email[MAX_STRING];
    char telefone[MAX_STRING];
    int ativo;
    struct Usuarios* prox;
} Usuarios;

// Tabelas Hash
extern Livros* tabelaLivros[MAX_TAM];
extern Usuarios* tabelaUsuarios[MAX_TAM];

//
void descarregarArquivos();
void inicializarTabelas();
void lerStr(char *str, int tam);
int hashISBN(char* isbn);
int hashID(int id);
int apenasNumeros(char *str);
int apenasLetras(char *str);
int lerInteiro();

// Métodos de Atualizar
void atualizarNumCopias(char* isbn);
void atualizarDado();
void atualizarArquivoLivro();
void atualizarArquivoUsuario();
void excluirLivro();

// Métodos dos Livros 
Livros* ConsultarISBN(int x, char* isbn);
void cadastrarLivro(char* isbn);
void exibirLivro(Livros* Livro);
void emprestarLivro();
void devolutivaLivros();
void Livros_Mais_Emprestados();
void exibirTodosLivros();
int validarISBNFormatado(char *isbn);

// Métodos dos Usuários
Usuarios* ConsultarID(int x, int id);
void cadastrarUsuario();
void exibirUsuario(Usuarios* Usuario);
void ativarUsuario();
void desativarUsuario();
void exibirTodosUsuarios();
void relatorioUsuariosAtivos();
void relatorioEmprestimos();
int validarEmail(char *email);
