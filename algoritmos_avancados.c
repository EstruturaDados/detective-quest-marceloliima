#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 11

// ===============================
// ESTRUTURAS DE DADOS
// ===============================

// Estrutura das Salas (Árvore Binária de Navegação)
typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala *esq;
    struct Sala *dir;
} Sala;

// Estrutura das Pistas Coletadas (BST)
typedef struct Pista {
    char texto[100];
    struct Pista *esq;
    struct Pista *dir;
} Pista;

// Elemento da Tabela Hash (Encadeamento para colisões)
typedef struct NodoHash {
    char pista[100];
    char suspeito[50];
    struct NodoHash *proximo;
} NodoHash;

// Tabela Hash
typedef struct {
    NodoHash *tabela[TAM_HASH];
} TabelaHash;


// ===============================
// FUNÇÕES DA TABELA HASH
// ===============================

// Função de Hash simples (djb2) para mapear a string a um índice
unsigned long gerarHash(char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % TAM_HASH;
}

/**
 * DOCUMENTAÇÃO OBRIGATÓRIA:
 * inserirNaHash() – Insere a associação estática entre uma pista (chave)
 * e um suspeito (valor) na tabela hash para permitir a consulta rápida.
 */
void inserirNaHash(TabelaHash *th, char *pista, char *suspeito) {
    unsigned long indice = gerarHash(pista);
    
    NodoHash *novo = (NodoHash*) malloc(sizeof(NodoHash));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->proximo = th->tabela[indice];
    
    th->tabela[indice] = novo;
}

/**
 * DOCUMENTAÇÃO OBRIGATÓRIA:
 * encontrarSuspeito() – Consulta a tabela hash passando o texto da pista como 
 * chave e retorna o nome do suspeito associado a ela. Retorna NULL se não achar.
 */
char* encontrarSuspeito(TabelaHash *th, char *pista) {
    unsigned long indice = gerarHash(pista);
    NodoHash *atual = th->tabela[indice];
    
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito;
        }
        atual = atual->proximo;
    }
    return NULL;
}

// Inicializa os ponteiros da tabela hash como NULL
void inicializarHash(TabelaHash *th) {
    for (int i = 0; i < TAM_HASH; i++) {
        th->tabela[i] = NULL;
    }
}


// ===============================
// FUNÇÕES DE ÁRVORES (SALA E BST)
// ===============================

/**
 * DOCUMENTAÇÃO OBRIGATÓRIA:
 * criarSala() – Cria dinamicamente um cômodo da mansão, alocando memória,
 * definindo seu nome, a pista estática do local e limpando os ponteiros filhos.
 */
Sala* criarSala(char* nome, char* pista) {
    Sala* nova = (Sala*) malloc(sizeof(Sala));
    if (nova != NULL) {
        strcpy(nova->nome, nome);
        strcpy(nova->pista, pista);
        nova->esq = NULL;
        nova->dir = NULL;
    }
    return nova;
}

/**
 * DOCUMENTAÇÃO OBRIGATÓRIA:
 * inserirPista() / adicionarPista() – Insere a pista coletada na árvore de pistas (BST)
 * de forma ordenada (alfabeticamente), evitando duplicatas caso o jogador revisite a sala.
 */
Pista* inserirPista(Pista* raiz, char* texto) {
    if (raiz == NULL) {
        Pista* nova = (Pista*) malloc(sizeof(Pista));
        strcpy(nova->texto, texto);
        nova->esq = NULL;
        nova->dir = NULL;
        return nova;
    }

    int comparacao = strcmp(texto, raiz->texto);
    if (comparacao < 0) {
        raiz->esq = inserirPista(raiz->esq, texto);
    } else if (comparacao > 0) {
        raiz->dir = inserirPista(raiz->dir, texto);
    }
    // Se for igual (comparacao == 0), não faz nada (evita duplicar na BST)

    return raiz;
}

// Exibe as pistas armazenadas na BST em ordem alfabética (In-order traversal)
void exibirPistas(Pista* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esq);
        printf(" - %s\n", raiz->texto);
        exibirPistas(raiz->dir);
    }
}


// ===============================
// LÓGICA DE JOGO E EXPLORAÇÃO
// ===============================

/**
 * DOCUMENTAÇÃO OBRIGATÓRIA:
 * explorarSalas() – Navega de forma interativa pela árvore de cômodos (mapa), 
 * detecta e exibe pistas e as adiciona automaticamente à BST de pistas coletadas.
 */
Pista* explorarSalas(Sala* atual, Pista* pistas) {
    char op;

    while (atual != NULL) {
        printf("\n==================================================\n");
        printf(" 🧭 VOCÊ ESTÁ EM: %s\n", atual->nome);

        // Se houver pista na sala, faz a coleta automática
        if (strlen(atual->pista) > 0) {
            printf(" 🔎 PISTA ENCONTRADA: \"%s\"\n", atual->pista);
            pistas = inserirPista(pistas, atual->pista);
        } else {
            printf(" 🔎 Nenhuma pista relevante por aqui.\n");
        }
        printf("==================================================\n");

        printf("Escolha sua direção:\n");
        printf("[e] Ir para a Esquerda\n");
        printf("[d] Ir para a Direita\n");
        printf("[s] Encerrar Exploração e ir para o Julgamento\n");
        printf("Opção: ");
        scanf(" %c", &op);

        if (op == 'e') {
            if (atual->esq != NULL)
                atual = atual->esq;
            else
                printf("\n🚫 [Parede] Sem caminho à esquerda nesta sala!\n");
        }
        else if (op == 'd') {
            if (atual->dir != NULL)
                atual = atual->dir;
            else
                printf("\n🚫 [Parede] Sem caminho à direita nesta sala!\n");
        }
        else if (op == 's') {
            printf("\nFinishing exploration... Convocando os suspeitos!\n");
            break;
        }
        else {
            printf("\n⚠️ Opção inválida! Digite 'e', 'd' ou 's'.\n");
        }
    }
    return pistas;
}

// Função auxiliar recursiva que cruza os dados da BST com a Hash
// Conta quantas pistas na BST apontam para o suspeito acusado
int contarPistasDoSuspeito(Pista* raiz_pista, TabelaHash* th, char* suspeitoAcusado) {
    if (raiz_pista == NULL) return 0;

    int contagem = 0;
    
    // Consulta a Hash para ver quem é o dono desta pista da BST
    char* suspeitoDaPista = encontrarSuspeito(th, raiz_pista->texto);
    
    if (suspeitoDaPista != NULL && strcmp(suspeitoDaPista, suspeitoAcusado) == 0) {
        contagem = 1;
    }

    // Soma com os resultados das subárvores esquerda e direita
    return contagem + 
           contarPistasDoSuspeito(raiz_pista->esq, th, suspeitoAcusado) + 
           contarPistasDoSuspeito(raiz_pista->dir, th, suspeitoAcusado);
}

/**
 * DOCUMENTAÇÃO OBRIGATÓRIA:
 * verificarSuspeitoFinal() – Conduz a fase de julgamento. Solicita a acusação do jogador,
 * varre as pistas coletadas utilizando a tabela hash e avalia se há pelo menos 2 evidências.
 */
void verificarSuspeitoFinal(Pista* pistasColetadas, TabelaHash* th) {
    char acusado[50];

    printf("\n==================================================\n");
    printf("         TRIBUNAL: A HORA DA ACUSAÇÃO             \n");
    printf("==================================================\n");
    printf("Digite o nome do suspeito que você deseja acusar: ");
    
    // Limpa o buffer e lê o nome (aceitando espaços, ex: "Mordomo James")
    scanf(" %[^\n]", acusado);

    // Conta recursivamente na BST cruzando dados com a Hash
    int totalEvidencias = contarPistasDoSuspeito(pistasColetadas, th, acusado);

    printf("\n================ BANCO DE PROVAS =================\n");
    printf("Suspeito Acusado: %s\n", acusado);
    printf("Quantidade de evidências coletadas contra ele: %d\n", totalEvidencias);
    printf("==================================================\n");

    // Condição de vitória/derrota baseada nas regras de negócio (mínimo 2 pistas)
    if (totalEvidencias >= 2) {
        printf("      🏆 PARABÉNS! INVESTIGAÇÃO BEM SUCEDIDA!      \n");
        printf("Você reuniu provas suficientes. %s foi preso!\n", acusado);
    } else {
        printf("      ❌ CASO ARQUIVADO POR FALTA DE PROVAS!        \n");
        printf("O juiz rejeitou a acusação contra %s.\n", acusado);
        printf("Você precisa coletar pelo menos 2 pistas do mesmo suspeito para condená-lo.\n");
    }
    printf("==================================================\n");
}


// ===============================
// MAIN (PONTO DE ENTRADA)
// ===============================
int main() {
    Pista* pistasColetadas = NULL;
    TabelaHash tabelaSuspeitos;
    inicializarHash(&tabelaSuspeitos);

    // ===== 1. POPULAR TABELA HASH (Regras Estáticas do Caso) =====
    // Vamos definir 3 suspeitos e associar 2 pistas para cada um deles.
    inserirNaHash(&tabelaSuspeitos, "Pegadas na entrada", "Mordomo James");
    inserirNaHash(&tabelaSuspeitos, "Livro aberto na mesa", "Mordomo James");
    
    inserirNaHash(&tabelaSuspeitos, "Vaso quebrado", "Lady Agatha");
    inserirNaHash(&tabelaSuspeitos, "Janela aberta", "Lady Agatha");
    
    inserirNaHash(&tabelaSuspeitos, "Facas fora do lugar", "Cozinheiro Pierre");
    inserirNaHash(&tabelaSuspeitos, "Mancha de vinho", "Cozinheiro Pierre");

    // ===== 2. MONTAR MAPA DA MANSÃO (Árvore Binária Fixa) =====
    Sala* hall = criarSala("Hall de Entrada", "Pegadas na entrada");
    Sala* salaEstar = criarSala("Sala de Estar", "Vaso quebrado");
    Sala* cozinha = criarSala("Cozinha", "Facas fora do lugar");
    Sala* biblioteca = criarSala("Biblioteca", "Livro aberto na mesa");
    Sala* sotao = criarSala("Sótão", "Janela aberta");

    // Montando as conexões do mapa manual
    hall->esq = salaEstar;
    hall->dir = cozinha;

    salaEstar->esq = biblioteca;
    salaEstar->dir = sotao;

    // ===== 3. FLUXO PRINCIPAL DO JOGO =====
    printf("==================================================\n");
    printf("         BEM-VINDO AO DETECTIVE QUEST             \n");
    printf("                NÍVEL: MESTRE                     \n");
    printf("==================================================\n");
    
    // Fase de exploração interativa
    pistasColetadas = explorarSalas(hall, pistasColetadas);

    // Listagem das pistas coletadas (Requisito Funcional)
    printf("\n==================================================\n");
    printf("        PISTAS COLETADAS (ORDEM ALFABÉTICA)       \n");
    printf("==================================================\n");
    if (pistasColetadas == NULL) {
        printf(" Nenhuma pista foi coletada durante o trajeto.\n");
    } else {
        exibirPistas(pistasColetadas);
    }

    // Fase do Julgamento Final
    verificarSuspeitoFinal(pistasColetadas, &tabelaSuspeitos);

    // Limpeza rápida de memória (Opcional, boa prática)
    // (Em um sistema completo criaríamos funções freeSala e freeBST)
    free(hall); free(salaEstar); free(cozinha); free(biblioteca); free(sotao);

    return 0;
}
