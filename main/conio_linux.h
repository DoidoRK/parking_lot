/*************************************************************
      - Esta biblioteca foi desenvolvida com base nas
        'VT100 ESC sequences' de um terminal UNIX.
      - Nao foram desenvolvidas todas as funcoes possiveis,
        apenas as mais utilizadas pela CONIO.H do DOS
      - Para maiores detalhes acesse:
         http://ctdp.tripod.com/os/linux/howlinuxworks/
 **************************************************************/
#ifndef _CONIO_LINUX_H_
#define	_CONIO_LINUX_H_
#include <stdio.h>
/* Definicao de teclas */
#define ESC    27
/******** Definicao de cores *********/
#define BLACK   0
#define RED     1
#define GREEN   2
#define YELLOW  3
#define BLUE    4
#define MAGENTA 5
#define CYAN    6
#define WHITE   7
/*********** Outras definicoes *******/
#define TRUE    1
#define FALSE   0
// ----------- Guarda as informacoes da tela ----------- //
int GdBgColor   = 47,
    GdFontColor = 30,
    GdFontBold  = 0 ;
// ----- Limpa a tela e Posiciona cursor no inicio ----- //
void clrscr()
{
    printf("%c[2J%c[?6h", ESC, ESC);
}
// --- Posiciona o cursor em um determinado ponto na tela --- //
void gotoxy(int x, int y)
{
    /*****************************
     *** y = Linha/ x = Coluna ***
     *****************************/
    printf("%c[%d;%dH", ESC, y, x);
}
// --------- Muda a cor de fundo do terminal --------- //
/********************************************************
 ******* UTILIZAR AS CONSTANTES DEFINIDAS !!!!! *********
 ********************************************************/
void setfontcolor(int color)
{
    printf("%c[3%dm", ESC, color);
    // ---- Guarda a nova cor definida
    GdFontColor = color;
}
// --------- Muda a cor da fonte no terminal --------- //
/********************************************************
 ******* UTILIZAR AS CONSTANTES DEFINIDAS !!!!! *********
 ********************************************************/
void setbgrcolor(int color)
{
    printf("%c[4%dm", ESC, color);
    // ---- Guarda a nova cor definida
    GdBgColor = color;
}
// ------- O texo fica em negrito no terminal -------- //
/********************************************************
 **** UTILIZAR AS CONSTANTES DEFINIDAS (TRUE/FALSE) *****
 ********************************************************/
void setfontbold(int status)
{
    printf("%c[%dm", ESC, status);
    // ---- Guarda o novo estilo definido
    GdFontBold = status;
    /****************************************************
      **** Na troca de negrito, o terminal perde as *****
      **** configuracoes ja definidas, portanto,    *****
      **** chamamos as funcoes de definicao de cor  *****
      **** novamente com os valores guardados       *****
      ***************************************************/
    setfontcolor(GdFontColor);
    setbgrcolor(GdBgColor);
}

// ------- O texto fica sublinhado no terminal -------- //
/********************************************************
 **** UTILIZAR AS CONSTANTES DEFINIDAS (TRUE,FALSE) *****
 ********************************************************/
void setunderline(int status)
{
    if (status) status = 4;
    printf("%c[%dm", ESC, status);
     /****************************************************
      **** Na troca de negrito, o terminal perde as *****
      **** configuracoes ja definidas, portanto,    *****
      **** chamamos as funcoes de definicao de cor  *****
      **** novamente com os valores guardados       *****
      ***************************************************/
    setfontcolor(GdFontColor);
    setbgrcolor(GdBgColor);
    setfontbold(GdFontBold);
}
// -------- O texto fica piscando no terminal --------- //
/********************************************************
 **** UTILIZAR AS CONSTANTES DEFINIDAS (TRUE/FALSE) *****
 ********************************************************/
void setblink(int status)
{
    if (status) status = 5;
    printf("%c[%dm", ESC, status);
    /****************************************************
      **** Na troca de negrito, o terminal perde as *****
      **** configuracoes ja definidas, portanto,    *****
      **** chamamos as funcoes de definicao de cor  *****
      **** novamente com os valores guardados       *****
      ***************************************************/
    setfontcolor(GdFontColor);
    setbgrcolor(GdBgColor);
    setfontbold(GdFontBold);
}

/****************************************************
 ******** Limpa a linha inteira onde o cursor *******
 ********           esta posicionado          *******
 ****************************************************/
void clrline()
{
    printf("%c[2K%cE", ESC, ESC);
}
#endif /* _CONIO_LINUX_H_ */