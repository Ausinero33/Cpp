#pragma once
#include <Windows.h>
#include <chrono>
#include <string>
#include <thread>

enum COLOUR
{
    FG_BLACK = 0x0000,
    FG_DARK_BLUE = 0x0001,
    FG_DARK_GREEN = 0x0002,
    FG_DARK_CYAN = 0x0003,
    FG_DARK_RED = 0x0004,
    FG_DARK_MAGENTA = 0x0005,
    FG_DARK_YELLOW = 0x0006,
    FG_GREY = 0x0007,
    FG_DARK_GREY = 0x0008,
    FG_BLUE = 0x0009,
    FG_GREEN = 0x000A,
    FG_CYAN = 0x000B,
    FG_RED = 0x000C,
    FG_MAGENTA = 0x000D,
    FG_YELLOW = 0x000E,
    FG_WHITE = 0x000F,
    BG_BLACK = 0x0000,
    BG_DARK_BLUE = 0x0010,
    BG_DARK_GREEN = 0x0020,
    BG_DARK_CYAN = 0x0030,
    BG_DARK_RED = 0x0040,
    BG_DARK_MAGENTA = 0x0050,
    BG_DARK_YELLOW = 0x0060,
    BG_GREY = 0x0070,
    BG_DARK_GREY = 0x0080,
    BG_BLUE = 0x0090,
    BG_GREEN = 0x00A0,
    BG_CYAN = 0x00B0,
    BG_RED = 0x00C0,
    BG_MAGENTA = 0x00D0,
    BG_YELLOW = 0x00E0,
    BG_WHITE = 0x00F0,
};

enum PIXEL_TYPE
{
    PIXEL_SOLID = 0x2588,
    PIXEL_THREEQUARTERS = 0x2593,
    PIXEL_HALF = 0x2592,
    PIXEL_QUARTER = 0x2591,
};

/*
* Ventana CMD
* 
* 
* 
* 
* Gonzalo Burgos de la Hera
*/
class WindowCMD {
private:
    /*
    * Clase abstracta que guarda info de los timings
    */
    struct Time {
    protected:
        std::chrono::time_point<std::chrono::system_clock>* t1, * frameStart;
    public:
        double fElapsedTime;
        int nFrames;
        virtual void timeFunction() = 0;

        Time() {
            fElapsedTime = 0;
        }
    };

    /*
    * Timings para framerate no fijo. PONER FUNCIÓN timeFunction().
    */
    struct UTime : Time {
        std::chrono::time_point<std::chrono::system_clock> tp1, tp2;

        UTime() {
            tp1 = std::chrono::system_clock::now();
            tp2 = std::chrono::system_clock::now();
            fElapsedTime = 0;
        }

        virtual void timeFunction() {
            tp2 = std::chrono::system_clock::now();
            std::chrono::duration<float> elapsedTime = tp2 - tp1;
            tp1 = tp2;
            fElapsedTime = elapsedTime.count();
        }
    };
    /*
    * Timings para framerate fijo. PONER FUNCIÓN timeFunction().
    */
    struct FTime : Time {
        float fixedElapsedt;
        FTime(float fps, std::chrono::time_point<std::chrono::system_clock>* t1, std::chrono::time_point<std::chrono::system_clock>*frameStart) {
            fixedElapsedt = 1.0 / fps;
            this->t1 = t1;
            this->frameStart = frameStart;
            nFrames = 0;
        }

        virtual void timeFunction() {
            nFrames++;
            double tTrans = nFrames * fixedElapsedt;

		    auto tTransReal = std::chrono::system_clock::now();
		    std::chrono::duration<double> tTransAjustado = tTransReal - *t1;
		    double error = tTransAjustado.count() - tTrans;

			auto frameEnd = std::chrono::system_clock::now();
		    std::chrono::duration<double> elapsedTime = frameEnd - *frameStart;
		    double frameTime = elapsedTime.count();

		    if (fixedElapsedt > frameTime) {
			    int t = (fixedElapsedt - frameTime - error) * 1000;
                fElapsedTime = fixedElapsedt + t / 1000;
                std::this_thread::sleep_for(std::chrono::milliseconds(t));
            }
            else {
                fElapsedTime = frameTime;
            }
        }
    };

    
public:
    std::chrono::time_point<std::chrono::system_clock> t1, frameStart;
    short width, height;
    CHAR_INFO* m_bufScreen;
    SMALL_RECT m_rectWindow;
    HANDLE hConsole;
    DWORD dwBytesWritten;
    Time *time;

    /*
    * Constructor framerate sin limitar.
    */
    WindowCMD(short width, short heigh) {
        this->width = width;
        this->height = heigh;
        m_bufScreen = new CHAR_INFO[width * height];
        m_rectWindow = { 0, 0, (short)(width - 1), (short)(height - 1) };
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        SetConsoleActiveScreenBuffer(hConsole);
        SetConsoleScreenBufferSize(hConsole, { width, heigh });
        setFont();
        SetConsoleWindowInfo(hConsole, TRUE, &m_rectWindow);
        ShowConsoleCursor(false);

        dwBytesWritten = 0;

        time = new UTime();
    }

    /*
    * Constructor framerate limitado.
    */
    WindowCMD(short width, short heigh, float fps) {
        this->width = width;
        this->height = heigh;
        m_bufScreen = new CHAR_INFO[width * height];
        m_rectWindow = { 0, 0, (short)(width - 1), (short)(height - 1) };
        hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

        SetConsoleActiveScreenBuffer(hConsole);
        SetConsoleScreenBufferSize(hConsole, { width, heigh });
        setFont();
        SetConsoleWindowInfo(hConsole, TRUE, &m_rectWindow);
        ShowConsoleCursor(false);

        dwBytesWritten = 0;

        t1 = std::chrono::system_clock::now();
        frameStart = std::chrono::system_clock::now();

        this->t1 = t1;
        this->frameStart = frameStart;
        time = new FTime(fps, &this->t1, &this->frameStart);
    }

    void draw() {
        WriteConsoleOutput(hConsole, m_bufScreen, { width, height }, { 0,0 }, &m_rectWindow);
        std::wstring st1 = std::to_wstring(1 / time->fElapsedTime);
        const wchar_t *c = L" + ";
        st1.append(c);
        std::wstring st3 = std::to_wstring(time->fElapsedTime);
        st1.append(st3);
        SetConsoleTitle(st1.c_str());
    }

    void setChar(int x, int y, unsigned short c, short color) {
        m_bufScreen[y * width + x].Char.UnicodeChar = c;
        m_bufScreen[y * width + x].Attributes = color;
    }

    void resetScreen() {
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                m_bufScreen[j * width + i].Char.UnicodeChar = 0x0020;
                m_bufScreen[j * width + i].Attributes = 0x0000;
            }
        }
    }

    void ShowConsoleCursor(bool showFlag) {
        CONSOLE_CURSOR_INFO cursorInfo;

        GetConsoleCursorInfo(hConsole, &cursorInfo);
        cursorInfo.bVisible = showFlag; // set the cursor visibility
        SetConsoleCursorInfo(hConsole, &cursorInfo);
    }

    void setFont() {
        CONSOLE_FONT_INFOEX cfi;
        cfi.cbSize = sizeof(cfi);
        cfi.nFont = 0;
        cfi.dwFontSize.X = 5;
        cfi.dwFontSize.Y = 5;
        cfi.FontFamily = FF_DONTCARE;
        cfi.FontWeight = FW_NORMAL;
        wcscpy_s(cfi.FaceName, L"Consolas");
        SetCurrentConsoleFontEx(hConsole, false, &cfi);
    }

    void setScreen(CHAR_INFO *newScreeen) {
        m_bufScreen = newScreeen;
    }
};
