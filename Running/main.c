#include <time.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct date_time DateTime;
struct run {
    double length;
    struct tm start_time;
    struct tm end_time;
    double duration;
    struct tm pace;
};

typedef struct run Run;

int time_difference(struct tm end,struct tm start){

    int horas = end.tm_hour - start.tm_hour;
    int minutos = end.tm_min - start.tm_min;
    minutos = minutos + horas*60;
    return minutos;

}

int main()
{
    Run r1;
    printf("Enter date of the training (DD/MM/YYYY)\n");
    scanf("%d/%d/%d",&r1.start_time.tm_mday
                    ,&r1.start_time.tm_mon
                    ,&r1.start_time.tm_year);
    r1.end_time.tm_mday = r1.start_time.tm_mday;
    r1.end_time.tm_mon = r1.start_time.tm_mon;
    r1.end_time.tm_year = r1.start_time.tm_year;

    //cubrir errores de dias meses erroneos etc
    printf("What time did you start? (HH.MM)\n");
    scanf("%2d.%2d",&r1.start_time.tm_hour,&r1.start_time.tm_min);
    printf("What time did you finish? (HH.MM)\n");
    scanf("%2d.%2d",&r1.end_time.tm_hour,&r1.end_time.tm_min);
    printf("How many kilometres did you run?\n");
    scanf("%.1d",&r1.length);
    r1.duration = time_difference(r1.end_time,r1.start_time);
    printf("Duration of the training is %.f minutes",r1.duration);


    return 0;
}
