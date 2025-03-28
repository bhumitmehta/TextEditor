#include<iostream>
#include<fstream>
#include<string>
#include<cstdlib>
#include<ctime>
#include<sstream>
#include<cmath>
#include<cstring>
#include<cstdio>
#include<cstdlib>
#include<FL/Fl.H>
#include<FL/Fl_Double_Window.H>
#include<FL/Fl_Text_Editor.H>
#include<FL/Fl_Input.H>
#include<FL/Fl_Button.H>
#include<FL/Fl_Return_Button.H>
#include<FL/Fl_Menu_Item.H>
#include<FL/Fl_Menu_Bar.H>
#include<FL/Fl_File_Chooser.H>
#include<FL/Fl_Text_Buffer.H>
#include<FL/Fl_Text_Display.H>
#include<FL/Fl_Text_Editor.H>
//include entire FLTK library in one include an one go
using namespace std;


int changed = 0;
char filename[256] = "";
Fl_Text_Buffer *textbuf;
char title[256] = "Untitled";


void new_cb(Fl_Widget*,void*);
void open_cb(Fl_Widget*,void*);
void insert_cb(Fl_Widget*,void*);
void save_cb(void);     
void saveas_cb(void);
void save_file(const char*);
void view_cb(Fl_Widget*,void*);
void find2_cb(Fl_Widget*,void*);
void find_cb(Fl_Widget*,void*);
void replace2_cb(Fl_Widget*,void*);
void replace_cb(Fl_Widget*,void*);
void paste_cb(Fl_Widget*,void*);
void cut_cb(Fl_Widget*,void*);
void copy_cb(Fl_Widget*,void*);
void close_cb(void);
void delete_cb(Fl_Widget*,void*);
void undo_cb(Fl_Widget*,void*);
void quit_cb(Fl_Widget*,void*);
int prompt(const char*);
int check_save(void);
void load_file(char*,int);
void set_title(Fl_Window* w);
void save_file(char *newfile);

int loading = 0;
class EditorWindow : public Fl_Double_Window {
    public:
      EditorWindow(int w, int h, const char* t) : Fl_Double_Window(w, h, t) {
          replace_dlg = NULL;
          replace_find = NULL;
          replace_with = NULL;
          replace_all = NULL;
          replace_next = NULL;
          replace_cancel = NULL;
          editor = NULL;
          search[0] = '\0';
      }
      ~EditorWindow() { }
  
      Fl_Window          *replace_dlg;
      Fl_Input           *replace_find;
      Fl_Input           *replace_with;
      Fl_Button          *replace_all;
      Fl_Return_Button   *replace_next;
      Fl_Button          *replace_cancel;
  
      Fl_Text_Editor     *editor;
      char               search[256];
  };

  Fl_Window *replace_dlg = new Fl_Window(300, 105, "Replace");
  Fl_Input *replace_find = new Fl_Input(70, 10, 200, 25, "Find:");
  Fl_Input *replace_with = new Fl_Input(70, 40, 200, 25, "Replace:");
  Fl_Button *replace_all = new Fl_Button(10, 70, 90, 25, "Replace All");
  Fl_Button *replace_next = new Fl_Button(105, 70, 120, 25, "Replace Next");
  Fl_Button *replace_cancel = new Fl_Button(230, 70, 60, 25, "Cancel");

  Fl_Menu_Item menuitems[] = {
    { "&File",              0, 0, 0, FL_SUBMENU },
      { "&New File",        0, (Fl_Callback *)new_cb },
      { "&Open File...",    FL_CTRL + 'o', (Fl_Callback *)open_cb },
      // { "&Insert File...",  FL_CTRL + 'i', (Fl_Callback *)insert_cb, 0, FL_MENU_DIVIDER },
      { "&Save File",       FL_CTRL + 's', (Fl_Callback *)save_cb },
      { "Save File &As...", FL_CTRL + FL_SHIFT + 's', (Fl_Callback *)saveas_cb, 0, FL_MENU_DIVIDER },
      // { "New &View", FL_ALT + 'v', (Fl_Callback *)view_cb, 0 },
      // { "&Close View", FL_CTRL + 'w', (Fl_Callback *)close_cb, 0, FL_MENU_DIVIDER },
      { "E&xit", FL_CTRL + 'q', (Fl_Callback *)quit_cb, 0 },
      { 0 },
  
    { "&Edit", 0, 0, 0, FL_SUBMENU },
      // { "&Undo",       FL_CTRL + 'z', (Fl_Callback *)undo_cb, 0, FL_MENU_DIVIDER },
      { "Cu&t",        FL_CTRL + 'x', (Fl_Callback *)cut_cb },
      { "&Copy",       FL_CTRL + 'c', (Fl_Callback *)copy_cb },
      { "&Paste",      FL_CTRL + 'v', (Fl_Callback *)paste_cb },
      { "&Delete",     0, (Fl_Callback *)delete_cb },
      { 0 },
  
    { "&Search", 0, 0, 0, FL_SUBMENU },
      { "&Find...",       FL_CTRL + 'f', (Fl_Callback *)find_cb },
      { "F&ind Again",    FL_CTRL + 'g', find2_cb },
      { "&Replace...",    FL_CTRL + 'r', replace_cb },
      { "Re&place Again", FL_CTRL + 't', replace2_cb },
      { 0 },
  
    { 0 }
  };

//   void set_title(Fl_Window* w) {
//     if (filename[0] == '\0') strcpy(title, "Untitled");
//     else {
//       char *slash;
//       slash = strrchr(filename, '/');
//   #ifdef WIN32
//       if (slash == NULL) slash = strrchr(filename, '\\');
//   #endif
//       if (slash != NULL) strcpy(title, slash + 1);
//       else strcpy(title, filename);
//     }
  
//     if (changed) strcat(title, " (modified)");
  
//     w->label(title);
//   }

  void save_file(char *newfile) {
    if (textbuf->savefile(newfile))
      fl_alert("Error writing to file \'%s\':\n%s.", newfile, strerror(errno));
    else
      strcpy(filename, newfile);
    changed = 0;
    textbuf->call_modify_callbacks();
  }

  
void load_file(char *newfile, int ipos) {
  cout<<"Load file"<<endl;
  loading = 1;
  int insert = (ipos != -1);
  changed = insert;
  if (!insert) strcpy(filename, "");
  int r;
  cout<<"Load file 1"<<endl;
  if (!insert){
    cout<<"Load file 2"<<endl;
     r = textbuf->loadfile(newfile);
  }
  else r = textbuf->insertfile(newfile, ipos);
  cout<<"Load file 2"<<endl;
  if (r){
    fl_alert("Error reading from file \'%s\':\n%s.", newfile, strerror(errno));
    cout<<"Load file 2"<<endl;
  }
  else
    if (!insert) strcpy(filename, newfile);
  cout<<"Load file 3"<<endl;
  loading = 0;
  cout<<"Load file 4"<<endl;
  textbuf->call_modify_callbacks();
}

  int check_save(void) {
    if (!changed) return 1;
  
    int r = fl_choice("The current file has not been saved.\n"
                      "Would you like to save it now?",
                      "Cancel", "Save", "Discard");
  
    if (r == 1) {
      save_cb(); // Save the file...
      return !changed;
    }
  
    return (r == 2) ? 1 : 0;
  }

  void saveas_cb(void) {
    char *newfile;
  
    newfile = fl_file_chooser("Save File As?", "*", filename);
    if (newfile != NULL) save_file(newfile);
  }

  void save_cb(void) {
    if (filename[0] == '\0') {
      // No filename - get one!
      saveas_cb();
      return;
    }
    else save_file(filename);
  }

  void replcan_cb(Fl_Widget*, void* v) {
    EditorWindow* e = (EditorWindow*)v;
    e->replace_dlg->hide();
  }

  void replall_cb(Fl_Widget*, void* v) {
    EditorWindow* e = (EditorWindow*)v;
    const char *find = e->replace_find->value();
    const char *replace = e->replace_with->value();
  
    find = e->replace_find->value();
    if (find[0] == '\0') {
      // Search string is blank; get a new one...
      e->replace_dlg->show();
      return;
    }
  
    e->replace_dlg->hide();
  
    e->editor->insert_position(0);
    int times = 0;
  
    // Loop through the whole string
    for (int found = 1; found;) {
      int pos = e->editor->insert_position();
      found = textbuf->search_forward(pos, find, &pos);
  
      if (found) {
        // Found a match; update the position and replace text...
        textbuf->select(pos, pos+strlen(find));
        textbuf->remove_selection();
        textbuf->insert(pos, replace);
        e->editor->insert_position(pos+strlen(replace));
        e->editor->show_insert_position();
        times++;
      }
    }
  
    if (times) fl_message("Replaced %d occurrences.", times);
    else fl_alert("No occurrences of \'%s\' found!", find);
  }
   
  void replace2_cb(Fl_Widget*, void* v) {
    EditorWindow* e = (EditorWindow*)v;
    const char *find = e->replace_find->value();
    const char *replace = e->replace_with->value();
  
    if (find[0] == '\0') {
      // Search string is blank; get a new one...
      e->replace_dlg->show();
      return;
    }
  
    e->replace_dlg->hide();
  
    int pos = e->editor->insert_position();
    int found = textbuf->search_forward(pos, find, &pos);
  
    if (found) {
      // Found a match; update the position and replace text...
      textbuf->select(pos, pos+strlen(find));
      textbuf->remove_selection();
      textbuf->insert(pos, replace);
      textbuf->select(pos, pos+strlen(replace));
      e->editor->insert_position(pos+strlen(replace));
      e->editor->show_insert_position();
    }
    else fl_alert("No occurrences of \'%s\' found!", find);
  }

  void replace_cb(Fl_Widget*, void* v) {
    EditorWindow* e = (EditorWindow*)v;
    e->replace_dlg->show();
  }

  void quit_cb(Fl_Widget*, void*) {
    if (changed && !check_save())
      return;
  
    exit(0);
  }

  void paste_cb(Fl_Widget*, void* v) {
    EditorWindow* e = (EditorWindow*)v;
    Fl_Text_Editor::kf_paste(0, e->editor);
  }

  void open_cb(Fl_Widget*, void*) {
    cout<<"Open file"<<endl;
    if (!check_save()) return;
  
    char *newfile = fl_file_chooser("Open File?", "*", filename);
    if (newfile != NULL) load_file(newfile, -1);
  }
  

  void new_cb(Fl_Widget*, void*) {
    if (!check_save()) return;
  
    filename[0] = '\0';
    textbuf->select(0, textbuf->length());
    textbuf->remove_selection();
    changed = 0;
    textbuf->call_modify_callbacks();
  }

  void find2_cb(Fl_Widget* w, void* v) {
    EditorWindow* e = (EditorWindow*)v;
    if (e->search[0] == '\0') {
      // Search string is blank; get a new one...
      find_cb(w, v);
      return;
    }
  
    int pos = e->editor->insert_position();
    int found = textbuf->search_forward(pos, e->search, &pos);
    if (found) {
      // Found a match; select and update the position...
      textbuf->select(pos, pos+strlen(e->search));
      e->editor->insert_position(pos+strlen(e->search));
      e->editor->show_insert_position();
    }
    else fl_alert("No occurrences of \'%s\' found!", e->search);
  }

  void find_cb(Fl_Widget* w, void* v) {
    EditorWindow* e = (EditorWindow*)v;
    const char *val;
  
    val = fl_input("Search String:", e->search);
    if (val != NULL) {
      // User entered a string - go find it!
      strcpy(e->search, val);
      find2_cb(w, v);
    }
}

  void delete_cb(Fl_Widget*, void* v) {
    textbuf->remove_selection();
  }

  void cut_cb(Fl_Widget*, void* v) {
    EditorWindow* e = (EditorWindow*)v;
    Fl_Text_Editor::kf_cut(0, e->editor);
  }

  void copy_cb(Fl_Widget*, void* v) {
    EditorWindow* e = (EditorWindow*)v;
    Fl_Text_Editor::kf_copy(0, e->editor);
  }

  void changed_cb(int, int nInserted, int nDeleted,int, const char*, void* v) {
    if ((nInserted || nDeleted) && !loading) changed = 1;
    EditorWindow *w = (EditorWindow *)v;
    set_title(w);
    if (loading) w->editor->show_insert_position();
  }
  void set_title(Fl_Window* w) {
    if (filename[0] == '\0') strcpy(title, "Untitled");
    else {
      char *slash;
      slash = strrchr(filename, '/');
  #ifdef WIN32
      if (slash == NULL) slash = strrchr(filename, '\\');
  #endif
      if (slash != NULL) strcpy(title, slash + 1);
      else strcpy(title, filename);
    }
  
    if (changed) strcat(title, " (modified)");
  
    w->label(title);
  }


int main(){
    EditorWindow* w = new EditorWindow(640, 480, "t");
    w->editor = new Fl_Text_Editor(0, 30, 640, 370);
    textbuf = new Fl_Text_Buffer();
    w->editor->buffer(textbuf);
    Fl_Menu_Bar *m = new Fl_Menu_Bar(0, 0, 640, 30);
    m->copy(menuitems);
    textbuf->add_modify_callback(changed_cb, w);
    textbuf->call_modify_callbacks();
    w->editor->textfont(FL_COURIER);
    w->show();
    return Fl::run();
}
