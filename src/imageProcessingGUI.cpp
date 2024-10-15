#include <wx/wx.h>
#include <wx/image.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h" 

using namespace std;

class MyFrame : public wxFrame {
public:
    MyFrame();
    ~MyFrame();

private:
    void OnOpen(wxCommandEvent& event);
    void OnFilter1(wxCommandEvent& event);  // Função de filtro 1
    void OnFilter2(wxCommandEvent& event);  // Função de filtro 2
    void OnFilter3(wxCommandEvent& event);  // Função de filtro 3
    void OnRevert(wxCommandEvent& event);   // Função para reverter ao original
    wxBitmap LoadImage(const wxString& filename);
    void ClearImageData();  // Libera a memória da imagem quando necessário

    unsigned char* m_imageData;           // Armazena os dados brutos da imagem original
    unsigned char* m_filteredImageData;   // Dados da imagem filtrada
    int m_imgWidth, m_imgHeight, m_imgChannels;  // Largura, altura e canais da imagem

    wxStaticBitmap* m_imageDisplay;   // Widget para exibir a imagem
    wxStaticText* m_filePathLabel;    // Label para exibir o caminho do arquivo

    wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(wxID_OPEN, MyFrame::OnOpen)
    EVT_BUTTON(1001, MyFrame::OnFilter1)
    EVT_BUTTON(1002, MyFrame::OnFilter2)
    EVT_BUTTON(1003, MyFrame::OnFilter3)
    EVT_BUTTON(1004, MyFrame::OnRevert) 
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
    : wxFrame(nullptr, wxID_ANY, "Impact Lab: Image Processing GUI", wxDefaultPosition, wxSize(1200, 800)),
      m_imageData(nullptr), m_filteredImageData(nullptr), m_imgWidth(0), m_imgHeight(0), m_imgChannels(0) {

    wxMenuBar* menuBar = new wxMenuBar();
    wxMenu* menuFile = new wxMenu();
    menuFile->Append(wxID_OPEN, "&Open\tCtrl-O", "Open an image file");
    menuBar->Append(menuFile, "&File");
    SetMenuBar(menuBar);

    // Painel e layout
    wxPanel* panel = new wxPanel(this);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    // Botão "Abrir Imagem"
    wxButton* openButton = new wxButton(panel, wxID_ANY, "Abrir Imagem", wxDefaultPosition, wxSize(120, 30));
    openButton->Bind(wxEVT_BUTTON, &MyFrame::OnOpen, this);
    sizer->Add(openButton, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

    // Label para o caminho do arquivo
    m_filePathLabel = new wxStaticText(panel, wxID_ANY, "Caminho do arquivo: ", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    sizer->Add(m_filePathLabel, 0, wxALIGN_CENTER | wxBOTTOM, 10);

    // Área de visualização de imagem de tamanho fixo usando wxStaticBitmap
    m_imageDisplay = new wxStaticBitmap(panel, wxID_ANY, wxBitmap(), wxDefaultPosition, wxSize(600, 400));
    sizer->Add(m_imageDisplay, 0, wxALIGN_CENTER | wxALL, 10);

    // Botões para filtros
    wxBoxSizer* filterSizer = new wxBoxSizer(wxHORIZONTAL);
    
    wxButton* filterButton1 = new wxButton(panel, 1001, "Filtro 1", wxDefaultPosition, wxSize(100, 30));
    wxButton* filterButton2 = new wxButton(panel, 1002, "Filtro 2", wxDefaultPosition, wxSize(100, 30));
    wxButton* filterButton3 = new wxButton(panel, 1003, "Filtro 3", wxDefaultPosition, wxSize(100, 30));
    wxButton* revertButton = new wxButton(panel, 1004, "Reverter", wxDefaultPosition, wxSize(150, 30));
    
    filterSizer->Add(filterButton1, 0, wxALIGN_CENTER | wxRIGHT, 5);
    filterSizer->Add(filterButton2, 0, wxALIGN_CENTER | wxRIGHT, 5);
    filterSizer->Add(filterButton3, 0, wxALIGN_CENTER | wxRIGHT, 5);
    filterSizer->Add(revertButton, 0, wxALIGN_CENTER);

    sizer->Add(filterSizer, 0, wxALIGN_CENTER | wxTOP, 10);

    panel->SetSizer(sizer);
}

MyFrame::~MyFrame() {
    ClearImageData();
}

void MyFrame::ClearImageData() {
    if (m_imageData) {
        stbi_image_free(m_imageData);
        m_imageData = nullptr;
    }
    if (m_filteredImageData) {
        free(m_filteredImageData);
        m_filteredImageData = nullptr;
    }
}

wxBitmap MyFrame::LoadImage(const wxString& filename) {
    ClearImageData();  // Limpa dados anteriores, se existirem

    m_imageData = stbi_load(filename.ToStdString().c_str(), &m_imgWidth, &m_imgHeight, &m_imgChannels, 0);

    if (m_imageData) {
        wxImage img(m_imgWidth, m_imgHeight, m_imageData, true);
        img.Rescale(600, 400);  // Redimensiona para caber no display da GUI
        return wxBitmap(img);
    } else {
        wxLogError("Falha ao carregar a imagem: %s", filename);
        return wxBitmap();
    }
}

void MyFrame::OnOpen(wxCommandEvent& event) {
    wxFileDialog openFileDialog(this, "Open image file", "", "", 
        "Image files (*.jpg;*.jpeg;*.png)|*.jpg;*.jpeg;*.png", 
        wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_OK) {
        wxString path = openFileDialog.GetPath();
        wxBitmap bitmap = LoadImage(path);
        
        if (bitmap.IsOk()) {
            m_imageDisplay->SetBitmap(bitmap);
            m_filePathLabel->SetLabel("Caminho do arquivo: " + path);
        }
    }
}

void MyFrame::OnFilter1(wxCommandEvent& event) {
    if (!m_imageData) {
        wxLogError("Nenhuma imagem carregada.");
        return;
    }

    m_filteredImageData = (unsigned char*)malloc(m_imgWidth * m_imgHeight * m_imgChannels);
    if (!m_filteredImageData) {
        wxLogError("Falha ao alocar memória para imagem filtrada.");
        return;
    }

    memcpy(m_filteredImageData, m_imageData, m_imgWidth * m_imgHeight * m_imgChannels);

    for (int i = 1; i < m_imgWidth * m_imgHeight * m_imgChannels; i += m_imgChannels) {
        m_filteredImageData[i] = 255;  // Define o canal G (verde) para o valor máximo
    }

    wxImage filteredImg(m_imgWidth, m_imgHeight, m_filteredImageData, true);
    m_imageDisplay->SetBitmap(wxBitmap(filteredImg));
}

void MyFrame::OnFilter2(wxCommandEvent& event) {
    if (!m_imageData) {
        wxLogError("Nenhuma imagem carregada.");
        return;
    }

    m_filteredImageData = (unsigned char*)malloc(m_imgWidth * m_imgHeight * m_imgChannels);
    if (!m_filteredImageData) {
        wxLogError("Falha ao alocar memória para imagem filtrada.");
        return;
    }

    memcpy(m_filteredImageData, m_imageData, m_imgWidth * m_imgHeight * m_imgChannels);

    // Aplicar filtro de escala de cinza
    for (int i = 0; i < m_imgWidth * m_imgHeight * m_imgChannels; i += m_imgChannels) {
        unsigned char gray = static_cast<unsigned char>(
            0.2126 * m_filteredImageData[i] + 0.7152 * m_filteredImageData[i + 1] + 0.0722 * m_filteredImageData[i + 2]
        );
        m_filteredImageData[i] = gray;        // Vermelho
        m_filteredImageData[i + 1] = gray;    // Verde
        m_filteredImageData[i + 2] = gray;    // Azul
    }

    wxImage filteredImg(m_imgWidth, m_imgHeight, m_filteredImageData, true);
    m_imageDisplay->SetBitmap(wxBitmap(filteredImg));
}

void MyFrame::OnFilter3(wxCommandEvent& event) {
    wxLogMessage("Filtro 3 aplicado.");
}

// Função para reverter a imagem ao original
void MyFrame::OnRevert(wxCommandEvent& event) {
    if (!m_imageData) {
        wxLogError("Nenhuma imagem carregada.");
        return;
    }

    wxImage originalImg(m_imgWidth, m_imgHeight, m_imageData, true);
    m_imageDisplay->SetBitmap(wxBitmap(originalImg));
}
