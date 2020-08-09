#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <cstdio>
#include <vector>
#include <string>

typedef std::vector<std::string> row_t;
typedef std::vector<row_t> rows_t;
const char *filename = "AnalyzeTTF.tsv";

bool save_rows(const rows_t& rows)
{
    if (FILE *fp = fopen(filename, "a"))
    {
        for (auto& row : rows)
        {
            bool first = true;
            for (auto& field : row)
            {
                if (!first)
                {
                    fprintf(fp, "\t");
                }
                else
                {
                    first = false;
                }
                fprintf(fp, "%s", field.c_str());
            }
            fprintf(fp, "\n");
        }
        fclose(fp);
        return true;
    }
    return false;
}

bool DoAnalyzeTTF(rows_t& rows, const char *font_file, const char *font_name, bool bold = false, bool italic = false)
{
    static BYTE s_abBuffer[1024 * 12];
    if (HDC hdc = CreateCompatibleDC(NULL))
    {
        LOGFONTA lf;
        ZeroMemory(&lf, sizeof(lf));
        for (LONG lfHeight = -100; lfHeight <= 100; ++lfHeight)
        {
            lf.lfHeight = lfHeight;
            lstrcpyA(lf.lfFaceName, font_name);
            if (bold)
                lf.lfWeight = FW_BOLD;
            if (italic)
                lf.lfItalic = TRUE;
            if (HFONT hFont = CreateFontIndirectA(&lf))
            {
                HGDIOBJ hFontOld = SelectObject(hdc, hFont);
                {
                    TEXTMETRICA tm;
                    if (GetTextMetrics(hdc, &tm))
                    {
                        row_t row;
                        row.push_back(font_file);
                        row.push_back(font_name);
                        row.push_back(bold ? "TRUE" : "FALSE");
                        row.push_back(italic ? "TRUE" : "FALSE");
                        row.push_back(std::to_string(lfHeight));
                        row.push_back(std::to_string(tm.tmHeight));
                        row.push_back(std::to_string(tm.tmAscent));
                        row.push_back(std::to_string(tm.tmDescent));
                        row.push_back(std::to_string(tm.tmInternalLeading));
                        row.push_back(std::to_string(tm.tmExternalLeading));
                        row.push_back(std::to_string(tm.tmAveCharWidth));
                        row.push_back(std::to_string(tm.tmMaxCharWidth));
                        row.push_back(std::to_string(tm.tmWeight));
                        row.push_back(std::to_string(tm.tmOverhang));
                        row.push_back(std::to_string(tm.tmDigitizedAspectX));
                        row.push_back(std::to_string(tm.tmDigitizedAspectY));
                        row.push_back(std::to_string(tm.tmFirstChar));
                        row.push_back(std::to_string(tm.tmLastChar));
                        row.push_back(std::to_string(tm.tmDefaultChar));
                        row.push_back(std::to_string(tm.tmBreakChar));
                        row.push_back(std::to_string(tm.tmItalic));
                        row.push_back(std::to_string(tm.tmUnderlined));
                        row.push_back(std::to_string(tm.tmStruckOut));
                        row.push_back(std::to_string(tm.tmPitchAndFamily));
                        row.push_back(std::to_string(tm.tmCharSet));
                        OUTLINETEXTMETRICA *potm = (OUTLINETEXTMETRICA *)s_abBuffer;
                        auto& otm = *potm;
                        otm.otmSize = sizeof(s_abBuffer);
                        if (GetOutlineTextMetricsA(hdc, sizeof(s_abBuffer), &otm))
                        {
                            row.push_back(std::to_string(otm.otmFiller));
                            row.push_back(std::to_string(otm.otmPanoseNumber.bFamilyType));
                            row.push_back(std::to_string(otm.otmPanoseNumber.bSerifStyle));
                            row.push_back(std::to_string(otm.otmPanoseNumber.bWeight));
                            row.push_back(std::to_string(otm.otmPanoseNumber.bProportion));
                            row.push_back(std::to_string(otm.otmPanoseNumber.bContrast));
                            row.push_back(std::to_string(otm.otmPanoseNumber.bStrokeVariation));
                            row.push_back(std::to_string(otm.otmPanoseNumber.bArmStyle));
                            row.push_back(std::to_string(otm.otmPanoseNumber.bLetterform));
                            row.push_back(std::to_string(otm.otmPanoseNumber.bMidline));
                            row.push_back(std::to_string(otm.otmPanoseNumber.bXHeight));
                            row.push_back(std::to_string(otm.otmfsSelection));
                            row.push_back(std::to_string(otm.otmfsType));
                            row.push_back(std::to_string(otm.otmsCharSlopeRise));
                            row.push_back(std::to_string(otm.otmsCharSlopeRun));
                            row.push_back(std::to_string(otm.otmItalicAngle));
                            row.push_back(std::to_string(otm.otmEMSquare));
                            row.push_back(std::to_string(otm.otmAscent));
                            row.push_back(std::to_string(otm.otmDescent));
                            row.push_back(std::to_string(otm.otmLineGap));
                            row.push_back(std::to_string(otm.otmsCapEmHeight));
                            row.push_back(std::to_string(otm.otmsXHeight));
                            row.push_back(std::to_string(otm.otmrcFontBox.left));
                            row.push_back(std::to_string(otm.otmrcFontBox.top));
                            row.push_back(std::to_string(otm.otmrcFontBox.right));
                            row.push_back(std::to_string(otm.otmrcFontBox.bottom));
                            row.push_back(std::to_string(otm.otmMacAscent));
                            row.push_back(std::to_string(otm.otmMacDescent));
                            row.push_back(std::to_string(otm.otmMacLineGap));
                            row.push_back(std::to_string(otm.otmusMinimumPPEM));
                            row.push_back(std::to_string(otm.otmptSubscriptSize.x));
                            row.push_back(std::to_string(otm.otmptSubscriptSize.y));
                            row.push_back(std::to_string(otm.otmptSubscriptOffset.x));
                            row.push_back(std::to_string(otm.otmptSubscriptOffset.y));
                            row.push_back(std::to_string(otm.otmptSuperscriptSize.x));
                            row.push_back(std::to_string(otm.otmptSuperscriptSize.y));
                            row.push_back(std::to_string(otm.otmptSuperscriptOffset.x));
                            row.push_back(std::to_string(otm.otmptSuperscriptOffset.y));
                            row.push_back(std::to_string(otm.otmsStrikeoutSize));
                            row.push_back(std::to_string(otm.otmsStrikeoutPosition));
                            row.push_back(std::to_string(otm.otmsUnderscoreSize));
                            row.push_back(std::to_string(otm.otmsUnderscorePosition));
                            row.push_back(LPSTR(&otm) + (LONG_PTR)otm.otmpFamilyName);
                            row.push_back(LPSTR(&otm) + (LONG_PTR)otm.otmpFaceName);
                            row.push_back(LPSTR(&otm) + (LONG_PTR)otm.otmpStyleName);
                            row.push_back(LPSTR(&otm) + (LONG_PTR)otm.otmpFullName);
                        }
                        rows.push_back(row);
                    }
                }
                SelectObject(hdc, hFontOld);
                DeleteObject(hFont);
            }
        }
        DeleteDC(hdc);
    }
    return true;
}

int main(void)
{
    fclose(fopen(filename, "w"));
    rows_t rows;
    {
        row_t row;
        row.push_back("font_file");
        row.push_back("font_name");
        row.push_back("bold");
        row.push_back("italic");
        row.push_back("lfHeight");
        row.push_back("tm.tmHeight");
        row.push_back("tm.tmAscent");
        row.push_back("tm.tmDescent");
        row.push_back("tm.tmInternalLeading");
        row.push_back("tm.tmExternalLeading");
        row.push_back("tm.tmAveCharWidth");
        row.push_back("tm.tmMaxCharWidth");
        row.push_back("tm.tmWeight");
        row.push_back("tm.tmOverhang");
        row.push_back("tm.tmDigitizedAspectX");
        row.push_back("tm.tmDigitizedAspectY");
        row.push_back("tm.tmFirstChar");
        row.push_back("tm.tmLastChar");
        row.push_back("tm.tmDefaultChar");
        row.push_back("tm.tmBreakChar");
        row.push_back("tm.tmItalic");
        row.push_back("tm.tmUnderlined");
        row.push_back("tm.tmStruckOut");
        row.push_back("tm.tmPitchAndFamily");
        row.push_back("tm.tmCharSet");
        row.push_back("otm.otmFiller");
        row.push_back("otm.otmPanoseNumber.bFamilyType");
        row.push_back("otm.otmPanoseNumber.bSerifStyle");
        row.push_back("otm.otmPanoseNumber.bWeight");
        row.push_back("otm.otmPanoseNumber.bProportion");
        row.push_back("otm.otmPanoseNumber.bContrast");
        row.push_back("otm.otmPanoseNumber.bStrokeVariation");
        row.push_back("otm.otmPanoseNumber.bArmStyle");
        row.push_back("otm.otmPanoseNumber.bLetterform");
        row.push_back("otm.otmPanoseNumber.bMidline");
        row.push_back("otm.otmPanoseNumber.bXHeight");
        row.push_back("otm.otmfsSelection");
        row.push_back("otm.otmfsType");
        row.push_back("otm.otmsCharSlopeRise");
        row.push_back("otm.otmsCharSlopeRun");
        row.push_back("otm.otmItalicAngle");
        row.push_back("otm.otmEMSquare");
        row.push_back("otm.otmAscent");
        row.push_back("otm.otmDescent");
        row.push_back("otm.otmLineGap");
        row.push_back("otm.otmsCapEmHeight");
        row.push_back("otm.otmsXHeight");
        row.push_back("otm.otmrcFontBox.left");
        row.push_back("otm.otmrcFontBox.top");
        row.push_back("otm.otmrcFontBox.right");
        row.push_back("otm.otmrcFontBox.bottom");
        row.push_back("otm.otmMacAscent");
        row.push_back("otm.otmMacDescent");
        row.push_back("otm.otmMacLineGap");
        row.push_back("otm.otmusMinimumPPEM");
        row.push_back("otm.otmptSubscriptSize.x");
        row.push_back("otm.otmptSubscriptSize.y");
        row.push_back("otm.otmptSubscriptOffset.x");
        row.push_back("otm.otmptSubscriptOffset.y");
        row.push_back("otm.otmptSuperscriptSize.x");
        row.push_back("otm.otmptSuperscriptSize.y");
        row.push_back("otm.otmptSuperscriptOffset.x");
        row.push_back("otm.otmptSuperscriptOffset.y");
        row.push_back("otm.otmsStrikeoutSize");
        row.push_back("otm.otmsStrikeoutPosition");
        row.push_back("otm.otmsUnderscoreSize");
        row.push_back("otm.otmsUnderscorePosition");
        row.push_back("otm.otmpFamilyName");
        row.push_back("otm.otmpFaceName");
        row.push_back("otm.otmpStyleName");
        row.push_back("otm.otmpFullName");
        rows.push_back(row);
    }
    DoAnalyzeTTF(rows, "arial.ttf", "Arial");
    DoAnalyzeTTF(rows, "arialbd.ttf", "Arial", true);
    DoAnalyzeTTF(rows, "arialbi.ttf", "Arial", true, true);
    DoAnalyzeTTF(rows, "ariali.ttf", "Arial", false, true);
    DoAnalyzeTTF(rows, "ariblk.ttf", "Arial Black");
    DoAnalyzeTTF(rows, "comic.ttf", "Comic Sans MS");
    DoAnalyzeTTF(rows, "comicbd.ttf", "Comic Sans MS", true);
    DoAnalyzeTTF(rows, "comici.ttf", "Comic Sans MS", false, true);
    DoAnalyzeTTF(rows, "comicz.ttf", "Comic Sans MS", true, true);
    DoAnalyzeTTF(rows, "cour.ttf", "Courier New");
    DoAnalyzeTTF(rows, "courbd.ttf", "Courier New", true);
    DoAnalyzeTTF(rows, "courbi.ttf", "Courier New", true, true);
    DoAnalyzeTTF(rows, "couri.ttf", "Courier New", false, true);
    DoAnalyzeTTF(rows, "lucon.ttf", "Lucida Console");
    DoAnalyzeTTF(rows, "marlett.ttf", "Marlett");
    DoAnalyzeTTF(rows, "symbol.ttf", "Symbol");
    DoAnalyzeTTF(rows, "tahoma.ttf", "Tahoma");
    DoAnalyzeTTF(rows, "tahomabd.ttf", "Tahoma", true);
    DoAnalyzeTTF(rows, "verdana.ttf", "Verdana");
    DoAnalyzeTTF(rows, "verdanab.ttf", "Verdana", true);
    DoAnalyzeTTF(rows, "verdanai.ttf", "Verdana", false);
    DoAnalyzeTTF(rows, "verdanaz.ttf", "Verdana", true, true);
    DoAnalyzeTTF(rows, "webdings.ttf", "Webdings");
    save_rows(rows);
    return 0;
}
