#include "Toast.h"
#include "Timer.h"
#include "VGL.h"
#include <string.h>
#include "Quad.h"
#include "Text.h"
#include "ResourceLibrary.h"

#define TOAST_MAX_LENGTH 32
#define TOAST_DURATION 4.0f
#define TOAST_FADE     3.0f

#define TEXT_SCALE_X 0.7f
#define TEXT_SCALE_Y 0.8f
#define CHAR_DIM 0.05f

#define TOAST_X -0.25f
#define TOAST_Y 0.75f
#define TOAST_WIDTH 0.5f
#define TOAST_HEIGHT 0.15f

#define TEXT_OFFSET_X 0.04f
#define TEXT_OFFSET_Y 0.05f

static int s_nToastActive = 0;
static Timer s_timer;

static float s_arBackColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
static float s_arTextColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
static char s_arText[TOAST_MAX_LENGTH + 1] = {0};

static Quad* s_pQuad;
static Text* s_pText;

void Toast(const char* pText)
{
    int nTextLength = 0;

    if (s_pQuad == 0 &&
        s_pText == 0)
    {
        s_pQuad = new Quad();
        s_pText = new Text();
    }

    memcpy(s_arText,pText, TOAST_MAX_LENGTH);
    s_pText->SetText(s_arText);
    s_nToastActive = 1;
    s_timer.Start();

    nTextLength = strlen(s_arText);

    s_pQuad->SetBox(-(TEXT_OFFSET_X + nTextLength/2 * TEXT_SCALE_X * CHAR_DIM), 
                    TOAST_Y,
                    nTextLength*TEXT_SCALE_X*CHAR_DIM + 2*TEXT_OFFSET_X, TOAST_HEIGHT);
    s_pText->SetPosition(-(nTextLength/2 * TEXT_SCALE_X * CHAR_DIM),
                         TOAST_Y + TEXT_OFFSET_Y);
    s_pText->SetScale(TEXT_SCALE_X, TEXT_SCALE_Y);
    
}

void RenderToast()
{
    float fTime = 0.0f;

    if (s_nToastActive == 0)
    {
        return;
    }

    if (s_pQuad == 0 ||
        s_pText == 0)
    {
        return;
    }

    s_timer.Stop();
    fTime = s_timer.Time();

    if (fTime >= TOAST_DURATION)
    {
        s_nToastActive = 0;
        return;
    }
    
    if (fTime > TOAST_FADE)
    {
        s_arBackColor[3] = (TOAST_DURATION - fTime)/(TOAST_DURATION - TOAST_FADE);
        s_arTextColor[3] = (TOAST_DURATION - fTime)/(TOAST_DURATION - TOAST_FADE);
    }
    else
    {
        s_arBackColor[3] = 1.0f;
        s_arTextColor[3] = 1.0f;
    }

    s_pQuad->SetColor(s_arBackColor[0],
                      s_arBackColor[1],
                      s_arBackColor[2],
                      s_arBackColor[3]);
    s_pText->SetColor(s_arTextColor[0],
                      s_arTextColor[1],
                      s_arTextColor[2],
                      s_arTextColor[3]);

    s_pQuad->Render();
    s_pText->Render();
}