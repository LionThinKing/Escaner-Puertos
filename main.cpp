#include <windows.h>

enum ID{ID_SSERVIDOR=0,ID_SERVIDOR=1,ID_SPORT=2,ID_PUERTO1=3,ID_PUERTO2=4,ID_BUTTON=10,ID_SPUERTO1=5,ID_SPUERTO2=6,ID_LISTBOX=7,ID_SESTADO=8,ID_ESTADO=9};
HANDLE  hThread=0;
DWORD   dwData=0;
HWND    hClient;

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
char szClassName[ ] = "WindowsApp";

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil)

{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default color as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND+2;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           "Windows App",       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           544,                 /* The programs width */
           375,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nFunsterStil);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */


DWORD WINAPI GetState(LPVOID Data)
{
    struct hostent *He;
    struct sockaddr_in con;
    char *Servidor=(char*)malloc(100);
    char *Puerto=(char*)malloc(100);
    SOCKET Sock=socket(AF_INET,SOCK_STREAM,0);
    memset(Servidor,0,100);
    memset(Puerto,0,100);
    WSADATA wData;
    WSAStartup(MAKEWORD(2,0),&wData);
    
    SendMessage(GetDlgItem(hClient,ID_LISTBOX),(UINT)LB_RESETCONTENT,(WPARAM)0,(LPARAM)0);
    
    
    int P1,P2;
    
    GetWindowText(GetDlgItem(hClient,(ID_SERVIDOR)),Servidor,100);    
    GetWindowText(GetDlgItem(hClient,(ID_PUERTO1)),Puerto,6);
    P1=atoi(Puerto);
    
    GetWindowText(GetDlgItem(hClient,(ID_PUERTO2)),Puerto,6);
    P2=atoi(Puerto);
    
    if((He=gethostbyname(Servidor))==NULL)
    {
        SetWindowText(GetDlgItem(hClient,ID_ESTADO),"[X] Imposible Resolver Host");
        return 1;
    }
    
    if(P2==0)
    {    
        con.sin_family = AF_INET;
        con.sin_port = htons(P1);
        con.sin_addr = *((struct in_addr *)He->h_addr);
        memset(con.sin_zero,0,8);
        
        if(connect(Sock,(struct sockaddr *)&con,sizeof(struct sockaddr))==-1)
        {
            SetWindowText(GetDlgItem(hClient,ID_ESTADO),"[X] Puerto Cerrado");
        }
        else
        {
            SetWindowText(GetDlgItem(hClient,ID_ESTADO),"[OK] Puerto Abierto");
            itoa(P1,Puerto,10);
            SendMessage(GetDlgItem(hClient,ID_LISTBOX),(UINT)LB_ADDSTRING,(WPARAM)0,(LPARAM)Puerto);
        }
    }    
    closesocket(Sock);
    EnableWindow(GetDlgItem(hClient,ID_BUTTON),true);

    
}


LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
         case WM_CREATE:
                CreateWindowEx(0,"STATIC", "Servidor: ", WS_VISIBLE | WS_CHILD, 10, 10, 60, 20,hwnd, (HMENU)(ID_SSERVIDOR), NULL, NULL);
                CreateWindow("EDIT","",WS_OVERLAPPED | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_TABSTOP | WS_EX_NOPARENTNOTIFY | WS_EX_CLIENTEDGE | WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR,80,10,400,20,hwnd,(HMENU)(ID_SERVIDOR),NULL,NULL);
                CreateWindow("EDIT","",WS_OVERLAPPED | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_TABSTOP | WS_EX_NOPARENTNOTIFY | WS_EX_CLIENTEDGE | WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR,80,40,40,20,hwnd,(HMENU)(ID_PUERTO1),NULL,NULL);
                CreateWindow("EDIT","",WS_OVERLAPPED | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_TABSTOP | WS_EX_NOPARENTNOTIFY | WS_EX_CLIENTEDGE | WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR,150,40,40,20,hwnd,(HMENU)(ID_PUERTO2),NULL,NULL);
                CreateWindowEx(0,"STATIC", "Puertos: ", WS_VISIBLE | WS_CHILD, 10, 40, 60, 20,hwnd, (HMENU)(ID_SPUERTO1), NULL, NULL);
                CreateWindowEx(0,"STATIC", "--", WS_VISIBLE | WS_CHILD, 130, 40, 10, 20,hwnd, (HMENU)(ID_SPUERTO2), NULL, NULL);   
                CreateWindowEx(0,"listbox", "", LBS_STANDARD | WS_CHILD | WS_VISIBLE | WS_TABSTOP, 10, 70, 470, 200,hwnd, (HMENU)(ID_LISTBOX), NULL, NULL);
                CreateWindowEx(0,"BUTTON", "Start",WS_CHILD | WS_VISIBLE | WS_TABSTOP, 360, 40, 120, 25,hwnd, (HMENU)ID_BUTTON, NULL, NULL);
                
                CreateWindowEx(0,"STATIC", "Estado: ", WS_VISIBLE | WS_CHILD, 10, 300, 55, 20,hwnd, (HMENU)(ID_SESTADO), NULL, NULL);
                CreateWindow("EDIT","",WS_OVERLAPPED | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_TABSTOP | WS_EX_NOPARENTNOTIFY | WS_EX_CLIENTEDGE | WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR,70,300,410,20,hwnd,(HMENU)(ID_ESTADO),NULL,NULL);            
                EnableWindow(GetDlgItem(hwnd,ID_ESTADO),false);                             
                hClient=hwnd;
            break;
            
            case WM_COMMAND:
                switch (LOWORD(wParam))
                {
                    case ID_BUTTON:
                        EnableWindow(GetDlgItem(hwnd,ID_BUTTON),false);
                    if(!(hThread=CreateThread(NULL, 0, GetState, &dwData, 0, NULL)))
                    {
                        ExitProcess(dwData);
                    }
                    break;
                }
            break;
        
        
        
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
