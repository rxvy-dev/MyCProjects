/*
 * ===============================================
 *   ChatBot (written in C)
 * ===============================================
 *   Author   : rxvy
 *   Language : C (my first C project!)
 *   Made for : Arch-based Linux (Auxo Linux), uses pacman
 *   Version  : 1.0
 *   License  : free to use, edit, and share
 *
 *   What it does:
 *     - Chats back using a big if / else if chain
 *     - Answers PC, Linux, and package manager questions
 *     - Runs a few system commands (fastfetch, date, whoami)
 *     - Has easter eggs (sudo, rm -rf /, gentoo, etc.)
 *
 *   Note: it has 0 parameters, it's just if and else if.
 *
 *   Compile : gcc bot.c -o bot
 *   Run     : ./bot
 *   Exit    : type "bye" or "quit"
 * ===============================================
 */

// used for printf, fgets(reading what a user types)
#include <stdio.h>
// used for strstr, strcpn
#include <string.h>
// used for system, rand(might add random later)
#include <stdlib.h>
// used for checking if the user exited
#include <sys/wait.h> 
// used for usleep(time), sleep(time)
#include <unistd.h>

int main() {
    char input[128];
    while(1) {
printf("  _              _\n");         
printf(" /  |_   _. _|_ |_)  _ _|_\n"); 
printf(" |_ | | (_|  |_ |_) (_) |_ \n");
printf("\n");
printf("?:");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
// ------------------
// Main Conversations
// ------------------
        if (strstr(input, "hey") || strstr(input, "hi") || strstr(input, "hai")) {
            printf("Generating\n");
            fflush(stdout);
            usleep(800000);
            printf("Whats up!\n");
    }

    else if (strstr(input, "C") || strstr(input, "python")) {
        printf("Generating\n");
        fflush(stdout);
        usleep(800000);
        printf("Both are AMAZING Languages!\n");
    }

    else if (strstr(input, "shut up")) {
        printf("Generating\n");
        fflush(stdout);
        usleep(800000);
        printf("your arguing with a C coded chatbot, grow up\n");
    }

    else if (strstr(input, "stupid")) {
        printf("Generating\n");
        fflush(stdout);
        usleep(800000);
        printf("im just if and else if bro i have 0 paramenters\n");
    }

    else if (strstr(input, "windows") || strstr(input, "winslop")) {
        printf("Generating\n");
        fflush(stdout);
        usleep(800000);
        printf("AHH NO BLOAT...\n");
    }

    else if (strstr(input, "linux") || strstr(input, "arch")) {
        printf("Generating\n");
        fflush(stdout);
        usleep(800000);
        printf("I USE ARCH BTW.\n");
    }

    else if (strstr(input, "smart")) {
        printf("Generating\n");
        fflush(stdout);
        usleep(800000);
        printf("Nuh Uh...\n");
    }

    else if (strstr(input, "microsoft")) {
        printf("Generating\n");
        fflush(stdout);
        usleep(800000);
        printf("Microslop*\n");
    }
    
    else if (strstr(input, "yes") || strstr(input, "yep") || strstr(input, "yah")) {
        printf("Generating\n");
        fflush(stdout);
        usleep(800000);
        printf("NO NO NO...\n");
    }

    else if (strstr(input, "no") || strstr(input, "nah") || strstr(input, "nope")) {
        printf("Generating\n");
        fflush(stdout);
        usleep(800000);
        printf("YEP BUD.\n");
    }

    else if (strstr(input, "open source")) {
        printf("Generating\n");
        fflush(stdout);
        usleep(800000);        
        printf("HOLY AURA.\n");
    }

    else if (strstr(input, "closed source")) {
        printf("Generating\n");
        fflush(stdout);
        usleep(800000);
        printf("EW\n");
    }

    else if (strstr(input, "bye") || strstr(input, "quit")) {
        printf("Cya!\n");
        break;
    }

    else if (strstr(input, "joke")) {
        printf("Generating\n");
        fflush(stdout);
        usleep(800000);
        printf("Deleting the france language now.\n");
    }

    else if (strstr(input, "sudo")) {
        printf("Generating\n");
        fflush(stdout);
        usleep(800000);
        printf("you are not in the sudoers file. this incident will be reported.\n");
    }

    else if (strstr(input, "rm -rf /")) {
        printf("Deleting /*\n");
        fflush(stdout);
        sleep(1.0);
        printf("Sorry you do not have permission to do this.\n");
    }

    else if (strstr(input, "gentoo")) {
        printf("Generating\n");
        fflush(stdout);
        usleep(800000);
        printf("SEE YOU IN 2 DAYS (compiling...)\n");
    }


// ----------
//  Commands               Note: this is built for pacman because i use Auxo Linux
// ----------
    else if (strstr(input, "fastfetch")) {
        int status = system("fastfetch");
        
        int exitcode = WEXITSTATUS(status);

        if (exitcode == 127) {
            printf("Fastfetch not installed, installing now\n");
            system("sudo pacman -S fastfetch");
        }
    }

    else if (strstr(input, "clear")) {
        system("clear");
    }

    else if (strstr(input, "time") || strstr(input, "date")) {
        printf("digging for time..\n");
        system("date");
    }

    else if (strstr(input, "whoami")) {
        system("whoami");
    }
// -------------
//     Facts
// -------------
    else if (strstr(input, "computer") || strstr(input, "pc")) {
        printf("Generating\n");
        fflush(stdout);
        usleep(800000);
        printf("A computer (also known as a PC) is an electronic machine that stores, processes, and displays information by following programmed instructions\n");
    }

    else if (strstr(input, "ram")) {
        printf("Generating\n");
        fflush(stdout);
        usleep(800000);
        printf("RAM (Random Access Memory) is your computer's short-term memory that temporarily holds active data and programs for fast access by the processor\n");
    }

    else if (strstr(input, "hhd")) {
        printf("Generating\n");
        fflush(stdout);
        usleep(800000);
        printf("An HHD (Hybrid Hard Drive) is a storage device that combines a traditional spinning hard disk drive (HDD) with a small amount of flash memory (SSD cache)\n");
    }

    else if (strstr(input, "ssd")) {
        printf("Generating\n");
        fflush(stdout);
        usleep(800000);
        printf("An SSD (Solid-State Drive) is a modern data storage device that uses flash memory chips to save and access data instantly without any moving parts\n");
    }

    else if (strstr(input, "cpu")) {
        printf("Generating\n");
        fflush(stdout);
        usleep(800000);
        printf("The CPU (Central Processing Unit) is the brain of the computer that executes instructions, performs calculations, and manages data flow.\n");
    }

    else if (strstr(input, "gpu")) {
        printf("Generating\n");
        fflush(stdout);
        usleep(800000);
        printf("The GPU (Graphics Processing Unit) is a specialized processor designed to handle visual rendering, 3D graphics, images, and video processing.\n");
    }

    else if (strstr(input, "motherboard")) {
        printf("Generating\n");
        fflush(stdout);
        usleep(800000);
        printf("The motherboard is the main printed circuit board that physically connects and allows communication between the CPU, RAM, storage, and all other components.\n");
    }

    else if (strstr(input, "power supply")) {
        printf("Generating\n");
        fflush(stdout);
        usleep(800000);
        printf("The PSU (Power Supply Unit) converts electrical power from an outlet into usable, regulated voltage for all the internal components of the computer.\n");
    }

    else if (strstr(input, "hdd")) {
        printf("Generating\n");
        fflush(stdout);
        usleep(800000);
        printf("An HDD (Hard Disk Drive) is a legacy magnetic storage device that uses mechanical spinning platters and a moving read/write head to read data.\n");
    }

    else if (strstr(input, "operating system")) {
        printf("Generating\n");
        fflush(stdout);
        usleep(800000);
        printf("An operating system is the master control software that manages all hardware resources and applications on a computer.\n");
    }

    else if (strstr(input, "ghz")) {
        printf("Generating\n");
        fflush(stdout);
        usleep(800000);
        printf("Gigahertz (GHz) measures a CPU's clock speed, indicating how many billions of calculations the processor can perform per second.\n");
    }

    else if (strstr(input, "story")) {
        printf("Generating\n");
        fflush(stdout);
        usleep(800000);
        printf("Hey, thanks for wanting to know my story, i started my coding adventure with python and i coded multiple chatbots in that it was fun and now im on C and its WAY funner! anyway have fun with this code!!!\n");
    }

    else if (strstr(input, "pacman")) {
        printf("Generating\n");
        fflush(stdout);
        usleep(800000);
        printf("Pacman is the default package manager for Arch Linux that tracks, installs, updates, and removes software packages using a simple compressed file format.\n");
    }

    else if (strstr(input, "aur")) {
        printf("Generating\n");
        fflush(stdout);
        usleep(800000);
        printf("The AUR (Arch User Repository) is a community-driven repository where users upload package build scripts (PKGBUILDs) allowing anyone to compile and install software not found in the official repositories.\n");
    }

    else if (strstr(input, "emerge")) {
        printf("Generating\n");
        fflush(stdout);
        usleep(800000);
        printf("Emerge is the core package management interface for Gentoo's Portage system, which downloads source code, applies local optimization flags, and compiles packages natively on your machine.\n");
    }

    else if (strstr(input, "flatpak")) {
        printf("Generating\n");
        fflush(stdout);
        usleep(800000);
        printf("Flatpak is a universal sandboxed package deployment utility designed to package and distribute desktop applications across any Linux distribution without relying on system-level runtimes.\n");
        
    }

    else if (strstr(input, "apt")) {
        printf("Generating\n");
        fflush(stdout);
        usleep(800000);
        printf("APT (Advanced Package Tool) is the default package manager for Debian, Ubuntu, and their derivatives, utilizing pre-compiled .deb binaries and handling dependencies automatically.\n");
    }

    else if (strstr(input, "xbps")) {
        printf("Generating\n");
        fflush(stdout);
        usleep(800000);
        printf("XBPS (X Binary Package System) is the fast, native package manager for Void Linux, written from scratch to handle package installations, removals, and dependency resolution using simple metadata.\n");
    }

    else if (strstr(input, "dnf")) {
        printf("Generating\n");
        fflush(stdout);
        usleep(800000);
        printf("DNF (Dandified YUM) is the next-generation package manager for RPM-based distributions like Fedora and Red Hat Enterprise Linux, known for strict dependency tracking and clean metadata management.\n");
    }

    else if (strstr(input, "wayland") || strstr(input, "x11")) {
        printf("Generating\n");
        fflush(stdout);
        usleep(800000);
        printf("Wayland and X11 (Xorg) are display server protocols that enable the operating system to communicate with your graphics hardware so applications can draw windows and interface elements on your screen.\n");
    }

    else if (strstr(input, "kernel")) {
        printf("Generating\n");
        fflush(stdout);
        usleep(800000);
        printf("The kernel is the core program of an operating system that holds absolute control over everything in the system, acting as the primary bridge between hardware components and running software applications.\n");
    }

    else if (strstr(input, "root")) {
        printf("Generating\n");
        fflush(stdout);
        usleep(800000);
        printf("Root is the conventional name of the administrative user account on Unix-like operating systems, possessing universal read, write, and execute permissions across the entire file system.\n");
    }

    else if (strstr(input, "nixos")) {
         printf("Generating\n");
        fflush(stdout);
        usleep(800000);
        printf("NixOS is a declarative Linux distribution built on the Nix package manager where the entire system configuration, including packages, services, and user settings, is defined inside a single monolithic configuration file (configuration.nix)\n");
    }

    else if (strstr(input, "grub")) {
        printf("Generating\n");
        fflush(stdout);
        usleep(800000);
        printf("GRUB (Grand Unified Bootloader) is the primary program that starts your computer and loads the Linux operating system kernel into memory\n");   
    }

    else if (strstr(input, "gnu")) {
        printf("Generating\n");
        fflush(stdout);
        usleep(800000);
        printf("GNU is a free software operating system and project created to give computer users complete freedom over their software\n");
    }

    else {
        printf("Generating\n");
        fflush(stdout);
        usleep(800000);
        printf("You like %s thats cool too!\n", input);
    }

 }

}
