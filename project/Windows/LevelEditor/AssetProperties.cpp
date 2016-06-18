#include "AssetProperties.h"
#include "VInput.h"
#include "DiffuseMaterial.h"
#include "FullbrightMaterial.h"
#include "RimlitMaterial.h"
#include "ToonMaterial.h"
#include "LevelEditor.h"
#include "OrientedBoxCollider.h"

#define TF_SCALE_X 0.35f
#define TF_SCALE_Y 0.64f

#define T_SCALE_X 0.35f
#define T_SCALE_Y 0.64f

#define BT_SCALE_X 0.35f
#define BT_SCALE_Y 0.64f

#define TEXT_COLOR s_arColor2

#define BUTTON_HEIGHT 0.055f
#define BUTTON_OFF_Y 0.008f

AssetProperties::AssetProperties()
{
    m_rect.m_fX = -1.0f;
    m_rect.m_fY = -1.0f;
    m_rect.m_fWidth = WINDOW_WIDTH;
    m_rect.m_fHeight = 1.13f;

    m_pAsset = 0;
    m_pPrevHover = 0;
    m_nCurCollider = 0;
}

AssetProperties::~AssetProperties()
{

}

void AssetProperties::HandleInput()
{
    if (m_nVisible == 0 ||
        m_pAsset   == 0)
    {
        return;
    }

    if (m_pPrevHover != 0)
    {
        ColorButtonOff(*m_pPrevHover);
    }

    switch (m_pAsset->GetType())
    {
    case ASSET_MESH:
        HandleInput_Mesh();
        break;
    case ASSET_TEXTURE:
        HandleInput_Texture();
        break;
    case ASSET_SOUND:
        HandleInput_Sound();
        break;
    default:
        return;
    }
}

void AssetProperties::HandleInput_Mesh()
{
    float fX = 0.0f;
    float fY = 0.0f;
    GetPointerPositionNormalized(fX,fY);

    MeshAsset* pAsset = reinterpret_cast<MeshAsset*>(m_pAsset);

    if (IsKeyDown(VKEY_ENTER)||
        IsPointerJustUp())
    {
        if (pAsset->m_pMaterial->GetType() == MATERIAL_TOON &&
            m_tfIntervals.IsSelected())
        {
            reinterpret_cast<ToonMaterial*>(pAsset->m_pMaterial)->SetLightingIntervalCount(atoi(m_tfIntervals.GetText()));
            UpdateView();
        }

        if (pAsset->m_pMaterial->GetType() == MATERIAL_RIMLIT &&
            m_tfRimSize.IsSelected())
        {
            reinterpret_cast<RimlitMaterial*>(pAsset->m_pMaterial)->SetRimSize((float) atof(m_tfRimSize.GetText()));
            UpdateView();
        }

        for (int i = 0; i < 4; i++)
        {
            if (m_tfColor[i].IsSelected())
            {
                float arNewColor[4];
                memcpy(arNewColor, pAsset->m_pMaterial->GetColor(), sizeof(float) * 4);
                arNewColor[i] = (float) atof(m_tfColor[i].GetText());
                pAsset->m_pMaterial->SetColor(arNewColor);
                UpdateView();
            }
            
            if (pAsset->m_pMaterial->GetType() == MATERIAL_RIMLIT &&
                m_tfRimColor[i].IsSelected())
            {
                float arNewColor[4];
                memcpy(arNewColor, reinterpret_cast<RimlitMaterial*>(pAsset->m_pMaterial)->GetRimColor(), sizeof(float) * 4);
                arNewColor[i] = (float) atof(m_tfRimColor[i].GetText());
                reinterpret_cast<RimlitMaterial*>(pAsset->m_pMaterial)->SetRimColor(arNewColor);
                UpdateView();
            }

            ListNode* pCurColliderNode = pAsset->m_lColliders.Get(m_nCurCollider);

            if (pAsset->m_lColliders.Count() >= 0 &&
                pCurColliderNode             != 0)
            {
                OrientedBoxCollider* pCollider = reinterpret_cast<OrientedBoxCollider*>(pCurColliderNode->m_pData);
                
                if (m_tfColliderColor[i].IsSelected())
                {
                    float arNewColor[4];
                    memcpy(arNewColor, pCollider->GetColor(), sizeof(float) * 4);
                    arNewColor[i] = (float) atof(m_tfColliderColor[i].GetText());
                    pCollider->SetColor(arNewColor);
                    UpdateView();
                }

                if (i < 3 &&
                    m_tfColliderCenter[i].IsSelected())
                {
                    float arCenter[3];
                    memcpy(arCenter, pCollider->GetRelativePosition(), sizeof(float) * 3);
                    arCenter[i] = (float) atof(m_tfColliderCenter[i].GetText());
                    pCollider->SetRelativePosition(arCenter[0], arCenter[1], arCenter[2]);
                    UpdateView();
                }

                if (i < 3 &&
                    m_tfColliderExtents[i].IsSelected())
                {
                    float arExtents[3];
                    memcpy(arExtents, pCollider->GetHalfExtents(), sizeof(float) * 3);
                    arExtents[i] = (float) atof(m_tfColliderExtents[i].GetText());
                    pCollider->SetHalfExtents(arExtents[0], arExtents[1], arExtents[2]);
                    UpdateView();
                }

                if (i < 3 &&
                    m_tfColliderRotation[i].IsSelected())
                {
                    float arRotation[3];
                    memcpy(arRotation, pCollider->GetRelativeRotation(), sizeof(float) * 3);
                    arRotation[i] = (float) atof(m_tfColliderRotation[i].GetText());
                    pCollider->SetRelativeRotation(arRotation[0], arRotation[1], arRotation[2]);
                    UpdateView();
                }
            }
        }
    }
    if (IsPointerJustUp())
    {
        if(m_btMaterialType.IsVisible() &&
           m_btMaterialType.IsPointerHovering())
        {
            // Change material type
            int nType = pAsset->m_pMaterial->GetType();
            nType++;

            if (nType >= MATERIAL_NUM_TYPES)
                nType = MATERIAL_DIFFUSE;

            // Create the new material object
            Material* pNewMat = 0;
            switch (nType)
            {
            case MATERIAL_DIFFUSE:
                pNewMat = new DiffuseMaterial();
                break;
            case MATERIAL_FULLBRIGHT:
                pNewMat = new FullbrightMaterial();
                break;
            case MATERIAL_RIMLIT:
                pNewMat = new RimlitMaterial();
                break;
            case MATERIAL_TOON:
                pNewMat = new ToonMaterial();
                break;
            default:
                pNewMat = new DiffuseMaterial();
                break;
            }

            // Replace all old materials with the new material
            //reinterpret_cast<LevelEditor*>(m_pEditor)->ReplaceMaterial(pAsset->m_pMaterial, pNewMat);

            // Delete the old material
            delete pAsset->m_pMaterial;
            pAsset->m_pMaterial = 0;

            // Set the new material
            pAsset->m_pMaterial = pNewMat;

            UpdateView();
        }
        else if (m_btRimStyle.IsVisible() &&
                 m_btRimStyle.IsPointerHovering())
        {
            if (pAsset->m_pMaterial->GetType() == MATERIAL_RIMLIT)
            {
                int nStyle = reinterpret_cast<RimlitMaterial*>(pAsset->m_pMaterial)->GetRimStyle();
                if (nStyle == RIM_STYLE_HARD)
                    nStyle = RIM_STYLE_SOFT;
                else
                    nStyle = RIM_STYLE_HARD;
                reinterpret_cast<RimlitMaterial*>(pAsset->m_pMaterial)->SetRimStyle(nStyle);
            }

            UpdateView();
        }
        else if (m_btCollider.IsVisible() &&
                 m_btCollider.IsPointerHovering())
        {
            m_nCurCollider++;

            if (m_nCurCollider >= pAsset->m_lColliders.Count())
                m_nCurCollider = 0;
            else if (m_nCurCollider < 0)
                m_nCurCollider = 0;

            UpdateView();
        }
        else if (m_btAddCollider.IsVisible() &&
                 m_btAddCollider.IsPointerHovering())
        {
            pAsset->m_lColliders.Add(new OrientedBoxCollider());

            UpdateView();
        }
        else if (m_btRemoveCollider.IsVisible() &&
                 m_btRemoveCollider.IsPointerHovering() &&
                 pAsset->m_lColliders.Count() > 0)
        {
            OrientedBoxCollider* pCollider = reinterpret_cast<OrientedBoxCollider*>(pAsset->m_lColliders.Get(pAsset->m_lColliders.Count() - 1)->m_pData);
            pAsset->m_lColliders.Remove(pCollider);

            // free data that the Add Button created
            delete pCollider;
            pCollider = 0;

            // Make sure the current collider variable is still in bounds
            if (m_nCurCollider >= pAsset->m_lColliders.Count())
                m_nCurCollider = pAsset->m_lColliders.Count() - 1;
            else if (m_nCurCollider < 0)
                m_nCurCollider = 0;

            UpdateView();
        }
        else if (m_btAssignDefaultTexture.IsVisible() &&
                 m_btAssignDefaultTexture.IsPointerHovering())
        {
            Asset* pSelAsset = reinterpret_cast<LevelEditor*>(m_pEditor)->GetSelectedAsset();
            if (pSelAsset->GetType() == ASSET_TEXTURE)
            {
                pAsset->m_pDefaultTextureAsset = pSelAsset;
                UpdateView();
            }
        }
    }
    else if (IsPointerDown())
    {
        Button* pDown = 0;

        if (m_btMaterialType.IsTouched())
        {
            ColorButtonDown(m_btMaterialType);
            pDown = &m_btMaterialType;
        }

        else if (m_btRimStyle.IsTouched())
        {
            ColorButtonDown(m_btRimStyle);
            pDown = &m_btRimStyle;
        }

        else if (m_btAssignDefaultTexture.IsTouched())
        {
            ColorButtonDown(m_btAssignDefaultTexture);
            pDown = &m_btAssignDefaultTexture;
        }

        else if (m_btCollider.IsTouched())
        {
            ColorButtonDown(m_btCollider);
            pDown = &m_btCollider;
        }

        else if (m_btAddCollider.IsTouched())
        {
            ColorButtonDown(m_btAddCollider);
            pDown = &m_btAddCollider;
        }

        else if (m_btRemoveCollider.IsTouched())
        {
            ColorButtonDown(m_btRemoveCollider);
            pDown = &m_btRemoveCollider;
        }

        m_pPrevHover = pDown;
    }
    else
    {
        Button* pHover = 0;

        if (m_btMaterialType.IsPointerHovering())
        {
            ColorButtonHover(m_btMaterialType);
            pHover = &m_btMaterialType;
        }

        else if (m_btRimStyle.IsPointerHovering())
        {
            ColorButtonHover(m_btRimStyle);
            pHover = &m_btRimStyle;
        }

        else if (m_btAssignDefaultTexture.IsPointerHovering())
        {
            ColorButtonHover(m_btAssignDefaultTexture);
            pHover = &m_btAssignDefaultTexture;
        }

        else if (m_btCollider.IsPointerHovering())
        {
            ColorButtonHover(m_btCollider);
            pHover = &m_btCollider;
        }

        else if (m_btAddCollider.IsPointerHovering())
        {
            ColorButtonHover(m_btAddCollider);
            pHover = &m_btAddCollider;
        }

        else if (m_btRemoveCollider.IsPointerHovering())
        {
            ColorButtonHover(m_btRemoveCollider);
            pHover = &m_btRemoveCollider;
        }

        m_pPrevHover = pHover;
    }

    // Update all text fields
    int nJustUp = IsPointerJustUp();
    m_tfIntervals.Update(nJustUp, fX, fY);
    m_tfRimSize.Update(nJustUp, fX, fY);

    for (int i = 0; i < 3; i++)
    {
        m_tfColliderCenter[i].Update(nJustUp, fX, fY);
        m_tfColliderExtents[i].Update(nJustUp, fX, fY);
        m_tfColliderRotation[i].Update(nJustUp, fX, fY);
    }

    for (int i = 0; i < 4; i++)
    {
        m_tfColor[i].Update(nJustUp, fX, fY);
        m_tfRimColor[i].Update(nJustUp, fX, fY);
        m_tfColliderColor[i].Update(nJustUp, fX, fY);
    }
}

void AssetProperties::HandleInput_Texture()
{
    float fX = 0.0f;
    float fY = 0.0f;
    GetPointerPositionNormalized(fX,fY);

    TextureAsset* pAsset = reinterpret_cast<TextureAsset*>(m_pAsset);

    if (IsPointerJustUp())
    {

    }
    else if (IsPointerDown())
    {
        Button* pDown = 0;

        if (m_btTextureFilter.IsTouched())
        {
            ColorButtonDown(m_btTextureFilter);
            pDown = &m_btTextureFilter;
        }

        m_pPrevHover = pDown;
    }
    else
    {
        Button* pHover = 0;

        if (m_btTextureFilter.IsPointerHovering())
        {
            ColorButtonHover(m_btTextureFilter);
            pHover = &m_btTextureFilter;
        }

        m_pPrevHover = pHover;
    }
}

void AssetProperties::HandleInput_Sound()
{
    float fX = 0.0f;
    float fY = 0.0f;
    GetPointerPositionNormalized(fX,fY);
}

void AssetProperties::InitializeView()
{
    Window::InitializeView();

    m_textHeader.SetText("Asset Properties");

    // GENERAL
    m_tName1.SetText("Name:");
    m_tName1.SetScale(T_SCALE_X, T_SCALE_Y);
    m_tName1.SetColor(TEXT_COLOR);

    m_tName2.SetScale(T_SCALE_X, T_SCALE_Y);
    m_tName2.SetColor(TEXT_COLOR);

    // MESH

    // Material Section
    m_tMaterialType.SetText("Material:");
    m_tMaterialType.SetColor(TEXT_COLOR);
    m_tMaterialType.SetScale(T_SCALE_X, T_SCALE_Y);

    ColorButtonOff(m_btMaterialType);
    m_btMaterialType.SetTextScale(BT_SCALE_X, BT_SCALE_Y);

    m_tColor.SetText("Color");
    m_tColor.SetColor(TEXT_COLOR);
    m_tColor.SetScale(T_SCALE_X, T_SCALE_Y);

    m_tRimColor.SetText("Rim");
    m_tRimColor.SetColor(TEXT_COLOR);
    m_tRimColor.SetScale(T_SCALE_X, T_SCALE_Y);

    m_tRimStyle.SetText("Rim Style");
    m_tRimStyle.SetColor(TEXT_COLOR);
    m_tRimStyle.SetScale(T_SCALE_X, T_SCALE_Y);

    ColorButtonOff(m_btRimStyle);
    m_btRimStyle.SetTextScale(BT_SCALE_X, BT_SCALE_Y);

    m_tRimSize.SetText("Rim Size");
    m_tRimSize.SetColor(TEXT_COLOR);
    m_tRimSize.SetScale(T_SCALE_X, T_SCALE_Y);

    SetTextFieldColors(m_tfRimSize);
    m_tfRimSize.SetTextScale(TF_SCALE_X, TF_SCALE_Y);
    m_tfRimSize.SetMaxSize(6);

    m_tIntervals.SetText("Intervals");
    m_tIntervals.SetColor(TEXT_COLOR);
    m_tIntervals.SetScale(T_SCALE_X, T_SCALE_Y);

    SetTextFieldColors(m_tfIntervals);
    m_tfIntervals.SetTextScale(TF_SCALE_X, TF_SCALE_Y);
    m_tfIntervals.SetMaxSize(3);

    // Default Texture Section
    m_tDefaultTexture.SetText("Texture:");
    m_tDefaultTexture.SetColor(TEXT_COLOR);
    m_tDefaultTexture.SetScale(T_SCALE_X, T_SCALE_Y);

    ColorButtonOff(m_btDefaultTexture);
    m_btDefaultTexture.SetTextScale(BT_SCALE_X, BT_SCALE_Y);

    ColorButtonOff(m_btAssignDefaultTexture);
    m_btAssignDefaultTexture.SetTextScale(BT_SCALE_X, BT_SCALE_Y);
    m_btAssignDefaultTexture.SetTextString("v");

    // Collider Section
    m_tCollider.SetText("Collider:");
    m_tCollider.SetColor(TEXT_COLOR);
    m_tCollider.SetScale(T_SCALE_X, T_SCALE_Y);

    ColorButtonOff(m_btCollider);
    m_btCollider.SetTextScale(BT_SCALE_X, BT_SCALE_Y);

    ColorButtonOff(m_btAddCollider);
    m_btAddCollider.SetTextScale(BT_SCALE_X, BT_SCALE_Y);
    m_btAddCollider.SetTextString("+");

    ColorButtonOff(m_btRemoveCollider);
    m_btRemoveCollider.SetTextScale(BT_SCALE_X, BT_SCALE_Y);
    m_btRemoveCollider.SetTextString("-");

    m_tColliderCenter.SetText("Pos");
    m_tColliderCenter.SetColor(TEXT_COLOR);
    m_tColliderCenter.SetScale(T_SCALE_X, T_SCALE_Y);

    m_tColliderExtents.SetText("Ext");
    m_tColliderExtents.SetColor(TEXT_COLOR);
    m_tColliderExtents.SetScale(T_SCALE_X, T_SCALE_Y);
    
    m_tColliderRotation.SetText("Rot");
    m_tColliderRotation.SetColor(TEXT_COLOR);
    m_tColliderRotation.SetScale(T_SCALE_X, T_SCALE_Y);

    m_tColliderColor.SetText("Color");
    m_tColliderColor.SetColor(TEXT_COLOR);
    m_tColliderColor.SetScale(T_SCALE_X, T_SCALE_Y);

    // TEXTURE

    m_tTextureFilter.SetText("Filter:");
    m_tTextureFilter.SetColor(TEXT_COLOR);
    m_tTextureFilter.SetScale(T_SCALE_X, T_SCALE_Y);
    
    ColorButtonOff(m_btTextureFilter);
    m_btTextureFilter.SetTextScale(BT_SCALE_X, BT_SCALE_Y);

    // Set textfield-array data
    for (int i = 0; i < 3; i++)
    {
        SetTextFieldColors(m_tfColliderCenter[i]);
        m_tfColliderCenter[i].SetTextScale(0.3f, 0.64f);
        m_tfColliderCenter[i].SetMaxSize(8);

        SetTextFieldColors(m_tfColliderExtents[i]);
        m_tfColliderExtents[i].SetTextScale(0.3f, 0.64f);
        m_tfColliderExtents[i].SetMaxSize(8);

        SetTextFieldColors(m_tfColliderRotation[i]);
        m_tfColliderRotation[i].SetTextScale(0.3f, 0.64f);
        m_tfColliderRotation[i].SetMaxSize(8);
    }

    for (int i = 0; i < 4; i++)
    {
        SetTextFieldColors(m_tfColor[i]);
        m_tfColor[i].SetMaxSize(6);
        m_tfColor[i].SetTextScale(0.25f, 0.64f);

        SetTextFieldColors(m_tfRimColor[i]);
        m_tfRimColor[i].SetMaxSize(6);
        m_tfRimColor[i].SetTextScale(0.25f, 0.64f);

        SetTextFieldColors(m_tfColliderColor[i]);
        m_tfColliderColor[i].SetMaxSize(6);
        m_tfColliderColor[i].SetTextScale(0.25f, 0.64f);
    }
}

void AssetProperties::UpdateView()
{
    Window::UpdateView();

    if (m_pAsset == 0)
    {
        // No asset selected for detailed properties display
        HideViews();
    }
    else
    {
        if (m_pAsset->GetType() == ASSET_MESH)
            UpdateView_Mesh();
        else if (m_pAsset->GetType() == ASSET_TEXTURE)
            UpdateView_Texture();
        else if (m_pAsset->GetType() == ASSET_SOUND)
            UpdateView_Sound();
    }
}

void AssetProperties::UpdateView_Mesh()
{
    HideViews();

    float fX = m_rect.m_fX + m_fPadding;
    float fY = m_rect.m_fY + m_rect.m_fHeight - HEADER_HEIGHT - 0.07f;

    if (m_pAsset == 0 ||
        m_pAsset->GetType() != ASSET_MESH)
    {
        return;
    }

    // Set name of asset first
    m_tName1.SetPosition(fX, fY);
    m_tName1.SetVisible(1);
    
    m_tName2.SetText(m_pAsset->m_arName);
    m_tName2.SetPosition(fX + 0.08f, fY);
    m_tName2.SetVisible(1);

    fY -= m_fSpacing;

    MeshAsset* pAsset = reinterpret_cast<MeshAsset*>(m_pAsset);

    if (pAsset->m_pMaterial != 0)
    {
        // Common material views
        m_tMaterialType.SetPosition(fX, fY);
        m_tMaterialType.SetVisible(1);

        m_btMaterialType.SetRect(fX + 0.14f,
                                 fY - BUTTON_OFF_Y,
                                 0.18f,
                                 BUTTON_HEIGHT);
        m_btMaterialType.SetVisible(1);
        fY -= m_fSpacing;

        m_tColor.SetPosition(fX, fY);
        m_tColor.SetScale(0.3f, T_SCALE_Y);
        m_tColor.SetVisible(1);
        
        for(int i = 0; i < 4; i++)
        {
            m_tfColor[i].SetRect(fX + 0.01f + 0.065f * (i+1),
                                 fY - BUTTON_OFF_Y,
                                 0.06f,
                                 BUTTON_HEIGHT);
            m_tfColor[i].SetText_Float(pAsset->m_pMaterial->GetColor()[i]);
            m_tfColor[i].SetVisible(1);
        }

        fY -= m_fSpacing;

        switch(pAsset->m_pMaterial->GetType())
        {
        case MATERIAL_DIFFUSE:
            m_btMaterialType.SetTextString("Diffuse");
            break;
        case MATERIAL_FULLBRIGHT:
            m_btMaterialType.SetTextString("Fullbright");
            break;
        case MATERIAL_RIMLIT:
        {
            m_btMaterialType.SetTextString("Rimlit");
            m_tRimStyle.SetPosition(fX, fY);
            m_tRimStyle.SetVisible(1);
            m_btRimStyle.SetRect(fX + 0.14f,
                                 fY - BUTTON_OFF_Y,
                                 0.1f,
                                 BUTTON_HEIGHT);
            if (reinterpret_cast<RimlitMaterial*>(pAsset->m_pMaterial)->GetRimStyle() == RIM_STYLE_SOFT)
                m_btRimStyle.SetTextString("Soft");
            else
                m_btRimStyle.SetTextString("Hard");
            m_btRimStyle.SetVisible(1);

            fY -= m_fSpacing;

            m_tRimSize.SetPosition(fX, fY);
            m_tRimSize.SetVisible(1);
            m_tfRimSize.SetRect(fX + 0.14f,
                                fY - BUTTON_OFF_Y,
                                0.1f,
                                BUTTON_HEIGHT);
            m_tfRimSize.SetText_Float(reinterpret_cast<RimlitMaterial*>(pAsset->m_pMaterial)->GetRimSize());
            m_tfRimSize.SetVisible(1);

            fY -= m_fSpacing;

            m_tRimColor.SetPosition(fX, fY);
            m_tRimColor.SetVisible(1);
            
            for (int i = 0; i < 4; i++)
            {
                m_tfRimColor[i].SetRect(fX + 0.01f + 0.065f * (i+1),
                                        fY - BUTTON_OFF_Y,
                                        0.06f,
                                        BUTTON_HEIGHT);
                m_tfRimColor[i].SetText_Float(reinterpret_cast<RimlitMaterial*>(pAsset->m_pMaterial)->GetRimColor()[i]);
                m_tfRimColor[i].SetVisible(1);
            }

            fY -= m_fSpacing;
            break;
        }
        case MATERIAL_TOON:
            m_btMaterialType.SetTextString("Toon");

            m_tIntervals.SetPosition(fX, fY);
            m_tIntervals.SetVisible(1);
            
            m_tfIntervals.SetRect(fX + 0.135f, fY - BUTTON_OFF_Y, 0.06f, BUTTON_HEIGHT);
            m_tfIntervals.SetText_Int(reinterpret_cast<ToonMaterial*>(pAsset->m_pMaterial)->GetLightingIntervalCount());
            m_tfIntervals.SetVisible(1);

            fY -= m_fSpacing;

            break;
        default:
            m_btMaterialType.SetTextString("Unknown");
            break;
        }

        // Default Texture
        m_tDefaultTexture.SetPosition(fX, fY);
        m_tDefaultTexture.SetVisible(1);

        m_btDefaultTexture.SetRect(fX + 0.12f,
                                   fY - BUTTON_OFF_Y,
                                   0.16f,
                                   BUTTON_HEIGHT);
        m_btDefaultTexture.SetVisible(1);
        if (pAsset->m_pDefaultTextureAsset != 0)
        {
            m_btDefaultTexture.SetTextString(pAsset->m_pDefaultTextureAsset->m_arName);

            if (strlen(pAsset->m_pDefaultTextureAsset->m_arName) > 10)
            {
                m_btDefaultTexture.SetTextScale(0.25f, BT_SCALE_Y);
            }
            else
            {
                m_btDefaultTexture.SetTextScale(BT_SCALE_X, BT_SCALE_Y);
            }
        }

        m_btAssignDefaultTexture.SetRect(fX + 0.29f,
                                         fY - BUTTON_OFF_Y,
                                         0.035f,
                                         BUTTON_HEIGHT);
        m_btAssignDefaultTexture.SetVisible(1);

        fY -= m_fSpacing;

        // Collider Properties
        m_tCollider.SetPosition(fX, fY);
        m_tCollider.SetVisible(1);

        m_btCollider.SetRect(fX + 0.15f,
                             fY - BUTTON_OFF_Y,
                             0.035f,
                             BUTTON_HEIGHT);
        m_btCollider.SetVisible(1);
        if (pAsset->m_lColliders.Count() == 0)
        {
            m_btCollider.SetTextString("X");
        }
        else
        {
            // Verify the bounds of m_nCurCollider
            if (m_nCurCollider >= pAsset->m_lColliders.Count())
                m_nCurCollider = pAsset->m_lColliders.Count() - 1;
            if (m_nCurCollider < 0)
                m_nCurCollider = 0;

            m_btCollider.SetTextString_Int(m_nCurCollider);
        }

        m_btAddCollider.SetRect(fX + 0.20f,
                                fY - BUTTON_OFF_Y,
                                0.035f,
                                BUTTON_HEIGHT);
        m_btAddCollider.SetVisible(1);

        m_btRemoveCollider.SetRect(fX + 0.25f,
                                   fY - BUTTON_OFF_Y,
                                   0.035f,
                                   BUTTON_HEIGHT);
        m_btRemoveCollider.SetVisible(1);

        fY -= m_fSpacing;

        if (pAsset->m_lColliders.Count() > 0)
        {
            OrientedBoxCollider* pCollider = reinterpret_cast<OrientedBoxCollider*>(pAsset->m_lColliders.Get(m_nCurCollider)->m_pData);

            m_tColliderCenter.SetPosition(fX, fY);
            m_tColliderCenter.SetVisible(1);

            m_tColliderExtents.SetPosition(fX, fY - m_fSpacing);
            m_tColliderExtents.SetVisible(1);

            m_tColliderRotation.SetPosition(fX, fY - 2*m_fSpacing);
            m_tColliderRotation.SetVisible(1);

            m_tColliderColor.SetPosition(fX, fY - 3*m_fSpacing);
            m_tColliderColor.SetVisible(1);

            for (int i = 0; i < 3; i++)
            {
                m_tfColliderCenter[i].SetRect(fX + 0.05f + 0.095f*i,
                                              fY - BUTTON_OFF_Y,
                                              0.085f,
                                              BUTTON_HEIGHT);
                m_tfColliderCenter[i].SetVisible(1);
                m_tfColliderCenter[i].SetText_Float(pCollider->GetRelativePosition()[i], 2);

                m_tfColliderExtents[i].SetRect(fX + 0.05f + 0.095f*i,
                                               fY - BUTTON_OFF_Y - m_fSpacing,
                                               0.085f,
                                               BUTTON_HEIGHT);
                m_tfColliderExtents[i].SetVisible(1);
                m_tfColliderExtents[i].SetText_Float(pCollider->GetHalfExtents()[i], 2);

                m_tfColliderRotation[i].SetRect(fX + 0.05f + 0.095f*i,
                                                fY - BUTTON_OFF_Y - m_fSpacing*2,
                                                0.085f,
                                                BUTTON_HEIGHT);
                m_tfColliderRotation[i].SetVisible(1);
                m_tfColliderRotation[i].SetText_Float(pCollider->GetRelativeRotation()[i], 2);
            }

            for (int i = 0; i < 4; i++)
            {
                m_tfColliderColor[i].SetRect(fX + 0.01f + 0.065f * (i+1),
                                             fY - BUTTON_OFF_Y - m_fSpacing*3,
                                             0.06f,
                                             BUTTON_HEIGHT);
                m_tfColliderColor[i].SetVisible(1);
                m_tfColliderColor[i].SetText_Float(pCollider->GetColor()[i]);
            }

            fY -= m_fSpacing*4;
        }
        
    }
}

void AssetProperties::UpdateView_Texture()
{
    HideViews();

    float fX = m_rect.m_fX + m_fPadding;
    float fY = m_rect.m_fY + m_rect.m_fHeight - HEADER_HEIGHT - 0.07f;

    // Set name of asset first
    m_tName1.SetPosition(fX, fY);
    m_tName1.SetVisible(1);
    
    m_tName2.SetText(m_pAsset->m_arName);
    m_tName2.SetPosition(fX + 0.08f, fY);
    m_tName2.SetVisible(1);

    fY -= m_fSpacing;
}

void AssetProperties::UpdateView_Sound()
{
    HideViews();
}

void AssetProperties::HideViews()
{
    m_tName1.SetVisible(0);
    m_tName2.SetVisible(0);

    m_tMaterialType.SetVisible(0);
    m_btMaterialType.SetVisible(0);
    m_tColor.SetVisible(0);
    m_tRimColor.SetVisible(0);
    m_tRimStyle.SetVisible(0);
    m_btRimStyle.SetVisible(0);
    m_tRimSize.SetVisible(0);
    m_tfRimSize.SetVisible(0);
    m_tIntervals.SetVisible(0);
    m_tfIntervals.SetVisible(0);

    m_tDefaultTexture.SetVisible(0);
    m_btDefaultTexture.SetVisible(0);
    m_btAssignDefaultTexture.SetVisible(0);

    m_tCollider.SetVisible(0);
    m_btCollider.SetVisible(0);
    m_btAddCollider.SetVisible(0);
    m_btRemoveCollider.SetVisible(0);

    m_tColliderCenter.SetVisible(0);
    m_tColliderExtents.SetVisible(0);
    m_tColliderRotation.SetVisible(0);
    m_tColliderColor.SetVisible(0);

    // Texture Properties Views
    m_tTextureFilter.SetVisible(0);
    m_btTextureFilter.SetVisible(0);

    for (int i = 0; i < 3; i++)
    {
        m_tfColliderCenter[i].SetVisible(0);
        m_tfColliderExtents[i].SetVisible(0);
        m_tfColliderRotation[i].SetVisible(0);
    }

    for (int i = 0; i < 4; i++)
    {
        m_tfColor[i].SetVisible(0);
        m_tfRimColor[i].SetVisible(0);
        m_tfColliderColor[i].SetVisible(0);
    }
}

void AssetProperties::RegisterScene(Scene* pScene)
{
    if (pScene == 0)
    {
        return;
    }

    Window::RegisterScene(pScene);

    // General Properties Views
    pScene->AddGlyph(&m_tName1);
    pScene->AddGlyph(&m_tName2);

    // Mesh Properties Views
    pScene->AddGlyph(&m_tMaterialType);
    m_btMaterialType.AddToScene(*pScene);
    pScene->AddGlyph(&m_tColor);
    m_tfColor[0].AddToScene(*pScene);
    m_tfColor[1].AddToScene(*pScene);
    m_tfColor[2].AddToScene(*pScene);
    m_tfColor[3].AddToScene(*pScene);
    pScene->AddGlyph(&m_tRimColor);
    m_tfRimColor[0].AddToScene(*pScene);
    m_tfRimColor[1].AddToScene(*pScene);
    m_tfRimColor[2].AddToScene(*pScene);
    m_tfRimColor[3].AddToScene(*pScene);
    pScene->AddGlyph(&m_tRimStyle);
    m_btRimStyle.AddToScene(*pScene);
    pScene->AddGlyph(&m_tRimSize);
    m_tfRimSize.AddToScene(*pScene);
    pScene->AddGlyph(&m_tIntervals);
    m_tfIntervals.AddToScene(*pScene);
    
    pScene->AddGlyph(&m_tDefaultTexture);
    m_btDefaultTexture.AddToScene(*pScene);
    m_btAssignDefaultTexture.AddToScene(*pScene);

    pScene->AddGlyph(&m_tCollider);
    m_btCollider.AddToScene(*pScene);
    m_btAddCollider.AddToScene(*pScene);
    m_btRemoveCollider.AddToScene(*pScene);

    pScene->AddGlyph(&m_tColliderCenter);
    m_tfColliderCenter[0].AddToScene(*pScene);
    m_tfColliderCenter[1].AddToScene(*pScene);
    m_tfColliderCenter[2].AddToScene(*pScene);
    pScene->AddGlyph(&m_tColliderExtents);
    m_tfColliderExtents[0].AddToScene(*pScene);
    m_tfColliderExtents[1].AddToScene(*pScene);
    m_tfColliderExtents[2].AddToScene(*pScene);
    pScene->AddGlyph(&m_tColliderRotation);
    m_tfColliderRotation[0].AddToScene(*pScene);
    m_tfColliderRotation[1].AddToScene(*pScene);
    m_tfColliderRotation[2].AddToScene(*pScene);
    pScene->AddGlyph(&m_tColliderColor);
    m_tfColliderColor[0].AddToScene(*pScene);
    m_tfColliderColor[1].AddToScene(*pScene);
    m_tfColliderColor[2].AddToScene(*pScene);
    m_tfColliderColor[3].AddToScene(*pScene);

    // Texture Properties Views
    pScene->AddGlyph(&m_tTextureFilter);
    m_btTextureFilter.AddToScene(*pScene);
}

void AssetProperties::SetDetailedAsset(Asset* pAsset)
{
    m_pAsset = pAsset;
    UpdateView();
}
