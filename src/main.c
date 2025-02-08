#include "base.h"
#include "main.h"
#include "time.h"

#include "screen.h"
#include "input.h"
#include "menu_main.h"

UserContext user_context;


int main(int argc, char *argv[]) {
    if (argc != 3) {
        output("{CLS}{FG GREEN}Oh oh! Mad Dictator is not configured to launch correctly.\n");
        output("Send feedback to the Sysop to inform them.\n");
        printf("Error: Expected 2 arguments but got %d\n", argc);
        return 1;
    }

    output("{CLS}{FG RED}{CENTER 3**** NOTICE ****}\n\n{FG MAGENTA}Mad Dictator {FG GREEN}is a work in progress and many features are not implemented yet.\n");
    output("Send bugs and comments to {FG WHITE}KEVINUM{FG GREEN} via mail on this BBS.\n");
    output("\nEnjoy the game! {FG YELLOW}Press any key...");
    getkey();

    srand(time(NULL));
    user_context.player_id = atoi(argv[1]);
    strncpy(user_context.handle, argv[2], PLAYER_HANDLE_LEN);

    show_main_menu();

    output("{RESET}{CLS}");
    output("{CLS}{FG CYAN}Thanks for playing Mad Dictator. Report back for duty soon.\n");
    output("{RESET}");

    return 0;
}

