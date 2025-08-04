#include <ncurses.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_PATH 1024
#define MAX_ITEMS 1024

typedef struct {
    char path[MAX_PATH];
    char *items[MAX_ITEMS];
    int item_count;
    int selected;
    int start_line;
} Panel;

void free_panel(Panel *panel) {
    for (int i = 0; i < panel->item_count; i++) {
        free(panel->items[i]);
    }
}

void load_directory(Panel *panel) {
    free_panel(panel);
    panel->item_count = 0;
    panel->selected = 0;
    panel->start_line = 0;

    DIR *dir = opendir(panel->path);
    if (!dir) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        if (panel->item_count < MAX_ITEMS) {
            panel->items[panel->item_count] = strdup(entry->d_name);
            panel->item_count++;
        }
    }
    closedir(dir);
}

void draw_panel(WINDOW *win, Panel *panel, int width, int height) {
    werase(win);
    box(win, 0, 0);

    mvwprintw(win, 0, 2, " %s ", panel->path);

    int max_display = height - 2;
    if (panel->start_line > panel->item_count - max_display)
        panel->start_line = (panel->item_count - max_display > 0) ? panel->item_count - max_display : 0;

    for (int i = 0; i < max_display && (i + panel->start_line) < panel->item_count; i++) {
        int idx = i + panel->start_line;
        if (idx == panel->selected)
            wattron(win, A_REVERSE);
        mvwprintw(win, i + 1, 1, "%s", panel->items[idx]);
        wattroff(win, A_REVERSE);
    }

    wrefresh(win);
}

int main() {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    int height, width;
    getmaxyx(stdscr, height, width);

    WINDOW *left_win = newwin(height, width / 2, 0, 0);
    WINDOW *right_win = newwin(height, width - width / 2, 0, width / 2);

    Panel left_panel = { .path = "/", .item_count=0 };
    Panel right_panel = { .path = "/", .item_count=0 };
    
    load_directory(&left_panel);
    load_directory(&right_panel);

    bool active_left = true;

    int ch;
    
while (1) {
        draw_panel(left_win, &left_panel, width/2, height);
        draw_panel(right_win, &right_panel, width - width/2, height);

        ch = getch();

        Panel *active_panel = active_left ? &left_panel : &right_panel;

        switch (ch) {
            case KEY_UP:
                if (active_panel->selected > 0)
                    active_panel->selected--;
                if (active_panel->selected < active_panel->start_line)
                    active_panel->start_line--;
                break;
            case KEY_DOWN:
                if (active_panel->selected < active_panel->item_count -1)
                    active_panel->selected++;
                if (active_panel->selected >= active_panel->start_line + (height -2))
                    active_panel->start_line++;
                break;
            case '\t': 
                active_left = !active_left;
                break;
            case '\n': {
                char selected_name[MAX_PATH];
                strcpy(selected_name, active_panel->items[active_panel->selected]);

                char new_path[MAX_PATH];
                if (strcmp(active_panel->path,"/") == 0)
                    snprintf(new_path,sizeof(new_path), "/%s", selected_name);
                else
                    snprintf(new_path,sizeof(new_path), "%s/%s", active_panel->path, selected_name);

                struct stat st;
                if (stat(new_path,&st)==0 && S_ISDIR(st.st_mode)) {
                    strncpy(active_panel->path,new_path,sizeof(active_panel->path));
                    load_directory(active_panel);
                } else {
                    mvprintw(0,height-1,"Это не директория: %s", selected_name);
                    getch();
                }
                break;
            }
            case 'q':
            case 'Q':
                goto cleanup;
            default:
                break;
        }
}

cleanup:
for(int i=0;i<left_panel.item_count;i++) free(left_panel.items[i]);
for(int i=0;i<right_panel.item_count;i++) free(right_panel.items[i]);
delwin(left_win);
delwin(right_win);
endwin();
return 0;
}