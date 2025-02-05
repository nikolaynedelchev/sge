#pragma once
#include "TextEditor/TextEditor.h"
#include <raylib.h>
#include <map>
#include "ImageViewer.h"
namespace ndn::rssedit
{

class RssEdit
{
public:
    int Run();
private:
    void DrawTextEditor();
    void DrawRssBrowser();
    void DrawRssRend();
    void Draw();
    void Loop();
    void LoadImage(const std::string& fileName);

    float m_rssSplitterY = 400.0f;
    TextEditor m_textEditor;
    std::map<std::string, RenderTexture2D> m_rssTextures;
    ImageViewer m_imgViewer;
};

}
