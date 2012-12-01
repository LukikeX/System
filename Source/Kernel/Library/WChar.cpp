#include "WChar.h"

WChar WChar::CP437[] = {
    "Ç", "ü", "é", "â", "ä", "à", "å", "ç", "ê", "ë", "è", "ï", "î", "ì", "Ä", "Å",
    "É", "æ", "Æ", "ô", "ö", "ò", "û", "ù", "ÿ", "Ö", "Ü", "¢", "£", "¥", "₧", "ƒ",
    "á", "í", "ó", "ú", "ñ", "Ñ", "ª", "º", "¿", "⌐", "¬", "½", "¼", "¡", "«", "»",
    "░", "▒", "▓", "│", "┤", "╡", "╢", "╖", "╕", "╣", "║", "╗", "╝", "╜", "╛", "┐",
    "└", "┴", "┬", "├", "─", "┼", "╞", "╟", "╚", "╔", "╩", "╦", "╠", "═", "╬", "¤",
    "╨", "╤", "╥", "╙", "╘", "╒", "╓", "╫", "╪", "┘", "┌", "█", "▄", "▌", "▐", "▀",
    "α", "ß", "Γ", "π", "Σ", "σ", "µ", "τ", "Φ", "Θ", "Ω", "δ", "∞", "φ", "ε", "∩",
    "≡", "±", "≥", "≤", "⌠", "⌡", "÷", "≈", "°", "∙", "·", "√", "ⁿ", "²", "■", "⍽"
};

WChar::WChar() {
    value = 0;
}

WChar::WChar(char c) {
    affectAscii(c);
}

WChar::WChar(const char* c, uchar encoding) {
    if (encoding == UE_UTF8)
        affectUtf8(c);
}

uint WChar::ucharLen(const char* c, uchar encoding) {
    if (encoding == UE_UTF8) {
        if (!(c[0] & 0x80))
            return 1;
        else if ((c[0] & 0xE0) == 0xC0)
            return 2;
        else if ((c[0] & 0xF0) == 0xE0)
            return 3;
        else if ((c[0] & 0xF8) == 0xF0)
            return 4;
    }
    return 1;
}

uint WChar::utfLen(const char* c, uchar encoding) {
    uint i = 0, ret = 0;

    while (WChar(c + i, encoding)) {
        i += ucharLen(c + i, encoding);
        ret++;
    }
    return ret;
}

void WChar::affectAscii(char c) {
    int a = c;
    if (a >= 0)
        value = a;
    else
        value = CP437[a + 128];
}

uint WChar::affectUtf8(const char* c) {
    if (!(c[0] & 0x80)) {
        value = c[0];
        return 1;
    }

    if ((c[0] & 0xE0) == 0xC0) {
        value = ((c[0] & 0x1F) << 6) | (c[1] & 0x3F);
        if (value < 128) 
            value = 0;
        return 2;
    }

    if ((c[0] & 0xF0) == 0xE0) {
        value = ((c[0] & 0x0F) << 12) | ((c[1] & 0x3F) << 6) | (c[2] & 0x3F);

        if (value < 2048)
            value = 0;

        if (value >= 0xD800 && value <= 0xDFFF)
            value = 0;

        if (value >= 0xFFFE && value <= 0xFFFF) 
            value = 0;

        return 3;
    }

    if ((c[0] & 0xF8) == 0xF0) {
        value = ((c[0] & 0x0E) << 18) | ((c[1] & 0x3F) << 12) | ((c[2] & 0x3F) << 6) | (c[3] & 0x3F);

        if (value < 65536)
            value = 0;
        return 4;
    }

    value = 0;
    return 1;
}

uchar WChar::toAscii() {
    if (value < 128)
        return (char)value;

    for (uint i = 0; i < 128; i++) {
        if (CP437[i] == value)
            return (i + 128);
    }
    return '?';
}

WChar::ucharReprT WChar::toUtf8() {
    ucharReprT r;
    r.i = 0;
    
    if (value < 128)
        r.c[0] = value;
    else if (value < 4096) {
        r.c[0] = 0xC0 | ((value & 0x07C0) >> 6);
        r.c[1] = 0x80 | (value & 0x3F);
    } else if (value < 65536) {
        r.c[0] = 0xE0 | ((value & 0xF000) >> 12);
        r.c[1] = 0x80 | ((value & 0x0FC0) >> 6);
        r.c[2] = 0x80 | (value & 0x003F);
    } else {
        r.c[0] = 0xF0 | ((value & 0x1C0000) >> 18);
        r.c[1] = 0x80 | ((value & 0x3F000) >> 12);
        r.c[2] = 0x80 | ((value & 0x0FC0) >> 6);
        r.c[3] = 0x80 | (value & 0x003F);
    }
    return r;
}