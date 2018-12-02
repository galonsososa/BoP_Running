#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Run {
    double length;
    struct tm start_time;
    struct tm end_time;
    double duration;
    struct tm pace;
}Run;

typedef struct List{
    Run data;
    struct List * next;
} List;

bool equals_dates(struct tm date1,struct tm date2){
    if ((date1.tm_mday == date2.tm_mday) &&
        (date1.tm_mon == date2.tm_mon) &&
        (date1.tm_year == date2.tm_year)){
            return true;
        }
    return false;
}

bool run_betwenn_dates(Run r,struct tm date1,struct tm date2){
    if ((r.start_time.tm_mday>=date1.tm_mday) && (r.start_time.tm_mday <= date2.tm_mday) &&
        (r.start_time.tm_mon>=date1.tm_mon) && (r.start_time.tm_mon <= date2.tm_mon) &&
        (r.start_time.tm_year>=date1.tm_year) && (r.start_time.tm_year <= date2.tm_year)){
            printf("el run betw dates sale true");
            return true;
        }else printf("sale false");
    return false;
}

bool run_between(Run r,struct tm date1,struct tm date2){
    int run_date = r.start_time.tm_mday + r.start_time.tm_mon*30 + r.start_time.tm_year*365;//convert the date into number of days in order to compare it
    int first_date = date1.tm_mday + date1.tm_mon*30 + date1.tm_year*365;
    int second_date = date2.tm_mday + date2.tm_mon*30 + date2.tm_year*365;
    if ((run_date>=first_date) && (run_date<=second_date)){
        return true;
    }
    return false;
}

//if the first date is after the second returns true
bool first_date_after_second(struct tm date1,struct tm date2){
    int first_date = date1.tm_mday + date1.tm_mon*30 + date1.tm_year*365;
    int second_date = date2.tm_mday + date2.tm_mon*30 + date2.tm_year*365;
    if (second_date<first_date) return true;
    return false;
}

//equals function: if two Runs have the exact same start time and length we consider it as the same Run
bool equals_run(Run r1,Run r2){
    if ((r1.start_time.tm_mday == r2.start_time.tm_mday)&&
        r1.start_time.tm_mon == r2.start_time.tm_mon &&
        r1.start_time.tm_year == r2.start_time.tm_year &&
        r1.start_time.tm_hour == r2.start_time.tm_hour &&
        r1.start_time.tm_min == r2.start_time.tm_min &&
        r2.length == r2.length){
            return true;
        }
    return false;
}



//inserting in the list
List *insert_run(List *head, Run run) {
    List *newel = (List *) malloc(sizeof(List));
    newel->data = run;
    newel->next  = head;
    return newel;
}

//looks for a run with the given date in the list
Run filter_by_date(List * head,struct tm date){
    List *p;
    for (p=head;p!=NULL;p=p->next){
        if(equals_dates(date,p->data.start_time)){
            return p->data;
        }
    }
    printf("\nNo run found on that date\n");
    exit(0);
    return;
}

void filter_by_period(List* head,struct tm date1,struct tm date2){
    int i = 0;
    List *p;
    for (p=head;p!=NULL;p=p->next){
        if (run_between(p->data,date1,date2)){
            run_print(p->data);
            i++;
        }
    }
    if (i==0){
        printf("No runs found on that period of time\n\n");
    }
}

List *search_run(List *head, Run run) {
    List *p;
    for (p = head; p != NULL; p = p->next)
        if (equals_run(p->data,run))
            return p;
    return NULL;
}

//end_time - start_time;output in minutes format
int time_difference(struct tm end,struct tm start){

    int horas = end.tm_hour - start.tm_hour;
    int minutos = end.tm_min - start.tm_min;
    minutos = minutos + horas*60;
    return minutos;
}

void run_into_file(int day,int month,int year,int start_hour,int start_min,int end_hour,
                   int end_min,double length){ //saves the run properties into a file
    FILE *fp;
    int status;

    fp = fopen("Running.txt","a"); //file opening in append mode
    if (fp==NULL) return 1;
    /*saves into file with format:
       DD/MM/YYYY HH:MM-HH:MM X.X
    */
    fprintf(fp,"%02d/%02d/%4d %02d:%02d-%02d:%02d %lf\n",
            day,month,year,
            start_hour,start_min,end_hour,end_min,
            length);


    status = fclose(fp);
    if (status!=0){
     return 1;
    } else{
        printf("--------------------------------------\n");
        printf("Succesfully saved run into Running.txt\n");
        printf("--------------------------------------\n");
    }
}

double extract_decimals(double num){

    int intpart = (int)num;
    double decpart = num - intpart;
    return decpart;

}
double pace_calculator(int duration,double length){
    return duration/length;
}

List* read_file(List *head){

    Run r;
    FILE *fp = fopen("Running.txt","r");
    char *str;

    if(fp == NULL) {
      perror("Error opening file 'Running.txt'");
      return;
    }
    while (fscanf(fp,"%d/%d/%d %d:%d-%d:%d %lf\n",
           &r.start_time.tm_mday,&r.start_time.tm_mon,&r.start_time.tm_year,
           &r.start_time.tm_hour,&r.start_time.tm_min,&r.end_time.tm_hour,&r.end_time.tm_min,
           &r.length)==8) {
        r.duration = time_difference(r.end_time,r.start_time);
        double pace = pace_calculator(r.duration,r.length);
        r.pace.tm_min = pace;
        r.pace.tm_sec = extract_decimals(pace) * 60;

        head = insert_run(head,r);
    }
    fclose(fp);
    return head;
}


//returns the latest run in time of the both given
Run date_comparator(Run r1,Run r2){
    int first = r1.start_time.tm_mday + r1.start_time.tm_mon*30 + r1.start_time.tm_year*365;
    int second = r2.start_time.tm_mday + r2.start_time.tm_mon*30 + r2.start_time.tm_year*365;
    if (first>second){
        return r1;
    }
    return r2;
}

//returns the last run of the list(according to date)
Run last_run(List *head){
    List*p;
    Run r;
    r.start_time.tm_mday = 1;
    for(p=head;p!=NULL;p=p->next){
        r = date_comparator(r,p->data);
    }
    return r;
}

void last_run_pace(Run r){
    r.duration = time_difference(r.end_time,r.start_time);
    double pace = pace_calculator(r.duration,r.length);
    r.pace.tm_min = pace;
    r.pace.tm_sec = extract_decimals(pace) * 60;

    printf("\nLast run on %02d/%02d/%04d.Your next objective is to improve %02d:%02d/km\n\n",r.start_time.tm_mday,r.start_time.tm_mon,r.start_time.tm_year,r.pace.tm_min,r.pace.tm_sec);
}



//Run structure to string
void run_print(Run r){
    printf("Started on the %02d/%02d/%4d from %02d:%02d to %02d:%02d\n%.1f km in %.f minutes\nPace of %02d:%02d/km\n\n",
           r.start_time.tm_mday,r.start_time.tm_mon,r.start_time.tm_year,r.start_time.tm_hour,r.start_time.tm_min,r.end_time.tm_hour,r.end_time.tm_min,
           r.length,r.duration,r.pace.tm_min,r.pace.tm_sec);
}

void list_print(List *head) {
    List *p;
    for (p = head; p != NULL; p = p->next)
        run_print(p->data);
    printf("\n");
}



void list_free(List *head) {
    List *p = head;
    while (p != NULL) {
        List *tmp = p->next;
        free(p);
        p = tmp;
    }
}



int main()
{
  while(1){


    int input;
    printf("1 - Add new run data\n");
    printf("2 - Obtain information about a date's run\n");
    printf("3 - Obtain information about a period of time\n");
    printf("4 - Objective pace\n");
    printf("5 - Developer's info\n");
    printf("6 - Exit\n");
    scanf("%d",&input);
    switch(input){
        case 1:{
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

            printf("How many kilometres did you run?\n\n");
            scanf("%lf",&r1.length);

            r1.duration = time_difference(r1.end_time,r1.start_time);
            printf("Duration of the training was %.f minutes\n",r1.duration);


            double pace = pace_calculator(r1.duration,r1.length);
            r1.pace.tm_min = pace;
            r1.pace.tm_sec = extract_decimals(pace) * 60; //converts decimal time into MM:SS format
            printf("Pace per kilometer is %02d:%02d\n\n",r1.pace.tm_min,r1.pace.tm_sec);

            //saving the obtained data into the text file
            run_into_file(r1.start_time.tm_mday,r1.start_time.tm_mon,r1.start_time.tm_year,
                        r1.start_time.tm_hour,r1.start_time.tm_min,r1.end_time.tm_hour,r1.end_time.tm_min,
                        r1.length);
            break;
        }
        case 2:{
            List *head = NULL;
            head = read_file(head);
            struct tm date;
            printf("Enter a date(DD/MM/YYYY)\n\n");
            scanf("%02d/%02d/%04d",&date.tm_mday,&date.tm_mon,&date.tm_year);
            run_print(filter_by_date(head,date));

            break;
        }
        case 3:{
            List*head = NULL;
            head = read_file(head);
            struct tm date1;
            struct tm date2;
            printf("Enter two dates forming a period(DD/MM/YYYY)\n");
            scanf("%02d/%02d/%04d",&date1.tm_mday,&date1.tm_mon,&date1.tm_year);
            scanf("%02d/%02d/%04d",&date2.tm_mday,&date2.tm_mon,&date2.tm_year);
            printf("\n");
            if(first_date_after_second(date1,date2)) {
                printf("------------------------------------------------------------\n");
                printf("First date has to be before the second date.Please try again\n");
                printf("------------------------------------------------------------\n\n");
                break;
            }
            filter_by_period(head,date1,date2);

            break;
        }
        case 4:{

            List *head = NULL;
            head = read_file(head);
            last_run_pace(last_run(head));

            break;
        }
        case 5:{
            printf(" -------------------------------------------\n");
            printf(" C program developed by Gonzalo Alonso Sosa\n");
            printf(" December 2018 - BME - Basics of Progamming I\n");
            printf(" -------------------------------------------\n\n");
            break;
        }
        case 6: {
            exit(0);
            break;
        }
        default:
            printf("----------------------------------\n");
            printf("You have entered an invalid option\n");
            printf("----------------------------------\n");
            printf("Please enter one of these options:\n\n");
    }
  }

    return 0;


}
