#include "TextField.h"
#include "VInput.h"
#include "Log.h"
#include <string.h>
#include <stdio.h>

#define DEFAULT_X_OFFSET 0.003f
#define DEFAULT_Y_OFFSET 0.01f

TextField::TextField()
{
    m_nSelected = 0;

    // Set default colors
    m_arBackColor[0] = 0.2f;
    m_arBackColor[1] = 0.1f;
    m_arBackColor[2] = 0.1f;
    m_arBackColor[3] = 1.0f;

    m_arBorderColor[0] = 0.7f;
    m_arBorderColor[1] = 0.3f;
    m_arBorderColor[2] = 0.3f;
    m_arBorderColor[3] = 1.0f;

    m_arTextColor[0] = 0.9f;
    m_arTextColor[1] = 0.9f;
    m_arTextColor[2] = 0.9f;
    m_arTextColor[3] = 1.0f;
    
    m_arSelBackColor[0] = 0.2f;
    m_arSelBackColor[1] = 0.1f;
    m_arSelBackColor[2] = 0.1f;
    m_arSelBackColor[3] = 1.0f;

    m_arSelBorderColor[0] = 1.0f;
    m_arSelBorderColor[1] = 1.0f;
    m_arSelBorderColor[2] = 1.0f;
    m_arSelBorderColor[3] = 1.0f;

    m_arSelTextColor[0] = 1.0f;
    m_arSelTextColor[1] = 1.0f;
    m_arSelTextColor[2] = 1.0f;
    m_arSelTextColor[3] = 1.0f;

    m_fTextOffsetX = DEFAULT_X_OFFSET;
    m_fTextOffsetY = DEFAULT_Y_OFFSET;

    m_nMaxSize    = DEFAULT_MAX_SIZE;
    m_nBufferSize = DEFAULT_MAX_SIZE;

    m_pTextString = new char[m_nBufferSize];
    memset(m_pTextString, 0, m_nBufferSize);

    // Disable select, sets the colors in quad/text
    SetSelect(0);

    // Enable the border for the quad
    m_quad.EnableBorder(1);

    m_nVisible = 1;
    m_pNextField = 0;
    m_nClearOnSelect = 1;
}

TextField::~TextField()
{
    if (m_pTextString != 0)
    {
        delete m_pTextString;
        m_pTextString = 0;
    }
}

void TextField::SetBackColor(float fRed,
                             float fGreen,
                             float fBlue,
                             float fAlpha)
{
    m_arBackColor[0] = fRed;
    m_arBackColor[1] = fGreen;
    m_arBackColor[2] = fBlue;
    m_arBackColor[3] = fAlpha;

    if (m_nSelected == 0)
    {
        m_quad.SetColor(m_arBackColor);
    }
}

void TextField::SetBorderColor(float fRed,
                               float fGreen,
                               float fBlue,
                               float fAlpha)
{
    m_arBorderColor[0] = fRed;
    m_arBorderColor[1] = fGreen;
    m_arBorderColor[2] = fBlue;
    m_arBorderColor[3] = fAlpha;

    if (m_nSelected == 0)
    {
        m_quad.SetBorderColor(m_arBorderColor[0],
                              m_arBorderColor[1],
                              m_arBorderColor[2],
                              m_arBorderColor[3]);
    }
}

void TextField::SetTextColor(float fRed,
                             float fGreen,
                             float fBlue,
                             float fAlpha)
{
    m_arTextColor[0] = fRed;
    m_arTextColor[1] = fGreen;
    m_arTextColor[2] = fBlue;
    m_arTextColor[3] = fAlpha;

    if (m_nSelected == 0)
    {
        m_text.SetColor(m_arTextColor);
    }
}

void TextField::SetSelectBackColor(float fRed,
                                   float fGreen,
                                   float fBlue,
                                   float fAlpha)
{
    m_arSelBackColor[0] = fRed;
    m_arSelBackColor[1] = fGreen;
    m_arSelBackColor[2] = fBlue;
    m_arSelBackColor[3] = fAlpha;

    if (m_nSelected != 0)
    {
        m_quad.SetColor(m_arSelBackColor);
    }
}

void TextField::SetSelectBorderColor(float fRed,
                                     float fGreen,
                                     float fBlue,
                                     float fAlpha)
{
    m_arSelBorderColor[0] = fRed;
    m_arSelBorderColor[1] = fGreen;
    m_arSelBorderColor[2] = fBlue;
    m_arSelBorderColor[3] = fAlpha;

    if (m_nSelected != 0)
    {
        m_quad.SetBorderColor(m_arSelBorderColor[0],
                              m_arSelBorderColor[1],
                              m_arSelBorderColor[2],
                              m_arSelBorderColor[3]);
    }
}


void TextField::SetSelectTextColor(float fRed,
                                   float fGreen,
                                   float fBlue,
                                   float fAlpha)
{
    m_arSelTextColor[0] = fRed;
    m_arSelTextColor[1] = fGreen;
    m_arSelTextColor[2] = fBlue;
    m_arSelTextColor[3] = fAlpha;

    if (m_nSelected != 0)
    {
        m_text.SetColor(m_arSelTextColor);
    }
}

void TextField::SetRect(float fX,
                        float fY,
                        float fWidth,
                        float fHeight)
{
    m_quad.SetBox(fX,
                  fY,
                  fWidth,
                  fHeight);
    m_text.SetPosition(fX + m_fTextOffsetX,
                       fY + m_fTextOffsetY);
    
    m_rect.m_fX      = fX;
    m_rect.m_fY      = fY;
    m_rect.m_fWidth  = fWidth;
    m_rect.m_fHeight = fHeight;
}

void TextField::SetSelect(int nSelect)
{
    m_nSelected = nSelect;

    if (m_nSelected != 0)
    {
        m_quad.SetColor(m_arSelBackColor[0],
                        m_arSelBackColor[1],
                        m_arSelBackColor[2],
                        m_arSelBackColor[3]);
        m_quad.SetBorderColor(m_arSelBorderColor[0],
                              m_arSelBorderColor[1],
                              m_arSelBorderColor[2],
                              m_arSelBorderColor[3]);
        m_text.SetColor(m_arSelTextColor[0],
                        m_arSelTextColor[1],
                        m_arSelTextColor[2],
                        m_arSelTextColor[3]);

        if (m_nClearOnSelect != 0)
        {
            SetText("");
        }
    }
    else
    {
        m_quad.SetColor(m_arBackColor[0],
                        m_arBackColor[1],
                        m_arBackColor[2],
                        m_arBackColor[3]);
        m_quad.SetBorderColor(m_arBorderColor[0],
                              m_arBorderColor[1],
                              m_arBorderColor[2],
                              m_arBorderColor[3]);
        m_text.SetColor(m_arTextColor[0],
                        m_arTextColor[1],
                        m_arTextColor[2],
                        m_arTextColor[3]);
    }
}

int TextField::IsSelected()
{
    return m_nSelected;
}

void TextField::ExtractInt(int& nInt)
{
    if(m_nSelected)
    {
        nInt = atoi(m_pTextString);
    }
}

void TextField::ExtractFloat(float& fFloat)
{
    if (m_nSelected)
    {
        fFloat = (float) atof(m_pTextString);
    }
}

void TextField::ExtractString(char* pString, int nSize)
{
    if (m_nSelected)
    {
        strncpy(pString, m_pTextString, nSize);
    }
}


void TextField::Update(int nMouseDown,
                       float fX,
                       float fY)
{
    int i = 0;
    int nShift = 0;

    if (m_nVisible == 0)
        return;

    if (nMouseDown != 0)
    {
        if (m_rect.Contains(fX,fY))
        {
            SetSelect(1);
#if defined(ANDROID)
            ShowSoftKeyboard();
#endif
        }
        else
        {
            SetSelect(0);
        }
    }

    if (IsKeyDown(VKEY_SHIFT))
    {
        nShift = 1;
    }

    if (m_nSelected != 0)
    {
        // Check all keys :x
        for (i = 0; i < VINPUT_MAX_KEYS; i++)
        {
            if (IsKeyJustDownRepeat(i))
            {
                    // Otherwise, attempt to place a char into the textfield.
                    AddChar(ResolveChar(i, nShift));
                    m_text.SetText(m_pTextString);
            }
        }

        // Check if backspace is pressed
        if (IsKeyJustDownRepeat(VKEY_BACKSPACE))
        {
            if (IsKeyDown(VKEY_CONTROL))
            {
                memset(m_pTextString, 0, m_nBufferSize);
            }
            else
            {
                RemoveChar();
            }

            m_text.SetText(m_pTextString);
            return;
        }
    }
}

void TextField::Update(int nMouseDown,
                       float fX,
                       float fY,
                       TextField*& pSelected)
{
    Update(nMouseDown, fX, fY);

    if (m_nSelected != 0)
    {
        pSelected = this;
    }
}

int TextField::GetSelected()
{
    return m_nSelected;
}

void TextField::ClearText()
{
    memset(m_pTextString, 0, m_nBufferSize);
    m_text.SetText(m_pTextString);
}

char* TextField::GetText()
{
    return m_pTextString;
}

void TextField::AddChar(char c)
{
    int nStrSize = strlen(m_pTextString);

    if (c == 0)
    {
        return;
    }
    if (nStrSize < m_nMaxSize - 1)
    {
        m_pTextString[nStrSize] = c;
    }
}

void TextField::RemoveChar()
{
    int nStrSize = strlen(m_pTextString);

    if (nStrSize > 0)
    {
        m_pTextString[nStrSize - 1] = 0x00;
    }
}

char TextField::ResolveChar(int nKey, int nShift)
{
    char cRetChar = 0x00;

    // Letters
    if (nKey >= VKEY_A &&
        nKey <= VKEY_Z)
    {
        if (nShift == 0)
        {
            cRetChar = ((char) (nKey - VKEY_A)) + 'a';
        }
        else
        {
            cRetChar = ((char) (nKey - VKEY_A)) + 'A';
        }
    }
    else if (nKey >= VKEY_0 &&
             nKey <= VKEY_9)
    {
        if (nShift == 0)
        {
            cRetChar = ((char) (nKey - VKEY_0)) + '0';
        }
        else
        {
            // Add symbols
            switch(nKey)
            {
            case VKEY_0:
                cRetChar = ')';
                break;
            case VKEY_1:
                cRetChar = '!';
                break;
            case VKEY_2:
                cRetChar = '@';
                break;
            case VKEY_3:
                cRetChar = '#';
                break;
            case VKEY_4:
                cRetChar = '$';
                break;
            case VKEY_5:
                cRetChar = '%';
                break;
            case VKEY_6:
                cRetChar = '^';
                break;
            case VKEY_7:
                cRetChar = '&';
                break;
            case VKEY_8:
                cRetChar = '*';
                break;
            case VKEY_9:
                cRetChar = '(';
                break;
            default:
                break;
            }
        }
    }
    else if (nKey >= VKEY_NUMPAD0 &&
             nKey <= VKEY_NUMPAD9)
    {
        cRetChar = ((char) (nKey - VKEY_NUMPAD0)) + '0';
    }
    else if (nKey == VKEY_SPACE)
    {
        cRetChar = ' ';
    }
    else if (nShift != 0)
    {
        // Special Chars with Shift pressed
        switch(nKey)
        {
        case VKEY_PERIOD:
            cRetChar = '>';
            break;
        case VKEY_COMMA:
            cRetChar = '<';
            break;
        case VKEY_PLUS:
            cRetChar = '+';
            break;
        case VKEY_MINUS:
            cRetChar = '_';
            break;

        case VKEY_COLON:
            cRetChar = ':';
            break;
        case VKEY_QUESTION:
            cRetChar = '?';
            break;
        case VKEY_SQUIGGLE:
            cRetChar = '~';
            break;
        case VKEY_LEFT_BRACKET:
            cRetChar = '{';
            break;
        case VKEY_BACK_SLASH:
            cRetChar = '|';
            break;
        case VKEY_RIGHT_BRACKET:
            cRetChar = '}';
            break;
        case VKEY_QUOTE:
            cRetChar = '"';
            break;
        default:
            break;
        }
    }
    else
    {
        // Special Chars without Shift pressed
        switch(nKey)
        {
        case VKEY_PERIOD:
            cRetChar = '.';
            break;
        case VKEY_COMMA:
            cRetChar = ',';
            break;
        case VKEY_PLUS:
            cRetChar = '=';
            break;
        case VKEY_MINUS:
            cRetChar = '-';
            break;

        case VKEY_COLON:
            cRetChar = ';';
            break;
        case VKEY_QUESTION:
            cRetChar = '/';
            break;
        case VKEY_SQUIGGLE:
            cRetChar = '\'';
            break;
        case VKEY_LEFT_BRACKET:
            cRetChar = '[';
            break;
        case VKEY_BACK_SLASH:
            cRetChar = '\\';
            break;
        case VKEY_RIGHT_BRACKET:
            cRetChar = ']';
            break;
        case VKEY_QUOTE:
            cRetChar = '\'';
            break;
        default:
            break;
        }
    }

    return cRetChar;
}

void TextField::SetMaxSize(int nMaxSize)
{
    char* pNewBuffer;

    if (nMaxSize > m_nBufferSize)
    {
        // We need to allocate a new buffer that can fit
        // the new max size of the text string, but we need
        // to preserve the contents of the text string.
        pNewBuffer = new char[nMaxSize];
        memset(pNewBuffer, 0, nMaxSize);
        memcpy(pNewBuffer, m_pTextString, m_nMaxSize);
        
        // Destroy the old text buffer and point the new one
        // that was just created.
        delete [] m_pTextString;
        m_pTextString = pNewBuffer;

        m_nBufferSize = nMaxSize;
    }
    
    m_nMaxSize = nMaxSize;
}

void TextField::SetText(char* pStr)
{
    strncpy(m_pTextString, pStr, m_nMaxSize);
    m_text.SetText(m_pTextString);
}

void TextField::SetText_Float(float fFloat)
{
    memset(m_pTextString, 0, m_nMaxSize);
    _snprintf(m_pTextString, m_nMaxSize-1, "%f", fFloat);
    m_text.SetText(m_pTextString);
}

void TextField::SetText_Float(float fFloat, int nPlaces)
{
    memset(m_pTextString, 0, m_nMaxSize);
    _snprintf(m_pTextString, m_nMaxSize-1, "%.*f", nPlaces, fFloat);
    m_text.SetText(m_pTextString);
}

void TextField::SetText_Int(int nInt)
{
    memset(m_pTextString, 0, m_nMaxSize);
    _snprintf(m_pTextString, m_nMaxSize-1, "%d", nInt);
    m_text.SetText(m_pTextString);
}

void TextField::AddToScene(Scene& scene)
{
    scene.AddGlyph(&m_quad);
    scene.AddGlyph(&m_text);
}

void TextField::SetVisible(int nVisible)
{
    m_quad.SetVisible(nVisible);
    m_text.SetVisible(nVisible);
    
    m_nVisible = nVisible;
}

void TextField::SetTextScale(float fScaleX,
                             float fScaleY)
{
    m_text.SetScale(fScaleX, fScaleY);
}

void TextField::SetBackColor(const float* arColor)
{
    memcpy(m_arBackColor, arColor, 4 * sizeof(float));

    if (m_nSelected == 0)
    {
        m_quad.SetColor(m_arBackColor);
    }
}

void TextField::SetBorderColor(const float* arColor)
{
    memcpy(m_arBorderColor, arColor, 4 * sizeof(float));

    if (m_nSelected == 0)
    {
        m_quad.SetBorderColor(m_arBorderColor[0],
                              m_arBorderColor[1],
                              m_arBorderColor[2],
                              m_arBorderColor[3]);
    }
}

void TextField::SetTextColor(const float* arColor)
{
    memcpy(m_arTextColor, arColor, 4 * sizeof(float));

    if (m_nSelected == 0)
    {
        m_text.SetColor(m_arTextColor);
    }
}

void TextField::SetSelectBackColor(const float* arColor)
{
    memcpy(m_arSelBackColor, arColor, 4 * sizeof(float));

    if (m_nSelected != 0)
    {
        m_quad.SetColor(m_arSelBackColor);
    }
}

void TextField::SetSelectBorderColor(const float* arColor)
{
    memcpy(m_arSelBorderColor, arColor, 4 * sizeof(float));

    if (m_nSelected != 0)
    {
        m_quad.SetBorderColor(m_arSelBorderColor[0],
                              m_arSelBorderColor[1],
                              m_arSelBorderColor[2],
                              m_arSelBorderColor[3]);
    }
}
void TextField::SetSelectTextColor(const float* arColor)
{
    memcpy(m_arSelTextColor, arColor, 4 * sizeof(float));

    if (m_nSelected != 0)
    {
        m_text.SetColor(m_arSelTextColor);
    }
}

void TextField::SetNextField(TextField* pNext)
{
    m_pNextField = pNext;    
}

void TextField::Tab()
{
    if (m_pNextField != 0)
    {
        // There is a next field so we can tab.
        m_pNextField->SetSelect(1);
    }
    
    SetSelect(0);
}
