// Copyright (c) 2024 Jay Prall
// SPDX-License-Identifier: MIT



#ifndef COLORSPACE_H
#define COLORSPACE_H

COLORREF CMYK2RGB(BYTE c, BYTE m, BYTE y, BYTE k);

void RGB2CMYK(BYTE r, BYTE g, BYTE b, BYTE& c, BYTE& m, BYTE& y, BYTE& k);



    BYTE    byRed = 0;
    BYTE    byGreen = 0;
    BYTE    byBlue = 0;

    BYTE    byHue = 0;
    BYTE    bySaturation = 0;
    BYTE    byBrightness = 0;

    BYTE    byCyan = 0;
    BYTE    byMagenta = 0;
    BYTE    byYellow = 0;
    BYTE    byBlack = 0;

#endif //XCMYK_H
