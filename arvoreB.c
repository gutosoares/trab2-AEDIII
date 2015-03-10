/*
* Trabalho baseado no seguinte código fonte.
* O trabalho original sofreu modificações para atender
* os requisitos do trabalho 2.
*
* Algoritmos e Estruturas de Dados III
* Professor: André Pimenta
* Alunos: Rogério Dias, Pedro Victor, Pedro Augusto
* Turma: 10A
* Sistema de Referências Bibliográficas utilizando Árvore-B
* IDE utilizada: NetBeans versão 7.2.2
* Sistema Operacional: Ubuntu 12.04
*/

/**
* @file arvoreB.c
* @brief Arquivo com as funções de implementação da árvore B.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvoreB.h"



Index*ind = NULL;
FILE *Btree = NULL;
FILE *Data = NULL;

Index*indID = NULL;
FILE *BtreeID = NULL;
Index*indMAT = NULL;
FILE *BtreeMAT = NULL;

DataIndex *indDados = NULL;

void ParseString(char *string) {
    int len = strlen(string);
    int i = 0;
    for (i = 0; i < len; i++) {
        if (string[i] == '#') {
            printf("\n");
            return;
        }
        if (string[i] != '@') {
            printf("%c", string[i]);
        } else {
            printf(" ");
        }
    }
}

BinaryPage* copyPagetoBinaryPage (Page page) {
	
	
	BinaryPage* bPage = malloc(sizeof (struct BinaryPage));
	bPage->NumberOfKeys = page.NumberOfKeys;
	int i;
	for(i=0; i < MAXKEYS; i++ ){
	
		bPage->RegisterArray[i] = page.RegisterArray[i];
	}	
	
	for(i=0; i < MAXKEYS + 1; i++ ){
		
		bPage->RRNsArray[i] = page.RRNsArray[i];
	}
	return bPage;
}

Page* copyBinaryPagetoPage (BinaryPage bPage) {
	
	Page* page = malloc(sizeof(struct Page));
	page->NumberOfKeys = bPage.NumberOfKeys;
	int i;
	for(i=0; i < MAXKEYS; i++ ){
	
		page->RegisterArray[i] = bPage.RegisterArray[i];
	}	
	
	for(i=0; i < MAXKEYS + 1; i++ ){
		
		page->RRNsArray[i] = bPage.RRNsArray[i];
	}
	return page;
}


/**
* @brief Inicializa a nova página
*/
void Initialize(Page*page) {

    page->NumberOfKeys = 0;
    page->RRN = ind->CurrentRRN;
    ind->CurrentRRN++;

    int i = 0;
    for (i = 0; i < MAXKEYS; i++) {
        strcpy(page->RegisterArray[i].Chave, CHAVE_VAZIA);
        page->RegisterArray[i].FileRRN = -1;
    }
    for (i = 0; i < MAXKEYS + 1; i++) {
        page->RRNsArray[i] = NIL;
    }
}

/**
* @brief Escreve a página em disco
* @param page Nova página
* @param RRN da nova página
*/

void WritePage(Page* page, int RRN) {
	
	BinaryPage *bPage;
	bPage = copyPagetoBinaryPage (*page);
		
    fseek(Btree, sizeof (struct Index) +RRN * sizeof (struct BinaryPage), SEEK_SET);
    fwrite(bPage, sizeof (struct BinaryPage), 1, Btree);

	free(bPage);
    free(page);
}

/**
* @brief Retorna um ponteiro para uma página específica
* @param RRN RRN da página
*/
Page* getPage(int RRN) {
    rewind(Btree);
    fseek(Btree, sizeof (struct Index) +RRN * sizeof (struct BinaryPage), SEEK_SET);
    BinaryPage bPage;
    fread(&bPage, sizeof (struct BinaryPage), 1, Btree);
    Page* page;
    page = copyBinaryPagetoPage(bPage);
    page->RRN = RRN;

    return page;
}

/**
* @brief Cria uma referência para o index da página
*/
void WriteIndex() {
    fseek(Btree, 0 * sizeof (struct Index), SEEK_SET);
    fwrite(&(*ind), sizeof (struct Index), 1, Btree);
}


/**
* @brief Abre uma Árvore-B no disco
*/

int Driver(char* filename) {

    
	int tam = strlen(filename);
    char flID[30];
    char flMAT[30];
    
    strncpy(flID, filename, tam - 4);
    strcat(flID, "_ID.dat\0");
    strncpy(flMAT, filename, tam - 4);
    strcat(flMAT, "_MAT.dat\0");
    
    Data = fopen(filename, "rb+");
    BtreeID = fopen(flID, "rb+");
    BtreeMAT = fopen(flMAT, "rb+");
    
    if (BtreeMAT == NULL && BtreeID == NULL && Data == NULL) {
        BtreeID = fopen(flID, "wb+");
        BtreeMAT = fopen(flMAT, "wb+");
        Data = fopen(filename, "wb+");

		indDados = malloc(sizeof (struct DataIndex));
		indDados->TotalRRN = 0;
		indDados->AtivosRRN = 0;
		
		indID = malloc(sizeof (struct Index));
        indID->CurrentRRN = 0;
        indID->TreeRRN = 0;
        
        indMAT = malloc(sizeof (struct Index));
        indMAT->CurrentRRN = 0;
        indMAT->TreeRRN = 0;	
	
	
		/* Inicializa arvore MAT */
        Btree = BtreeMAT;
		ind = indMAT;
        /*Grava o cabeçalho.*/
        fseek(Btree, 0 * sizeof (struct Index), SEEK_SET);
        fwrite(&(*ind), sizeof (struct Index), 1, Btree);

        /*Allocate the first page*/
        Page *Root = malloc(sizeof (struct Page));
        Initialize(Root);
        WritePage(Root, Root->RRN);
		
		
		/* Inicializa arvore ID */
		Btree = BtreeID;
		ind = indID;
        /*Grava o cabeçalho.*/
        fseek(Btree, 0 * sizeof (struct Index), SEEK_SET);
        fwrite(&(*ind), sizeof (struct Index), 1, Btree);

        /*Allocate the first page*/
        Root = malloc(sizeof (struct Page));
        Initialize(Root);
        WritePage(Root, Root->RRN);
        
        
    } else {

		Btree = BtreeMAT;
		indMAT = malloc(sizeof (struct Index));
        fseek(Btree, 0 * sizeof (struct Index), SEEK_SET);
        fread(&(*indMAT), sizeof (struct Index), 1, Btree);
        
        indDados = malloc(sizeof (struct DataIndex));
		fseek(Data, 0, SEEK_SET);
		fread(&(*indDados), sizeof (struct DataIndex), 1, Data);
		       
		Btree = BtreeID;
		indID = malloc(sizeof (struct Index));
        fseek(Btree, 0 * sizeof (struct Index), SEEK_SET);
        fread(&(*indID), sizeof (struct Index), 1, Btree);
        
        ind = indID;
        
    }
}

/**
* @brief Busca Binária
* @param page Página
* @param *Key Chave a ser buscada
* @return Chave do meio
*/
int BinarySearch(Page* page, char *Key, int*found) {
    int first = 0, last = page->NumberOfKeys - 1, middle = 0;
    while (first <= last) {
        middle = (first + last) / 2;
        if (strcmp(Key, (char*) page->RegisterArray[middle].Chave) < 0) {
            last = middle - 1;
        } else if (strcmp(Key, (char*) page->RegisterArray[middle].Chave) > 0) {
            first = middle + 1;
        } else {
            *found = TRUE;
            return middle;
        }
    }
    return first;
}

/**
* @brief Busca de Chaves
* @param RRN RRN da página
* @param *Key Chave a ser buscada
* @param *FOUND_RRN RNN da página encontrado
* @param *FOUND_POS Posição da Chave
* @return NULL se o RRN for nulo ou a chave buscada junto com a posição
*/
int SearchKey(int RRN, char *Key, int *FOUND_RRN, int *FOUND_POS) {
    Page* page = getPage(RRN);
    if (RRN == NIL) {
        free(page);
        /*Not Found*/
        return NIL;
    } else {
        /*Position and return*/
        int Position = 0, found = -1;

        /*Search on page*/
        Position = BinarySearch(&(*page), Key, &found);

        if (found == TRUE) {
            *FOUND_RRN = (int) RRN;
            *FOUND_POS = (int) Position;
            free(page);
            return TRUE;
        } else {
            free(page);
            return (SearchKey(page->RRNsArray[Position], Key, FOUND_RRN, FOUND_POS));
        }
    }
}

/**
* @brief Inserir chave na árvore
* @param page Página
* @param *Key Chave a ser inserida
*/
void InsertKey(Page* page, char *Key, int R_SON, int DataRRN) {
    int Position = 0;
    int found = -1, i = page->NumberOfKeys;
    Position = BinarySearch(page, Key, &found);

    while (i > Position && strcmp(Key, page->RegisterArray[i - 1].Chave) < 0) {
        page->RegisterArray[i] = page->RegisterArray[i - 1];
        page->RRNsArray[i + 1] = page->RRNsArray[i];
        i--;
    }
    strcpy(page->RegisterArray[Position].Chave, Key);
    page->RegisterArray[Position].FileRRN = DataRRN;
    page->RRNsArray[Position + 1] = R_SON;
    (page->NumberOfKeys)++;
}

/**
* @brief Função de split
* @param *key Chave
* @param mid Diz aonde o split vai ocorrer
* @param *promo_key Chave promovida
*/
void Split(char *key, int r_child, Page *p_oldpage, char *promo_key, int *promo_r_child, Page *p_newpage, int *DataRRNPromoted) {

    int i;
    Register workkeys[MAXKEYS + 1]; /* temporarily holds keys, before split */
    short workch[MAXKEYS + 2]; /* temporarily holds children, before split */

    for (i = 0; i < MAXKEYS; i++) { /* move keys and children from */
        workkeys[i] = p_oldpage->RegisterArray[i];
        workch[i] = p_oldpage->RRNsArray[i];
    }
    workch[i] = p_oldpage->RRNsArray[i];
    for (i = MAXKEYS; (strcmp(key, workkeys[i - 1].Chave) < 0) && i > 0; i--) { /* insert new key */
        workkeys[i] = workkeys[i - 1];
        workch[i + 1] = workch[i];
    }

    strcpy(workkeys[i].Chave, key);
    workch[i + 1] = r_child;

    Initialize(p_newpage); /* and promote rrn of new page */
    *promo_r_child = p_newpage->RRN; /* create new page for split, */


    for (i = 0; i < Order; i++) { /* move first half of keys and */
        p_oldpage->RegisterArray[i] = workkeys[i];
        /* children to old page, second */
        p_oldpage->RRNsArray[i] = workch[i]; /* half to new page */

        p_newpage->RegisterArray[i] = workkeys[i + 1 + Order];

        p_newpage->RRNsArray[i] = workch[i + 1 + Order];

        strcpy(p_oldpage->RegisterArray[i + Order].Chave, CHAVE_VAZIA);
        p_oldpage->RegisterArray[i + Order].FileRRN = -1;

        /* mark second half of old */
        p_oldpage->RRNsArray[i + 1 + Order] = NIL; /* page as empty */
    }

    p_oldpage->RRNsArray[Order] = workch[Order];
    p_newpage->RRNsArray[Order] = workch[i + 1 + Order];
    p_newpage->NumberOfKeys = MAXKEYS - Order;
    p_oldpage->NumberOfKeys = Order;

    /* promote middle key */
    strcpy(promo_key, workkeys[Order].Chave);
    *DataRRNPromoted = workkeys[Order].FileRRN;
}

/**
* @brief Criar nova raiz
* @param *Key Chave
* @param right Filho à direita
* @param left Filho à esquerda
* @return Raiz criada
*/
Page* createRoot(char *Key, int right, int left) {
    Page*root = malloc(sizeof (struct Page));
    Initialize(root);
    strcpy(root->RegisterArray[0].Chave, Key);
    root->RRNsArray[0] = left;
    root->RRNsArray[1] = right;
    root->NumberOfKeys = 1;
    ind->TreeRRN = root->RRN;
    return root;
}

/**
* @brief Procura uma página para inserir chave
* @param CurrentRRN RRN atual
* @param *Key Chave a ser inserida
* @param DataRRN RRN do registro relativo a chave inserida na árvore presete no arquivo de dados
* @param *PROMO_R_CHILD Filho promovido
* @param *PROMO_KEY Chave promovida
*/
int Insert(int CurrentRRN, char *Key, int DataRRN, int *PROMO_R_CHILD, char *PROMO_KEY, int *DataRRNPromoted) {

    char p_b_key[TAM_CHAVE + 1];
    int p_b_rrn;

    if (CurrentRRN == NIL) { /*Chegou na folha.*/
        strcpy(&(*PROMO_KEY), Key);
        *PROMO_R_CHILD = NIL;
        DataRRN = *DataRRNPromoted;
        return PROMOTION;
    } else {
        Page *page = getPage(CurrentRRN);

        int found = -1;
        int Position = BinarySearch(page, Key, &found);

        if (found == TRUE) {
            return ERROR;
        }
        int RETURN_VALUE = Insert(page->RRNsArray[Position], Key, DataRRN, &p_b_rrn, p_b_key, DataRRNPromoted);

        if (RETURN_VALUE == ERROR || RETURN_VALUE == NO_PROMOTION) {
            free(page);
            return RETURN_VALUE;
        } else if (page->NumberOfKeys < MAXKEYS) {/*Há espaço!!*/
            InsertKey(page, p_b_key, p_b_rrn, DataRRN);
            WritePage(page, CurrentRRN);
            return NO_PROMOTION;
        } else {
            Page *newpage = malloc(sizeof (struct Page));
            Split(p_b_key, p_b_rrn, page, PROMO_KEY, PROMO_R_CHILD, newpage, DataRRNPromoted);
            WritePage(page, page->RRN);
            WritePage(newpage, newpage->RRN);
            return PROMOTION;
        }
    }
}

/**
* @brief Dispara a primeira chamada recursiva para inserir uma chave na árvore e trata as promoções na raiz
* @param *Key Chave a ser inserida
* @param DataRRN RRN do registro relativo a chave inserida na árvore presente no arquivo de dados
* @return ERROR se a chave ja existir na arvore
*/
int DriverRoot(char *Key, int DataRRN) {
    int PROMO_R_CHILD;
    int DataRRNPromoted;
    char PROMO_KEY[TAM_CHAVE + 1];

    int Result = -1;
    Page *Root = NULL;
    Result = Insert(ind->TreeRRN, Key, DataRRN, &PROMO_R_CHILD, PROMO_KEY, &DataRRNPromoted);
    if (Result == PROMOTION) {
        Root = createRoot(PROMO_KEY, PROMO_R_CHILD, ind->TreeRRN);
        WritePage(Root, Root->RRN);
    }else if (Result == ERROR) {
		return ERROR;
	}
}

/**
* @brief Organiza as chaves e os ponteiros de uma página dada
* @param *page Página dada
* @param Position Posição
*/
void Organize(Page *page, int Position) {
    if (Position == (MAXKEYS - 1)) {/*Last position just remove.*/
        strcpy(page->RegisterArray[Position].Chave, CHAVE_VAZIA);
        page->RegisterArray[Position].FileRRN = -1;
        page->NumberOfKeys--;
    } else {
        int i;
        for (i = Position; i < page->NumberOfKeys - 1; i++) {
            page->RegisterArray[i] = page->RegisterArray[i + 1];
        }
        strcpy(page->RegisterArray[i].Chave, CHAVE_VAZIA);
        page->NumberOfKeys--;

        for (i = Position; i < page->NumberOfKeys + 1; i++) {
            page->RRNsArray[i] = page->RRNsArray[i + 1];
        }
        page->RRNsArray[i] = -1;
    }
}

/**
* @brief Deleta página, apaga a referência
* @param *ProblemRRN RRN da página problemática
*/
void DeletePage(int *ProblemRRN) {
    Page *PageForDelete = getPage(*ProblemRRN);
    PageForDelete->NumberOfKeys = 0;
    int i;
    for (i = 0; i < MAXKEYS; i++) {
        PageForDelete->RegisterArray[i].FileRRN = -1;
    }
    PageForDelete->RRN = -1;
    Organize(PageForDelete, 0);
    WritePage(PageForDelete, *ProblemRRN);
}

/**
* @brief Dispara a primeira recursão
* @param *Key Chave
*/
int DriverRemove(char *Key) {

    /*Primeira chamada da raíz*/
    int Result = ERROR;
    int Trocando = FALSE;
    char ChaveTroca[TAM_CHAVE + 1];
    int ProblemRRN;

    Result = Remove(ind->TreeRRN, Key, &Result, ChaveTroca, NULL, &Trocando, &ProblemRRN);

    if (Result == NOT_FOUND) {
        //printf("Chave \"%s\" não foi encontrada para remoção.\n", Key);
        return NOT_FOUND;
    } else {
        //printf("Chave removida.");
        return TRUE;
    }
}

/**
* @brief Teste se a página é folha ou não
* @param *page Página que será testada
* @return Retorna se a página é folha ou não
*/
int isPageLeaf(Page *page) {
    int i = 0, test = LEAF;

    while (i < page->RRNsArray[page->NumberOfKeys]) {
        if (page->RRNsArray[i] != NIL) {
            test = NOT_LEAF;
        }
        i++;
    }

    if (test == LEAF) {
        return LEAF;
    } else {
        return NOT_LEAF;
    }
}

/**
* @brief Concatenação
* @param *CurrentPage Página atual
* @param *ProblemRRN RRN da página que precisa de tratamento
* @param DataRRN
*/
int Concatenate(Page *CurrentPage, int *ProblemRRN) {

    /*Pesquisa a posição no Array de RRNS, qual o RRN que deu problema.*/
    int i = 0, Position = -1;
    for (i = 0; i <= CurrentPage->NumberOfKeys + 1; i++) {
        if ((*ProblemRRN) == CurrentPage->RRNsArray[i]) {
            Position = i;
        }
    }

    /*Está na última posição, não há irmãs à direita. Só devemos olhar para a esquerda.*/
    if ((*ProblemRRN) == CurrentPage->RRNsArray[CurrentPage->NumberOfKeys]) {

        Page *LeftSister = getPage(CurrentPage->RRNsArray[Position - 1]);
        Page *ProblemPage = getPage(*ProblemRRN);

        /*Array temporário para segurar as chaves da irmã à esquerda*/
        Register workkeys[LeftSister->NumberOfKeys];

        /*Copia as chaves da página problemática para o vetor temporário*/
        int i = 0;
        for (i = 0; i < ProblemPage->NumberOfKeys; i++) {
            strcpy(workkeys[i].Chave, ProblemPage->RegisterArray[i].Chave);
            workkeys[i].FileRRN = ProblemPage->RegisterArray[i].FileRRN;
        }


        /*Apaga o RRN para a página que foi excluída na página pai.*/
        CurrentPage->RRNsArray[Position] = -1;

        /*Desce o pai*/
        char Dad[TAM_CHAVE + 1];
        strcpy(&(*Dad), CurrentPage->RegisterArray[Position - 1].Chave);
        int DataRRN = CurrentPage->RegisterArray[Position - 1].FileRRN;


        /*Insere o pai na página irmã .*/

        InsertKey(LeftSister, Dad, -1, DataRRN);

        /*Insere as chaves da página problemática na sua irmã à esquerda.*/
        for (i = 0; i < ProblemPage->NumberOfKeys; i++) {
            InsertKey(LeftSister, workkeys[i].Chave, -1, workkeys[i].FileRRN);
        }

        /*Deleta a página problemática*/
        DeletePage(ProblemRRN);

        Organize(CurrentPage, Position - 1);

        if (CurrentPage->NumberOfKeys < Order) {
            WritePage(LeftSister, LeftSister->RRN);
            return UNDERFLOW;
        }
        WritePage(LeftSister, LeftSister->RRN);
        return TRUE;
    } else {

        /*Não há irmãs à esquerda, olha só para a direita.*/
        if ((*ProblemRRN) == CurrentPage->RRNsArray[0]) {
            Page *RightSister = getPage(CurrentPage->RRNsArray[Position + 1]);

            Page *ProblemPage = getPage(*ProblemRRN);

            /*Array temporário para segurar as chaves da irmã à direita*/
            Register workkeys[RightSister->NumberOfKeys];


            /*Copia as chaves da página problemática para o vetor temporário*/
            int i = 0;

            for (i = 0; i < ProblemPage->NumberOfKeys; i++) {
                strcpy(workkeys[i].Chave, ProblemPage->RegisterArray[i].Chave);
                workkeys[i].FileRRN = ProblemPage->RegisterArray[i].FileRRN;
            }


            /*Apaga o RRN para a página que foi excluída na página pai.*/
            CurrentPage->RRNsArray[Position] = -1;



            /*Desce o pai*/
            char Dad[TAM_CHAVE + 1];
            strcpy(&(*Dad), CurrentPage->RegisterArray[Position].Chave);
            int DataRRN = CurrentPage->RegisterArray[Position].FileRRN;


            /*Insere as chaves da página problemática na sua irmã à direita.*/
            for (i = 0; i < ProblemPage->NumberOfKeys; i++) {
                InsertKey(RightSister, workkeys[i].Chave, -1, workkeys[i].FileRRN);
            }


            /*Deleta a página problemática*/
            DeletePage(ProblemRRN);

            /*Insere o pai na página irmã .*/
            InsertKey(RightSister, Dad, -1, DataRRN);


            CurrentPage->RRNsArray[Position] = RightSister->RRN;
            Organize(CurrentPage, Position);


            if (CurrentPage->NumberOfKeys < Order) {
                WritePage(RightSister, RightSister->RRN);
                return UNDERFLOW;
            }
            WritePage(RightSister, RightSister->RRN);
            return TRUE;
        } else {

            Page *RightSister = getPage(CurrentPage->RRNsArray[Position + 1]);


            Page *ProblemPage = getPage(*ProblemRRN);

            /*Array temporário para segurar as chaves da irmã à direita*/
            Register workkeys[RightSister->NumberOfKeys];

            /*Copia as chaves da página problemática para o vetor temporário*/
            int i = 0;
            for (i = 0; i < ProblemPage->NumberOfKeys; i++) {
                strcpy(workkeys[i].Chave, ProblemPage->RegisterArray[i].Chave);
                workkeys[i].FileRRN = ProblemPage->RegisterArray[i].FileRRN;
            }


            /*Deleta a página problemática*/
            DeletePage(ProblemRRN);

            /*Apaga o RRN para a página que foi excluída na página pai.*/
            CurrentPage->RRNsArray[Position] = -1;

            /*Desce o pai*/
            char Dad[TAM_CHAVE + 1];
            strcpy(&(*Dad), CurrentPage->RegisterArray[Position].Chave);
            int DataRRN = CurrentPage->RegisterArray[Position].FileRRN;


            /*Insere o pai na página irmã .*/
            InsertKey(RightSister, Dad, -1, DataRRN);

            /*Insere as chaves da página problemática na sua irmã à direita.*/
            for (i = 0; i < ProblemPage->NumberOfKeys; i++) {
                InsertKey(RightSister, workkeys[i].Chave, -1, workkeys[i].FileRRN);
            }

            CurrentPage->RRNsArray[Position] = RightSister->RRN;
            Organize(CurrentPage, Position);

            if (CurrentPage->NumberOfKeys < Order) {
                WritePage(RightSister, RightSister->RRN);
                return UNDERFLOW;
            }
            WritePage(RightSister, RightSister->RRN);
        }
    }
}

/**
* @brief Função de redistribuição para o caso de remoção #3
* @param *CurrentPage Página atual
* @param *ProblemRRN RRN da página que precisa ser tratada
*/
int Redistribution(Page *CurrentPage, int *ProblemRRN) {

    /*Pesquisa a posição no Array de RRNS, qual o RRN que deu problema.*/
    int i = 0, Position = -1;
    for (i = 0; i <= CurrentPage->NumberOfKeys + 1; i++) {
        if ((*ProblemRRN) == CurrentPage->RRNsArray[i]) {
            Position = i;
        }
    }

    /*Não há irmãs à esquerda, olha só para a direita.*/
    if ((*ProblemRRN) == CurrentPage->RRNsArray[0]) {
        Page *RightSister = getPage(CurrentPage->RRNsArray[Position + 1]);


        /*Olha-se a irmã da direita a fim de se verificar a possibilidade de Redistribuição de chaves.*/
        if (RightSister->NumberOfKeys > Order) {

            Page*ProblemPage = getPage(CurrentPage->RRNsArray[Position]);

            /*Desce o pai*/
            char Dad[TAM_CHAVE + 1];
            strcpy(Dad, CurrentPage->RegisterArray[Position].Chave);
            int DataRRN = CurrentPage->RegisterArray[Position].FileRRN;

            /*Insere o pai na página problemática.*/
            InsertKey(ProblemPage, Dad, -1, DataRRN);

            /*salva-se a página problemática*/
            WritePage(ProblemPage, ProblemPage->RRN);

            CurrentPage->RegisterArray[Position] = RightSister->RegisterArray[0];

            Organize(RightSister, 0);
            WritePage(RightSister, RightSister->RRN);
            return TRUE;
        } else {
            /*Redistribuição não foi possível. Concatenar.*/
            free(RightSister);
            return CONCATENATE;
        }
    }
    if ((*ProblemRRN) == CurrentPage->RRNsArray[CurrentPage->NumberOfKeys]) { /*Está na última posição, não há irmãs à direita. Só devemos olhar para a esquerda.*/

        Page *LeftSister = getPage(CurrentPage->RRNsArray[Position - 1]);

        if (LeftSister->NumberOfKeys > Order) {
            Page*ProblemPage = getPage(CurrentPage->RRNsArray[Position]);

            /*Desce o pai*/
            char Dad[TAM_CHAVE + 1];
            strcpy(Dad, CurrentPage->RegisterArray[Position].Chave);
            int DataRRN = CurrentPage->RegisterArray[Position].FileRRN;

            /*Insere o pai na página problemática.*/
            InsertKey(ProblemPage, Dad, -1, DataRRN);

            /*salva-se a página problemática*/
            WritePage(ProblemPage, ProblemPage->RRN);


            CurrentPage->RegisterArray[Position] = LeftSister->RegisterArray[LeftSister->NumberOfKeys - 1];


            Organize(LeftSister, LeftSister->NumberOfKeys - 1);
            WritePage(LeftSister, LeftSister->RRN);
            return TRUE;
        } else {
            free(LeftSister);
            return CONCATENATE;
        }
    } else {

        if (*ProblemRRN != CurrentPage->RRNsArray[0] && *ProblemRRN != CurrentPage->RRNsArray[CurrentPage->NumberOfKeys + 1]) {

            /*Olha-se a irmã da direita a fim de se verificar a possibilidade de Redistribuição de chaves.*/
            Page *RightSister = getPage(CurrentPage->RRNsArray[Position + 1]);

            if (RightSister->NumberOfKeys > Order) {

                Page*ProblemPage = getPage(CurrentPage->RRNsArray[Position]);

                /*Desce o pai*/
                char Dad[TAM_CHAVE + 1];
                strcpy(Dad, CurrentPage->RegisterArray[Position].Chave);
                int DataRRN = CurrentPage->RegisterArray[Position].FileRRN;

                /*Insere o pai na página problemática.*/
                InsertKey(ProblemPage, Dad, -1, DataRRN);

                /*salva-se a página problemática*/
                WritePage(ProblemPage, ProblemPage->RRN);


                CurrentPage->RegisterArray[Position] = RightSister->RegisterArray[0];

                Organize(RightSister, 0);
                WritePage(RightSister, RightSister->RRN);
                return TRUE;
            } else {

                Page *LeftSister = getPage(CurrentPage->RRNsArray[Position - 1]);

                if (LeftSister->NumberOfKeys > Order) {

                    Page*ProblemPage = getPage(CurrentPage->RRNsArray[Position]);

                    /*Desce o pai*/
                    char Dad[TAM_CHAVE + 1];
                    strcpy(Dad, CurrentPage->RegisterArray[Position - 1].Chave);
                    int DataRRN = CurrentPage->RegisterArray[Position - 1].FileRRN;

                    /*Insere o pai na página problemática.*/
                    InsertKey(ProblemPage, Dad, -1, DataRRN);

                    /*salva-se a página problemática*/
                    WritePage(ProblemPage, ProblemPage->RRN);

                    char PromotedSon[TAM_CHAVE + 1];
                    strcpy(PromotedSon, LeftSister->RegisterArray[LeftSister->NumberOfKeys - 1].Chave);
                    int DataPromotedSon = LeftSister->RegisterArray[LeftSister->NumberOfKeys - 1].FileRRN;

                    strcpy(CurrentPage->RegisterArray[Position - 1].Chave, PromotedSon);
                    CurrentPage->RegisterArray[Position - 1].FileRRN = DataPromotedSon;

                    Organize(LeftSister, LeftSister->NumberOfKeys - 1);

                    WritePage(LeftSister, LeftSister->RRN);
                    return TRUE;
                } else {
                    free(LeftSister);
                    return CONCATENATE;
                }
            }
        }
    }
}

/**
* @brief Função de remoção
* @param CurrentRRN RRN atual
* @param *Key Chave a ser removida
* @param *Troca Armazena a posição de troca
* @param *ChaveTroca Armazena sucessor
* @param *Dad Página Pai
* @param *Trocando Flag indicadora de procura/troca
* @param *ProblemRRN Ponteiro para a página que precisa de tratamento
*/
int Remove(int CurrentRRN, char *Key, int *Troca, char *ChaveTroca, Page *Dad, int *Trocando, int *ProblemRRN) {

    Page*CurrentPage = getPage(CurrentRRN);

    if (CurrentRRN == NIL) {
        return ERROR;
    }

    if (CurrentRRN != NIL) {
        strcpy(&(*ChaveTroca), &(*CurrentPage->RegisterArray[0].Chave));
        if (isPageLeaf(CurrentPage) == LEAF && *Trocando == TRUE) {
            strcpy(Dad->RegisterArray[*Troca].Chave, ChaveTroca);
            Organize(CurrentPage, 0);
            if (CurrentPage->NumberOfKeys < Order) {
                *Trocando = FALSE;
                *ProblemRRN = CurrentRRN;
                return UNDERFLOW;
            }
            *Trocando = FALSE;
            WritePage(CurrentPage, CurrentPage->RRN);
            return NOT_UNDERFLOW;
        }
    }


    int found = -1;
    int Position = BinarySearch(CurrentPage, Key, &found);

    if (found == TRUE) {

        /*É folha, simplesmente removemos e verificamos se há Underflow na página em questão.*/
        if (isPageLeaf(CurrentPage) == LEAF) {
            Organize(CurrentPage, Position); /*Remove chave, primeiro caso.*/

            if (CurrentPage->NumberOfKeys < Order) {
                WritePage(CurrentPage, CurrentPage->RRN); /*Escreve página alterada*/
                *ProblemRRN = CurrentRRN;
                return UNDERFLOW; /*Retorna que houve Underflow para o nível de cima.*/
            }

            WritePage(CurrentPage, CurrentPage->RRN); /*Escreve página alterada*/
            return TRUE; /*Chave removida.*/
        }

        /*Se achar em uma página não folha, procuramos seu sucessor direto.*/
        if (isPageLeaf(CurrentPage) == NOT_LEAF) {

            *Trocando = TRUE;

            Dad = &(*CurrentPage); /*Armazena a página pai*/

            *Troca = Position; /*Armazena a posição de troca.*/

            /*Chama a recursão procurando o sucessor direto da chave.*/
            int Return = Remove(CurrentPage->RRNsArray[Position + 1], Key, Troca, ChaveTroca, Dad, Trocando, ProblemRRN);

            *Trocando = FALSE;

            if (Return == UNDERFLOW) {
                /*Tratamento de Underflow. Verificamos se é possível redistribuição*/

                if (Redistribution(CurrentPage, ProblemRRN) == CONCATENATE) {
                    /*Concatenar*/
                    Concatenate(CurrentPage, ProblemRRN);
                } else {
                    WritePage(CurrentPage, CurrentRRN);
                    return TRUE; /*Chave removida.*/
                }
            }
            WritePage(CurrentPage, CurrentRRN);
        }
    } else {
        int Return = Remove(CurrentPage->RRNsArray[Position], Key, Troca, ChaveTroca, Dad, Trocando, ProblemRRN);

        if (Return == UNDERFLOW) {
            /*Tratamento de Underflow. Verificamos se é possível redistribuição*/
            if (Redistribution(CurrentPage, ProblemRRN) == CONCATENATE) {
                /*Concatenar*/
                int ConcatenateResult = Concatenate(CurrentPage, ProblemRRN);
                if (ConcatenateResult == UNDERFLOW) {
                    WritePage(CurrentPage, CurrentRRN);
                    return UNDERFLOW;
                }
            }
        }
        WritePage(CurrentPage, CurrentRRN);
    }
    return NOT_FOUND;
}

/**
* @brief Função para fazer a impressão da Árvore
*/
void printtree(int t) {
    int i;

    if (t != NIL) {
        Page *page = getPage(t);

        printf("RRN %i|", page->RRN);
        printf("Number of Keys %d|", page->NumberOfKeys);

        for (i = 0; i < MAXKEYS; i++) {
            printf(" %i |", page->RRNsArray[i]);
            printf(" %s,%i |", page->RegisterArray[i].Chave, page->RegisterArray[i].FileRRN);
            if (i == MAXKEYS - 1) {
                printf(" %i |", page->RRNsArray[i + 1]);
            }
        }
        puts("");
        for (i = 0; i <= MAXKEYS; i++)
            printtree(page->RRNsArray[i]);
    }
}

/**
* @brief Função para fazer a impressão dos Alunos
*/
void ImprimeAluno(Aluno aluno) {
	
	printf("<%s> <%s> <%s> <%s> <%.2lf>\n", aluno.Nome, aluno.Identidade, aluno.CPF, aluno.Matricula, aluno.RSG);
}

/**
* @brief Função para fazer a impressão das Páginas
*/
void Imprime(int RRN, int x){
	
	Page *P = getPage(RRN);
	int i;
	for(i = 0; i < 3*x; i++){
		printf(" ");
	}
	for(i = 0; i < P->NumberOfKeys; i++){
		printf("%s ", P->RegisterArray[i].Chave);
	}
	printf("\n");
	for(i = 0; i <= P->NumberOfKeys; i++){
		Imprime(P->RRNsArray[i], x+1);
	}
		
}

