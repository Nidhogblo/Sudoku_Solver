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
#include <stdio.h>
#include <tchar.h>
#include "Sudoku.h"
#include "Definiciones.cpp"

//Function to hide the console
void ocultarConsola(){
     HWND console;
     AllocConsole();
     console = FindWindowA("ConsoleWindowClass", NULL);
     ShowWindow(console,0);
}

//Declare windows procedure
LRESULT CALLBACK ProcedimientoVentana(HWND hwnd,UINT msg, WPARAM wParam,LPARAM lParam);

int WINAPI WinMain(HINSTANCE hIns,HINSTANCE hInstanciaPrevia,LPSTR lpCmdLinea,int nCmdShow){
    ocultarConsola();
    HWND ventana; //The handle for our window
    MSG mensaje; //Here the messages to the applications are saved
    WNDCLASSEX clase; //Data structure for the windowclass

    //The window structure
    clase.cbSize = sizeof(WNDCLASSEX);
    clase.cbClsExtra = 0;
    clase.style = CS_HREDRAW | CS_VREDRAW;
    clase.lpfnWndProc = ProcedimientoVentana; //This function is called by windows
    clase.hInstance = hIns;
    clase.lpszClassName = _T("INICIO");

    //To use default icon and mouse pointer
    clase.hIcon = LoadIcon(NULL,0);
    clase.hIconSm =LoadIcon(NULL,IDI_INFORMATION);
    clase.hCursor = LoadCursor(NULL,IDC_ARROW);

    //Registrer the window class. If it fails quit the program
    if(!RegisterClassEx(&clase)){
        MessageBox( NULL,_T("No se pudo ejecutar la aplicacion"),_T("Error"),MB_ICONERROR);
        return EXIT_FAILURE;
    }

    ventana = CreateWindowEx(
        0, //Extended posibilities for variation
        _T("INICIO"), //Class name
        _T("Sudoku Resolver"), //Title text
        WS_SYSMENU, //Default window
        400,80, //Position of the window
        580, //Window width
        630, //Window height
        HWND_DESKTOP, //The window is a child window to Desktop
        NULL, //No menu
        hIns, //Program Instance handler
        NULL); //No window creation data

    //Make the window visible on the screen
    ShowWindow(ventana,nCmdShow);

    //Run the message loop. It will run until GetMessage returns 0
    while(GetMessage(&mensaje,NULL,0,0)>0){
            //Translate virtual-key messages into character messages
            TranslateMessage(&mensaje);
            //Send message to WindowProcedure
            DispatchMessage(&mensaje);
    }
    //The program return value is 0, the value that PostQuitMessage gave
    return mensaje.wParam;
}

LRESULT CALLBACK ProcedimientoVentana(HWND hwnd,UINT msg, WPARAM wParam,LPARAM lParam){
    switch(msg){
                case WM_CREATE:{
                    //Create the buttons
                    CreateWindowEx(0,_T("BUTTON"),_T("Exit"),BS_CENTER|WS_VISIBLE|WS_CHILD,400,500,150,30,hwnd,(HMENU)10,NULL,NULL);
                    CreateWindowEx(0,_T("BUTTON"),_T("Solve"),BS_CENTER|WS_VISIBLE|WS_CHILD,18,500,150,30,hwnd,(HMENU)11,NULL,NULL);
                    CreateWindowEx(0,_T("BUTTON"),_T("Credits"),BS_CENTER|WS_VISIBLE|WS_CHILD,209,450,150,30,hwnd,(HMENU)12,NULL,NULL);
                    CreateWindowEx(0,_T("BUTTON"),_T("DataBase"),BS_CENTER|WS_VISIBLE|WS_CHILD,209,520,150,30,hwnd,(HMENU)13,NULL,NULL);                    
                    
                    //Create the edit fields and static text
                    long long iIdCelda = 10;
                    int coordenadasX = 490;
                    int coordenadasY = 30;
                    char columnRowNumber[2];
                    for (int i = 1; i < 10; i++){
                        itoa(i,columnRowNumber,10);
                        iIdCelda+=91;
                        coordenadasY+=40;
                        coordenadasX-=360;
                        CreateWindowA("Static",columnRowNumber,ES_CENTER|WS_CHILD|WS_VISIBLE, coordenadasX-55,coordenadasY,10,20,hwnd,NULL,NULL,NULL);
                        for (int k = 1; k < 10; k++){
                            if (i==1){
                                itoa(k,columnRowNumber,10);
                                
                                CreateWindowA("Static",columnRowNumber,ES_CENTER|WS_CHILD|WS_VISIBLE, coordenadasX,coordenadasY-55,20,15,hwnd,NULL,NULL,NULL);
                            }
                            HMENU idCelda = (HMENU)iIdCelda;
                            CreateWindowEx(0,_T("EDIT"),_T(""),ES_NUMBER|ES_AUTOHSCROLL|ES_LEFT|WS_CHILD|WS_VISIBLE,coordenadasX,coordenadasY,20,20,hwnd,idCelda,NULL,NULL);
                            coordenadasX+=40;
                            iIdCelda++;
                        }   
                    }
                    break;
                }

                //If a button is clicked
                case WM_COMMAND:{
                    switch(LOWORD(wParam)){
                        case ID_BTN_SALIR:{
                            if(MessageBox(hwnd,_T("¿Are you sure you want to leave?"),_T("Exit"),MB_OKCANCEL|MB_ICONQUESTION)==IDOK){
                                SendMessage(hwnd,WM_DESTROY,0,0);
                            }
                            break;
                        }

                        case ID_BTN_RESOLVER:{
                            //All boxes start with a value of 0
                            int matrizCeldas[9][9];
                            for (int i = 0; i < 9; i++){                               
                                for (int k = 0; k < 9; k++){                                  
                                        matrizCeldas[i][k] = 0;  
                                    }                                  
                                }
                            
                            //Get the values of the inputs
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
                                    else{
                                        matrizCeldas[i][k] = 0;
                                    }
                                    iIdCelda++;
                                }
                            }

                            //Create and solve a sudoku object
                            Sudoku sudoku(matrizCeldas);

                            //Create static text to display the sudoku game solved
                            int coordenadasX = 490;
                            int coordenadasY = 30;           
                            for (int i = 0; i < 9; i++){
                                char text[2]; 
                                coordenadasY+=40;
                                coordenadasX-=360;
                                for (int k = 0; k < 9; k++){
                                    itoa(sudoku.matrix[i][k],text,10); 
                                    CreateWindowA("Static",text,ES_CENTER|WS_CHILD|WS_VISIBLE, coordenadasX,coordenadasY,20,20,hwnd,NULL,NULL,NULL);
                                    coordenadasX+=40;
                                }   
                            }
                            
                            //Example of a Get to Oracle: https://apex.oracle.com/pls/apex/sudokusolver/sudoku/update?vfila1=123456789&vfila2=123456789...
                            //Send data to database in Oracle
                            string url = "https://apex.oracle.com/pls/apex/sudokusolver/sudoku/update?vfila";
                            for (int i = 0; i < 9; i++)
                            {  
                                char text[2];
                                itoa(i+1,text,10);
                                url+=text;
                                url+="=";
                                for (int k = 0; k < 9; k++)
                                {
                                    char text[2];
                                    itoa(sudoku.matrix[i][k],text,10);
                                    url+=text;
                                }
                                if (i<8)
                                {
                                    url+="^&";
                                    url+="vfila";
                                }    
                            }
                            string command = "start iexplore "+url;
                            char commandChar[100];
                            strcpy(commandChar,command.c_str());                           
                            system(commandChar);
                            Sleep(5000);
                            system("taskkill /IM iexplore.exe /F");  
                            break;
                        }
                        case ID_BTN_CREDITOS:{
                            UINT btn;
                            MessageBoxA(hwnd,"Contributors:\nEmmanuel Humberto Solorzano Cabrera\nPablo Gutierrez Costales","Creators",btn);
                            break;
                        }
                        case ID_BTN_DATABASE:{
                            char command[] = "start chrome https://apex.oracle.com/pls/apex/r/sudokusolver/sudoku-solver";
                            system(command);  
                            break;
                        }

                    }
                    break;
                }

                case WM_DESTROY:{
                    PostQuitMessage(0);
                    break;
                }

                default:{
                return DefWindowProc(hwnd, msg,wParam,lParam);
                }
    }
    return 0;
}
