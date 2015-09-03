#include "BlurEffect.h"
#include "Log.h"
#include "VGL.h"
#include "VInput.h"
#include "Settings.h"

static float s_arPosition[8] = {-1.0f, -1.0f,
                                -1.0f,  1.0f,
                                 1.0f,  1.0f,
                                 1.0f, -1.0f};
static float s_arTexCoord[8] = {0.0f, 0.0f,
                                0.0f, 1.0f,
                                1.0f, 1.0f,
                                1.0f, 0.0f};
BlurEffect::BlurEffect()
{
    m_nType = EFFECT_BLUR;
    m_nBlurFactor = 4;
    m_nSampleDistance = 1;
}

BlurEffect::~BlurEffect()
{

}

void BlurEffect::Render(void*         pScene,
                        unsigned int  hFBO,
                        unsigned int  hColorAttach,
                        unsigned int  hDepthAttach)
{
    static int s_nTestFactor = 1;

    unsigned int hProg = GetShaderProgram(BLUR_EFFECT_PROGRAM);
    int hTexture        = -1;
    int hDimensions     = -1;
    int hBlurFactor     = -1;
    int hSampleDistance = -1;
    int hPosition       = -1;
    int hTexCoord       = -1;

    glUseProgram(hProg);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    hTexture        = glGetUniformLocation(hProg, "uTexture");
    hDimensions     = glGetUniformLocation(hProg, "uDimensions");
    hBlurFactor     = glGetUniformLocation(hProg, "uBlurFactor");
    hSampleDistance = glGetUniformLocation(hProg, "uSampleDistance");
    hPosition       = glGetAttribLocation(hProg, "aPosition");
    hTexCoord       = glGetAttribLocation(hProg, "aTexCoord");

    glBindTexture(GL_TEXTURE_2D, hColorAttach);
    glUniform1i(hTexture, 0);
    glUniform1i(hBlurFactor, 6);
    glUniform1i(hSampleDistance, m_nSampleDistance);
    glUniform2f(hDimensions,
                static_cast<float>(g_nScreenWidth),
                static_cast<float>(g_nScreenHeight));

    glEnableVertexAttribArray(hPosition);
    glEnableVertexAttribArray(hTexCoord);

    glVertexAttribPointer(hPosition,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          s_arPosition);
    glVertexAttribPointer(hTexCoord,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          s_arTexCoord);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void BlurEffect::SetBlurFactor(int nFactor)
{
    if (nFactor >= MIN_BLUR_FACTOR &&
        nFactor <= MAX_BLUR_FACTOR)
    {
        m_nBlurFactor = nFactor;
    }
    else
    {
        LogWarning("Invalid blur factor in BlurEffect::SetBlurFactor()");
    }   
}

void BlurEffect::SetSampleDistance(int nDistance)
{
    if (nDistance >= MIN_SAMPLE_DISTANCE &&
        nDistance <= MAX_SAMPLE_DISTANCE)
    {
        m_nSampleDistance = nDistance;
    }
    else
    {
        LogWarning("Invalid sample distance in BlurEffect::SetSampleDistance()");
    }
}
