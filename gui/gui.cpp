#include <wx/wxprec.h>

#ifndef WX_PRECOMP

#include <wx/wx.h>
#include <wx/filedlg.h>
#include <wx/wfstream.h>
#include <wx/file.h>
#include <wx/dcclient.h>
#include <wx/string.h>
#include <wx/button.h>

#include <iostream>

using namespace std;

#endif

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

    ~MyFrame();

    void Init();

private:
    wxClientDC *dc = nullptr;

    wxButton *(fifteenElements[16]);

    void AddMenu();

    void OpenFile(wxCommandEvent &event);

    void ReadFileContent(wxFile *file);

    void DrawFifteen(int x, int y, wxString fifteen);

    void EraseFifteen();

    void OnExit(wxCommandEvent &event);

    void OnAbout(wxCommandEvent &event);

    void DrawTextString(const wxString &text, const wxPoint &pt);

wxDECLARE_EVENT_TABLE();
};

enum
{
    ID_Load_file = 1
};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)

                EVT_MENU(ID_Load_file, MyFrame::OpenFile)
                EVT_MENU(wxID_EXIT, MyFrame::OnExit)
                EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)

wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame("Hello World", wxPoint(50, 50), wxSize(450, 340));
    frame->Show(true);

    frame->Init();

    return true;
}

MyFrame::MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
        : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    for (int i = 0; i < 16; i++) {
        this->fifteenElements[i] = nullptr;
    }

    this->AddMenu();

    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");
}

MyFrame::~MyFrame()
{
    if (this->dc) {
        delete this->dc;
    }
}

void MyFrame::Init()
{
    this->dc = new wxClientDC(this);
}

void MyFrame::AddMenu()
{
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(wxFD_OPEN, "&Open..\tCtrl-O", "Load file with data");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar(menuBar);
}

void MyFrame::OpenFile(wxCommandEvent &event)
{
    wxFileDialog fileDialog(
            this,
            "Open input file",
            "../tests",
            "",
            "Input files (*.in)|*.in",
            wxFD_OPEN | wxFD_FILE_MUST_EXIST
    );

    if (fileDialog.ShowModal() == wxID_OK) {
        wxFileInputStream inputStream(fileDialog.GetPath());

        if (inputStream.IsOk()) {
            this->ReadFileContent(inputStream.GetFile());
        } else {
            wxLogError("Cannot open file '%s'.", fileDialog.GetPath());
        }
    }
}

void MyFrame::ReadFileContent(wxFile *file)
{
    wxString fileContent;

    file->ReadAll(&fileContent);

    int x = wxAtoi(fileContent[0]);
    int y = wxAtoi(fileContent[2]);

    wxString fifteen = fileContent.substr(4);
    fifteen.Replace("\n", " ");

    this->DrawFifteen(x, y, fifteen);
}

void MyFrame::DrawFifteen(int x, int y, wxString fifteen)
{
    this->EraseFifteen();

    int startPos = 0;
    int endPos;

    for (int i = 0; i < x * y; i++) {
        endPos = fifteen.find(' ', startPos);

        wxString fieldValue = fifteen.substr(startPos, endPos - startPos);

        if (fieldValue.compare("0") != 0) {
            wxButton *element = new wxButton(
                    this,
                    wxID_ANY,
                    fieldValue,
                    wxPoint(30 * (i % y), 30 * (i / y)),
                    wxSize(30, 30)
            );

            this->fifteenElements[i] = element;
        }

        startPos = endPos + 1;
    }
}

void MyFrame::EraseFifteen()
{
    for (int i = 0; i < 16; i++) {
        delete this->fifteenElements[i];

        this->fifteenElements[i] = nullptr;
    }
}

void MyFrame::OnExit(wxCommandEvent &event)
{
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent &event)
{
    wxMessageBox(
            "This is a wxWidgets' Hello world sample",
            "About",
            wxOK | wxICON_INFORMATION
    );
}

void MyFrame::DrawTextString(const wxString &text, const wxPoint &pt)
{
    wxFont font(12, wxFONTFAMILY_SWISS, wxNORMAL, wxBOLD);
    this->dc->SetFont(font);
    this->dc->SetBackgroundMode(wxTRANSPARENT);
    this->dc->SetTextForeground(*wxBLACK);
    this->dc->SetTextBackground(*wxWHITE);
    this->dc->DrawText(text, pt);
}
