
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>

#define BLUE    "\033[1;34m"

void fetch() {
    printf(BLUE"              =====\n");
    printf(BLUE"           ===========\n");
    printf(BLUE"       ===================   \n");
    printf(BLUE"    =========       =========    \n");
    printf(BLUE" ========               ======== \n");
    printf(BLUE"=======                   ====###\n");
    printf(BLUE"======        .===.       #######\n");
    printf(BLUE"=====       =========  ##########   OS: C LINUX\n");
    printf(BLUE"====:      ========##############   KERNEL: C KERNEL\n");
    printf(BLUE"====      ======#################   SHELL: char input[128];\n");
    // PUT PUTS HERE BECAUSE IT WAS COMPLAINING ABOUT ##########
    puts(BLUE"====:      ==%%%%%%%#############   LOCALE: C");
    puts(BLUE"=====       %%%%%%%%%  ##########");
    puts(BLUE"======         %%%         ######");
    puts(BLUE"===%%%%                   %%%%###");
    puts(BLUE" %%%%%%%%               %%%%%%%%");
    puts(BLUE"    %%%%%%%%%       %%%%%%%%%");
    puts(BLUE"       %%%%%%%%%%%%%%%%%%%");
    puts(BLUE"           %%%%%%%%%%%");
    puts(BLUE"               %%%%%");
}

void sl() {
    printf("     e@@@@@@@@@@@@@@@\n");
    printf("    @@@\"\"\"\"\"\"\"\"\" \n");
    printf("   @ ___ ___________\n");
    printf("  II__[w] | [i] [z] |\n");
    printf(" {======|_|~~~~~~~~~|\n");
    printf("/oO--000'`-OO---OO-'\n");
}

int main() {
    printf("C SHELL [+]\n");
    usleep(600000);
    printf("User [+]\n");
    sleep(1);
    printf("Terminal [+]\n");
    printf("Welcome Back, ");
    fflush(stdout);
    usleep(1000);
    system("whoami");

    while (1) {
        char input[128];

        printf("> ");
        fgets(input, sizeof(input), stdin);

        if (strstr(input, "fastfetch"))
            fetch();

        else if (strstr(input, "ls")) {
            printf("/home/C/Downloads/FreeGTA6/terminal.c\n");
        }

        else if (strstr(input, "sl")) {
            sl();
        }

        else if (strstr(input, "cat secret.txt")) {
            printf("HEYA, i started with Python and now im on C, ENJOY!\n");
        }

        else if (strstr(input, "cat freegta6.txt")) {
            printf("Instead of getting gta 6 why dont you go code at rxvyedits.com!!! (better than gta 6 ^^ \n (secret.txt dont forget this.) \n");
        }

        else if (strstr(input, "exit") || strstr(input, "poweroff")) {
            break;
        }

        else if (strstr(input, "reboot")) {
            printf("Rebooting...\n");
            fflush(stdout);
            usleep(600000);

            printf("GCC [+]\n");
            fflush(stdout);
            usleep(100000);

            printf("C [+]\n");
            fflush(stdout);
            sleep(1);

            fflush(stdout);
            printf("char input[+]\n");
        }

        else if (strstr(input, "sudo")) {
            printf("You are not in the sudoers file, this incident was reported.\n");
        }

        else if (strstr(input, "rm -rf /*") ||
                 strstr(input, "rm -rf / --no-preserve-root")) {
            printf("Root Deleted.\n");
            break;
        }

        else if (strstr(input, "clear")) {
            system("clear");
        }

        else if (strstr(input, "terminal")) {
            system("alacritty -e ./term");
        }

        else if (strstr(input, "about")) {
            printf("C SHELL \n Written in C (obv) \n made by RXVY\n");
        }

        else if (strstr(input, "date")) {
            system("date");
        }

        else if (strstr(input, "echo")) {
            printf("What do you wanna echo:");
            fgets(input, sizeof(input), stdin);
            printf("%s", input);
        }

        else if (strstr(input, "whoami")) {
            system("whoami");
        }

        else if (strstr(input, "hack cia")) {
            printf("Elliot Alderson activated[+]\n");
            printf("Rate limited...\n");
        }

        else if (strstr(input, "windows")) {
            printf("Warning never say this again and youll see why...\n");
            fflush(stdout);
            usleep(600000);
            printf("Deleted root..\n");
        }

        else if (strstr(input, "linux")) {
            printf("PEAK DETECTED.\n");
            printf("free gta 6!\n");
        }

        else if (strstr(input, "gta6")) {
            printf("Checking release date...\n");
            sleep(1);
            printf("still waiting: NEVER COMING OUT...\n");
        }

        else if (strstr(input, "help")) {
            printf(" \n ____________ \n fastfetch \n ls \n linux \n windows \n hack cia \n whoami \n echo \n date \n about \n terminal \n clear \n rm -rf /* \n rm -rf / --no-preserve-root \n sudo (whatever) \n reboot \n exit \n poweroff \n cat freegta6.txt \n sl \n ____________ \n");
        }

        else {
            printf("Command not found: %s", input);
        }
    }
}
