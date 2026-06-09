#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ===============================
// ESTRUTURA DAS SALAS (MAPA)
// ===============================
typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala *esq;
    struct Sala *dir;
} Sala;

// ===============================
// ESTRUTURA DAS PISTAS (BST)
// ===============================
typedef struct Pista {
    char texto[100];
    struct Pista *esq;
    struct Pista *dir;
} Pista;

// ===============================
// CRIAR SALA
// ===============================
Sala* criarSala(char* nome, char* pista) {
    Sala* nova = (Sala*) malloc(sizeof(Sala));

    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);

    nova->esq = NULL;
    nova->dir = NULL;

    return nova;
}

// ===============================
// INSERIR PISTA NA BST
// ===============================
Pista* inserirPista(Pista* raiz, char* texto) {

    if (raiz == NULL) {
        Pista* nova = (Pista*) malloc(sizeof(Pista));
        strcpy(nova->texto, texto);
        nova->esq = NULL;
        nova->dir = NULL;
        return nova;
    }

    if (strcmp(texto, raiz->texto) < 0) {
        raiz->esq = inserirPista(raiz->esq, texto);
    } else {
        raiz->dir = inserirPista(raiz->dir, texto);
    }

    return raiz;
}

// ===============================
// EXIBIR PISTAS EM ORDEM
// ===============================
void exibirPistas(Pista* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esq);
        printf("- %s\n", raiz->texto);
        exibirPistas(raiz->dir);
    }
}

// ===============================
// EXPLORAR SALAS E COLETAR PISTAS
// ===============================
Pista* explorarSalasComPistas(Sala* atual, Pista* pistas) {

    char op;

    while (atual != NULL) {

        printf("\n=============================\n");
        printf("Você está em: %s\n", atual->nome);

        // coleta de pista
        if (strlen(atual->pista) > 0) {
            printf("🔎 Pista encontrada: %s\n", atual->pista);
            pistas = inserirPista(pistas, atual->pista);
        }

        printf("=============================\n");

        printf("e = esquerda | d = direita | s = sair\n");
        printf("Opção: ");
        scanf(" %c", &op);

        if (op == 'e') {
            if (atual->esq != NULL)
                atual = atual->esq;
            else
                printf("Sem caminho à esquerda!\n");
        }

        else if (op == 'd') {
            if (atual->dir != NULL)
                atual = atual->dir;
            else
                printf("Sem caminho à direita!\n");
        }

        else if (op == 's') {
            break;
        }

        else {
            printf("Opção inválida!\n");
        }
    }

    return pistas;
}

// ===============================
// MAIN
// ===============================
int main() {

    Pista* pistas = NULL;

    // ===== MAPA DA MANSÃO =====

    Sala* hall = criarSala("Hall de Entrada", "Pegadas na entrada");
    Sala* salaEstar = criarSala("Sala de Estar", "Vaso quebrado");
    Sala* cozinha = criarSala("Cozinha", "Facas fora do lugar");
    Sala* biblioteca = criarSala("Biblioteca", "Livro aberto na mesa");
    Sala* sotao = criarSala("Sótão", "Janela aberta");

    // ligação da árvore
    hall->esq = salaEstar;
    hall->dir = cozinha;

    salaEstar->esq = biblioteca;
    salaEstar->dir = sotao;

    // ===== EXPLORAÇÃO =====

    pistas = explorarSalasComPistas(hall, pistas);

    // ===== RESULTADO FINAL =====

    printf("\n=============================\n");
    printf("PISTAS COLETADAS (ORDEM ALFABÉTICA)\n");
    printf("=============================\n");

    exibirPistas(pistas);

    return 0;
}
