#ifndef SYSTEMGRAPHICS_HPP
#define SYSTEMGRAPHICS_HPP

#include <sstream>
#include <fstream>

#define BLACK 0x0000

class olcSprite
{
public:
	olcSprite();

	olcSprite(int w, int h);

	olcSprite(std::wstring sFile);

	int nWidth = 0;
	int nHeight = 0;

private:
	short *m_Glyphs = nullptr;
	short *m_Colours = nullptr;

	void Create(int w, int h);

public:
	void SetGlyph(int x, int y, short c);

	void SetColour(int x, int y, short c);

	short GetGlyph(int x, int y);

	short GetColour(int x, int y);

	short SampleGlyph(float x, float y);

	short SampleColour(float x, float y);

	bool Save(std::wstring sFile);

	bool Load(std::wstring sFile);

};

#endif