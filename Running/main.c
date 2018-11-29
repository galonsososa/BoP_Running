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
    struct list * next;
} List;

//equals function: if two Runs have the exact same start time and length we consider it as the same Run
bool equals(Run r1,Run r2){
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

List *insert_run(List *head, Run run) {
    List *newel = (List *) malloc(sizeof(List)); /* New element */
    newel->data = run; /* Copy data to store to the list element */
    newel->next  = head; /* After the new element goes the rest of the list (even if it is empty) */
    return newel; /* The new element will be the new head of the list */
}

List *search_run(List *head, Run run) {
    List *p;
    for (p = head; p != NULL; p = p->next)
        if (equals(p->data,run))
            return p;
    return NULL;
}


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
    fprintf(fp,"%02d/%02d/%4d %02d:%02d-%02d:%02d %.1f\n",
            day,month,year,
            start_hour,start_min,end_hour,end_min,
            length);


    status = fclose(fp);
    if (status!=0){
     return 1;
    } else printf("---------------------------------------\nSuccesfully saved run into Running.txt\n---------------------------------------");

}

/*antiguo
void file_to_run(){
    FILE *fp = fopen("Running.txt","r");
    if (fp==NULL) return -1;

    //puts into the set a run
    Run r;
    fscanf(fp,"%d/%d/%d\n%d:%d-%d:%d\n%f\n--\n",
           &r.start_time.tm_mday,&r.start_time.tm_mon,&r.start_time.tm_year,
           &r.start_time.tm_hour,&r.start_time.tm_min,&r.end_time.tm_hour,&r.end_time.tm_min,
           &r.length);

    //puts into the set a run
   // printf("%d/%d/%d",day,month,year);//en vez de imprimir crear un array en memoria y meter los datos de una carrera
    fclose(fp);
}*/

Run string_to_run(char * str){
    Run r;
    sscanf(str,"%d/%d/%d %d:%d-%d:%d %f\n",
           &r.start_time.tm_mday,&r.start_time.tm_mon,&r.start_time.tm_year,
           &r.start_time.tm_hour,&r.start_time.tm_min,&r.end_time.tm_hour,&r.end_time.tm_min,
           &r.length);
    return r;
}

void read_file(List *head){

    FILE *fp = fopen("Running.txt","r");
    char *str;

    if(fp == NULL) {
      perror("Error opening file 'Running.txt'");
      return(-1);
    }
    while (fgets(str,50,fp)!=NULL){
        fgets(str,50,fp);
        head = insert_run(head,string_to_run(str));
    }

    fclose(fp);
}

double extract_decimals(double num){

    int intpart = (int)num;
    double decpart = num - intpart;
    return decpart;

}
double pace_calculator(int duration,double length){
    return duration/length;
}

void list_print(List *head) {
    List *p;
    for (p = head; p != NULL; p = p->next)
        printf("%d ", p->data);
    printf("\n");
}

void list_free(List *head) {
    List *p = head;
    /* Traversing the list */
    while (p != NULL) {
        /* Just writing free(p); is wrong, the rest of the list would be lost */
        /* hence p=p->nxt; would be invalid */
        List *tmp = p->next; /* Store the pointer to the rest of the list */
        free(p); /* Now we can delete the current element */
        p = tmp; /* Keep releasing the list from the next element on */
    }
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
    printf("Run distance %f",r1.length);

    r1.duration = time_difference(r1.end_time,r1.start_time);
    printf("Duration of the training is %.f minutes\n",r1.duration);

    double pace = pace_calculator(r1.duration,r1.length);
    r1.pace.tm_min = pace;
    r1.pace.tm_sec = extract_decimals(pace) * 60; //converts decimal time into MM:SS format
    printf("Pace per kilometer is %02d:%02d\n",r1.pace.tm_min,r1.pace.tm_sec);

    run_into_file(r1.start_time.tm_mday,r1.start_time.tm_mon,r1.start_time.tm_year,
                  r1.start_time.tm_hour,r1.start_time.tm_min,r1.end_time.tm_hour,r1.end_time.tm_min,
                  r1.length);

    //reading the file and saving it into a list
    List *head = NULL;
    read_file(head);
    list_print(head);
    list_free(head);


    return 0;


}
