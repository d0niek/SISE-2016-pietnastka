// wxWidgets "Hello world" Program
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifndef WX_PRECOMP

#include <wx/wx.h>
#include <wx/filedlg.h>
#include <wx/wfstream.h>
#include <wx/file.h>
#include <wx/dcclient.h>
#include <wx/string.h>

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

private:
    void AddMenu();

    void OpenFile(wxCommandEvent &event);

    void ReadFileContent(wxFile *file);

    void OnExit(wxCommandEvent &event);

    void OnAbout(wxCommandEvent &event);

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
    return true;
}

MyFrame::MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
        : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    this->AddMenu();

    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");
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
