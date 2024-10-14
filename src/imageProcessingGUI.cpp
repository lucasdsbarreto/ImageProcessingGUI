#include <wx/wx.h>
#include <wx/image.h>
#include <wx/dcclient.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class MyFrame : public wxFrame {
public:
    MyFrame();

private:
    void OnOpen(wxCommandEvent& event);
    void OnPaint(wxPaintEvent& event);
    wxBitmap LoadImage(const wxString& filename);

    wxBitmap m_bitmap;              // Bitmap para armazenar a imagem carregada
    wxStaticText* m_filePathLabel;  // Label para exibir o caminho do arquivo

    wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(wxID_OPEN, MyFrame::OnOpen)
    EVT_PAINT(MyFrame::OnPaint)
wxEND_EVENT_TABLE()

class MyApp : public wxApp {
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
    MyFrame* frame = new MyFrame();
    frame->Show(true);
    return true;
}

MyFrame::MyFrame()
    : wxFrame(nullptr, wxID_ANY, "Impact Lab: Image Processing GUI", wxDefaultPosition, wxSize(1200, 800)) {
    wxMenuBar* menuBar = new wxMenuBar();
    wxMenu* menuFile = new wxMenu();
    menuFile->Append(wxID_OPEN, "&Open\tCtrl-O", "Open an image file");
    menuBar->Append(menuFile, "&File");
    SetMenuBar(menuBar);

    // Painel para organizar o layout
    wxPanel* panel = new wxPanel(this);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    // Adicionando um botão "Abrir Imagem"
    wxButton* openButton = new wxButton(panel, wxID_ANY, "Abrir Imagem", wxDefaultPosition, wxSize(120, 30));
    openButton->Bind(wxEVT_BUTTON, &MyFrame::OnOpen, this);
    
    sizer->Add(openButton, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);
    
    // Adicionando um label para mostrar o caminho do arquivo
    m_filePathLabel = new wxStaticText(panel, wxID_ANY, "Caminho do arquivo: ", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    sizer->Add(m_filePathLabel, 0, wxALIGN_CENTER | wxBOTTOM, 10);

    panel->SetSizer(sizer);
}

wxBitmap MyFrame::LoadImage(const wxString& filename) {
    int width, height, channels;
    unsigned char* data = stbi_load(filename.ToStdString().c_str(), &width, &height, &channels, 0);

    if (data) {
        wxImage img(width, height, data, true);
        stbi_image_free(data);
        return wxBitmap(img);  // Converte wxImage para wxBitmap
    } else {
        wxLogError("Failed to load image: %s", filename);
        return wxBitmap();
    }
}

void MyFrame::OnOpen(wxCommandEvent& event) {
    wxFileDialog openFileDialog(this, "Open image file", "", "", 
        "Image files (*.jpg;*.jpeg;*.png)|*.jpg;*.jpeg;*.png", 
        wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_OK) {
        wxString path = openFileDialog.GetPath();
        m_bitmap = LoadImage(path);  // Carrega a imagem e a armazena em m_bitmap
        
        // Atualiza o label com o caminho do arquivo
        m_filePathLabel->SetLabel("Caminho do arquivo: " + path);
        
        Refresh();  // Redesenha a janela para exibir a nova imagem
    }
}

void MyFrame::OnPaint(wxPaintEvent& event) {
    wxPaintDC dc(this);
    if (m_bitmap.IsOk()) {
        // Obtém as dimensões da janela e da imagem
        int windowWidth, windowHeight;
        GetClientSize(&windowWidth, &windowHeight);
        int imgWidth = m_bitmap.GetWidth();
        int imgHeight = m_bitmap.GetHeight();

        int x = (windowWidth - imgWidth) / 2;  
        int y = 50 + 30 + 10;  

        dc.DrawBitmap(m_bitmap, x, y, false);  
    }
}
