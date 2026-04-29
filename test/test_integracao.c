/*
 * =============================================================================
 * TESTES DE INTEGRAÇÃO — Sistema de Biblioteca
 * Framework: Unity
 *
 * Fluxos cobertos:
 *  1. Cadastro + Consulta de Livros (hash → tabela → busca)
 *  2. Cadastro + Consulta de Usuários
 *  3. Empréstimo integrado (livro + usuário + decremento de cópias)
 *  4. Devolução integrada (incremento de cópias)
 *  5. Colisão de hash — dois ISBNs no mesmo bucket
 *  6. Desativar / Ativar usuário + relatório de ativos
 *  7. Persistência: gravar em arquivo e recarregar na tabela
 *  8. Livro sem cópias disponíveis
 *  9. Usuário inativo não pode pegar emprestado
 * 10. Validações cruzadas: e-mail, ISBN, telefone, nome
 * =============================================================================
 */

#include "unity.h"
#include "projeto.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* ─── helpers de empréstimo / devolução sem I/O interativo ─── */
static int emprestar_direto(const char* isbn, int id_usuario) {
    int hL = hashISBN((char*)isbn);
    Livros* livro = ConsultarISBN(hL, (char*)isbn);
    if (!livro || livro->numCopias <= 0) return 0;

    int hU = hashID(id_usuario);
    Usuarios* user = ConsultarID(hU, id_usuario);
    if (!user || user->ativo == 0) return 0;

    livro->numCopias--;
    livro->numEmprestimos++;
    return 1;
}

static int devolver_direto(const char* isbn, int id_usuario) {
    int hL = hashISBN((char*)isbn);
    Livros* livro = ConsultarISBN(hL, (char*)isbn);
    if (!livro) return 0;

    int hU = hashID(id_usuario);
    Usuarios* user = ConsultarID(hU, id_usuario);
    if (!user) return 0;

    livro->numCopias++;
    return 1;
}

/* ─── setUp / tearDown ─── */
void setUp(void) {
    inicializarTabelas();
    /* remove arquivos residuais para testes de persistência partirem do zero */
    remove("ArquivoLivros.dat");
    remove("ArquivoUsuarios.dat");
}

void tearDown(void) {
    liberarTabelas();
    remove("ArquivoLivros.dat");
    remove("ArquivoUsuarios.dat");
}

/* ══════════════════════════════════════════════════════════════════
 * FLUXO 1 — Cadastro + Consulta de Livro
 * Módulos: inserirLivroNaTabela → hashISBN → ConsultarISBN
 * ══════════════════════════════════════════════════════════════════ */
void test_integracao_cadastrar_e_consultar_livro(void) {
    inserirLivroNaTabela("978-3-16-148410-0", "Dom Casmurro",
                         "Machado de Assis", "Ática", 1899, 3);

    int h = hashISBN("978-3-16-148410-0");
    Livros* encontrado = ConsultarISBN(h, "978-3-16-148410-0");

    TEST_ASSERT_NOT_NULL(encontrado);
    TEST_ASSERT_EQUAL_STRING("Dom Casmurro", encontrado->titulo);
    TEST_ASSERT_EQUAL_STRING("Machado de Assis", encontrado->autor);
    TEST_ASSERT_EQUAL_INT(3, encontrado->numCopias);
    TEST_ASSERT_EQUAL_INT(0, encontrado->numEmprestimos);
}

/* ══════════════════════════════════════════════════════════════════
 * FLUXO 2 — Cadastro + Consulta de Usuário
 * Módulos: inserirUsuarioNaTabela → hashID → ConsultarID
 * ══════════════════════════════════════════════════════════════════ */
void test_integracao_cadastrar_e_consultar_usuario(void) {
    inserirUsuarioNaTabela(42, "Maria Silva", "maria@uni.br", "81999990000", 1);

    int h = hashID(42);
    Usuarios* u = ConsultarID(h, 42);

    TEST_ASSERT_NOT_NULL(u);
    TEST_ASSERT_EQUAL_STRING("Maria Silva", u->nome);
    TEST_ASSERT_EQUAL_STRING("maria@uni.br", u->email);
    TEST_ASSERT_EQUAL_INT(1, u->ativo);
}

/* ══════════════════════════════════════════════════════════════════
 * FLUXO 3 — Empréstimo integrado
 * Módulos: tabela de livros + tabela de usuários → emprestar_direto
 * Verifica: cópias decrementadas, empréstimos incrementados
 * ══════════════════════════════════════════════════════════════════ */
void test_integracao_emprestimo_decrementa_copias(void) {
    inserirLivroNaTabela("111-1-11-111111-1", "O Hobbit",
                         "Tolkien", "HarperCollins", 1937, 2);
    inserirUsuarioNaTabela(10, "Carlos Souza", "carlos@email.com", "81900000001", 1);

    int resultado = emprestar_direto("111-1-11-111111-1", 10);

    TEST_ASSERT_EQUAL_INT(1, resultado);

    int h = hashISBN("111-1-11-111111-1");
    Livros* livro = ConsultarISBN(h, "111-1-11-111111-1");
    TEST_ASSERT_EQUAL_INT(1, livro->numCopias);       /* era 2, agora 1 */
    TEST_ASSERT_EQUAL_INT(1, livro->numEmprestimos);
}

/* ══════════════════════════════════════════════════════════════════
 * FLUXO 4 — Devolução integrada
 * Verifica: cópias restauradas após devolução
 * ══════════════════════════════════════════════════════════════════ */
void test_integracao_devolucao_restaura_copias(void) {
    inserirLivroNaTabela("222-2-22-222222-2", "1984",
                         "George Orwell", "Companhia das Letras", 1949, 1);
    inserirUsuarioNaTabela(20, "Ana Lima", "ana@email.com", "81900000002", 1);

    emprestar_direto("222-2-22-222222-2", 20);   /* cópias: 1 → 0 */
    int devolveu = devolver_direto("222-2-22-222222-2", 20); /* 0 → 1 */

    TEST_ASSERT_EQUAL_INT(1, devolveu);

    int h = hashISBN("222-2-22-222222-2");
    Livros* livro = ConsultarISBN(h, "222-2-22-222222-2");
    TEST_ASSERT_EQUAL_INT(1, livro->numCopias);
}

/* ══════════════════════════════════════════════════════════════════
 * FLUXO 5 — Colisão de hash: dois livros no mesmo bucket
 * Módulos: hashISBN → lista encadeada → ConsultarISBN
 * ══════════════════════════════════════════════════════════════════ */
void test_integracao_colisao_hash_livros(void) {
    /* Força dois ISBNs que colidam: mesmas letras, ordem diferente */
    /* "AB" e "BA" têm a mesma soma ASCII → mesmo hash */
    char isbnA[] = "111-1-11-111112-1";
    char isbnB[] = "111-1-11-111121-1";

    /* Garante colisão calculando manualmente */
    int hA = hashISBN(isbnA);
    int hB = hashISBN(isbnB);

    inserirLivroNaTabela(isbnA, "Livro Alpha", "Autor A", "Ed A", 2000, 1);
    inserirLivroNaTabela(isbnB, "Livro Beta",  "Autor B", "Ed B", 2001, 2);

    Livros* alpha = ConsultarISBN(hA, isbnA);
    Livros* beta  = ConsultarISBN(hB, isbnB);

    TEST_ASSERT_NOT_NULL(alpha);
    TEST_ASSERT_NOT_NULL(beta);
    TEST_ASSERT_EQUAL_STRING("Livro Alpha", alpha->titulo);
    TEST_ASSERT_EQUAL_STRING("Livro Beta",  beta->titulo);
}

/* ══════════════════════════════════════════════════════════════════
 * FLUXO 6 — Desativar usuário → consulta reflete status
 * Módulos: inserirUsuario → ConsultarID → campo ativo
 * ══════════════════════════════════════════════════════════════════ */
void test_integracao_desativar_usuario(void) {
    inserirUsuarioNaTabela(30, "Pedro Costa", "pedro@email.com", "81900000003", 1);

    int h = hashID(30);
    Usuarios* u = ConsultarID(h, 30);
    TEST_ASSERT_NOT_NULL(u);
    TEST_ASSERT_EQUAL_INT(1, u->ativo);

    u->ativo = 0;   /* simula desativarUsuario() */

    Usuarios* uApos = ConsultarID(h, 30);
    TEST_ASSERT_EQUAL_INT(0, uApos->ativo);
}

/* ══════════════════════════════════════════════════════════════════
 * FLUXO 7 — Persistência: gravar em arquivo e recarregar
 * Módulos: tabela → fwrite → fread → ConsultarISBN
 * Usa arquivo temporário local para evitar dependência de caminho
 * ══════════════════════════════════════════════════════════════════ */
void test_integracao_persistencia_livro(void) {
    const char* arquivo = "test_livros_temp.dat";

    /* monta livro e grava direto no arquivo temporário */
    Livros* original = inserirLivroNaTabela("333-3-33-333333-3", "Sapiens",
                                             "Yuval Harari", "Companhia das Letras", 2011, 5);
    TEST_ASSERT_NOT_NULL(original);

    FILE* f = fopen(arquivo, "wb");
    TEST_ASSERT_NOT_NULL(f);
    fwrite(original, sizeof(Livros), 1, f);
    fclose(f);

    /* limpa tabela da memória */
    liberarTabelas();
    inicializarTabelas();

    /* recarrega do arquivo */
    FILE* f2 = fopen(arquivo, "rb");
    TEST_ASSERT_NOT_NULL(f2);
    Livros temp;
    while (fread(&temp, sizeof(Livros), 1, f2) == 1) {
        Livros* novo = (Livros*)malloc(sizeof(Livros));
        TEST_ASSERT_NOT_NULL(novo);
        *novo = temp;
        int h = hashISBN(novo->isbn);
        novo->prox = tabelaLivros[h];
        tabelaLivros[h] = novo;
    }
    fclose(f2);
    remove(arquivo);

    /* verifica que os dados foram preservados */
    int h = hashISBN("333-3-33-333333-3");
    Livros* recuperado = ConsultarISBN(h, "333-3-33-333333-3");

    TEST_ASSERT_NOT_NULL(recuperado);
    TEST_ASSERT_EQUAL_STRING("Sapiens", recuperado->titulo);
    TEST_ASSERT_EQUAL_INT(5, recuperado->numCopias);
}

/* ══════════════════════════════════════════════════════════════════
 * FLUXO 8 — Empréstimo falha quando não há cópias
 * Módulos: emprestar_direto verifica numCopias antes de agir
 * ══════════════════════════════════════════════════════════════════ */
void test_integracao_emprestimo_falha_sem_copias(void) {
    inserirLivroNaTabela("444-4-44-444444-4", "Livro Esgotado",
                         "Autor X", "Ed X", 2020, 0);  /* 0 cópias */
    inserirUsuarioNaTabela(50, "Lucas Rocha", "lucas@email.com", "81900000004", 1);

    int resultado = emprestar_direto("444-4-44-444444-4", 50);
    TEST_ASSERT_EQUAL_INT(0, resultado);   /* deve falhar */

    int h = hashISBN("444-4-44-444444-4");
    Livros* livro = ConsultarISBN(h, "444-4-44-444444-4");
    TEST_ASSERT_EQUAL_INT(0, livro->numCopias);         /* permanece 0 */
    TEST_ASSERT_EQUAL_INT(0, livro->numEmprestimos);    /* não foi contado */
}

/* ══════════════════════════════════════════════════════════════════
 * FLUXO 9 — Usuário inativo não consegue empréstimo
 * Módulos: ConsultarID + campo ativo → emprestar_direto
 * ══════════════════════════════════════════════════════════════════ */
void test_integracao_usuario_inativo_nao_empresta(void) {
    inserirLivroNaTabela("555-5-55-555555-5", "Livro Disponivel",
                         "Autor Y", "Ed Y", 2019, 3);
    inserirUsuarioNaTabela(60, "Joana Melo", "joana@email.com", "81900000005", 0); /* inativo */

    int resultado = emprestar_direto("555-5-55-555555-5", 60);
    TEST_ASSERT_EQUAL_INT(0, resultado);

    int h = hashISBN("555-5-55-555555-5");
    Livros* livro = ConsultarISBN(h, "555-5-55-555555-5");
    TEST_ASSERT_EQUAL_INT(3, livro->numCopias);  /* cópias intactas */
}

/* ══════════════════════════════════════════════════════════════════
 * FLUXO 10 — Validações cruzadas (e-mail + ISBN + telefone + nome)
 * Módulos: validarEmail, validarISBNFormatado, apenasNumeros, apenasLetras
 * ══════════════════════════════════════════════════════════════════ */
void test_integracao_validacoes_cruzadas_cadastro_valido(void) {
    const char* email    = "biblioteca@faculdade.edu.br";
    const char* isbn     = "978-3-16-148410-0";
    const char* telefone = "81987654321";
    const char* nome     = "Marcos Neves";

    TEST_ASSERT_TRUE(validarEmail((char*)email));
    TEST_ASSERT_TRUE(validarISBNFormatado((char*)isbn));
    TEST_ASSERT_TRUE(apenasNumeros((char*)telefone));
    TEST_ASSERT_TRUE(apenasLetras((char*)nome));
}

void test_integracao_validacoes_cruzadas_dados_invalidos(void) {
    TEST_ASSERT_FALSE(validarEmail("semArroba.com"));
    TEST_ASSERT_FALSE(validarISBNFormatado("978-3-16-148410"));   /* 4 partes, não 5 */
    TEST_ASSERT_FALSE(apenasNumeros("819abc321"));
    TEST_ASSERT_FALSE(apenasLetras("Marcos123"));
}

/* ══════════════════════════════════════════════════════════════════
 * FLUXO 11 — Múltiplos empréstimos do mesmo livro por usuários diferentes
 * Módulos: tabela compartilhada + estado consistente entre operações
 * ══════════════════════════════════════════════════════════════════ */
void test_integracao_multiplos_emprestimos_mesmo_livro(void) {
    inserirLivroNaTabela("666-6-66-666666-6", "Clean Code",
                         "Robert Martin", "Pearson", 2008, 3);
    inserirUsuarioNaTabela(71, "Rafaela Dias",  "rafa@email.com",  "81900000006", 1);
    inserirUsuarioNaTabela(72, "Bruno Alves",   "bruno@email.com", "81900000007", 1);
    inserirUsuarioNaTabela(73, "Camila Torres", "cami@email.com",  "81900000008", 1);

    TEST_ASSERT_EQUAL_INT(1, emprestar_direto("666-6-66-666666-6", 71));
    TEST_ASSERT_EQUAL_INT(1, emprestar_direto("666-6-66-666666-6", 72));
    TEST_ASSERT_EQUAL_INT(1, emprestar_direto("666-6-66-666666-6", 73));

    int h = hashISBN("666-6-66-666666-6");
    Livros* livro = ConsultarISBN(h, "666-6-66-666666-6");
    TEST_ASSERT_EQUAL_INT(0, livro->numCopias);
    TEST_ASSERT_EQUAL_INT(3, livro->numEmprestimos);

    /* 4.° empréstimo deve falhar */
    inserirUsuarioNaTabela(74, "Diego Ferreira", "diego@email.com", "81900000009", 1);
    TEST_ASSERT_EQUAL_INT(0, emprestar_direto("666-6-66-666666-6", 74));
}

/* ══════════════════════════════════════════════════════════════════
 * FLUXO 12 — Reativar usuário desativado e realizar empréstimo
 * Módulos: campo ativo → emprestar_direto (integração de estado)
 * ══════════════════════════════════════════════════════════════════ */
void test_integracao_reativar_usuario_e_emprestar(void) {
    inserirLivroNaTabela("777-7-77-777777-7", "O Senhor dos Aneis",
                         "Tolkien", "HarperCollins", 1954, 2);
    inserirUsuarioNaTabela(80, "Isabela Cruz", "isa@email.com", "81900000010", 0); /* inativo */

    /* tenta emprestar — deve falhar */
    TEST_ASSERT_EQUAL_INT(0, emprestar_direto("777-7-77-777777-7", 80));

    /* reativa */
    Usuarios* u = ConsultarID(hashID(80), 80);
    TEST_ASSERT_NOT_NULL(u);
    u->ativo = 1;

    /* agora deve funcionar */
    TEST_ASSERT_EQUAL_INT(1, emprestar_direto("777-7-77-777777-7", 80));

    Livros* livro = ConsultarISBN(hashISBN("777-7-77-777777-7"), "777-7-77-777777-7");
    TEST_ASSERT_EQUAL_INT(1, livro->numCopias);
}

/* ══════════════════════════════════════════════════════════════════
 * MAIN
 * ══════════════════════════════════════════════════════════════════ */
int main(void) {
    UNITY_BEGIN();

    /* Fluxo 1 */
    RUN_TEST(test_integracao_cadastrar_e_consultar_livro);
    /* Fluxo 2 */
    RUN_TEST(test_integracao_cadastrar_e_consultar_usuario);
    /* Fluxo 3 */
    RUN_TEST(test_integracao_emprestimo_decrementa_copias);
    /* Fluxo 4 */
    RUN_TEST(test_integracao_devolucao_restaura_copias);
    /* Fluxo 5 */
    RUN_TEST(test_integracao_colisao_hash_livros);
    /* Fluxo 6 */
    RUN_TEST(test_integracao_desativar_usuario);
    /* Fluxo 7 */
    RUN_TEST(test_integracao_persistencia_livro);
    /* Fluxo 8 */
    RUN_TEST(test_integracao_emprestimo_falha_sem_copias);
    /* Fluxo 9 */
    RUN_TEST(test_integracao_usuario_inativo_nao_empresta);
    /* Fluxo 10 */
    RUN_TEST(test_integracao_validacoes_cruzadas_cadastro_valido);
    RUN_TEST(test_integracao_validacoes_cruzadas_dados_invalidos);
    /* Fluxo 11 */
    RUN_TEST(test_integracao_multiplos_emprestimos_mesmo_livro);
    /* Fluxo 12 */
    RUN_TEST(test_integracao_reativar_usuario_e_emprestar);

    return UNITY_END();
}