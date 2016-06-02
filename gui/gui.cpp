#include <wx/wxprec.h>

#ifndef WX_PRECOMP

#include <wx/wx.h>
#include <wx/filedlg.h>
#include <wx/wfstream.h>
#include <wx/file.h>
#include <wx/dcclient.h>
#include <wx/string.h>
#include <wx/button.h>
#include <wx/radiobox.h>

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

    wxString openedFile;

    wxSize fifteenSize;

    wxButton *(fifteenElements[16]);

    int emptyElementPos;

    wxRadioBox *solutions = nullptr;

    wxButton *solveBtn = nullptr;

    void AddMenu();

    void OpenFile(wxCommandEvent &event);

    void ReadFileContent(wxFile *file);

    void DrawFifteen(wxString fifteen);

    void EraseFifteen();

    void DrawSolutionsOptions();

    void OnExit(wxCommandEvent &event);

    void OnAbout(wxCommandEvent &event);

    void OnSolve(wxCommandEvent &event);

    void Solve(wxFile *file);

    void SlideElement(wxUniChar direction);

    void DrawTextString(const wxString &text, const wxPoint &pt);

wxDECLARE_EVENT_TABLE();
};

enum
{
    ID_Load_file = 1,
    ID_RADIOBOX = 2,
    ID_SOLVE = 3,
};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)

                EVT_MENU(ID_Load_file, MyFrame::OpenFile)
                EVT_MENU(wxID_EXIT, MyFrame::OnExit)
                EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
                EVT_BUTTON(ID_SOLVE, MyFrame::OnSolve)

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
    delete this->dc;
    delete this->solutions;
    delete this->solveBtn;
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
    menuHelp->Append(wxID_ABOUT, "Help\tF1");

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

        this->openedFile = fileDialog.GetFilename();
        this->openedFile.Replace(".in", "");

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

    this->fifteenSize.Set(x, y);

    wxString fifteen = fileContent.substr(4);
    fifteen.Replace("\n", " ");

    this->DrawFifteen(fifteen);
    this->DrawSolutionsOptions();
}

void MyFrame::DrawFifteen(wxString fifteen)
{
    this->EraseFifteen();

    int startPos = 0;
    int endPos;

    for (int i = 0; i < this->fifteenSize.GetX() * this->fifteenSize.GetY(); i++) {
        endPos = fifteen.find(' ', startPos);

        wxString fieldValue = fifteen.substr(startPos, endPos - startPos);

        if (fieldValue.compare("0") != 0) {
            wxButton *element = new wxButton(
                    this,
                    wxID_ANY,
                    fieldValue,
                    wxPoint(30 * (i % this->fifteenSize.GetY()), 30 * (i / this->fifteenSize.GetY())),
                    wxSize(30, 30)
            );

            this->fifteenElements[i] = element;
        } else{
            this->emptyElementPos = i;
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

void MyFrame::DrawSolutionsOptions()
{
    wxArrayString strings;
    strings.Add(wxT("BFS"));
    strings.Add(wxT("DFS"));
    strings.Add(wxT("iDFS"));

    this->solutions = new wxRadioBox(
            this,
            ID_RADIOBOX,
            wxT("Solutions options"),
            wxPoint(0, 30 * 5),
            wxDefaultSize,
            strings
    );

    this->solveBtn = new wxButton(
            this,
            ID_SOLVE,
            "Solve",
            wxPoint(0, this->solutions->GetPosition().y + this->solutions->GetSize().GetHeight() + 5)
    );
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

void MyFrame::OnSolve(wxCommandEvent &event)
{
    wxString option = this->solutions->GetStringSelection();
    wxString solutionFile("../out/");

    solutionFile << this->openedFile << "_" << option.Lower() << ".out";

    wxFileInputStream inputStream(solutionFile);

    if (inputStream.IsOk()) {
        this->Solve(inputStream.GetFile());
    } else {
        wxLogError("Cannot open file '%s'.", solutionFile);
    }
}

void MyFrame::Solve(wxFile *file)
{
    wxString fileContent;
    file->ReadAll(&fileContent);

    // Skip solution size
    int endlPos = fileContent.find("\n");
    wxString solution = fileContent.substr(endlPos + 1);

    for (int i = 0; i < solution.length(); i++) {
        this->SlideElement(solution[i]);
    }
}

void MyFrame::SlideElement(wxUniChar direction)
{
    int element;
    int x = 0, y = 0;

    if (direction == 'l') {
        element = this->emptyElementPos - 1;
        x = 30;
    } else if (direction == 'u') {
        element = this->emptyElementPos - this->fifteenSize.GetY();
        y = 30;
    } else if (direction == 'r') {
        element = this->emptyElementPos + 1;
        x = -30;
    } else if (direction == 'd') {
        element = this->emptyElementPos + this->fifteenSize.GetY();
        y = -30;
    }

    wxPoint elementPos = this->fifteenElements[element]->GetPosition();
    this->fifteenElements[element]->Move(elementPos.x + x, elementPos.y + y);

    this->fifteenElements[this->emptyElementPos] = this->fifteenElements[element];
    this->fifteenElements[element] = nullptr;

    this->emptyElementPos = element;
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
