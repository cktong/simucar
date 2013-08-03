/******************************************/
/* SIGMA RUN DEFAULTS FOR TRANSLATED MODEL */
/*******************************************/
/*

         MODEL DEFAULTS
         --------------

Model Name:         carrent2229
Model Description:  A DETERMINISTIC QUEUE (M/D/1) WITH BREAKDOWNS
Output File:        untitled.out
Output Plot Style:  NOAUTO_FIT
Run Mode:           HI_SPEED
Trace Vars:         L[1],L[2],L[3],L[4],L1[1],L1[2],L1[3],L1[4],L2[1],L2[2],L2[3],L2[4],SERVICED[1],SERVICED[2],SERVICED[3],SERVICED[4],RUNC[1]
Random Number Seed: 12345
Initial Values:     7,5,10,8,.7,3,3,3,3,7,7,7,7,3,1,1,1,1,15,15,15,15,500,30,1,7,10000,15000,20000,25000,350,400,450,500,50
Ending Condition:   STOP_ON_TIME
Ending Time:        900.000
Trace Events:       ALL EVENTS TRACED
Hide Edges:         

*/
/*******************************************/


#include "sigmafns.h"
#include "sigmalib.h"

/* EVENT FUNCTIONS USED IN THE SIMULATION MODEL */
void   RUN(void);	/*** INITIALIZATION OF THE QUEUE AND THE SERVER ***/
void   CUSTM(void);	/*** ARRIVAL OF ANY CUSTOMER, where M is uniform rv[01] ***/
void   RENT(void);	/*** START OF RENTING ***/
void   RETURN(void);	/*** CAR RETURNED ***/
void   FAIL(void);	/*** THE OCCURRENCE OF A BREAKDOWN ***/
void   FIX(void);	/*** COMPLETION OF REPAIR ON THE MACHINE ***/
void   INPUT(void);	/*** Generate input ( 4 types of cars) ***/
void   WALKIN(void);	/*** A CUSTOMER THAT IS WALKIN ***/
void   RETIRE(void);	/***  ***/
void   LVE(void);	/*** Rental service terminate due to unable to meet the demand ***/
void   RESERV(void);	/*** This is where reservations will be "held" At this time if there are no cars available then we lose the reservation ***/
void   RSVP(void);	/*** RESERVATIONS MADE HERE ***/
void   costs(void);	/***  ***/

/*   STATE VARIABLE DELARATIONS   */
long   S[5];	/***  Number of available cars per each type  ***/
long   C[5];	/***  Total number of customers per each type of car  ***/
long   J;	/***  Type of car  ***/
double R;	/***  Probability that a customer put on reservation  ***/
double START;	/***  Rental starting time  ***/
double RENTTIME[5];	/***  Total rent time  ***/
long   FR;	/***  Failure rate  ***/
long   L[5];	/***  Total loss customer  ***/
double M;	/***  NUMBER DERIVED FROM UNIFORM DIST  ***/
long   I;	/***  WALKIN OR NOT  ***/
double FAILRENT[5];	/***  time of rental before failure  ***/
long   SERVICED[5];	/***  all customers serviced  ***/
long   NS;	/***  number of customers lost or not serviced  ***/
double INCOME;	/***  amount of money made in simulation  ***/
long   ARRMM[5][2];	/***  Arrival Beta min and max  ***/
double RES;	/***  reservation beta max  ***/
double REN[5][2];	/***  reservation betas  per type of car  ***/
double RETIRET;	/***  Retire time  ***/
long   CUMCOST[5];	/***  total cost per type  ***/
double BREAKC;	/***  charge for each break  ***/
double INFLOW[5];	/***  total cash inflow per type  ***/
long   RUNC[5];	/***  revenue from running car type j  ***/
long   L2[5];	/***  Reservation Loss  ***/
long   L1[5];	/***  Loss of walkin customer  ***/
double F;	/***  time that a  car has failed  ***/
long   K;	/***  counter to keep track of j  ***/
double P;	/***  probability of failing  ***/
long   INICOST[5];	/***  initial cost incl. purchase price & insurance  ***/
long   PRICEINS[5];	/***  purchase price & insurance cost per car  ***/
long   NETP[5];	/***  net profit  ***/
double FIXTIME;	/***  time it took to fix a car  ***/
double FIXT[2];	/***  fix time beta distribution  ***/
long   TF;	/***  time to fail  ***/

/*   EVENT FUNCTIONS   */
enum
   {
   run_end_event,
   RUN_event,
   CUSTM_event,
   RENT_event,
   RETURN_event,
   FAIL_event,
   FIX_event,
   INPUT_event,
   WALKIN_event,
   RETIRE_event,
   LVE_event,
   RESERV_event,
   RSVP_event,
   costs_event,
   };

/*    MAIN PROGRAM     */
int main(int argc, char** argv)
{
  int  next_event;
  char keytoclose = 'p';

  if(!startup_check(0))
    return -1;

  /* Initialize csiglib and simulation */
  while (initialize(argc, (const char * *)argv)) {;

  /* Schedule beginning of simulation */
  event_time = current_time;
  event_type = RUN_event;
  schedule_event();

  /* Schedule end of simulation */
  event_time = stop_time;
  event_type = run_end_event;
  event_priority = 9999;
  schedule_event();

/*  EVENT EXECUTION CONTROL LOOP */
  while (!run_error && !done) {
    /* Pull next event from event list */
    next_event = c_timing();

    /* increment the event count for this event */
    event_count[next_event]++;

    /* Call appropriate event routine */
    switch ( next_event ) {
      case run_end_event:  run_end();
               break;

      case RUN_event:  RUN();
               event_trace("RUN",event_count[next_event]);
               break;

      case CUSTM_event:  CUSTM();
               event_trace("CUSTM",event_count[next_event]);
               break;

      case RENT_event:  RENT();
               event_trace("RENT",event_count[next_event]);
               break;

      case RETURN_event:  RETURN();
               event_trace("RETURN",event_count[next_event]);
               break;

      case FAIL_event:  FAIL();
               event_trace("FAIL",event_count[next_event]);
               break;

      case FIX_event:  FIX();
               event_trace("FIX",event_count[next_event]);
               break;

      case INPUT_event:  INPUT();
               event_trace("INPUT",event_count[next_event]);
               break;

      case WALKIN_event:  WALKIN();
               event_trace("WALKIN",event_count[next_event]);
               break;

      case RETIRE_event:  RETIRE();
               event_trace("RETIRE",event_count[next_event]);
               break;

      case LVE_event:  LVE();
               event_trace("LVE",event_count[next_event]);
               break;

      case RESERV_event:  RESERV();
               event_trace("RESERV",event_count[next_event]);
               break;

      case RSVP_event:  RSVP();
               event_trace("RSVP",event_count[next_event]);
               break;

      case costs_event:  costs();
               event_trace("costs",event_count[next_event]);
               break;

      }
    }
  }
// experiments terminated
printf("Experiments ended! If runs end early: \n\r1. check fields in *.exp file. \n\r2. check if output file was already open. \n\r");
return 0;
}

void
event_trace(const char * name_of_event,const long count)
{
  c_timest(L[1], 1, 0);
  c_sampst(L[1], 1, 0);
  c_timest(L[2], 2, 0);
  c_sampst(L[2], 2, 0);
  c_timest(L[3], 3, 0);
  c_sampst(L[3], 3, 0);
  c_timest(L[4], 4, 0);
  c_sampst(L[4], 4, 0);
  c_timest(L1[1], 5, 0);
  c_sampst(L1[1], 5, 0);
  c_timest(L1[2], 6, 0);
  c_sampst(L1[2], 6, 0);
  c_timest(L1[3], 7, 0);
  c_sampst(L1[3], 7, 0);
  c_timest(L1[4], 8, 0);
  c_sampst(L1[4], 8, 0);
  c_timest(L2[1], 9, 0);
  c_sampst(L2[1], 9, 0);
  c_timest(L2[2], 10, 0);
  c_sampst(L2[2], 10, 0);
  c_timest(L2[3], 11, 0);
  c_sampst(L2[3], 11, 0);
  c_timest(L2[4], 12, 0);
  c_sampst(L2[4], 12, 0);
  c_timest(SERVICED[1], 13, 0);
  c_sampst(SERVICED[1], 13, 0);
  c_timest(SERVICED[2], 14, 0);
  c_sampst(SERVICED[2], 14, 0);
  c_timest(SERVICED[3], 15, 0);
  c_sampst(SERVICED[3], 15, 0);
  c_timest(SERVICED[4], 16, 0);
  c_sampst(SERVICED[4], 16, 0);
  c_timest(RUNC[1], 17, 0);
  c_sampst(RUNC[1], 17, 0);
  if(trace_flag) fprintf(output_fp, "%9.3f\t %6s\t%6d ",current_time,name_of_event,count);
  if(trace_flag) fprintf(output_fp, "	%7.3g 	%7.3g 	%7.3g 	%7.3g 	%7.3g 	%7.3g 	%7.3g 	%7.3g 	%7.3g 	%7.3g 	%7.3g 	%7.3g 	%7.3g 	%7.3g 	%7.3g 	%7.3g 	%7.3g \n"
,(double)L[1], (double)L[2], (double)L[3], (double)L[4], (double)L1[1], (double)L1[2], (double)L1[3], (double)L1[4], (double)L2[1], (double)L2[2], (double)L2[3], (double)L2[4], (double)SERVICED[1], (double)SERVICED[2], (double)SERVICED[3], (double)SERVICED[4], (double)RUNC[1]);
}



int
initialize(int argc, const char** argv)
{
static int first_time = 1;     /* First time in initialize? */
static FILE *input_fp;     /* For reading from the input file */
char *exp_file_name;       /* For constructing input file name */
char y_n = 'p';            /* yes/no for file overwrite*/

       char dir[256];
       char fname[256];
       char ext[256];
       char simulation[1024];
       char experient_name[1024];
        _splitpath( argv[0], NULL, dir, fname, ext );
       strcpy(simulation, fname);
       strcat(simulation, ext);
       strcpy(experient_name, fname);
       strcat(experient_name, ".exp");
     printf("Running the simulation: %s\n", simulation);
    if(strlen(dir) !=0)
       printf("In Path: %s\n",dir);
  if (first_time) {
    exp_file_name = _strdup(argv[0]);
    exp_file_name[strlen(exp_file_name)-1] = 'p';
    printf("\nLooking for experiment file: %s\n",experient_name);
    }

  if ((first_time && (input_fp=fopen(exp_file_name,"r"))!=NULL)
                                           || input_fp!=NULL) {
  if (first_time) {
     first_time = 0; /* Reset for next time into initialize */
     printf("Found. Use [Control]-C to abort replications.\n");
     }

  /* We have run control file of type *.exp          */
  /* Read next set of data from run control file.    */
  if (fscanf(input_fp,"%s %1s %ld %lf %d", output_file_name, &y_n, &rndsd, &stop_time, &trace_flag)<4
     || fscanf(input_fp,"%ld", &S[1])<1
     || fscanf(input_fp,"%ld", &S[2])<1
     || fscanf(input_fp,"%ld", &S[3])<1
     || fscanf(input_fp,"%ld", &S[4])<1
     || fscanf(input_fp,"%lf", &R)<1
     || fscanf(input_fp,"%ld", &ARRMM[1][0])<1
     || fscanf(input_fp,"%ld", &ARRMM[2][0])<1
     || fscanf(input_fp,"%ld", &ARRMM[3][0])<1
     || fscanf(input_fp,"%ld", &ARRMM[4][0])<1
     || fscanf(input_fp,"%ld", &ARRMM[1][1])<1
     || fscanf(input_fp,"%ld", &ARRMM[2][1])<1
     || fscanf(input_fp,"%ld", &ARRMM[3][1])<1
     || fscanf(input_fp,"%ld", &ARRMM[4][1])<1
     || fscanf(input_fp,"%lf", &RES)<1
     || fscanf(input_fp,"%lf", &REN[1][0])<1
     || fscanf(input_fp,"%lf", &REN[2][0])<1
     || fscanf(input_fp,"%lf", &REN[3][0])<1
     || fscanf(input_fp,"%lf", &REN[4][0])<1
     || fscanf(input_fp,"%lf", &REN[1][1])<1
     || fscanf(input_fp,"%lf", &REN[2][1])<1
     || fscanf(input_fp,"%lf", &REN[3][1])<1
     || fscanf(input_fp,"%lf", &REN[4][1])<1
     || fscanf(input_fp,"%ld", &TF)<1
     || fscanf(input_fp,"%lf", &FIXT[0])<1
     || fscanf(input_fp,"%lf", &FIXT[1])<1
     || fscanf(input_fp,"%lf", &BREAKC)<1
     || fscanf(input_fp,"%ld", &RUNC[1])<1
     || fscanf(input_fp,"%ld", &RUNC[2])<1
     || fscanf(input_fp,"%ld", &RUNC[3])<1
     || fscanf(input_fp,"%ld", &RUNC[4])<1
     || fscanf(input_fp,"%ld", &PRICEINS[1])<1
     || fscanf(input_fp,"%ld", &PRICEINS[2])<1
     || fscanf(input_fp,"%ld", &PRICEINS[3])<1
     || fscanf(input_fp,"%ld", &PRICEINS[4])<1
     ) {
     /* End of run control file */
     fclose(input_fp);
     return 0;
     }

  if (y_n != 'y' && y_n != 'Y' && y_n != 'n' && y_n != 'N') { 
  fprintf(stderr,"INPUT ERROR: Invalid append file flag in *.exp file: (y=append, n=overwrite old file)\n"); 
  return 0; 
  }

  if (y_n == 'y' || y_n == 'Y') {
     if ((output_fp = fopen(output_file_name,"a"))==NULL) { 
     /* Output file can't be opened. */
     fprintf(stderr,"\nINPUT ERROR: Cannot open output file %s in *.exp file\n",argv[1]);
  return 0; 
  }
  }
if (y_n == 'n' || y_n == 'N') {
     if ((output_fp = fopen(output_file_name,"w"))==NULL) { 
     /* Output file can't be opened. */
     fprintf(stderr,"\nINPUT ERROR: Cannot open output file %s in *.exp file\n",argv[1]);
     return 0;
     }
     }

  if (rndsd < 1 || rndsd > 65534) {
     fprintf(stderr,"\nINPUT ERROR: Random seed %ld is not between 0 and 65534\n",rndsd);
     return 0;
     }

  if (stop_time <= 0.0) {
     fprintf(stderr,"\nINPUT ERROR: Stopping time %lf is negative!\n",stop_time);
     return 0;
     }

  if (trace_flag != 0 && trace_flag != 1) {
     fprintf(stderr,"\nINPUT ERROR: Invalid trace_flag=%d: (1=full trace, 0=summary only)\n",trace_flag);
     return 0;
     }

  done = 0;
     }

 else if (first_time) { /* And open failed, implies data from stdin */
    first_time = 0; /* Reset for next time into initialize */
    printf("Not found, input data at the keyboard.\n");
     /* Give output file name */
     while(y_n != 'y' && y_n != 'Y' && y_n != 'n' && y_n != 'N')
         {
         printf("\nOUTPUT FILE (Enter File Name with Path):\n");
         scanf("%s", output_file_name);
         fflush(stdin);
         sprintf(filename,"%.20s", output_file_name);
         printf("WARNING:File %.20s must not be open!!\n If file does not exist it will be created.\n",filename);
         printf("Do you want the new output appended to this file? (yes/[no])\n");
         scanf("%1s",&y_n);
         fflush(stdin);
         }
     if(y_n == 'y' || y_n == 'Y') output_fp = fopen(filename,"a");
     if(y_n == 'n' || y_n == 'N') output_fp = fopen(filename,"w");

     /* Read in random number seed */
     printf("\n\nRANDOM NUMBER SEED (Enter Integer Between 0 and 65534):\n");
     scanf("%ld", &rndsd);
     fflush(stdin);

     /* Read in run stopping time */
     printf("\nSTOPPING TIME (Enter number of time units until run termination):\n");
     scanf("%lf", &stop_time);
     fflush(stdin);

     /* Read in trace_flag */
     printf("\n\nTRACE (1 = Event Trace, 0 = Summary Only):\n");
     scanf("%d", &trace_flag);
     fflush(stdin);

     /* Parameters for the initial event */;
     printf ( "\nEnter initial value for S[1]: \n");
     scanf  ( "%ld", &S[1]);
     printf ( "\nEnter initial value for S[2]: \n");
     scanf  ( "%ld", &S[2]);
     printf ( "\nEnter initial value for S[3]: \n");
     scanf  ( "%ld", &S[3]);
     printf ( "\nEnter initial value for S[4]: \n");
     scanf  ( "%ld", &S[4]);
     printf ( "\nEnter initial value for R: \n");
     scanf  ( "%lf", &R);
     printf ( "\nEnter initial value for ARRMM[1][0]: \n");
     scanf  ( "%ld", &ARRMM[1][0]);
     printf ( "\nEnter initial value for ARRMM[2][0]: \n");
     scanf  ( "%ld", &ARRMM[2][0]);
     printf ( "\nEnter initial value for ARRMM[3][0]: \n");
     scanf  ( "%ld", &ARRMM[3][0]);
     printf ( "\nEnter initial value for ARRMM[4][0]: \n");
     scanf  ( "%ld", &ARRMM[4][0]);
     printf ( "\nEnter initial value for ARRMM[1][1]: \n");
     scanf  ( "%ld", &ARRMM[1][1]);
     printf ( "\nEnter initial value for ARRMM[2][1]: \n");
     scanf  ( "%ld", &ARRMM[2][1]);
     printf ( "\nEnter initial value for ARRMM[3][1]: \n");
     scanf  ( "%ld", &ARRMM[3][1]);
     printf ( "\nEnter initial value for ARRMM[4][1]: \n");
     scanf  ( "%ld", &ARRMM[4][1]);
     printf ( "\nEnter initial value for RES: \n");
     scanf  ( "%lf", &RES);
     printf ( "\nEnter initial value for REN[1][0]: \n");
     scanf  ( "%lf", &REN[1][0]);
     printf ( "\nEnter initial value for REN[2][0]: \n");
     scanf  ( "%lf", &REN[2][0]);
     printf ( "\nEnter initial value for REN[3][0]: \n");
     scanf  ( "%lf", &REN[3][0]);
     printf ( "\nEnter initial value for REN[4][0]: \n");
     scanf  ( "%lf", &REN[4][0]);
     printf ( "\nEnter initial value for REN[1][1]: \n");
     scanf  ( "%lf", &REN[1][1]);
     printf ( "\nEnter initial value for REN[2][1]: \n");
     scanf  ( "%lf", &REN[2][1]);
     printf ( "\nEnter initial value for REN[3][1]: \n");
     scanf  ( "%lf", &REN[3][1]);
     printf ( "\nEnter initial value for REN[4][1]: \n");
     scanf  ( "%lf", &REN[4][1]);
     printf ( "\nEnter initial value for TF: \n");
     scanf  ( "%ld", &TF);
     printf ( "\nEnter initial value for FIXT[0]: \n");
     scanf  ( "%lf", &FIXT[0]);
     printf ( "\nEnter initial value for FIXT[1]: \n");
     scanf  ( "%lf", &FIXT[1]);
     printf ( "\nEnter initial value for BREAKC: \n");
     scanf  ( "%lf", &BREAKC);
     printf ( "\nEnter initial value for RUNC[1]: \n");
     scanf  ( "%ld", &RUNC[1]);
     printf ( "\nEnter initial value for RUNC[2]: \n");
     scanf  ( "%ld", &RUNC[2]);
     printf ( "\nEnter initial value for RUNC[3]: \n");
     scanf  ( "%ld", &RUNC[3]);
     printf ( "\nEnter initial value for RUNC[4]: \n");
     scanf  ( "%ld", &RUNC[4]);
     printf ( "\nEnter initial value for PRICEINS[1]: \n");
     scanf  ( "%ld", &PRICEINS[1]);
     printf ( "\nEnter initial value for PRICEINS[2]: \n");
     scanf  ( "%ld", &PRICEINS[2]);
     printf ( "\nEnter initial value for PRICEINS[3]: \n");
     scanf  ( "%ld", &PRICEINS[3]);
     printf ( "\nEnter initial value for PRICEINS[4]: \n");
     scanf  ( "%ld", &PRICEINS[4]);
   }

 else {
    /* this is not the first time and there is no .exp file */
    return 0;
    }
  
   /* PLACE CUSTOMIZED INITIALIZATIONS HERE */

if (trace_flag)
   {
   fprintf(output_fp,"    Time\t  Event\t Count");
   fprintf(output_fp,"	            L[1]");
   fprintf(output_fp,"	            L[2]");
   fprintf(output_fp,"	            L[3]");
   fprintf(output_fp,"	            L[4]");
   fprintf(output_fp,"	           L1[1]");
   fprintf(output_fp,"	           L1[2]");
   fprintf(output_fp,"	           L1[3]");
   fprintf(output_fp,"	           L1[4]");
   fprintf(output_fp,"	           L2[1]");
   fprintf(output_fp,"	           L2[2]");
   fprintf(output_fp,"	           L2[3]");
   fprintf(output_fp,"	           L2[4]");
   fprintf(output_fp,"	     SERVICED[1]");
   fprintf(output_fp,"	     SERVICED[2]");
   fprintf(output_fp,"	     SERVICED[3]");
   fprintf(output_fp,"	     SERVICED[4]");
   fprintf(output_fp,"	           RUNC[1] ");
   fprintf(output_fp,"\n");
   }
  /* Initialize CSIGLIB variables and files */
  c_initlk(rndsd);
  c_initfiles();

  return(1);
}



void
run_end()
{
  printf("\r\nNormal completion after %f time units\n",current_time);
  printf("The Next Seed In the Random Input Stream is %ld\n",rndsd);

  ///  Summary statistics ///
  fprintf(output_fp,"SUMMARY STATISTICS\n");
  printf("SUMMARY STATISTICS\n");
  c_timest(L[1], 1, 1);
   fprintf(output_fp, "L[1]:\n Time Ave. = \t%7.4g Time Sample Var. =\t%7.4g\n", transfer[4], transfer[7]);
   printf("L[1]:\n Time Ave. = 	%7.4g Time Sample Var. = 	%7.4g\n", transfer[4], transfer[7]);
  c_sampst(L[1], 1, 1);
   fprintf(output_fp, " Event Ave. =\t%7.4g Event Sample Var. =\t%7.4g\n", transfer[4], transfer[8]);
   fprintf(output_fp, " Minimum =\t%7.4g\n", transfer[7]);
   fprintf(output_fp, " Maximum =\t%7.4g\n", transfer[6]);
   printf(" Event Ave.  = 	%7.4g Event Sample Var.= 	%7.4g\n", transfer[4], transfer[8]);
   printf(" Minimum  = 	%7.4g\n", transfer[7]);
   printf(" Maximum  = 	%7.4g\n", transfer[6]);
  c_timest(L[2], 2, 1);
   fprintf(output_fp, "L[2]:\n Time Ave. = \t%7.4g Time Sample Var. =\t%7.4g\n", transfer[4], transfer[7]);
   printf("L[2]:\n Time Ave. = 	%7.4g Time Sample Var. = 	%7.4g\n", transfer[4], transfer[7]);
  c_sampst(L[2], 2, 1);
   fprintf(output_fp, " Event Ave. =\t%7.4g Event Sample Var. =\t%7.4g\n", transfer[4], transfer[8]);
   fprintf(output_fp, " Minimum =\t%7.4g\n", transfer[7]);
   fprintf(output_fp, " Maximum =\t%7.4g\n", transfer[6]);
   printf(" Event Ave.  = 	%7.4g Event Sample Var.= 	%7.4g\n", transfer[4], transfer[8]);
   printf(" Minimum  = 	%7.4g\n", transfer[7]);
   printf(" Maximum  = 	%7.4g\n", transfer[6]);
  c_timest(L[3], 3, 1);
   fprintf(output_fp, "L[3]:\n Time Ave. = \t%7.4g Time Sample Var. =\t%7.4g\n", transfer[4], transfer[7]);
   printf("L[3]:\n Time Ave. = 	%7.4g Time Sample Var. = 	%7.4g\n", transfer[4], transfer[7]);
  c_sampst(L[3], 3, 1);
   fprintf(output_fp, " Event Ave. =\t%7.4g Event Sample Var. =\t%7.4g\n", transfer[4], transfer[8]);
   fprintf(output_fp, " Minimum =\t%7.4g\n", transfer[7]);
   fprintf(output_fp, " Maximum =\t%7.4g\n", transfer[6]);
   printf(" Event Ave.  = 	%7.4g Event Sample Var.= 	%7.4g\n", transfer[4], transfer[8]);
   printf(" Minimum  = 	%7.4g\n", transfer[7]);
   printf(" Maximum  = 	%7.4g\n", transfer[6]);
  c_timest(L[4], 4, 1);
   fprintf(output_fp, "L[4]:\n Time Ave. = \t%7.4g Time Sample Var. =\t%7.4g\n", transfer[4], transfer[7]);
   printf("L[4]:\n Time Ave. = 	%7.4g Time Sample Var. = 	%7.4g\n", transfer[4], transfer[7]);
  c_sampst(L[4], 4, 1);
   fprintf(output_fp, " Event Ave. =\t%7.4g Event Sample Var. =\t%7.4g\n", transfer[4], transfer[8]);
   fprintf(output_fp, " Minimum =\t%7.4g\n", transfer[7]);
   fprintf(output_fp, " Maximum =\t%7.4g\n", transfer[6]);
   printf(" Event Ave.  = 	%7.4g Event Sample Var.= 	%7.4g\n", transfer[4], transfer[8]);
   printf(" Minimum  = 	%7.4g\n", transfer[7]);
   printf(" Maximum  = 	%7.4g\n", transfer[6]);
  c_timest(L1[1], 5, 1);
   fprintf(output_fp, "L1[1]:\n Time Ave. = \t%7.4g Time Sample Var. =\t%7.4g\n", transfer[4], transfer[7]);
   printf("L1[1]:\n Time Ave. = 	%7.4g Time Sample Var. = 	%7.4g\n", transfer[4], transfer[7]);
  c_sampst(L1[1], 5, 1);
   fprintf(output_fp, " Event Ave. =\t%7.4g Event Sample Var. =\t%7.4g\n", transfer[4], transfer[8]);
   fprintf(output_fp, " Minimum =\t%7.4g\n", transfer[7]);
   fprintf(output_fp, " Maximum =\t%7.4g\n", transfer[6]);
   printf(" Event Ave.  = 	%7.4g Event Sample Var.= 	%7.4g\n", transfer[4], transfer[8]);
   printf(" Minimum  = 	%7.4g\n", transfer[7]);
   printf(" Maximum  = 	%7.4g\n", transfer[6]);
  c_timest(L1[2], 6, 1);
   fprintf(output_fp, "L1[2]:\n Time Ave. = \t%7.4g Time Sample Var. =\t%7.4g\n", transfer[4], transfer[7]);
   printf("L1[2]:\n Time Ave. = 	%7.4g Time Sample Var. = 	%7.4g\n", transfer[4], transfer[7]);
  c_sampst(L1[2], 6, 1);
   fprintf(output_fp, " Event Ave. =\t%7.4g Event Sample Var. =\t%7.4g\n", transfer[4], transfer[8]);
   fprintf(output_fp, " Minimum =\t%7.4g\n", transfer[7]);
   fprintf(output_fp, " Maximum =\t%7.4g\n", transfer[6]);
   printf(" Event Ave.  = 	%7.4g Event Sample Var.= 	%7.4g\n", transfer[4], transfer[8]);
   printf(" Minimum  = 	%7.4g\n", transfer[7]);
   printf(" Maximum  = 	%7.4g\n", transfer[6]);
  c_timest(L1[3], 7, 1);
   fprintf(output_fp, "L1[3]:\n Time Ave. = \t%7.4g Time Sample Var. =\t%7.4g\n", transfer[4], transfer[7]);
   printf("L1[3]:\n Time Ave. = 	%7.4g Time Sample Var. = 	%7.4g\n", transfer[4], transfer[7]);
  c_sampst(L1[3], 7, 1);
   fprintf(output_fp, " Event Ave. =\t%7.4g Event Sample Var. =\t%7.4g\n", transfer[4], transfer[8]);
   fprintf(output_fp, " Minimum =\t%7.4g\n", transfer[7]);
   fprintf(output_fp, " Maximum =\t%7.4g\n", transfer[6]);
   printf(" Event Ave.  = 	%7.4g Event Sample Var.= 	%7.4g\n", transfer[4], transfer[8]);
   printf(" Minimum  = 	%7.4g\n", transfer[7]);
   printf(" Maximum  = 	%7.4g\n", transfer[6]);
  c_timest(L1[4], 8, 1);
   fprintf(output_fp, "L1[4]:\n Time Ave. = \t%7.4g Time Sample Var. =\t%7.4g\n", transfer[4], transfer[7]);
   printf("L1[4]:\n Time Ave. = 	%7.4g Time Sample Var. = 	%7.4g\n", transfer[4], transfer[7]);
  c_sampst(L1[4], 8, 1);
   fprintf(output_fp, " Event Ave. =\t%7.4g Event Sample Var. =\t%7.4g\n", transfer[4], transfer[8]);
   fprintf(output_fp, " Minimum =\t%7.4g\n", transfer[7]);
   fprintf(output_fp, " Maximum =\t%7.4g\n", transfer[6]);
   printf(" Event Ave.  = 	%7.4g Event Sample Var.= 	%7.4g\n", transfer[4], transfer[8]);
   printf(" Minimum  = 	%7.4g\n", transfer[7]);
   printf(" Maximum  = 	%7.4g\n", transfer[6]);
  c_timest(L2[1], 9, 1);
   fprintf(output_fp, "L2[1]:\n Time Ave. = \t%7.4g Time Sample Var. =\t%7.4g\n", transfer[4], transfer[7]);
   printf("L2[1]:\n Time Ave. = 	%7.4g Time Sample Var. = 	%7.4g\n", transfer[4], transfer[7]);
  c_sampst(L2[1], 9, 1);
   fprintf(output_fp, " Event Ave. =\t%7.4g Event Sample Var. =\t%7.4g\n", transfer[4], transfer[8]);
   fprintf(output_fp, " Minimum =\t%7.4g\n", transfer[7]);
   fprintf(output_fp, " Maximum =\t%7.4g\n", transfer[6]);
   printf(" Event Ave.  = 	%7.4g Event Sample Var.= 	%7.4g\n", transfer[4], transfer[8]);
   printf(" Minimum  = 	%7.4g\n", transfer[7]);
   printf(" Maximum  = 	%7.4g\n", transfer[6]);
  c_timest(L2[2], 10, 1);
   fprintf(output_fp, "L2[2]:\n Time Ave. = \t%7.4g Time Sample Var. =\t%7.4g\n", transfer[4], transfer[7]);
   printf("L2[2]:\n Time Ave. = 	%7.4g Time Sample Var. = 	%7.4g\n", transfer[4], transfer[7]);
  c_sampst(L2[2], 10, 1);
   fprintf(output_fp, " Event Ave. =\t%7.4g Event Sample Var. =\t%7.4g\n", transfer[4], transfer[8]);
   fprintf(output_fp, " Minimum =\t%7.4g\n", transfer[7]);
   fprintf(output_fp, " Maximum =\t%7.4g\n", transfer[6]);
   printf(" Event Ave.  = 	%7.4g Event Sample Var.= 	%7.4g\n", transfer[4], transfer[8]);
   printf(" Minimum  = 	%7.4g\n", transfer[7]);
   printf(" Maximum  = 	%7.4g\n", transfer[6]);
  c_timest(L2[3], 11, 1);
   fprintf(output_fp, "L2[3]:\n Time Ave. = \t%7.4g Time Sample Var. =\t%7.4g\n", transfer[4], transfer[7]);
   printf("L2[3]:\n Time Ave. = 	%7.4g Time Sample Var. = 	%7.4g\n", transfer[4], transfer[7]);
  c_sampst(L2[3], 11, 1);
   fprintf(output_fp, " Event Ave. =\t%7.4g Event Sample Var. =\t%7.4g\n", transfer[4], transfer[8]);
   fprintf(output_fp, " Minimum =\t%7.4g\n", transfer[7]);
   fprintf(output_fp, " Maximum =\t%7.4g\n", transfer[6]);
   printf(" Event Ave.  = 	%7.4g Event Sample Var.= 	%7.4g\n", transfer[4], transfer[8]);
   printf(" Minimum  = 	%7.4g\n", transfer[7]);
   printf(" Maximum  = 	%7.4g\n", transfer[6]);
  c_timest(L2[4], 12, 1);
   fprintf(output_fp, "L2[4]:\n Time Ave. = \t%7.4g Time Sample Var. =\t%7.4g\n", transfer[4], transfer[7]);
   printf("L2[4]:\n Time Ave. = 	%7.4g Time Sample Var. = 	%7.4g\n", transfer[4], transfer[7]);
  c_sampst(L2[4], 12, 1);
   fprintf(output_fp, " Event Ave. =\t%7.4g Event Sample Var. =\t%7.4g\n", transfer[4], transfer[8]);
   fprintf(output_fp, " Minimum =\t%7.4g\n", transfer[7]);
   fprintf(output_fp, " Maximum =\t%7.4g\n", transfer[6]);
   printf(" Event Ave.  = 	%7.4g Event Sample Var.= 	%7.4g\n", transfer[4], transfer[8]);
   printf(" Minimum  = 	%7.4g\n", transfer[7]);
   printf(" Maximum  = 	%7.4g\n", transfer[6]);
  c_timest(SERVICED[1], 13, 1);
   fprintf(output_fp, "SERVICED[1]:\n Time Ave. = \t%7.4g Time Sample Var. =\t%7.4g\n", transfer[4], transfer[7]);
   printf("SERVICED[1]:\n Time Ave. = 	%7.4g Time Sample Var. = 	%7.4g\n", transfer[4], transfer[7]);
  c_sampst(SERVICED[1], 13, 1);
   fprintf(output_fp, " Event Ave. =\t%7.4g Event Sample Var. =\t%7.4g\n", transfer[4], transfer[8]);
   fprintf(output_fp, " Minimum =\t%7.4g\n", transfer[7]);
   fprintf(output_fp, " Maximum =\t%7.4g\n", transfer[6]);
   printf(" Event Ave.  = 	%7.4g Event Sample Var.= 	%7.4g\n", transfer[4], transfer[8]);
   printf(" Minimum  = 	%7.4g\n", transfer[7]);
   printf(" Maximum  = 	%7.4g\n", transfer[6]);
  c_timest(SERVICED[2], 14, 1);
   fprintf(output_fp, "SERVICED[2]:\n Time Ave. = \t%7.4g Time Sample Var. =\t%7.4g\n", transfer[4], transfer[7]);
   printf("SERVICED[2]:\n Time Ave. = 	%7.4g Time Sample Var. = 	%7.4g\n", transfer[4], transfer[7]);
  c_sampst(SERVICED[2], 14, 1);
   fprintf(output_fp, " Event Ave. =\t%7.4g Event Sample Var. =\t%7.4g\n", transfer[4], transfer[8]);
   fprintf(output_fp, " Minimum =\t%7.4g\n", transfer[7]);
   fprintf(output_fp, " Maximum =\t%7.4g\n", transfer[6]);
   printf(" Event Ave.  = 	%7.4g Event Sample Var.= 	%7.4g\n", transfer[4], transfer[8]);
   printf(" Minimum  = 	%7.4g\n", transfer[7]);
   printf(" Maximum  = 	%7.4g\n", transfer[6]);
  c_timest(SERVICED[3], 15, 1);
   fprintf(output_fp, "SERVICED[3]:\n Time Ave. = \t%7.4g Time Sample Var. =\t%7.4g\n", transfer[4], transfer[7]);
   printf("SERVICED[3]:\n Time Ave. = 	%7.4g Time Sample Var. = 	%7.4g\n", transfer[4], transfer[7]);
  c_sampst(SERVICED[3], 15, 1);
   fprintf(output_fp, " Event Ave. =\t%7.4g Event Sample Var. =\t%7.4g\n", transfer[4], transfer[8]);
   fprintf(output_fp, " Minimum =\t%7.4g\n", transfer[7]);
   fprintf(output_fp, " Maximum =\t%7.4g\n", transfer[6]);
   printf(" Event Ave.  = 	%7.4g Event Sample Var.= 	%7.4g\n", transfer[4], transfer[8]);
   printf(" Minimum  = 	%7.4g\n", transfer[7]);
   printf(" Maximum  = 	%7.4g\n", transfer[6]);
  c_timest(SERVICED[4], 16, 1);
   fprintf(output_fp, "SERVICED[4]:\n Time Ave. = \t%7.4g Time Sample Var. =\t%7.4g\n", transfer[4], transfer[7]);
   printf("SERVICED[4]:\n Time Ave. = 	%7.4g Time Sample Var. = 	%7.4g\n", transfer[4], transfer[7]);
  c_sampst(SERVICED[4], 16, 1);
   fprintf(output_fp, " Event Ave. =\t%7.4g Event Sample Var. =\t%7.4g\n", transfer[4], transfer[8]);
   fprintf(output_fp, " Minimum =\t%7.4g\n", transfer[7]);
   fprintf(output_fp, " Maximum =\t%7.4g\n", transfer[6]);
   printf(" Event Ave.  = 	%7.4g Event Sample Var.= 	%7.4g\n", transfer[4], transfer[8]);
   printf(" Minimum  = 	%7.4g\n", transfer[7]);
   printf(" Maximum  = 	%7.4g\n", transfer[6]);
  c_timest(RUNC[1], 17, 1);
   fprintf(output_fp, "RUNC[1]:\n Time Ave. = \t%7.4g Time Sample Var. =\t%7.4g\n", transfer[4], transfer[7]);
   printf("RUNC[1]:\n Time Ave. = 	%7.4g Time Sample Var. = 	%7.4g\n", transfer[4], transfer[7]);
  c_sampst(RUNC[1], 17, 1);
   fprintf(output_fp, " Event Ave. =\t%7.4g Event Sample Var. =\t%7.4g\n", transfer[4], transfer[8]);
   fprintf(output_fp, " Minimum =\t%7.4g\n", transfer[7]);
   fprintf(output_fp, " Maximum =\t%7.4g\n", transfer[6]);
   printf(" Event Ave.  = 	%7.4g Event Sample Var.= 	%7.4g\n", transfer[4], transfer[8]);
   printf(" Minimum  = 	%7.4g\n", transfer[7]);
   printf(" Maximum  = 	%7.4g\n", transfer[6]);
  printf("Output written to, %s\n",output_file_name);
  fclose(output_fp);
  c_closedisk();
  done = 1;
}



/****************************/
/*     EVENT FUNCTIONS      */
/****************************/

/*** INITIALIZATION OF THE QUEUE AND THE SERVER ***/
void
RUN()
{
int  _edge_condition[1];

  /* Attribute Value(s) Passed to this Event */

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( 1==1 );

  /* schedule future events */
  if (_edge_condition[0])
    /***  INITIATE THE FIRST JOB ARRIVAL  ***/
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = 1;
    transfer[4] = 0;
    for ( t_index=5; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = INPUT_event;
    event_priority = 5;
    schedule_event();
    }

}


/*** ARRIVAL OF ANY CUSTOMER, where M is uniform rv[01] ***/
void
CUSTM()
{
int  _edge_condition[4];

  /* Attribute Value(s) Passed to this Event */
  J = (long) transfer[3];

  /* state changes */
  C[J]=C[J]+1;
  M=RND;

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( 1==1 );
  _edge_condition[1] = ( M>R&&S[J]>0 );
  _edge_condition[2] = ( M<=R );
  _edge_condition[3] = ( M>R&&S[J]<=0 );

  /* schedule future events */
  if (_edge_condition[0])
    /***  SCHEDULE THE NEXT ARRIVAL ( arrive according to some distribution specified by the customer type)  ***/
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = J;
    for ( t_index=4; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 3+ARRMM[J][1]*BET(2,2);
    event_type = CUSTM_event;
    event_priority = 6;
    schedule_event();
    }

  if (_edge_condition[1])
    /***  THE CUSTOMER BELONGS TO WALKIN  ***/
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = J;
    transfer[4] = M>R;
    for ( t_index=5; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = WALKIN_event;
    event_priority = 5;
    schedule_event();
    }

  if (_edge_condition[2])
    /***  Delay needs to be determined by something that depends on the customer type. CAR IS SUPPOSED TO BE RESERVED  ***/
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = J;
    transfer[4] = M>R;
    for ( t_index=5; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 1+RES*BET(1.2,1.2)-1;
    event_type = RESERV_event;
    event_priority = 3;
    schedule_event();
    }

  if (_edge_condition[3])
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = J;
    transfer[4] = I;
    for ( t_index=5; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = LVE_event;
    event_priority = 5;
    schedule_event();
    }

}


/*** START OF RENTING ***/
void
RENT()
{
int  _edge_condition[2];

  /* Attribute Value(s) Passed to this Event */
  ENT[0] = (long) transfer[3];
  ENT[1] = (long) transfer[4];
  ENT[2] = (long) transfer[5];
  ENT[3] = (long) transfer[6];
  ENT[4] = (long) transfer[7];
  ENT[5] = (long) transfer[8];
  ENT[6] = (long) transfer[9];

  /* state changes */
  P=RND;
  ENT[2]=CLK;

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( P<.7 );
  _edge_condition[1] = ( P>=.7 );

  /* schedule future events */
  if (_edge_condition[0])
    /***  THE CAR IS TAKEN OUT OF THE LOT FOR A RESERVATION OR WALKIN PERIOD  ***/
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = ENT[1];
    transfer[4] = 0;
    transfer[5] = ENT[0];
    transfer[6] = ENT[1];
    transfer[7] = ENT[2];
    transfer[8] = ENT[3];
    transfer[9] = ENT[4];
    transfer[10] = ENT[5];
    transfer[11] = ENT[6];
    for ( t_index=12; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + REN[J][0]+REN[J][1]*BET(1.2,1.2);
    event_type = RETURN_event;
    event_priority = 3;
    schedule_event();
    }

  if (_edge_condition[1])
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = ENT[0];
    transfer[4] = ENT[1];
    transfer[5] = ENT[2];
    transfer[6] = ENT[3];
    transfer[7] = ENT[4];
    transfer[8] = ENT[5];
    transfer[9] = ENT[6];
    for ( t_index=10; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 1+3*TF*RND;
    event_type = FAIL_event;
    event_priority = 5;
    schedule_event();
    }

}


/*** CAR RETURNED ***/
void
RETURN()
{
int  _edge_condition[1];

  /* Attribute Value(s) Passed to this Event */
  J = (long) transfer[3];
  F = transfer[4];
  ENT[0] = (long) transfer[5];
  ENT[1] = (long) transfer[6];
  ENT[2] = (long) transfer[7];
  ENT[3] = (long) transfer[8];
  ENT[4] = (long) transfer[9];
  ENT[5] = (long) transfer[10];
  ENT[6] = (long) transfer[11];

  /* state changes */
  S[J]=S[J]+PUT(FST,J);
  RENTTIME[J]=F+CLK-ENT[2];
  INFLOW[J]=INFLOW[J]+RUNC[J]*RENTTIME[J];
  SERVICED[J]=SERVICED[J]+1;
  CUMCOST[J]=CUMCOST[J]+BREAKC*FIXTIME;
  NETP[J]=INFLOW[J]-CUMCOST[J]-INICOST[J];

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( ENT[4]>1&&S[J]>0 );

  /* schedule future events */
  if (_edge_condition[0])
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = ENT[1];
    transfer[4] = ENT[0];
    for ( t_index=5; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = RETIRE_event;
    event_priority = 5;
    schedule_event();
    }

}


/*** THE OCCURRENCE OF A BREAKDOWN ***/
void
FAIL()
{
int  _edge_condition[1];

  /* Attribute Value(s) Passed to this Event */
  ENT[0] = (long) transfer[3];
  ENT[1] = (long) transfer[4];
  ENT[2] = (long) transfer[5];
  ENT[3] = (long) transfer[6];
  ENT[4] = (long) transfer[7];
  ENT[5] = (long) transfer[8];
  ENT[6] = (long) transfer[9];

  /* state changes */
  ENT[4]=ENT[4]+1;
  FAILRENT[J]=CLK-ENT[2];
  INFLOW[J]=INFLOW[J]+RUNC[J]*FAILRENT[J];

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( 1==1 );

  /* schedule future events */
  if (_edge_condition[0])
    /***  GET FIXED AFTER DELAY WHICH FOLLOWS A BETA DISTRIBUTION WITH MIN=1, MAX=6  ***/
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = ENT[1];
    transfer[4] = FAILRENT[J];
    transfer[5] = CLK;
    transfer[6] = ENT[0];
    transfer[7] = ENT[1];
    transfer[8] = ENT[2];
    transfer[9] = ENT[3];
    transfer[10] = ENT[4];
    transfer[11] = ENT[5];
    transfer[12] = ENT[6];
    for ( t_index=13; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + FIXT[0]+(FIXT[1]-FIXT[0])*BET(0.7,0.7);
    event_type = FIX_event;
    event_priority = 6;
    schedule_event();
    }

}


/*** COMPLETION OF REPAIR ON THE MACHINE ***/
void
FIX()
{
int  _edge_condition[1];

  /* Attribute Value(s) Passed to this Event */
  J = (long) transfer[3];
  F = transfer[4];
  FIXTIME = transfer[5];
  ENT[0] = (long) transfer[6];
  ENT[1] = (long) transfer[7];
  ENT[2] = (long) transfer[8];
  ENT[3] = (long) transfer[9];
  ENT[4] = (long) transfer[10];
  ENT[5] = (long) transfer[11];
  ENT[6] = (long) transfer[12];

  /* state changes */
  ENT[2]=CLK;
  FIXTIME=CLK-FIXTIME;

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( 1==1 );

  /* schedule future events */
  if (_edge_condition[0])
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = ENT[1];
    transfer[4] = F;
    transfer[5] = ENT[0];
    transfer[6] = ENT[1];
    transfer[7] = ENT[2];
    transfer[8] = ENT[3];
    transfer[9] = ENT[4];
    transfer[10] = ENT[5];
    transfer[11] = ENT[6];
    for ( t_index=12; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + (REN[J][1]/(1+3*RND))*BET(1.2,1.2);
    event_type = RETURN_event;
    event_priority = 5;
    schedule_event();
    }

}


/*** Generate input ( 4 types of cars) ***/
void
INPUT()
{
int  _edge_condition[3];

  /* Attribute Value(s) Passed to this Event */
  J = (long) transfer[3];
  K = (long) transfer[4];

  /* state changes */
  ENT[1]=J;
  ENT[0]=K;
  RNK[J]=0;
  K=K+0*PUT(INC,J);

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( J<=4&&K<S[J]-1 );
  _edge_condition[1] = ( J<4&&K>=S[J]-1 );
  _edge_condition[2] = ( K>=S[J]-1 );

  /* schedule future events */
  if (_edge_condition[0])
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = J;
    transfer[4] = K+1;
    for ( t_index=5; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = INPUT_event;
    event_priority = 5;
    schedule_event();
    }

  if (_edge_condition[1])
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = J+1;
    transfer[4] = 0;
    for ( t_index=5; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = INPUT_event;
    event_priority = 5;
    schedule_event();
    }

  if (_edge_condition[2])
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = J;
    for ( t_index=4; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = costs_event;
    event_priority = 5;
    schedule_event();
    }

}


/*** A CUSTOMER THAT IS WALKIN ***/
void
WALKIN()
{
int  _edge_condition[1];

  /* Attribute Value(s) Passed to this Event */
  J = (long) transfer[3];
  I = (long) transfer[4];

  /* state changes */
  S[J]=S[J]-(GET(FST,J));

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( 1==1 );

  /* schedule future events */
  if (_edge_condition[0])
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = ENT[0];
    transfer[4] = ENT[1];
    transfer[5] = ENT[2];
    transfer[6] = ENT[3];
    transfer[7] = ENT[4];
    transfer[8] = ENT[5];
    transfer[9] = ENT[6];
    for ( t_index=10; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = RENT_event;
    event_priority = 5;
    schedule_event();
    }

}


/***  ***/
void
RETIRE()
{
  /* Attribute Value(s) Passed to this Event */
  J = (long) transfer[3];
  ENT[0] = (long) transfer[4];

  /* state changes */
  S[J]=S[J]-GET(KEY,J);
}


/*** Rental service terminate due to unable to meet the demand ***/
void
LVE()
{
  /* Attribute Value(s) Passed to this Event */
  J = (long) transfer[3];
  I = (long) transfer[4];

  /* state changes */
  L[J]=L[J]+1;
  L1[J]=L1[J]+I*(S[J]<=0);
}


/*** This is where reservations will be "held" At this time if there are no cars available then we lose the reservation ***/
void
RESERV()
{
int  _edge_condition[2];

  /* Attribute Value(s) Passed to this Event */
  J = (long) transfer[3];
  I = (long) transfer[4];

  /* state changes */
  L2[J]=L2[J]+(S[J]<=0);

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( S[J]>0 );
  _edge_condition[1] = ( S[J]<=0 );

  /* schedule future events */
  if (_edge_condition[0])
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = J;
    transfer[4] = I;
    for ( t_index=5; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = RSVP_event;
    event_priority = 5;
    schedule_event();
    }

  if (_edge_condition[1])
    {
    for ( t_index=3; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = LVE_event;
    event_priority = 5;
    schedule_event();
    }

}


/*** RESERVATIONS MADE HERE ***/
void
RSVP()
{
int  _edge_condition[1];

  /* Attribute Value(s) Passed to this Event */
  J = (long) transfer[3];
  I = (long) transfer[4];

  /* state changes */
  S[J]=S[J]-(GET(FST,J));

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( 1==1 );

  /* schedule future events */
  if (_edge_condition[0])
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = ENT[0];
    transfer[4] = ENT[1];
    transfer[5] = ENT[2];
    transfer[6] = ENT[3];
    transfer[7] = ENT[4];
    transfer[8] = ENT[5];
    transfer[9] = ENT[6];
    for ( t_index=10; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 1;
    event_type = RENT_event;
    event_priority = 4;
    schedule_event();
    }

}


/***  ***/
void
costs()
{
int  _edge_condition[1];

  /* Attribute Value(s) Passed to this Event */
  J = (long) transfer[3];

  /* state changes */
  INICOST[J]=PRICEINS[J]*S[J];

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( 1==1 );

  /* schedule future events */
  if (_edge_condition[0])
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = J;
    for ( t_index=4; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = CUSTM_event;
    event_priority = 5;
    schedule_event();
    }

}

