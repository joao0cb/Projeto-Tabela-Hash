#include "unity.h"
#include "projeto.h" // Certifique-se de que as funções estão declaradas aqui

void setUp(void) {}
void tearDown(void) {}

// --- TESTES DE CENÁRIOS PRINCIPAIS ---
void test_validarEmail_correto(void) {
    TEST_ASSERT_TRUE(validarEmail("joao@estudante.com"));
}

void test_hashISBN_consistencia(void) {
    // Mesmos ISBNs devem gerar o mesmo hash
    TEST_ASSERT_EQUAL(hashISBN("123-45"), hashISBN("123-45"));
}

// --- TESTES DE CASOS DE ERRO ---
void test_validarEmail_sem_arroba(void) {
    TEST_ASSERT_FALSE(validarEmail("joao.com"));
}

void test_validarISBN_formato_errado(void) {
    // O sistema espera 5 partes separadas por '-'
    TEST_ASSERT_FALSE(validarISBNFormatado("123-456-789")); 
}

// --- TESTES DE LIMITE (BOUNDARY) ---
void test_apenasLetras_string_vazia(void) {
    TEST_ASSERT_TRUE(apenasLetras("")); // Uma string vazia tecnicamente não tem números
}

void test_apenasLetras_com_espacos(void) {
    TEST_ASSERT_TRUE(apenasLetras("Joao Victor")); // Limite: aceitar espaços
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_validarEmail_correto);
    RUN_TEST(test_hashISBN_consistencia);
    RUN_TEST(test_validarEmail_sem_arroba);
    RUN_TEST(test_validarISBN_formato_errado);
    RUN_TEST(test_apenasLetras_string_vazia);
    RUN_TEST(test_apenasLetras_com_espacos);
    return UNITY_END();
}