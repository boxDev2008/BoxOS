#include <kshell.h>
#include <kcsl.h>
#include <string.h>

#include <drivers/vga.h>
#include <drivers/ide.h>

#define MAX_SHELL_BUFFER_SIZE 256

typedef struct
{
    char buffer[MAX_SHELL_BUFFER_SIZE];
    char *user;
    char *host;
}
shell_t;

shell_t shell;

void Kshell_Initialize(void)
{
    shell.user = "user";
    shell.host = "boxos";
    
    Kcsl_SetColor(VGA_COLOR_LIGHT_GREEN);
    kprintf("%s", shell.user);
    Kcsl_SetColor(VGA_COLOR_LIGHT_CYAN);
    kprintf("@");
    Kcsl_SetColor(VGA_COLOR_LIGHT_GREEN);
    kprintf("%s", shell.host);
    kprintf("$ ");
    Kcsl_SetColor(VGA_COLOR_LIGHT_GREY);

    Kcsl_SetCursorPosition(Kcsl_GetColumn(), Kcsl_GetRow());
}

void Kshell_WriteChar(char ch)
{
    int len = strlen(shell.buffer);

    if (len < MAX_SHELL_BUFFER_SIZE)
    {
        shell.buffer[len] = ch;
        shell.buffer[len+1] = '\0';

        kprintf("%c", ch);

        Kcsl_SetCursorPosition(Kcsl_GetColumn(), Kcsl_GetRow());
    }
}

void Kshell_Backspace(void)
{
    int len = strlen(shell.buffer);
    if (len > 0)
    {
        shell.buffer[len - 1] = '\0';
        Kcsl_Backspace();

        Kcsl_SetCursorPosition(Kcsl_GetColumn(), Kcsl_GetRow());
    }
}

void Kshell_ExecuteCurrentCommand(void)
{
    if (strlen(shell.buffer) <= 0)
        return;
    else if (strcmp(shell.buffer, "help") == 0)
    {
        kprintf("\nyes uwu\n");
    }
    else if (strcmp(shell.buffer, "clear") == 0)
    {
        Kcsl_Clear();

        Kcsl_SetColumn(0);
        Kcsl_SetRow(0);
    }
    else if(strcmp(shell.buffer, "list disks") == 0)
        IDE_PrintDriveInfo();
    else
    {
        kprintf("\nInvalid command ");
        Kcsl_SetColor(VGA_COLOR_LIGHT_RED);
        kprintf("%s\n", shell.buffer);
        Kcsl_SetColor(VGA_COLOR_LIGHT_GREY);
    }

    shell.buffer[0] = '\0';
    
    Kcsl_SetColor(VGA_COLOR_LIGHT_GREEN);
    kprintf("%s", shell.user);
    Kcsl_SetColor(VGA_COLOR_LIGHT_CYAN);
    kprintf("@");
    Kcsl_SetColor(VGA_COLOR_LIGHT_GREEN);
    kprintf("%s", shell.host);
    kprintf("$ ");
    Kcsl_SetColor(VGA_COLOR_LIGHT_GREY);

    Kcsl_SetCursorPosition(Kcsl_GetColumn(), Kcsl_GetRow());
}