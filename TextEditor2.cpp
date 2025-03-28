#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Output.H>
#include <fstream>

class EditorWindow : public Fl_Window {
public:
    Fl_Text_Editor *editor;
    Fl_Text_Buffer *textbuf;
    Fl_Menu_Bar *menu;
    Fl_Output *status_bar;
    std::string filename;

    EditorWindow(int w, int h, const char* title = "FLTK Text Editor")
        : Fl_Window(w, h, title) {

        textbuf = new Fl_Text_Buffer();
        editor = new Fl_Text_Editor(0, 30, w, h - 60);
        editor->buffer(textbuf);

        menu = new Fl_Menu_Bar(0, 0, w, 30);
        menu->add("&File/&New",  FL_CTRL + 'n', new_cb, this);
        menu->add("&File/&Open", FL_CTRL + 'o', open_cb, this);
        menu->add("&File/&Save", FL_CTRL + 's', save_cb, this);
        menu->add("&File/&Quit", FL_CTRL + 'q', quit_cb, this);

        menu->add("&Edit/&Undo", FL_CTRL + 'z', undo_cb, this);
        menu->add("&Edit/&Copy", FL_CTRL + 'c', copy_cb, this);
        menu->add("&Edit/&Paste", FL_CTRL + 'v', paste_cb, this);
        menu->add("&Edit/&Cut", FL_CTRL + 'x', cut_cb, this);

        status_bar = new Fl_Output(0, h - 30, w, 30);
        status_bar->value("Ready");

        end();
    }

    static void new_cb(Fl_Widget*, void* v) {
        EditorWindow* win = (EditorWindow*)v;
        win->textbuf->text("");
        win->filename = "";
        win->update_status("New File");
    }

    static void open_cb(Fl_Widget*, void* v) {
        EditorWindow* win = (EditorWindow*)v;
        const char* newfile = fl_file_chooser("Open File", "*.*", NULL);
        if (newfile) {
            std::ifstream file(newfile);
            if (file) {
                std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                win->textbuf->text(content.c_str());
                win->filename = newfile;
                win->update_status(newfile);
            }
        }
    }

    static void save_cb(Fl_Widget*, void* v) {
        EditorWindow* win = (EditorWindow*)v;
        if (win->filename.empty()) {
            const char* newfile = fl_file_chooser("Save As", "*.*", NULL);
            if (newfile) {
                win->filename = newfile;
            } else {
                return;
            }
        }
        std::ofstream file(win->filename);
        if (file) {
            file << win->textbuf->text();
            win->update_status("Saved: " + win->filename);
        } else {
            fl_alert("Failed to save file!");
        }
    }

    static void quit_cb(Fl_Widget*, void* v) {
        if (fl_ask("Are you sure you want to quit?")) {
            exit(0);
        }
    }

    static void undo_cb(Fl_Widget*, void* v) { ((EditorWindow*)v)->textbuf->undo(); }
    static void copy_cb(Fl_Widget*, void* v) { ((EditorWindow*)v)->editor->copy(); }
    static void paste_cb(Fl_Widget*, void* v) { ((EditorWindow*)v)->editor->paste(); }
    static void cut_cb(Fl_Widget*, void* v) { ((EditorWindow*)v)->editor->cut(); }

    void update_status(const std::string& text) {
        status_bar->value(text.c_str());
    }
};

int main(int argc, char** argv) {
    EditorWindow win(800, 600);
    win.show(argc, argv);
    return Fl::run();
}
