#include <stdio.h>
#include <windows.h>
#include <time.h>

#define BAR_WIDTH 50

void set_color(WORD color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void worktime_timer(int hours)
{
    int seconds = hours * 3600;
    if (seconds <= 0)
    {
        // Reset color
        set_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        printf("Just go home!\n");
        return;
    }

    time_t start_time = time(NULL);

    for (int i = seconds; i >= 0; i--)
    {
        double percent = (double)(seconds - i) / seconds;
        int filled = (int)(percent * BAR_WIDTH);

        // Calculate ETA
        int elapsed = (int)(time(NULL) - start_time);
        int remaining = seconds - elapsed;
        if (remaining < 0)
            remaining = 0;

        int rem_h = remaining / 3600;
        int rem_m = (remaining % 3600) / 60;
        int rem_s = remaining % 60;

        // Build progress bar
        char bar[BAR_WIDTH + 1];
        for (int j = 0; j < BAR_WIDTH; j++)
            bar[j] = (j < filled) ? '#' : '-';
        bar[BAR_WIDTH] = '\0';

        // Color: Green when >50%, Yellow >10%, Red <10%
        if (percent < 0.5)
            set_color(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        else if (percent < 0.95)
            set_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        else
            set_color(FOREGROUND_RED | FOREGROUND_INTENSITY);

        // Print status (overwrite same line)
        printf(
            "\r[%s] %6.2f%%  |  Remaining: %02d:%02d:%02d ",
            bar,
            percent * 100,
            rem_h, rem_m, rem_s);
        fflush(stdout);

        Sleep(1000);
    }

    // Reset color
    set_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    printf("\nOff work!\n");
}

int main()
{
    printf("Worktime today (hours): ");
    int hours;
    scanf("%d", &hours);

    worktime_timer(hours);

    system("pause");
    return 0;
}
