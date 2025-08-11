# Sistema de Gerenciamento de Biblioteca 📚

Este projeto é um sistema em **C** para gerenciamento de livros, armazenados em arquivo binário utilizando **árvore binária de busca (ABB)** para organização.  
O sistema permite cadastro, consulta, remoção, listagem e importação de livros a partir de arquivos `.txt`, além de gerenciamento de registros livres.

---

## 🚀 Funcionalidades

### 1️⃣ Cadastrar um livro
- Solicita código, título, autor, editora, edição, ano, exemplares e preço.
- Insere o livro na árvore binária de forma ordenada.
- Reaproveita registros livres do arquivo binário.

### 2️⃣ Imprimir dados de um livro
- Busca por **código** e exibe todas as informações.
- Exibe *"Livro nao encontrado"* caso o código não exista.

### 3️⃣ Listar todos os livros
- Lista todos os livros cadastrados em ordem crescente de código.
- Mostra apenas código, título, autor e exemplares.
- Caso não haja livros, exibe *"Nenhum livro cadastrado"*.

### 4️⃣ Mostrar total de livros
- Exibe a quantidade total de livros cadastrados.

### 5️⃣ Remover um livro
- Solicita o código do livro a ser removido.
- Marca o registro como livre para reutilização.
- Caso não encontrado, exibe *"Livro não encontrado"*.

### 6️⃣ Carregar livros de um arquivo TXT
- Lê um arquivo `.txt` no formato:
- Ignora livros com códigos já existentes.
- Converte vírgulas no preço para ponto antes de salvar.

### 7️⃣ Imprimir lista de registros livres
- Mostra todos os índices livres no arquivo binário.
- Caso não haja, exibe *"Nenhum registro livre"*.

### 8️⃣ Imprimir árvore por níveis
- Exibe a árvore binária nível a nível.
- Caso vazia, mostra *"Arvore vazia"*.

### 0️⃣ Sair
- Salva todas as alterações e encerra o programa.


## 📂 Estrutura de Arquivos

