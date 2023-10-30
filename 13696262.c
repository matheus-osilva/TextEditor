#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define capa 1000 //capacidade das memórias
#define capc 10 //capacidade inicial de caracacteres
#define capl 5 //capacidade inicial de linhas
#define CAP0 1000 //capacidade da pilha
//Pilha ↓
typedef struct pilhaTCD {
    char* string;
    int topo;
    int cap;
} *pilha;

int PilhaVazia(pilha p) {
    return(p->string == NULL);
}

pilha CriaPilha() {
    pilha p = malloc(sizeof(struct pilhaTCD));
    p->topo = 0; p->cap = CAP0;
    p->string = malloc(p->cap*sizeof(char));
    return p;
}

void DestroiPilha(pilha p) {
    free(p->string); free(p);
    return;
}

//Varre a linha e copia o que está entre o cursor e o marcador
pilha EmpilhaC(char** l, pilha p, int m, int* pl, int* pc) {
    int i, menor, maior;
    if (*pc < m)
    {
        menor = *pc;
        maior = m;
    }
    else {menor = m; maior = *pc;}
    for(i = menor; i <= maior; i++) p->string[p->topo + i - menor] = l[*pl][i];
    p->string[p->topo + i - menor] = '\n';
    p->string[p->topo + i - menor + 1] = '\0';
    p->topo = p->topo + maior - menor + 2;
    return p;
}

//Varre a linha e copia e remove o que está entre o cursor e o marcador
pilha EmpilhaX(char** l, pilha p, int m, int* pl, int* pc) {
    int i, menor, maior;
    if (*pc < m)
    {
        menor = *pc;
        maior = m;
    }
    else {menor = m; maior = *pc;}
    for(i = menor; i <= maior; i++) p->string[p->topo + i - menor] = l[*pl][i];
    p->string[p->topo + i - menor] = '\n';
    p->string[p->topo + i - menor + 1] = '\0';
    for(i = maior; l[*pl][i] != '\0'; i++) l[*pl][menor + i - maior] = l[*pl][i + 1];
    l[*pl][menor + i - maior] = '\0';
    p->topo = p->topo + maior - menor + 2;
    return p;
}

//Desempilha a string do topo da pilha e cola depois do cursor deslocando o texto para a direita
char** Desempilha(char** l, pilha p, int* pl, int* pc) {
    if (p->string[0] == '\0') return l;
    //Considera o caso do cursor estar no último caractere da linha
    if (l[*pl][*pc] == '\0') {l[*pl][*pc] = ' '; l[*pl][*pc + 1] = '\0';}
    int i, j, ult;
    char memoria[capa];
    for (i = 0; l[*pl][i + *pc] != '\0'; i++) memoria[i] = l[*pl][i + *pc];
    memoria[i] = '\0';
    //O indíce 'i' é p->topo - 2 para desconsiderar os caracteres '\0' e '\n' que separam os elementos da pilha
    for (i = p->topo - 2; p->string[i] != '\n' && i >= 0; i--)
    //Ult é a variável que carrega o índice do primeiro caractere da string do topo da pilha
    ult = i;
    for (i = 0; p->string[ult + i] != '\n' && p->string[ult + i] != '\0'; i++) l[*pl][i + *pc] = p->string[ult + i];
    for (j = 0; memoria[j] != '\0'; j++) l[*pl][j + i + *pc] = memoria[j];
    l[*pl][j + i + *pc] = '\0';
    p->string[ult] = '\0';
    p->topo = ult;
    return l;
}

void ImprimePilha(pilha p) {
    int i;
    if (p->topo == 0)
    {
        printf("Pilha vazia\n");
        return;
    }
    for (i = 0; i < strlen(p->string); i++) printf("%c", p->string[i]);
    printf("\n");
    return;
}
//Pilha ↑

void ImprimeMemCur(int m, int* pl, int* pc){
    if (m < *pc)
        {
            printf("\n%*s", m, "");
            printf("M");
            printf("%*s", *pc - m - 1, "");
            printf("^");
            return;
        }
    else if (m == *pc)
        {
            printf("\n%*s", m, "");
            printf("^");
            printf("\n%*s", m, "");
            printf("M");
            return;
        }
    else if (m > *pc);
        {
            printf("\n%*s", *pc, "");
            printf("^");
            printf("%*s", m - *pc - 1, "");
            printf("M");
            return;
        }
    return;
}

void Imprime(char** l, int* pl, int* pc, int m){
    printf("%s", l[*pl]);
    ImprimeMemCur(m, pl, pc);
    return;
}

//Funções de movimento ↓
void Avanca(int* pc){
    *pc += 1;
    return;
}

void Recua(int* pc){
    if (*pc > 0) *pc -= 1;
    return;
}

void Inicio(int* pc){
    *pc = 0;
    return;
}

void ProxPalavra(char** l, int* pc, int* pl){
    int i, lin, col;
    i = 0;
    col = *pc;
    lin = *pl;
    for (i = col;l[lin][i] != ' ' && l[lin][i] != '\0'; i++);
    if (l[lin][i] == '\0') return;
    i += 1;
    *pc = i;
    return;
}

void InicioPalavra(char** l, int* pc, int* pl){
    int i, lin, col;
    i = 0;
    col = *pc;
    lin = *pl;
    for (i = col;l[lin][i-1] != ' ' && i > 0; i--);
    *pc = i;
    return;
}

void InicioX(char* comando, int* pc, int* pl, int* pt){
    int i, j, valor, contador;
    char memoria[strlen(comando) - 1];
    *pc = 0;
    contador = 1;
    valor = 0;
    for (i = 0; i < strlen(comando); i++) memoria[i] = comando[i + 1];
    memoria[i - 1] = '\0';
    //Laço para transformar os valores de char para int
    for (j = i - 2; j > -1; j--)
    {   
        valor += (memoria[j] - '0') * contador;
        contador *= 10;
    }
    if (valor > *pt - 1)
    {
        printf("Linha indisponivel");
        return;
    }
    *pl = valor;
    return;
}

void FimLinha(char** l, int* pc, int* pl){
    int i;
    for (i = 0; l[*pl][i] != '\0'; i++);
    *pc = i;
    return;
}

int UltimaLinha(char** l, int* pl, int* pt){
    int i, j;
    i = j = 0;
    for (i = 0; l[i][j] != EOF; i++)
    {
        for (j = 0; l[i][j] != '\0'; j++);
    }
    return i - 1;
}
//Funções de movimento ↑

char** CriaLinha(){
    int i, j;
    char **l;
    l = (char**) malloc(sizeof(char*) * capl);
    for (i = 0; i < capl; i++) l[i] = (char*) malloc(sizeof(char) * capc);
    for (i = 0; i < capl; i++) 
    {
        l[i][0] = '\0';
        l[i][capc - 1] = '\n';
    }
    return l;
}

//Realoca a estrutura principal para dobrar o tamanho de linhas
char** ExpandeTexto(char** l, int* pt, int* pl){
    int i, tam_novo, tamanho_linha;
    char** nova;
    tam_novo =  2 * (*pt);
    nova = (char**) malloc(sizeof(char*) * tam_novo);
    if (nova == NULL){
        printf("Erro na alocação de memória para expandir o texto\n");
        return NULL;
    }
    for (i = 0; i < tam_novo; i++) nova[i] = (char*) malloc(sizeof(char) * capc);
    for (i = 0; i < *pt; i++) 
    {
        free(nova[i]);
        nova[i] = l[i];
    }
    
    for (i = *pt; i < tam_novo; i++) 
    {
        nova[i][0] = '\0';
        nova[i][capc - 1] = '\n';
    }
    *pt = tam_novo;
    free(l);
    return nova;
}

//Realoca a estrutura principal para dobrar o tamanho de caracteres em cada linha
char* ExpandeLinha(char** l, int tamanho_linha, int* pl, int* pc){
    int i, tam_novo =  2 * tamanho_linha;
    char *nova_linha = (char*) malloc(tam_novo * sizeof(char));
    if (nova_linha == NULL){
        printf("Erro na alocação de memória para expandir linha\n");
        return NULL;
    }
    for (i = 0; i < tam_novo; i++) 
    {   
        if (i < tamanho_linha) 
        {
            nova_linha[i] = l[*pl][i]; 
        }
        else nova_linha[i] = '\0';
    }
    nova_linha[i - 1] = '\n';
    free(l[*pl]);
    return nova_linha;
}

char** AbreArq(char** l, char* comando, int* pl, int* pc, int* pt, int* ptl){
    int i, tam, tamanho_linha;
    i = 0;
    FILE *arq;
    char nomedoarquivo[strlen(comando) - 1], c;
    for (i = 1; i < strlen(comando); i++) nomedoarquivo[i-1] = comando[i];
    nomedoarquivo[i-1] = '\0';
    arq = fopen(nomedoarquivo, "r");
    if (arq == NULL)
    {
        printf("Não foi possível abrir o arquivo\n");
        return l;
    }
    c = fgetc(arq);
    while (1)
    {   
        for(tamanho_linha = 0; l[*pl][tamanho_linha] != '\n'; tamanho_linha++);
        tamanho_linha += 1;
        for(*pc = 0; c != '\n' && c != EOF; *pc += 1)
        {
            if (*pc  >= tamanho_linha - 2)
            {
                l[*pl] = ExpandeLinha(l, tamanho_linha, pl, pc);
                tamanho_linha *= 2;
            }
            l[*pl][*pc] = c;
            c = fgetc(arq); 
        }
        l[*pl][*pc] = '\0';
        l[*pl][*pc + 1] = '\n';
        if (c == '\n')
        {
            if (*pl == *pt - 1)
            {
                l = ExpandeTexto(l, pt, pl);
            }
            *pl += 1;
            c = fgetc(arq);
        }
        if (c == EOF)
        {
            l[*pl][*pc] = EOF;
            for (i = *pl + 1; i < *pt; i++)l[i][0] = '\0'; 
            *pc = 0;
            *pl = 0;
            fclose(arq);           
            return l;
        }
    }
    *pc = 0;
    *pl = 0;
    fclose(arq);
    return l;
}

void EscreveArq(char** l, char* comando, int* pt){
    int i, j;
    FILE *arq;
    char nomedoarquivo[strlen(comando) - 1], c;
    for (i = 1; i < strlen(comando); i++) nomedoarquivo[i-1] = comando[i];
    nomedoarquivo[i-1] = '\0';
    arq = fopen(nomedoarquivo, "w");
    if (arq == NULL)
    {
        printf("Não foi possível abrir o arquivo\n");
        return;
    }
    for (i = 0; i < *pt; i++)
    {
        if(l[i][0] != '\0' && i > 0)fputc('\n', arq);
        for (j = 0; l[i][j] != '\0'; j++)
        {
            if (l[i][j] == '\n')i++;
            if (l[i][j] == EOF)break;
            fputc(l[i][j], arq);
        }
        fputc('\n', arq);
    }
    fputc(EOF, arq);
    fclose(arq);
    return;
}

char** Insere (char** l, char* comando, int* pl, int* pc, int* pt, int* ptl){
    int i, lin, col, k, tamanho_linha;
    i = k = 0;
    col = *pc;
    lin = *pl;
    for(tamanho_linha = 0; l[*pl][tamanho_linha] != '\n'; tamanho_linha++);
    tamanho_linha += 1;
    //Garante que o algoritmo não tente inserir além do tamanho da string que representa a linha
    if (*pc > tamanho_linha - 1) *pc = tamanho_linha - 1;
    char memoria[capa];
    char stringinserir[strlen(comando) - 1];
    if (*pc + strlen(comando) >= tamanho_linha)
        {
            l[*pl] = ExpandeLinha(l, tamanho_linha, pl, pt);
            tamanho_linha *= 2;
        }
    for (i = 0; l[*pl][*pc + i] != '\0' && l[*pl][*pc + i] != EOF; i++) memoria[i] = l[*pl][*pc + i];
    if (l[*pl][*pc + i] == EOF) memoria[i] = EOF;
    memoria[i] = '\0';
    for (i = 1; i < strlen(comando); i++) stringinserir[i-1] = comando[i];
    for (i = col; i < strlen(comando) - 1 + col; i++) l[lin][i] = stringinserir[i - col];  
    for (k = 0; memoria[k] != '\0'; k++) l[*pl][k + i] = memoria[k];
    l[*pl][k + i] = '\0';
    *pc = i;
    return l;
}

char** SeparaLinha(char** l, int* pl, int* pc, int* pt){
    int i, j, ultimalinha;
    i = j = 0;
    char memoria[capa];
    for (i = 0; l[*pl][*pc + i] != '\0'; i++) memoria[i] = l[*pl][*pc + i];
    memoria[i] = '\0';
    l[*pl][*pc] = '\0';
    i = 0;
    for (i = 0; l[i][j] != EOF; i++)
    {
        for (j = 0; l[i][j] != '\0'; j++);
    }
    ultimalinha = i - 1;
    if (ultimalinha == *pt) l = ExpandeTexto(l, pt, pl);
    for (i = ultimalinha; i > *pl; i--)
    {
        for (j = 0; l[i][j] != '\0'; j++) l[i + 1][j] = l[i][j];
        l[i + 1][j] = '\0';
    }
    for (i = 0; memoria[i] != '\0'; i++) l[*pl + 1][i] = memoria[i]; 
    l[*pl + 1][i] = '\0';
    for (i = 0; l[ultimalinha + 1][i] != '\0'; i++);
    l[ultimalinha + 1][i] = EOF;
    return l;
}

char** UneLinha(char** l, int* pl, int* pc, int* pt){
    int i, j;
    char memoria[capa];
    for (i = 0; l[*pl + 1][i] != '\0'; i++) memoria[i] = l[*pl + 1][i];
    memoria[i] = '\0';
    for (j = 0; l[*pl][j] != '\0'; j++);    
    for (i = 0; memoria[i] != '\0'; i++) l[*pl][j + i] = memoria[i];
    l[*pl][j + i] = '\0';
    for (i = *pl + 1; i - 1 < *pt; i++) l[i] = l[i + 1];
    *pt -= 1;
    return l;   
}

void Busca(char* comando, char** l, int* pl, int* pc, int* pt){
    int lin, i, j, k, m, n, o = 0;
    char s[strlen(comando) - 1];
    lin = *pl;
    for (i = 1; i < strlen(comando); i++) s[i - 1] = comando[i];
    s[i - 1] = '\0';
    m = strlen(s);
    while (lin < *pt) {
        n = strlen(l[lin]);
        for (k = m; k <= n; k++) {
            j = 1;
            while (j <= m && s[m-j] == l[lin][k-j]) j++;
            if (j > m) { *pc = k - m; *pl = lin; return;}   
        }
        lin ++;
    }
    printf("Não foi encontrado nenhuma ocorrência");
    return;
}

char** Substitui(char* comando, char** l, int* pl, int* pc, int* pt){
    int div, lin, i, j, k, m, n, o, contador;
    char s[strlen(comando) - 1];
    char r[strlen(comando) - 1];
    char memoria[capa];
    lin = *pl;
    for (i = 0; comando[i] != '/'; i++);
    div = i;
    for (i = 1; i < div; i++) s[i - 1] = comando[i];
    s[i - 1] = '\0';
    for (i = div + 1; i < strlen(comando); i++) r[i - div - 1] = comando[i];
    r[i - div - 1] = '\0';
    m = strlen(s);
    while (lin < *pt) {
        n = strlen(l[lin]);
        for (k = m; k <= n; k++) {
            j = 1;
            while (j <= m && s[m-j] == l[lin][k-j]) j++;
            if (j > m)
            {
                for (i = k; l[lin][i] != '\0'; i++) memoria[i - k] = l[lin][i];
                memoria[i - k] = '\0';
                for (i = 0; i < strlen(r); i++) l[lin][i + k - m] = r[i];
                for (o = i + k - m; memoria[o - i - k + m] != '\0'; o++) l[lin][o] = memoria[o - i - k + m];
                l[lin][o] = '\0';
                n += (strlen(r) - strlen(s));
            }  
        }
        lin ++;
    }
    return; 
}
// Apaga o último caractere caso o cursor esteja além do último caractere
char** ApagaCaractere(char** l, int* pl, int* pc){
    int lin, col, i, tam;
    lin = *pl; col = *pc;
    tam = strlen(l[lin]);
    char memoria[tam - 1];
    for (i = 0; i < col; i++) memoria[i] = l[lin][i];
    for (i = col + 1; i < tam; i++) memoria[i - 1] = l[lin][i];
    for (i = 0; i < strlen(memoria); i++) l[lin][i] = memoria[i];
    l[lin][tam-1] = '\0';
    return l;
}

void main(){
    char **l;
    int linha, coluna, tamanhoarq, tamanholin, m, i;
    m = 0; tamanhoarq = capl; tamanholin = capc;
    int *pl, *pc, *pt, *ptl;
    l = CriaLinha();
    pl = &linha; pc = &coluna; pt = &tamanhoarq; ptl = &tamanholin;
    linha = coluna = 0;
    pilha p = CriaPilha();
    char comando[capa];
    printf("Editor de texto 2000\n");
    printf("%d, %d> ", linha, coluna);
    gets(comando);
    while (1)
    {
        i = 0;
        while (comando[i] != '\0')
        {
            if (comando[i] == 'I')
            {
                l = Insere(l, comando, pl, pc, pt, ptl);
                break;
            }
            if (comando[i] == 'A')
            {
                l = AbreArq(l, comando, pl, pc, pt, ptl);
                break;
            }
            if (comando[i] == 'E')
            {
                EscreveArq(l, comando, pt);
                break;
            }
            if (comando[i] == 'F')
            {
                if (l[*pl][*pc] != '\0') Avanca(pc);
            }
            if (comando[i] == 'T')
            {
                Recua(pc);
            }
            if (comando[i] == 'O')
            {
                Inicio(pc);
            }
            if (comando[i] == 'P')
            {
                ProxPalavra(l, pc, pl);
            }
            if (comando[i] == 'Q')
            {
                InicioPalavra(l, pc, pl);
            }
            if (comando[i] == '$')
            {
                FimLinha(l, pc, pl);
            }
            if (comando[i] == ':')
            {
                if (comando[i + 1] != '\0' && comando[i + 1] == 'F')
                {
                    *pl = UltimaLinha(l, pl, pt);
                }
                else
                {
                    InicioX(comando, pc, pl, pt);
                }
            }
            if (comando[i] == 'D')
            {
                l = ApagaCaractere(l, pl, pc);
            }
            if (comando[i] == 'M')
            {
                m = *pc;
            }
            if (comando[i] == 'V')
            {
                l = Desempilha(l, p, pl, pc);
            }
            if (comando[i] == 'C')
            {
                EmpilhaC(l, p, m, pl, pc);
            }
            if (comando[i] == 'X')
            {
                EmpilhaX(l, p, m, pl, pc);
            }
            if (comando[i] == 'B')
            {
                Busca(comando, l, pl, pc, pt);
            }
            if (comando[i] == 'S')
            {
                Substitui(comando, l, pl, pc, pt);
                break;
            }
            if (comando[i] == 'N')
            {
                SeparaLinha(l, pl, pc, pt);
            }
            if (comando[i] == 'U')
            {
                UneLinha(l, pl, pc, pt);
            }
            if (comando[i] == '!')
            {
                for (i = 0; i < *pt; i++)
                {
                    free(l[i]);    
                }
                free(l);
                DestroiPilha(p);
                return;
            }
            if (comando[i] == 'J')
            {
                if (*pl == *pt - 1) l = ExpandeTexto(l, pt, pl);
                *pl += 1;
            }
            if (comando[i] == 'H')
            {
                if (*pl == 0)
                {
                    break;
                }
                *pl -= 1;
            }
            if (comando[i] == 'Z')
            {
                ImprimePilha(p);
            }
            if (comando[i] == 'Y')
            {
                l = ExpandeTexto(l, pt, pl);
            }
            i++;
        }
        Imprime(l, pl, pc, m);
        printf("\n%d, %d> ", linha, coluna);
        gets(comando);
    }
}