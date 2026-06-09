#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura da sala (nó da árvore)
typedef struct Sala {
    char nome[50];
    struct Sala *esq;
    struct Sala *dir;
} Sala;

// Cria uma nova sala
Sala* criarSala(char* nome) {
    Sala* nova = (Sala*) malloc(sizeof(Sala));

    strcpy(nova->nome, nome);
    nova->esq = NULL;
    nova->dir = NULL;

    return nova;
}

// Exploração da mansão
void explorarSalas(Sala* atual) {
    char op;

    while (atual != NULL) {

        printf("\n===========================\n");
        printf("Você está em: %s\n", atual->nome);
        printf("===========================\n");

        // Se for sala final (folha)
        if (atual->esq == NULL && atual->dir == NULL) {
            printf("Você chegou a uma sala sem saídas!\n");
            break;
        }

        printf("Escolha o caminho:\n");
        printf("e - esquerda\n");
        printf("d - direita\n");
        printf("s - sair\n");
        printf("Opção: ");

        scanf(" %c", &op);

        if (op == 'e') {
            if (atual->esq != NULL) {
                atual = atual->esq;
            } else {
                printf("Não existe caminho à esquerda!\n");
            }
        }
        else if (op == 'd') {
            if (atual->dir != NULL) {
                atual = atual->dir;
            } else {
                printf("Não existe caminho à direita!\n");
            }
        }
        else if (op == 's') {
            printf("Exploração encerrada.\n");
            break;
        }
        else {
            printf("Opção inválida!\n");
        }
    }
}

int main() {

    // ===== CRIAÇÃO DA ÁRVORE =====

    Sala* hall = criarSala("Hall de Entrada");
    Sala* biblioteca = criarSala("Biblioteca");
    Sala* cozinha = criarSala("Cozinha");
    Sala* jardim = criarSala("Jardim");
    Sala* sotao = criarSala("Sótão");
    Sala* escritorio = criarSala("Escritório");

    // Conectando a árvore manualmente

    hall->esq = biblioteca;
    hall->dir = cozinha;

    biblioteca->esq = jardim;
    biblioteca->dir = sotao;

    cozinha->dir = escritorio;

    // ===== INÍCIO DO JOGO =====

    explorarSalas(hall);

    return 0;
}
