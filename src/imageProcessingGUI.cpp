// main.cpp
#include <wx/wx.h>
#include <wx/sizer.h>  // Necessário para gerenciar o layout com sizers

class MyApp : public wxApp {
public:
    virtual bool OnInit();
};

class MyFrame : public wxFrame {
public:
    MyFrame(const wxString& title);

private:
    void OnExit(wxCommandEvent& event);
    wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(wxID_EXIT, MyFrame::OnExit)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
    MyFrame* frame = new MyFrame("Image Processing App");
    frame->Show(true);
    return true;
}

MyFrame::MyFrame(const wxString& title) : wxFrame(NULL, wxID_ANY, title) {
    SetSize(800, 600);
    CreateStatusBar();
    SetStatusText("Welcome to Image Processing App!");

    // Menu
    wxMenu* menuFile = new wxMenu;
    menuFile->Append(wxID_EXIT, "&Exit\tCtrl-Q", "Quit this program");
    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    SetMenuBar(menuBar);

    // Layout
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Placeholder para a área da imagem
    wxStaticText* imageArea = new wxStaticText(this, wxID_ANY, "Image Display Area",
                                               wxDefaultPosition, wxSize(600, 400), wxALIGN_CENTER);
    mainSizer->Add(imageArea, 1, wxEXPAND | wxALL, 10);

    // Placeholder para os botões de filtro
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* filterButton1 = new wxButton(this, wxID_ANY, "Filter 1");
    wxButton* filterButton2 = new wxButton(this, wxID_ANY, "Filter 2");
    wxButton* filterButton3 = new wxButton(this, wxID_ANY, "Filter 3");
    buttonSizer->Add(filterButton1, 1, wxEXPAND | wxALL, 5);
    buttonSizer->Add(filterButton2, 1, wxEXPAND | wxALL, 5);
    buttonSizer->Add(filterButton3, 1, wxEXPAND | wxALL, 5);

    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxBOTTOM, 10);

    // Placeholder para o tempo de execução
    wxStaticText* execTimeLabel = new wxStaticText(this, wxID_ANY, "Execution Time: N/A",
                                                   wxDefaultPosition, wxSize(200, 30), wxALIGN_CENTER);
    mainSizer->Add(execTimeLabel, 0, wxALIGN_CENTER | wxALL, 10);

    SetSizer(mainSizer);
}

void MyFrame::OnExit(wxCommandEvent& event) {
    Close(true);
}
