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

    // Clock in time
    time_t clockin_time = time(NULL);
    struct tm *clockin_info = localtime(&clockin_time);
    char clockin_buffer[10];
    strftime(clockin_buffer, sizeof(clockin_buffer), "%H:%M:%S", clockin_info);

    // Clock out time
    time_t clockout_time = clockin_time + seconds;
    struct tm *clockout_info = localtime(&clockout_time);
    char clockout_buffer[10];
    strftime(clockout_buffer, sizeof(clockout_buffer), "%H:%M:%S", clockout_info);

    printf("Clock in:  %s\n", clockin_buffer);
    printf("Clock out: %s\n", clockout_buffer);

    for (int i = seconds; i >= 0; i--)
    {
        double percent = (double)(seconds - i) / seconds;
        int filled = (int)(percent * BAR_WIDTH);

        // Calculate ETA
        int remaining = i;
        int rem_h = remaining / 3600;
        int rem_m = (remaining % 3600) / 60;
        int rem_s = remaining % 60;

        // Build progress bar
        char bar[BAR_WIDTH + 1];
        memset(bar, '#', sizeof(char) * filled);
        memset(bar + filled, '-', sizeof(char) * (BAR_WIDTH - filled));
        bar[BAR_WIDTH] = '\0';

        // Color: 0 <= Green < 50%, 50% <= Yellow < 95%, 95% <= Red <= 100%
        if (percent < 0.5)
            set_color(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        else if (percent < 0.95)
            set_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        else
            set_color(FOREGROUND_RED | FOREGROUND_INTENSITY);

        // Print status (overwrite same line)
        printf(
            "\r[%s] %6.2f%%  |  Remaining: %02d:%02d:%02d",
            bar,
            percent * 100,
            rem_h, rem_m, rem_s);
        fflush(stdout);

        // Control the time elapsed speed
        Sleep(1000);
    }

    // Reset color
    set_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    printf("\nOff work!\n");
}

int main()
{
    printf("Worktime today (hours): ");
    fflush(stdout);
    int hours;

    // Check input validation
    if (scanf("%d", &hours) != 1)
    {
        set_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        printf("Invalid input.\n");
        return 1;
    }

    worktime_timer(hours);

    // Time out alert
    Beep(1000, 500);
    Sleep(50);
    Beep(1000, 500);
    Sleep(50);
    Beep(1000, 500);

    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        // Clear stdin
    }
    printf("Press Enter to exit...");
    getchar();

    return 0;
}
