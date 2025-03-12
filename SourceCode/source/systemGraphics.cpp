#include "systemGraphics.hpp"

olcSprite::olcSprite()
{

}

olcSprite::olcSprite(int w, int h)
{
    Create(w, h);
}

olcSprite::olcSprite(std::wstring sFile)
{
    if (!Load(sFile))
        Create(8, 8);
}

void olcSprite::Create(int w, int h)
{
    nWidth = w;
    nHeight = h;
    m_Glyphs = new short[w*h];
    m_Colours = new short[w*h];
    for (int i = 0; i < w*h; i++)
    {
        m_Glyphs[i] = L' ';
        m_Colours[i] = BLACK;
    }
}

void olcSprite::SetGlyph(int x, int y, short c)
{
    if (x <0 || x >= nWidth || y < 0 || y >= nHeight)
        return;
    else
        m_Glyphs[y * nWidth + x] = c;
}

void olcSprite::SetColour(int x, int y, short c)
{
    if (x <0 || x >= nWidth || y < 0 || y >= nHeight)
        return;
    else
        m_Colours[y * nWidth + x] = c;
}

short olcSprite::GetGlyph(int x, int y)
{
    if (x <0 || x >= nWidth || y < 0 || y >= nHeight)
        return L' ';
    else
        return m_Glyphs[y * nWidth + x];
}

short olcSprite::GetColour(int x, int y)
{
    if (x <0 || x >= nWidth || y < 0 || y >= nHeight)
        return BLACK;
    else
        return m_Colours[y * nWidth + x];
}

short olcSprite::SampleGlyph(float x, float y)
{
    int sx = (int)(x * (float)nWidth);
    int sy = (int)(y * (float)nHeight-1.0f);
    if (sx <0 || sx >= nWidth || sy < 0 || sy >= nHeight)
        return L' ';
    else
        return m_Glyphs[sy * nWidth + sx];
}

short olcSprite::SampleColour(float x, float y)
{
    int sx = (int)(x * (float)nWidth);
    int sy = (int)(y * (float)nHeight-1.0f);
    if (sx <0 || sx >= nWidth || sy < 0 || sy >= nHeight)
        return BLACK;
    else
        return m_Colours[sy * nWidth + sx];
}

bool olcSprite::Save(std::wstring sFile)
{
    FILE *f = nullptr;
    _wfopen_s(&f, sFile.c_str(), L"wb");
    if (f == nullptr)
        return false;

    fwrite(&nWidth, sizeof(int), 1, f);
    fwrite(&nHeight, sizeof(int), 1, f);
    fwrite(m_Colours, sizeof(short), nWidth * nHeight, f);
    fwrite(m_Glyphs, sizeof(short), nWidth * nHeight, f);

    fclose(f);

    return true;
}

bool olcSprite::Load(std::wstring sFile)
{
    delete[] m_Glyphs;
    delete[] m_Colours;
    nWidth = 0;
    nHeight = 0;

    FILE *f = nullptr;
    _wfopen_s(&f, sFile.c_str(), L"rb");
    if (f == nullptr)
        return false;

    std::fread(&nWidth, sizeof(int), 1, f);
    std::fread(&nHeight, sizeof(int), 1, f);

    Create(nWidth, nHeight);

    std::fread(m_Colours, sizeof(short), nWidth * nHeight, f);
    std::fread(m_Glyphs, sizeof(short), nWidth * nHeight, f);

    std::fclose(f);
    return true;
}