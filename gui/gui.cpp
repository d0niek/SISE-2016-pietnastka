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
#include <wx/sizer.h>
#include <wx/timer.h>

#include <iostream>

using namespace std;

#endif

class BasicDrawPanel;

class RenderTimer : public wxTimer
{
    BasicDrawPanel *drawPanel;
public:
    RenderTimer(BasicDrawPanel *drawPanel);

    void Notify();

    void start();
};

class BasicDrawPanel : public wxPanel
{
    wxString fifteen[16];
    int emptyElement;
    wxSize fifteenSize;
    wxString solution;

    void DrawFifteen(wxDC &dc);

    void DrawGrid(wxDC &dc);

    void FillGrid(wxDC &dc);

    void SlideElement(wxUniChar direction);

public:
    BasicDrawPanel(wxFrame *parent);

    void paintEvent(wxPaintEvent &evt);

    void paintNow();

    void render(wxDC &dc);

    void SetFifteen(wxString fifteenElements[], int emptyElement, const wxSize &fifteenSize);

    void SetSolution(const wxString &solution);

DECLARE_EVENT_TABLE()
};

class MyFrame;

class MyApp : public wxApp
{
    bool OnInit();
};

class MyFrame : public wxFrame
{
    RenderTimer *timer;
    BasicDrawPanel *drawPanel;
    wxString openedFile;
    wxRadioBox *solutions = nullptr;
    wxButton *solveBtn = nullptr;
    bool solved;

    void OnOpen(wxCommandEvent &event);

    void OnExit(wxCommandEvent &event);

    void OnAbout(wxCommandEvent &event);

    void OnSolve(wxCommandEvent &event);

    void AddMenu();

    void ReadFileContent(wxFile *file);

    void ReadFifteenAndSendItToDrawPanel(wxString fifteen);

    void DrawSolutionsOptions();

    wxString LoadSolution();

public:
    MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

    ~MyFrame();

wxDECLARE_EVENT_TABLE();
};

enum
{
    ID_Load_file = 1,
    ID_RADIOBOX = 2,
    ID_SOLVE = 3,
};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)

                EVT_MENU(ID_Load_file, MyFrame::OnOpen)
                EVT_MENU(wxID_EXIT, MyFrame::OnExit)
                EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
                EVT_BUTTON(ID_SOLVE, MyFrame::OnSolve)

wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(MyApp);

RenderTimer::RenderTimer(BasicDrawPanel *drawPanel) :
        wxTimer()
{
    this->drawPanel = drawPanel;
}

void RenderTimer::Notify()
{
    this->drawPanel->Refresh();
}

void RenderTimer::start()
{
    wxTimer::Start(10);
}

bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame("Hello World", wxPoint(50, 50), wxSize(450, 340));

    frame->Show();

    return true;
}

BEGIN_EVENT_TABLE(BasicDrawPanel, wxPanel)

                EVT_PAINT(BasicDrawPanel::paintEvent)

END_EVENT_TABLE()

BasicDrawPanel::BasicDrawPanel(wxFrame *parent) :
        wxPanel(parent, wxID_ANY, wxPoint(30, 30), wxSize(120, 120))
{
    this->emptyElement = -1;
}

void BasicDrawPanel::paintEvent(wxPaintEvent &evt)
{
    wxPaintDC dc(this);
    render(dc);
}

void BasicDrawPanel::paintNow()
{
    wxClientDC dc(this);
    render(dc);
}

void BasicDrawPanel::render(wxDC &dc)
{
    dc.SetBackground(*wxLIGHT_GREY_BRUSH);
    dc.Clear();

    if (!this->solution.empty()) {
        this->SlideElement(this->solution[0]);

        if (this->solution.length() > 1) {
            this->solution = this->solution.substr(1);
        } else {
            this->solution.clear();
        }

        wxMilliSleep(300);
    }

    if (this->emptyElement >= 0) {
        this->DrawFifteen(dc);
    }
}

void BasicDrawPanel::DrawFifteen(wxDC &dc)
{
    this->DrawGrid(dc);
    this->FillGrid(dc);
}

void BasicDrawPanel::DrawGrid(wxDC &dc)
{
    for (int i = 0; i < this->fifteenSize.x - 1; i++) {
        int y = 30 + (30 * i);
        dc.DrawLine(wxPoint(0, y), wxPoint(30 * this->fifteenSize.y, y));
    }

    for (int i = 0; i < this->fifteenSize.y - 1; i++) {
        int x = 30 + (30 * i);
        dc.DrawLine(wxPoint(x, 0), wxPoint(x, 30 * this->fifteenSize.x));
    }
}

void BasicDrawPanel::FillGrid(wxDC &dc)
{
    int size = this->fifteenSize.x * this->fifteenSize.y;

    for (int i = 0; i < size; i++) {
        if (this->fifteen[i].compare("0") != 0) {
            int x = 7 + 30 * (i % this->fifteenSize.y);
            int y = 7 + 30 * (i / this->fifteenSize.y);
            dc.DrawText(this->fifteen[i], wxPoint(x, y));
        }
    }
}

void BasicDrawPanel::SlideElement(wxUniChar direction)
{
    int element;

    if (direction == 'l') {
        element = this->emptyElement - 1;
    } else if (direction == 'u') {
        element = this->emptyElement - this->fifteenSize.GetY();
    } else if (direction == 'r') {
        element = this->emptyElement + 1;
    } else if (direction == 'd') {
        element = this->emptyElement + this->fifteenSize.GetY();
    }

    this->fifteen[this->emptyElement] = this->fifteen[element];
    this->emptyElement = element;
    this->fifteen[this->emptyElement] = "0";
}

void BasicDrawPanel::SetFifteen(wxString fifteen[], int emptyElement, const wxSize &fifteenSize)
{
    for (int i = 0; i < 16; i++) {
        this->fifteen[i] = fifteen[i];
    }
    this->emptyElement = emptyElement;
    this->fifteenSize = fifteenSize;
}

void BasicDrawPanel::SetSolution(const wxString &solution)
{
    this->solution = solution;
}

MyFrame::MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
        : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    this->drawPanel = new BasicDrawPanel(this);

    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(this->drawPanel);

    this->SetSizer(sizer);
    this->SetAutoLayout(true);

    this->timer = new RenderTimer(this->drawPanel);
    this->timer->start();

    this->AddMenu();

    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");
}

MyFrame::~MyFrame()
{
    delete this->timer;
    delete this->solutions;
    delete this->solveBtn;
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

void MyFrame::OnOpen(wxCommandEvent &event)
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

            this->solved = false;
        } else {
            wxLogError("Cannot open file '%s'.", fileDialog.GetPath());
        }
    }
}

void MyFrame::ReadFileContent(wxFile *file)
{
    wxString fileContent;
    file->ReadAll(&fileContent);

    this->ReadFifteenAndSendItToDrawPanel(fileContent);
    this->DrawSolutionsOptions();
}

void MyFrame::ReadFifteenAndSendItToDrawPanel(wxString fileContent)
{
    wxString fifteenElements[16];
    int emptyElement = -1;

    int x = wxAtoi(fileContent[0]);
    int y = wxAtoi(fileContent[2]);

    wxString fifteen = fileContent.substr(4);
    fifteen.Replace("\n", " ");

    int startPos = 0;
    int endPos;

    for (int i = 0; i < x * y; i++) {
        endPos = fifteen.find(' ', startPos);

        wxString fieldValue = fifteen.substr(startPos, endPos - startPos);
        fifteenElements[i] = fieldValue;

        if (fieldValue.compare("0") == 0) {
            emptyElement = i;
        }

        startPos = endPos + 1;
    }

    this->drawPanel->SetFifteen(fifteenElements, emptyElement, wxSize(x, y));
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
    this->timer->Stop();
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
    if (!this->solved) {
        wxString solution = this->LoadSolution();
        this->drawPanel->SetSolution(solution);

        this->solved = true;
    }
}

wxString MyFrame::LoadSolution()
{
    wxString option = this->solutions->GetStringSelection();

    wxString solutionFile("../out/");
    solutionFile << this->openedFile << "_" << option.Lower() << ".out";

    wxFileInputStream inputStream(solutionFile);

    if (inputStream.IsOk()) {
        wxFile *file = inputStream.GetFile();
        wxString solution;
        file->ReadAll(&solution);

        int endlPos = solution.find("\n");

        return solution.substr(endlPos + 1);
    } else {
        wxLogError("Cannot open file '%s'.", solutionFile);
    }
}
