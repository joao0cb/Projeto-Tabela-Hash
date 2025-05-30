# Sistema de Gerenciamento de Biblioteca

Projeto desenvolvido para a disciplina de **Estrutura de Dados Lineares** no curso de **Ciência da Computação** da **Universidade Católica de Pernambuco**, ministrada pelo professor Robson Lins.

## Equipe
- Bento Guilherme Gomes Oliveira
- João Victor Castelo Branco de Sena
- Lucas Mendes Nóbrega
- Caio Cordeiro Simões De Oliveira
- Theo Vieira Marcelino

## Objetivo
Este é um sistema de gerenciamento de biblioteca desenvolvido em C, que permite o cadastro, empréstimo, devolução de livros e a gestão de usuários. O projeto utiliza tabelas hash para armazenar livros e usuários, além de arquivos binários para persistência de dados.

## Funcionalidades
O sistema oferece as seguintes funcionalidades:

- Cadastro de livros e usuários.

- Empréstimo de livros.

- Devolução de livros com cálculo de multa por atraso.

- Exibição de todos os livros e usuários cadastrados.

- Ativação e desativação de usuários.

- Armazenamento de dados em arquivos binários e logs de transações.

## Estrutura de Dados
O sistema utiliza duas tabelas hash para gerenciar os dados dos livros e usuários:

- **Tabela de livros**: Utiliza o ISBN como chave para armazenar os livros.

- **Tabela de usuários**: Utiliza o ID do usuário como chave para armazenar os dados dos usuários

Os dados são persistidos em arquivos binários:

- **`ArquivoLivros.dat`**: Armazena os livros cadastrados.
- **`ArquivoUsuarios.dat`**: Armazena os usuários cadastrados.
- **`ArquivoEmprestimos.log`**: Registra os empréstimos realizados.
- **`Transacoes.txt`**: Registra todas as transações realizadas, como empréstimos e devoluções de livros.
