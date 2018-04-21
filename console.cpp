#include "console.h"

double _tickCount; // Biến đếm thời gian, sử dụng cho Mainloop

HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);// Lấy handle cửa sổ console hiện hành



void gotoxy(short x,short y)
{
    HANDLE hConsoleOutput;
    COORD Cursor_an_Pos = { x,y};
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsoleOutput , Cursor_an_Pos);
}

// Đặt màu cho chữ
void SetColor(WORD color)
{
    HANDLE hConsoleOutput;
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
    GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);

    WORD wAttributes = screen_buffer_info.wAttributes;
    color &= 0x000f;
    wAttributes &= 0xfff0;
    wAttributes |= color;

    SetConsoleTextAttribute(hConsoleOutput, wAttributes);
}

// Đặt màu nền cho chữ
void SetBGColor(WORD color)
{
    HANDLE hConsoleOutput;
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
    GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);

    WORD wAttributes = screen_buffer_info.wAttributes;
    color &= 0x000f;
    color <<= 4;
    wAttributes &= 0xff0f;
    wAttributes |= color;

    SetConsoleTextAttribute(hConsoleOutput, wAttributes);
}

// Thiết lập chế độ hiển thị, có fullscreen hay không
BOOL NT_SetConsoleDisplayMode(HANDLE hOutputHandle, DWORD dwNewMode)
{
    typedef BOOL (WINAPI *SCDMProc_t) (HANDLE, DWORD, LPDWORD);
    SCDMProc_t SetConsoleDisplayMode;
    HMODULE hKernel32;
    BOOL bFreeLib = FALSE, ret;
    const char KERNEL32_NAME[] = "kernel32.dll";

    hKernel32 = GetModuleHandleA(KERNEL32_NAME);
    if (hKernel32 == NULL)
    {
        hKernel32 = LoadLibraryA(KERNEL32_NAME);
        if (hKernel32 == NULL)
            return FALSE;

        bFreeLib = true;
    }

    SetConsoleDisplayMode =
        (SCDMProc_t)GetProcAddress(hKernel32, "SetConsoleDisplayMode");
    if (SetConsoleDisplayMode == NULL)
    {
        SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
        ret = FALSE;
    }
    else
    {
        DWORD tmp;
        ret = SetConsoleDisplayMode(hOutputHandle, dwNewMode, &tmp);
    }

    if (bFreeLib)
        FreeLibrary(hKernel32);

    return ret;
}

// Đặt chế độ FullScreen
void setFullScreen()
{
    NT_SetConsoleDisplayMode( GetStdHandle( STD_OUTPUT_HANDLE ), 1 );
}

// Thoát khỏi fullscreen
void exitFullScreen()
{
    NT_SetConsoleDisplayMode( GetStdHandle( STD_OUTPUT_HANDLE ), 0 );
}

// Ẩn hiện con trỏ nhấp nháy trong cửa sổ Console
void ShowCur(bool CursorVisibility)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursor = {1, CursorVisibility};
    SetConsoleCursorInfo(handle, &cursor);
}

// Xóa toàn bộ nội dung cửa sổ console - chôm từ MSDN
void cls( HANDLE hConsole )
{
   COORD coordScreen = { 0, 0 };    // home for the cursor
   DWORD cCharsWritten;
   CONSOLE_SCREEN_BUFFER_INFO csbi;
   DWORD dwConSize;

// Get the number of character cells in the current buffer.

   if( !GetConsoleScreenBufferInfo( hConsole, &csbi ))
      return;
   dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

   // Fill the entire screen with blanks.

   if( !FillConsoleOutputCharacter( hConsole, (TCHAR) ' ',
      dwConSize, coordScreen, &cCharsWritten ))
      return;

   // Get the current text attribute.

   if( !GetConsoleScreenBufferInfo( hConsole, &csbi ))
      return;

   // Set the buffer's attributes accordingly.

   if( !FillConsoleOutputAttribute( hConsole, csbi.wAttributes,
      dwConSize, coordScreen, &cCharsWritten ))
      return;

   // Put the cursor at its home coordinates.

   SetConsoleCursorPosition( hConsole, coordScreen );
}
// Viết lại hàm clrscr, không cần cũng được nhưng để nhìn vào code cho đẹp mắt
void clrscr()
{
	CONSOLE_SCREEN_BUFFER_INFO	csbiInfo;
	HANDLE	hConsoleOut;
	COORD	Home = {0,0};
	DWORD	dummy;

	hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsoleOut,&csbiInfo);

	FillConsoleOutputCharacter(hConsoleOut,' ',csbiInfo.dwSize.X * csbiInfo.dwSize.Y,Home,&dummy);
	csbiInfo.dwCursorPosition.X = 0;
	csbiInfo.dwCursorPosition.Y = 0;
	SetConsoleCursorPosition(hConsoleOut,csbiInfo.dwCursorPosition);
}
// Kiểm tra xem phím nào được nhấn, trả về true nếu phím đó đã được nhấn xuống
bool checkKey(int key)
{
    return GetAsyncKeyState(key);
}

/*
Khúc này mình tự sáng tác thêm để thực hiện vòng lặp chính trong game
Rất tiện dụng
Vì gọi mainloop bằng vòng lặp while sẽ chạy rất nhanh, khó kiểm soát được tốc độ
nên giải pháp đưa ra là dùng một bộ đếm, cứ sau một quãng thời gian tăng giá trị
nếu giá trị đó đạt đến một mức nhất định thì mới gọi hàm mainloop
Cách này kiểm soát tốc độ game khá tốt
Có thêm một cách khác là dùng Timer, nhưng ở đây mình làm vậy cho đơn giản
*/

// reset lại bộ đếm
void resetTick()
{
    _tickCount = 0;
}

// Đếm, thực hiện việc tăng biến đếm
void tickCount()
{
    _tickCount += 0.1;
}

// trả về giá trị hiện tại của biến đếm
double getTickCount()
{
    return _tickCount;
}

// Thực hiện việc đếm và gọi vòng lặp chính (mainloop)
// Tham số truyền vào gồm có: giá trị cực đại mà sau khi tăng đến mức đó, sẽ gọi tiếp mainloop và renderloop
//        gloop: game loop hay còn gọi là mainloop, thực hiện việc xử lý logic trong game
//        rloop: render loop, thực hiện việc in các hình ảnh, chữ,.. trong game ra màn hình
void Tick(double maxValue, void (*gloop) (), void (*rloop) ())
{
    tickCount();
        if (getTickCount() > maxValue){
            resetTick();
            gloop(); // Game loop
        }
         rloop(); // Render loop

}


