#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "editor_state.h"
#include "config.h"
#include "gap_buffer.h"

#include <ncurses.h>

int main(int argc, char*  argv[]){
  Config* config = NULL;
  bool result = GetConfig(&config);
  if(result == false || config == NULL){
    puts("boop");
    return -1;
  }

  GapBuffer* buffer = new_buffer(512);
  if(buffer == NULL){
    return -1;
  }

  WINDOW* main_window = initscr();
  //notimeout(main_window, true);
  keypad(main_window, true);

  enum EDITOR_STATE state = COMMAND_MODE;

  bool insert_char = config->default_insert_mode;
  bool quit = false;
  
  if(argc > 1) {
    FILE* file = fopen(argv[1], "r");
    if(file == NULL) {
      puts("Failed to open file.");
      return -1;
    }

    char c;
    while((c = fgetc(file)) != EOF) {
      insert_character(buffer, c);
    }

    fclose(file);
  }

  while(quit == false) {
    noecho();
    char* buffer_text = extract_text(buffer);
    mvwprintw(main_window, 0, 0, "%s", buffer_text);
    int ch = wgetch(main_window);
    
    switch(state){
      case INSERT_MODE: {     
        //ESCAPE?
        if(ch == 27){
          nodelay(main_window, TRUE);
          ch = getch();
          nodelay(main_window, FALSE);
          if(ch == ERR) {
            state = COMMAND_MODE;
            //Don't add this to the fuckin' buffer mate.
            break;
          }
        }
        else if (ch == KEY_BACKSPACE){
          backspace(buffer);
          break;
        }else if (ch == KEY_DL){
          delete(buffer);
          break;
        }
        
        if(insert_char == false) {
          delete(buffer);
        }

        insert_character(buffer, ch);

        break;
      }

      case COMMAND_MODE: {
          if(ch == 265) {
        //if(ch == KEY_F(1)) {
          state = INSERT_MODE;
          break;
        }

        //ESCAPE
        if(ch == 27){
          nodelay(main_window, TRUE);
          ch = wgetch(main_window);
          nodelay(main_window, FALSE);
          if(ch == ERR) {
            quit = true;
            break;
          }
        }
        break;
      }

      case TERMINAL_MODE: {
        break;
      }
    }

    wrefresh(main_window);
  }

  endwin();

  if(buffer){
    free_buffer(buffer);
  }

  if (config) {
      FreeConfig(config);
  }

  return 0;
}