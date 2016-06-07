#include "AssetBank.h"
#include "Log.h"
#include "VInput.h"
#include "Asset.h"
#include <Windows.h>

#define TOGGLE_BUTTON_HEIGHT 0.06f
#define TOGGLE_BUTTON_HORI_SPACE 0.13f
#define TOGGLE_BUTTON_TEXT_SCALE_X 0.4f
#define TOGGLE_BUTTON_TEXT_SCALE_Y 0.68f

#define BANK_ITEM_WIDTH 0.265f
#define BANK_ITEM_HEIGHT 0.05f

#define ACTION_BUTTON_WIDTH 0.048f
#define ACTION_BUTTON_HEIGHT 0.055f
#define ACTION_BUTTON_HORI_SPACE 0.08f
#define ACTION_BUTTON_TEXT_SCALE_X 0.5f
#define ACTION_BUTTON_TEXT_SCALE_Y 0.7f

AssetBank::AssetBank()
{
    m_rect.m_fX = -1.0f;
    m_rect.m_fY =  0.13f;
    m_rect.m_fWidth = WINDOW_WIDTH;
    m_rect.m_fHeight = 0.87f;

    m_pPrevHover = 0;

    m_nDispAsset = DISP_MESHES;
    m_nDispOffset = 0;
    m_pSelectedAsset = 0;
    m_nSelectedAsset = 0;
}

AssetBank::~AssetBank()
{
    // Delete the asset lists as all of the assets were allocated 
    // on the heap in the LoadMesh() LoadTexture() and LoadSound() methods
    ListNode* pNode = m_lMeshes.GetHead();
    while (pNode != 0)
    {
        delete reinterpret_cast<MeshAsset*>(pNode->m_pData);
        pNode->m_pData = 0;

        pNode = pNode->m_pNext;
    }

    pNode = m_lTextures.GetHead();
    while (pNode != 0)
    {
        delete reinterpret_cast<TextureAsset*>(pNode->m_pData);
        pNode->m_pData = 0;

        pNode = pNode->m_pNext;
    }

    pNode = m_lSounds.GetHead();
    while (pNode != 0)
    {
        delete reinterpret_cast<Asset*>(pNode->m_pData);
        pNode->m_pData = 0;

        pNode = pNode->m_pNext;
    }
}

void AssetBank::HandleInput()
{
    if (m_nVisible == 0)
    {
        // Do not handle input if window is not visible
        return;
    }

    float fX = 0.0f;
    float fY = 0.0f;
    GetPointerPositionNormalized(fX, fY);
    
    // Clear previous hovered button
    if (m_pPrevHover != 0)
    {
        ColorButtonOff(*m_pPrevHover);
    }

    if (IsKeyJustDown(VKEY_I) &&
        IsKeyDown(VKEY_CONTROL))
    {
        ImportAsset();
        UpdateView();
    }

    if (IsPointerDown())
    {
        Button* pDown = 0;

        if (m_btToggleMesh.IsTouched())
        {
            ColorButtonDown(m_btToggleMesh);
            pDown = &m_btToggleMesh;
        }

        if (m_btToggleTexture.IsTouched())
        {
            ColorButtonDown(m_btToggleTexture);
            pDown = &m_btToggleTexture;
        }

        if (m_btToggleSound.IsTouched())
        {
            ColorButtonDown(m_btToggleSound);
            pDown = &m_btToggleSound;
        }

        for (int i = 0; i < BANK_VISIBLE_ITEMS; i++)
        {
            if (m_arBankButtons[i].IsTouched())
            {
                ColorButtonDown(m_arBankButtons[i]);
                pDown = &m_arBankButtons[i];
            }
        }

        if (m_btAddAsset.IsTouched())
        {
            ColorButtonDown(m_btAddAsset);
            pDown = &m_btAddAsset;
        }

        if (m_btDeleteAsset.IsTouched())
        {
            ColorButtonDown(m_btDeleteAsset);
            pDown = &m_btDeleteAsset;
        }

        if (m_btCreateMatter.IsTouched())
        {
            ColorButtonDown(m_btCreateMatter);
            pDown = &m_btCreateMatter;
        }

        m_pPrevHover = pDown;
    }
    else
    {
        Button* pHover = 0;

        if (m_btToggleMesh.IsPointerHovering())
        {
            ColorButtonHover(m_btToggleMesh);
            pHover = &m_btToggleMesh;
        }

        if (m_btToggleTexture.IsPointerHovering())
        {
            ColorButtonHover(m_btToggleTexture);
            pHover = &m_btToggleTexture;
        }

        if (m_btToggleSound.IsPointerHovering())
        {
            ColorButtonHover(m_btToggleSound);
            pHover = &m_btToggleSound;
        }

        for (int i = 0; i < BANK_VISIBLE_ITEMS; i++)
        {
            if (m_arBankButtons[i].IsPointerHovering())
            {
                ColorButtonHover(m_arBankButtons[i]);
                pHover = &m_arBankButtons[i];
            }
        }

        if (m_btAddAsset.IsPointerHovering())
        {
            ColorButtonHover(m_btAddAsset);
            pHover = &m_btAddAsset;
        }

        if (m_btDeleteAsset.IsPointerHovering())
        {
            ColorButtonHover(m_btDeleteAsset);
            pHover = &m_btDeleteAsset;
        }

        if (m_btCreateMatter.IsPointerHovering())
        {
            ColorButtonHover(m_btCreateMatter);
            pHover = &m_btCreateMatter;
        }

        m_pPrevHover = pHover;
    }

    if (IsPointerJustUp())
    {
        ColorButtonOff(m_btToggleMesh);
        ColorButtonOff(m_btToggleTexture);
        ColorButtonOff(m_btToggleSound);

        if (m_btToggleMesh.IsPointerHovering())
        {
            m_nDispAsset = DISP_MESHES;
            m_nDispOffset = 0;
            UpdateView();
        }
        else if (m_btToggleTexture.IsPointerHovering())
        {
            m_nDispAsset = DISP_TEXTURES;
            m_nDispOffset = 0;
            UpdateView();
        }
        else if (m_btToggleSound.IsPointerHovering())
        {
            m_nDispAsset = DISP_SOUNDS;
            m_nDispOffset = 0;
            UpdateView();
        }
        else if (m_btAddAsset.IsPointerHovering())
        {
            ImportAsset();
            UpdateView();
        }
        else if (m_btDeleteAsset.IsPointerHovering())
        {
            
        }
        else if (m_btCreateMatter.IsPointerHovering())
        {
        
        }
        else
        {
            // Check each of the asset bank items
            for (int i = 0; i < BANK_VISIBLE_ITEMS; i++)
            {
                if(m_arBankButtons[i].IsVisible() &&
                   m_arBankButtons[i].IsPointerHovering())
                {
                    // First, figure out which asset this button refers to
                    List* pList = 0;
                    Asset* pSelAsset = 0;

                    switch(m_nDispAsset)
                    {
                    case DISP_MESHES:
                        pList = &m_lMeshes;
                        break;
                    case DISP_TEXTURES:
                        pList = &m_lTextures;
                        break;
                    case DISP_SOUNDS:
                        pList = &m_lSounds;
                        break;
                    default:
                        break;
                    }

                    if (pList != 0)
                    {
                        ListNode* pNode = pList->Get(i + m_nDispOffset);
                        if (pNode != 0)
                        {
                            pSelAsset = reinterpret_cast<Asset*>(pNode->m_pData);
                        }
                        else
                        {
                            // Node doesn't exist. Shouldn't reach this ever...
                            break;
                        }
                    }

                    // Second, check if the new selected asset is equivalent to
                    // the old selected asset.
                    if (m_pSelectedAsset == pSelAsset)
                    {
                        // pEditor->SetDetailedAsset().
                    }
                    else
                    {
                        // Un-down the previous selected item button
                        ColorButtonOff(m_arBankButtons[m_nSelectedAsset]);

                        m_pSelectedAsset = pSelAsset;
                        m_nSelectedAsset = i;
                    }
                }
            }
        }
    }

    // Check mouse wheel
    if (m_rect.Contains(fX, fY) &&
        GetScrollWheelDelta() != 0)
    {
        // Color the previously selected button off
        if (m_nSelectedAsset - m_nDispOffset >= 0 &&
            m_nSelectedAsset - m_nDispOffset < BANK_VISIBLE_ITEMS)
            ColorButtonOff(m_arBankButtons[m_nSelectedAsset - m_nDispOffset]);

        m_nDispOffset += -1 * GetScrollWheelDelta();

        List* pList = 0;

        switch(m_nDispAsset)
        {
        case DISP_MESHES:
            pList = &m_lMeshes;
            break;
        case DISP_TEXTURES:
            pList = &m_lTextures;
            break;
        case DISP_SOUNDS:
            pList = &m_lSounds;
            break;
        default:
            break;
        }

        if (m_nDispOffset >= pList->Count())
        {
            m_nDispOffset = pList->Count() - 1;
        }

        if (m_nDispOffset < 0)
        {
            m_nDispOffset = 0;
        }

        UpdateView();
    }

    // Always highlight the selected asset type
    if (m_nDispAsset == DISP_MESHES)
        ColorButtonDown(m_btToggleMesh);
    if (m_nDispAsset == DISP_TEXTURES)
        ColorButtonDown(m_btToggleTexture);
    if (m_nDispAsset == DISP_SOUNDS)
        ColorButtonDown(m_btToggleSound);
    // Always highlight the selected bank item
    if (m_nSelectedAsset - m_nDispOffset >= 0 &&
        m_nSelectedAsset - m_nDispOffset < BANK_VISIBLE_ITEMS)
        ColorButtonDown(m_arBankButtons[m_nSelectedAsset - m_nDispOffset]);
}

void AssetBank::UpdateView()
{
    Window::UpdateView();

    float fX = m_rect.m_fX + m_fPadding;
    float fY = m_rect.m_fY + m_rect.m_fHeight - HEADER_HEIGHT - 0.1f;

    // Toggle Buttons
    m_btToggleMesh.SetRect(fX,
                           fY,
                           0.085f,
                           TOGGLE_BUTTON_HEIGHT);

    m_btToggleTexture.SetRect(fX + 0.097f,
                              fY,
                              0.120f,
                              TOGGLE_BUTTON_HEIGHT);
    
    m_btToggleSound.SetRect(fX + 0.228f,
                            fY,
                            0.095f,
                            TOGGLE_BUTTON_HEIGHT);

    // Bank Item Buttons
    fX = m_rect.m_fX + m_fPadding + 0.03f;
    fY -= m_fSpacing;

    List* pList = 0;
    switch (m_nDispAsset)
    {
    case DISP_MESHES:
        pList = &m_lMeshes;
        break;
    case DISP_TEXTURES:
        pList = &m_lTextures;
        break;
    case DISP_SOUNDS:
        pList = &m_lSounds;
        break;
    default:
        pList = &m_lMeshes;
        break;
    }

    ListNode* pNode = pList->Get(m_nDispOffset);
    for (int i = 0; i < BANK_VISIBLE_ITEMS; i++)
    {
        m_arBankButtons[i].SetRect(fX,
                                   fY,
                                   BANK_ITEM_WIDTH,
                                   BANK_ITEM_HEIGHT);

        if (pNode != 0)
        {
            m_arBankButtons[i].SetVisible(1);
            m_arBankButtons[i].SetTextString(reinterpret_cast<Asset*>(pNode->m_pData)->m_arName);

            pNode = pNode->m_pNext;
        }
        else
        {
            m_arBankButtons[i].SetVisible(0);
        }

        fY -= m_fSpacing;
    }

    // Action Buttons
    fX = m_rect.m_fX + m_fPadding + 0.06f;
    fY -= 0.004f;

    m_btAddAsset.SetRect(fX,
                         fY,
                         ACTION_BUTTON_WIDTH,
                         ACTION_BUTTON_HEIGHT);

    m_btDeleteAsset.SetRect(fX + ACTION_BUTTON_HORI_SPACE,
                            fY,
                            ACTION_BUTTON_WIDTH,
                            ACTION_BUTTON_HEIGHT);

    m_btCreateMatter.SetRect(fX + 2*ACTION_BUTTON_HORI_SPACE,
                             fY,
                             ACTION_BUTTON_WIDTH,
                             ACTION_BUTTON_HEIGHT);
    fY -= m_fSpacing;
}

void AssetBank::InitializeView()
{
    Window::InitializeView();

    m_textHeader.SetText("Asset Bank");

    // Setup Buttons
    ColorButtonOff(m_btToggleMesh);
    ColorButtonOff(m_btToggleTexture);
    ColorButtonOff(m_btToggleSound);
    ColorButtonOff(m_btAddAsset);
    ColorButtonOff(m_btDeleteAsset);
    ColorButtonOff(m_btCreateMatter);

    m_btToggleMesh.SetTextString("Mesh");
    m_btToggleTexture.SetTextString("Texture");
    m_btToggleSound.SetTextString("Sound");
    m_btAddAsset.SetTextString("+");
    m_btDeleteAsset.SetTextString("-");
    m_btCreateMatter.SetTextString(">");

    m_btToggleTexture.SetTextOffsetX(0.005f);
    m_btAddAsset.SetTextOffsetX(0.01f);
    m_btDeleteAsset.SetTextOffsetX(0.01f);
    m_btCreateMatter.SetTextOffsetX(0.01f);

    m_btToggleMesh.SetTextScale(TOGGLE_BUTTON_TEXT_SCALE_X, TOGGLE_BUTTON_TEXT_SCALE_Y);
    m_btToggleTexture.SetTextScale(TOGGLE_BUTTON_TEXT_SCALE_X, TOGGLE_BUTTON_TEXT_SCALE_Y);
    m_btToggleSound.SetTextScale(TOGGLE_BUTTON_TEXT_SCALE_X, TOGGLE_BUTTON_TEXT_SCALE_Y);

    m_btAddAsset.SetTextScale(ACTION_BUTTON_TEXT_SCALE_X, ACTION_BUTTON_TEXT_SCALE_Y);
    m_btDeleteAsset.SetTextScale(ACTION_BUTTON_TEXT_SCALE_X, ACTION_BUTTON_TEXT_SCALE_Y);
    m_btCreateMatter.SetTextScale(ACTION_BUTTON_TEXT_SCALE_X, ACTION_BUTTON_TEXT_SCALE_Y);

    for (int i = 0; i < BANK_VISIBLE_ITEMS; i++)
    {
        ColorButtonOff(m_arBankButtons[i]);
        m_arBankButtons[i].SetTextString("Bank Item");
    }
}

void AssetBank::RegisterScene(Scene* pScene)
{
    if (pScene == 0)
        return;

    Window::RegisterScene(pScene);
        
    m_btToggleMesh.AddToScene(*pScene);
    m_btToggleTexture.AddToScene(*pScene);
    m_btToggleSound.AddToScene(*pScene);
    m_btAddAsset.AddToScene(*pScene);
    m_btDeleteAsset.AddToScene(*pScene);
    m_btCreateMatter.AddToScene(*pScene);

    for (int i = 0; i < BANK_VISIBLE_ITEMS; i++)
    {
        m_arBankButtons[i].AddToScene(*pScene);
    }
}

void AssetBank::ImportAsset()
{
    // Cryptic Win32 code borrowed from MSDN website (and slightly modified)
    OPENFILENAME ofn;       // common dialog box structure
    char szFile[260];       // buffer for file name
    HWND hwnd = GetActiveWindow();              // owner window
    HANDLE hf;              // file handle
    char* pStr = 0;

    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    // Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
    // use the contents of szFile to initialize itself.
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "All\0*.*\0Mesh\0*.obj\0Texture\0*.bmp\0Sound\0*.wav\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // Display the Open dialog box. 
    if (GetOpenFileName(&ofn) != 0) 
    {
        pStr = ofn.lpstrFile;

        // Check file extension.
        ofn.lpstrFile += strlen(ofn.lpstrFile);
        ofn.lpstrFile -= 3;
        
        // Load texture
        if (strcmp(ofn.lpstrFile, "bmp") == 0 ||
            strcmp(ofn.lpstrFile, "BMP") == 0)
        {
            LoadTexture(pStr);
        }

        // Load mesh
        if (strcmp(ofn.lpstrFile, "obj") == 0 ||
            strcmp(ofn.lpstrFile, "OBJ") == 0)
        {
            LoadMesh(pStr);
        }

        // Load sound
        if (strcmp(ofn.lpstrFile, "wav") == 0 ||
            strcmp(ofn.lpstrFile, "WAV") == 0)
        {
            LoadSound(pStr);
        }
    }
}


void AssetBank::LoadMesh(char* pStr)
{
    MeshAsset* pMeshAsset = new MeshAsset();
    pMeshAsset->m_pMesh = new StaticMesh();
    pMeshAsset->m_pMesh->Load(pStr);
    pMeshAsset->SetNameFromFile(pStr);
    pMeshAsset->SetFilePath(pStr);

    m_lMeshes.Add(pMeshAsset);
}

void AssetBank::LoadTexture(char* pStr)
{
    TextureAsset* pTextureAsset = new TextureAsset();
    pTextureAsset->m_pTexture = new Texture();
    pTextureAsset->m_pTexture->LoadBMP(pStr, 1);
    pTextureAsset->SetNameFromFile(pStr);
    pTextureAsset->SetFilePath(pStr);

    m_lTextures.Add(pTextureAsset);
}

void AssetBank::LoadSound(char* pStr)
{

}
