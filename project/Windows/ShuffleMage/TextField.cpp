#include "TextField.h"
#include "VInput.h"
#include "Log.h"
#include <string.h>

#define DEFAULT_X_OFFSET 0.01f
#define DEFAULT_Y_OFFSET 0.04f

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

    memset(arKeyDown, 0, NUM_KEYS);

    m_nMaxSize    = DEFAULT_MAX_SIZE;
    m_nBufferSize = DEFAULT_MAX_SIZE;

    m_pTextString = new char[m_nBufferSize];
    memset(m_pTextString, 0, m_nBufferSize);

    // Disable select, sets the colors in quad/text
    SetSelect(0);

    // Enable the border for the quad
    m_quad.EnableBorder(1);
}

TextField::~TextField()
{

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

void TextField::Update(int nMouseDown,
                       float fX,
                       float fY)
{
    int i = 0;

    if (nMouseDown != 0)
    {
        if (m_rect.Contains(fX,fY))
        {
            SetSelect(1);
            ShowSoftKeyboard();
        }
        else
        {
            SetSelect(0);
        }
    }

    if (m_nSelected != 0)
    {
        // Check only a reasonable set of keys
        for (i = VKEY_0; i <= VKEY_Z; i++)
        {
            if (IsKeyDown(i))
            {
                // Make sure the key wasn't down last frame
                if (arKeyDown[i] == 0)
                {
                    // Otherwise, attempt to place a char into the textfield.
                    AddChar(ResolveChar(i));
                    arKeyDown[i] = 1;
                    m_text.SetText(m_pTextString);
                }
            }
            else
            {
                arKeyDown[i] = 0;
            }
        }

        // Check if backspace is pressed
        if (IsKeyDown(VKEY_BACKSPACE))
        {
            if (arKeyDown[VKEY_BACKSPACE] == 0)
            {
                RemoveChar();
                arKeyDown[VKEY_BACKSPACE] = 1;
                m_text.SetText(m_pTextString);
                return;
            }
        }
        else
        {
            arKeyDown[VKEY_BACKSPACE] = 0;
        }
    }
}

int TextField::GetSelected()
{
    return m_nSelected;
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

char TextField::ResolveChar(int nKey)
{
    char cRetChar = 0x00;

    // Letters
    if (nKey >= VKEY_A &&
        nKey <= VKEY_Z)
    {
        cRetChar = ((char) (nKey - VKEY_A)) + 'A';
    }
    else if (nKey >= VKEY_0 &&
             nKey <= VKEY_9)
    {
        cRetChar = ((char) (nKey - VKEY_0)) + '0';
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

void TextField::AddToScene(Scene& scene)
{
    scene.AddGlyph(&m_quad);
    scene.AddGlyph(&m_text);
}

void TextField::SetVisible(int nVisible)
{
    m_quad.SetVisible(nVisible);
    m_text.SetVisible(nVisible);
}

void TextField::SetTextScale(float fScaleX,
                             float fScaleY)
{
    m_text.SetScale(fScaleX, fScaleY);
}
