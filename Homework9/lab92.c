
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <ncurses.h>
#define MAX_FILES 256
#define MAX_PATH_LENGTH 256

typedef struct {
    char files[MAX_FILES][MAX_PATH_LENGTH];
    int file_count;
    int selected_file;
    char current_path[MAX_PATH_LENGTH];
} Panel;

void populate_panel(Panel *panel, const char *path);
void draw_panel(const Panel *panel, int start_row, int start_col, int height, int width, bool is_active);
void handle_input(Panel *left_panel, Panel *right_panel, bool *active_panel_left);
void change_directory(Panel *panel, const char *new_path);

int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    Panel left_panel, right_panel;

    // Initialize panels
    strcpy(left_panel.current_path, "/");
    strcpy(right_panel.current_path, "/");

    populate_panel(&left_panel, left_panel.current_path);
    populate_panel(&right_panel, right_panel.current_path);

    left_panel.selected_file = 0;
    right_panel.selected_file = 0;

    bool active_panel_left = true;

    while (true) {
        clear();

        int height, width;
        getmaxyx(stdscr, height, width);

        // Calculate panel dimensions
        int panel_width = width / 2;
        int panel_height = height - 2; // Leave space for status bar

        // Draw panels
        draw_panel(&left_panel, 1, 0, panel_height, panel_width, active_panel_left);
        draw_panel(&right_panel, 1, panel_width, panel_height, panel_width, !active_panel_left);

        // Draw status bar
        mvprintw(height - 1, 0, "F1 Help  F2 Menu  Tab: Switch Panel  Enter: Open/Change Dir  Q: Quit");

        refresh();

        handle_input(&left_panel, &right_panel, &active_panel_left);
    }

    endwin();
    return 0;
}

void populate_panel(Panel *panel, const char *path) {
    DIR *dir;
    struct dirent *ent;
    panel->file_count = 0;

    if ((dir = opendir(path)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) { // Avoid infinite loops
                strncpy(panel->files[panel->file_count], ent->d_name, MAX_PATH_LENGTH - 1);
                panel->files[panel->file_count][MAX_PATH_LENGTH - 1] = '\0'; // Ensure null termination
                panel->file_count++;
                if (panel->file_count >= MAX_FILES) break; //Prevent buffer overflow
            }
        }
        closedir(dir);
    } else {
        perror("Could not open directory");
    }
}

void draw_panel(const Panel *panel, int start_row, int start_col, int height, int width, bool is_active) {
    box(stdscr, 0, 0);  // Draw a box around the entire screen
    mvvline(1, width, ACS_VLINE, height-2); //Vertical line dividing the two panels

    mvprintw(start_row - 1, start_col + 1, "%s", panel->current_path);

    for (int i = 0; i < panel->file_count && i < height - 1; i++) { // Limit files to panel height
        if (i == panel->selected_file && is_active) {
            attron(A_REVERSE);
        }
        mvprintw(start_row + i, start_col + 1, "%s", panel->files[i]);
        attroff(A_REVERSE);
    }
}

void handle_input(Panel *left_panel, Panel *right_panel, bool *active_panel_left) {
    int ch = getch();
    Panel *active_panel = *active_panel_left ? left_panel : right_panel;

    switch (ch) {
        case KEY_UP:
            if (active_panel->selected_file > 0) {
                active_panel->selected_file--;
            }
            break;
        case KEY_DOWN:
            if (active_panel->selected_file < active_panel->file_count - 1) {
                active_panel->selected_file++;
            }
            break;
        case 9:  // Tab key
            *active_panel_left = !(*active_panel_left);
            break;
        case 10: // Enter key
        {
            char new_path[MAX_PATH_LENGTH];
            snprintf(new_path, MAX_PATH_LENGTH, "%s/%s", active_panel->current_path, active_panel->files[active_panel->selected_file]);

            struct stat statbuf;
            if (stat(new_path, &statbuf) == 0) {
                if (S_ISDIR(statbuf.st_mode)) {
                    change_directory(active_panel, new_path);

                    active_panel->selected_file = 0; // Reset selection after directory change
                } else {
                    // TODO: Handle file opening (requires external command or more complex logic)
                    // For now, just print a message
                    mvprintw(0, 0, "Opening file: %s (not implemented yet)", new_path);
                    getch();  // Wait for a key press before continuing
                }
            } else {
                perror("stat failed");
                mvprintw(0, 0, "Error accessing file/directory: %s", new_path);
                getch();
            }
            break;
        }

        case 'q': // Quit
        case 'Q':
            endwin();
            exit(0);
            break;

        default:
            // Ignore other keys
            break;
    }

    // Repopulate the panels after any changes
    populate_panel(left_panel, left_panel->current_path);
    populate_panel(right_panel, right_panel->current_path);
}

void change_directory(Panel *panel, const char *new_path) {
    // Check if the new path is valid and a directory
    DIR *dir = opendir(new_path);
    if (dir) {
        closedir(dir);
        strncpy(panel->current_path, new_path, MAX_PATH_LENGTH - 1);
        panel->current_path[MAX_PATH_LENGTH - 1] = '\0';
        populate_panel(panel, panel->current_path);
        panel->selected_file = 0;
    } else {
        // Error handling (directory doesn't exist or cannot be opened)
        perror("Could not open directory");
        mvprintw(0, 0, "Cannot open directory: %s", new_path);  //Print error message
        getch(); //Wait for keypress to clear the screen
    }
}