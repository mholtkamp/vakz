#ifndef TEXT_FIELD_H
#define TEXT_FIELD_H

#include "Quad.h"
#include "Text.h"
#include "Rect.h"
#include "Scene.h"

class TextField
{
public:

    enum TextFieldEnum
    {
        NUM_KEYS         = 128,
        DEFAULT_MAX_SIZE = 32
    };

    TextField();

    ~TextField();

    void SetBackColor(float fRed,
                      float fGreen,
                      float fBlue,
                      float fAlpha);

    void SetBorderColor(float fRed,
                        float fGreen,
                        float fBlue,
                        float fAlpha);

    void SetTextColor(float fRed,
                      float fGreen,
                      float fBlue,
                      float fAlpha);

    void SetSelectBackColor(float fRed,
                            float fGreen,
                            float fBlue,
                            float fAlpha);

    void SetSelectBorderColor(float fRed,
                              float fGreen,
                              float fBlue,
                              float fAlpha);

    void SetSelectTextColor(float fRed,
                            float fGreen,
                            float fBlue,
                            float fAlpha);

    void SetRect(float fX,
                 float fY,
                 float fWidth,
                 float fHeight);

    void SetSelect(int nSelect);

    void Update(int   nMouseDown,
                float fX,
                float fY);

    int GetSelected();

    char* GetText();

    void SetMaxSize(int nMaxSize);

    void AddToScene(Scene& scene);

    void SetVisible(int nVisible);

    void SetTextScale(float fScaleX,
                      float fScaleY);

    void ClearText();
    

private:

    void AddChar(char c);

    void RemoveChar();

    char ResolveChar(int nKey, int nShift);

    Quad m_quad;
    Text m_text;
    Rect m_rect;

    int m_nSelected;

    float m_arBackColor[4];
    float m_arBorderColor[4];
    float m_arTextColor[4];

    float m_arSelBackColor[4];
    float m_arSelBorderColor[4];
    float m_arSelTextColor[4];

    float m_fTextOffsetX;
    float m_fTextOffsetY;

    char arKeyDown[NUM_KEYS];

    int m_nBufferSize;
    int m_nMaxSize;

    char* m_pTextString;

    
};

#endif