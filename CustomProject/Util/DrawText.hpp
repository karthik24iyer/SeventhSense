#pragma once

#include <Windows.h>
#include <iostream>
#include <cstring>

class DrawTxt {
    public: 
        void drawRect(int posX, int posY, int size, char c) {
            COLORREF color = RGB(255, 255, 255);
            switch (c) {
                case 'b': 
                    color = RGB(0, 0, 255);
                    break;
                case 'g':
                    color = RGB(0, 255, 0);
                    break;
                case 'r':
                    color = RGB(255, 0, 0);
                    break;
                case 'y':
                    color = RGB(255, 255, 0);
                    break;
                default :
                    color = RGB(255, 255, 255);
            }
            HDC screenDC = ::GetDC(0);
            SelectObject(screenDC, GetStockObject(DC_BRUSH));
            SetDCBrushColor(screenDC, color);
            Rectangle(screenDC, posX, posY, posX + size, posY + size);
            ReleaseDC(0, screenDC);
        }

        void drawMyText(float x, float y, const char* position)
        {
            //std::cout << "\n position=" << position;
            //const char* position = "K";

            int howmany = strlen(position);
            //HWND gamewindowhandle = FindWindow(NULL, windowcaption); //get the game hwnd 
            HDC hdc = GetDC(0);

            SetBkColor(hdc, RGB(255, 255, 255));                   //WHITE BACKGROUND
            //SetBkMode(hdc, TRANSPARENT);                        //MAKES BACKGROUND TRANSPARENT
                                                                     //SO ALL YOU SEE IS THE TEXT
            //MAKE SHADOW FOR TEXT (BLACK) and offset it by 1 pixel
            //SetTextColor(hdc, RGB(0, 0, 0));
            //TextOut(hdc, x + 1, y + 1, position, howmany);

            //MAKE SECOND SHADOW FOR TEXT (BLACK) and offset it by 2 pixels
            //TextOut(hdc, x + 2, y + 2, position, howmany);


            //PRINTS MY TEXT OVER THE BLACK TEXT IN RED, GIVES IT A DROP SHADOW APPEARANCE. 
            //SetTextColor(hdc, RGB(0, 255, 0));
            TextOut(hdc, x, y, position, howmany);

            //UpdateWindow(0);
            ReleaseDC(0, hdc);
        }
};