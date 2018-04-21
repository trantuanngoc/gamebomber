#ifndef CONSOLE_H_INCLUDED
#define CONSOLE_H_INCLUDED

#define    KEY_A    0x41
#define    KEY_B    0x42
#define    KEY_C    0x43
#define    KEY_D    0x44
#define    KEY_E    0x45
#define    KEY_F    0x46
#define    KEY_G    0x47
#define    KEY_H    0x48
#define    KEY_I    0x49
#define    KEY_J    0x4A
#define    KEY_K    0x4B
#define    KEY_L    0x4C
#define    KEY_M    0x4D
#define    KEY_N    0x4E
#define    KEY_O    0x4F
#define    KEY_P    0x50
#define    KEY_Q    0x51
#define    KEY_R    0x52
#define    KEY_S    0x53
#define    KEY_T    0x54
#define    KEY_U    0x55
#define    KEY_V    0x56
#define    KEY_W    0x57
#define    KEY_X    0x58
#define    KEY_Y    0x59
#define    KEY_Z    0x5A
#define     KEY_LEFT    VK_LEFT
#define     KEY_RIGHT   VK_RIGHT
#define     KEY_UP      VK_UP
#define     KEY_DOWN    VK_DOWN
#define     KEY_ESC     VK_ESCAPE

#include<windows.h>


// Đưa con trỏ đến tọa độ x:y
void gotoxy(short x,short y);

// Đặt màu cho chữ
void SetColor(WORD color);

// Đặt màu nền cho chữ
void SetBGColor(WORD color);


// Thiết lập chế độ hiển thị, có fullscreen hay không
BOOL NT_SetConsoleDisplayMode(HANDLE hOutputHandle, DWORD dwNewMode);

// Đặt chế độ FullScreen
void setFullScreen();


// Thoát khỏi fullscreen
void exitFullScreen();


// Ẩn hiện con trỏ nhấp nháy trong cửa sổ Console
void ShowCur(bool CursorVisibility);


// Xóa toàn bộ nội dung cửa sổ console - chôm từ MSDN
void cls( HANDLE hConsole );

// Viết lại hàm clrscr, không cần cũng được nhưng để nhìn vào code cho đẹp mắt
void clrscr();

// Kiểm tra xem phím nào được nhấn, trả về true nếu phím đó đã được nhấn xuống
bool checkKey(int key);


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
void resetTick();


// Đếm, thực hiện việc tăng biến đếm
void tickCount();


// trả về giá trị hiện tại của biến đếm
double getTickCount();


// Thực hiện việc đếm và gọi vòng lặp chính (mainloop)
// Tham số truyền vào gồm có: giá trị cực đại mà sau khi tăng đến mức đó, sẽ gọi tiếp mainloop và renderloop
//        gloop: game loop hay còn gọi là mainloop, thực hiện việc xử lý logic trong game
//        rloop: render loop, thực hiện việc in các hình ảnh, chữ,.. trong game ra màn hình
void Tick(double maxValue, void (*gloop) (), void (*rloop) ());


#endif // CONSOLE_H_INCLUDED
