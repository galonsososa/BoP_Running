#include <time.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct Run {
    double length;
    struct tm start_time;
    struct tm end_time;
    double duration;
    struct tm pace;
}Run;

int time_difference(struct tm end,struct tm start){

    int horas = end.tm_hour - start.tm_hour;
    int minutos = end.tm_min - start.tm_min;
    minutos = minutos + horas*60;
    return minutos;

}

void run_into_file(int day,int month,int year,int start_hour,int start_min,int end_hour,
                 int end_min,double length){
    FILE *fp;
    int status;

    fp = fopen("Running.txt","a"); //file opening in append mode
    if (fp==NULL) return 1;
    /*saves into file with format:
        {
        DD/MM/YYYY
        HH:MM-HH:MM
        d.d
        }
    */
    fprintf(fp,"{\n%02d/%02d/%4d\n%02d:%02d-%02d:%02d\n%1.f\n}\n",
            day,month,year,
            start_hour,start_min,end_hour,end_min,
            length);


    status = fclose(fp);
    if (status!=0){
     return 1;
    } else printf("Succesfully saved run into Running.txt\n");

}

double extract_decimals(double num){

    int intpart = (int)num;
    double decpart = num - intpart;
    return decpart;

}
double pace_calculator(int duration,double length){
    return duration/length;
}



int main()
{
    //DO YOU WISH TO ADD A FILE OR TO CREATE A NEW ONE?YES->LOADS THE DATA FROM A TEXT FILE;NO->CREATES A NEW TEXT FILE;
    //case 1 add a run
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
    scanf("%lf",&r1.length);

    r1.duration = time_difference(r1.end_time,r1.start_time);
    printf("Duration of the training is %.f minutes\n",r1.duration);

    double pace = pace_calculator(r1.duration,r1.length);
    r1.pace.tm_min = pace;
    r1.pace.tm_sec = extract_decimals(pace) * 60; //converts decimal time into MM:SS format
    printf("Pace per kilometer is %2d:%2d\n",r1.pace.tm_min,r1.pace.tm_sec);

    run_into_file(r1.start_time.tm_mday,r1.start_time.tm_mon,r1.start_time.tm_year,
                  r1.start_time.tm_hour,r1.start_time.tm_min,r1.end_time.tm_hour,r1.end_time.tm_min,
                  r1.length);



    return 0;


}
