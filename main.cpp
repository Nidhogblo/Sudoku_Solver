//Algoritmo con interfaz gráfica que resuelve un juego de sudoku con las entradas que brinde el usuario.
/*
Creado por:
Pablo Gutiérrez Costales
Emmanuel Humberto Solórzano Cabrera

Empezado el día:
Jueves 27 de Enero del 2022

*/

#include <windows.h>
#include <string.h>
#include "Definiciones.cpp"
#include <stdio.h>
#include <tchar.h>
#include "Sudoku.h"


void ocultarConsola(){
     HWND console;
     AllocConsole();
     console = FindWindowA("ConsoleWindowClass", NULL);
     ShowWindow(console,0);
}

LRESULT CALLBACK ProcediementoVentana(HWND hwnd,UINT msg, WPARAM wParam,LPARAM lParam);
int WINAPI WinMain(HINSTANCE hIns,HINSTANCE hInstanciaPrevia,LPSTR lpCmdLinea,int nCmdShow){
    //ocultarConsola();
    HWND ventana;
    MSG mensaje;
    WNDCLASSEX clase;
    clase.cbSize = sizeof(WNDCLASSEX);
    clase.cbClsExtra = 0;
    clase.style = CS_HREDRAW | CS_VREDRAW;
    clase.lpfnWndProc = ProcediementoVentana;
    clase.hInstance = hIns;
    clase.hIcon = LoadIcon(NULL,0);
    clase.hIconSm =LoadIcon(NULL,IDI_INFORMATION);
    clase.hCursor = LoadCursor(NULL,IDC_ARROW);
    clase.lpszClassName = _T("INICIO");
    if(!RegisterClassEx(&clase)){
        MessageBox( NULL,_T("No se pudo ejecutar la aplicacion"),_T("Error"),MB_ICONERROR);
        return EXIT_FAILURE;
    }

    ventana = CreateWindowEx(0,_T("Inicio"),_T("Sudoku Resolver"),WS_OVERLAPPEDWINDOW |WS_SYSMENU,
                            400,80,580,630,HWND_DESKTOP,NULL,hIns,NULL);



    CreateWindowEx(0,_T("BUTTON"),_T("Exit"),BS_CENTER|WS_VISIBLE|WS_CHILD,400,500,150,30,ventana,(HMENU)10,NULL,NULL);
    CreateWindowEx(0,_T("BUTTON"),_T("Solve"),BS_CENTER|WS_VISIBLE|WS_CHILD,18,500,150,30,ventana,(HMENU)11,hIns,NULL);
    CreateWindowEx(0,_T("BUTTON"),_T("Credits"),BS_CENTER|WS_VISIBLE|WS_CHILD,209,450,150,30,ventana,(HMENU)12,hIns,NULL);


    ShowWindow(ventana,nCmdShow);
    UpdateWindow(ventana);

    while(GetMessage(&mensaje,NULL,0,0)>0){
            TranslateMessage(&mensaje);
            DispatchMessage(&mensaje);
    }
    return 0;
}

LRESULT CALLBACK ProcediementoVentana(HWND hwnd,UINT msg, WPARAM wParam,LPARAM lParam){
    switch(msg){
                case WM_CREATE:{
                    long long iIdCelda = 10;
                    int coordenadasX = 490;
                    int coordenadasY = 30;
                    char hola[2];
                    
                    for (int i = 1; i < 10; i++){
                        itoa(i,hola,10);
                        iIdCelda+=91;
                        coordenadasY+=40;
                        coordenadasX-=360;
        
                        CreateWindowA("Static",hola,ES_CENTER|WS_CHILD|WS_VISIBLE, coordenadasX-55,coordenadasY,10,20,hwnd,NULL,NULL,NULL);
                        for (int k = 1; k < 10; k++){
                            if (i==1){
                                itoa(k,hola,10);
                                
                                CreateWindowA("Static",hola,ES_CENTER|WS_CHILD|WS_VISIBLE, coordenadasX,coordenadasY-55,20,15,hwnd,NULL,NULL,NULL);
                            }
                            HMENU idCelda = (HMENU)iIdCelda;
                            CreateWindowEx(0,_T("EDIT"),_T(""),ES_NUMBER|ES_AUTOHSCROLL|ES_LEFT|WS_CHILD|WS_VISIBLE,coordenadasX,coordenadasY,20,20,hwnd,idCelda,NULL,NULL);
                            coordenadasX+=40;
                            iIdCelda++;
                        }   
                    }
                    break;
                }

                case WM_COMMAND:{
                    switch(LOWORD(wParam)){
                        case ID_BTN_SALIR:{
                            if(MessageBox(hwnd,_T("¿Are you sure you want to leave?"),_T("Exit"),MB_OKCANCEL|MB_ICONQUESTION)==IDOK){
                                SendMessage(hwnd,WM_DESTROY,0,0);
                            }
                            break;
                        }

                        case ID_BTN_RESOLVER:{
                            int matrizCeldas[9][9];
                            for (int i = 0; i < 9; i++){                               
                                for (int k = 0; k < 9; k++){                                  
                                        matrizCeldas[i][k] = 0;  
                                    }                                  
                                }
                            




                            int iIdCelda = 10;
                            
                            int valorCelda;
                            for (int i = 0; i < 9; i++){
                                iIdCelda+=91;
                                for (int k = 0; k < 9; k++){
                                    char celda[2]="";
                                    GetDlgItemTextA(hwnd,iIdCelda,celda,2);
                                    valorCelda = atof(celda);
                                    if (valorCelda < 10 && valorCelda > 0){
                                        matrizCeldas[i][k] = valorCelda;  
                                    }
                                    iIdCelda++;
                                }
                            }

                            //Aquí sigue la lógica del programa
                            Sudoku sudoku(matrizCeldas);
                            int coordenadasX = 490;
                            int coordenadasY = 30;
                            char text[2];            
                            for (int i = 0; i < 9; i++){
                                coordenadasY+=40;
                                coordenadasX-=360;
                                for (int k = 0; k < 9; k++){
                                    itoa(sudoku.Matrix[i][k],text,10); 
                                    CreateWindowA("Static",text,ES_CENTER|WS_CHILD|WS_VISIBLE, coordenadasX,coordenadasY,20,20,hwnd,NULL,NULL,NULL);
                                    coordenadasX+=40;
                                }   
                            }
                            
                            break;
                        }
                        case ID_BTN_CREDITOS:{
                            UINT a;
                            MessageBoxA(hwnd,"Contributors:\nEmmanuel Humberto Solorzano Cabrera\nPablo Gutierrez Costales","Creators",a);
                            break;

                        }

                    }
                    break;
                }

                case WM_DESTROY:{
                    PostQuitMessage(0);
                    break;
                }

                case WM_CTLCOLORSTATIC:{
                    HDC hdcStatic = (HDC) wParam;
                    break;
                }

                case WM_PAINT:
                {
                    PAINTSTRUCT ps;
                    HDC hdc = BeginPaint(hwnd, &ps);



                    FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+2));

                    EndPaint(hwnd, &ps);
                }

                default:{
                return DefWindowProc(hwnd, msg,wParam,lParam);
                }
    }
    return 0;
}
