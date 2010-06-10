/* File appl.c, Programs Maps and Mapnumber */

/*
                            Appendix L
                   Two Adaptive Mapping Programs

     These programs use a two dimensional square array of model 
neurons to test the mapping algorithm discussed in Chapter 14.  We 
define a struct Neuron which contains the location of the unit,
its "activity," and its connections to the input stimulus lines.
The first program uses only two connections between the input stimulus
and the neuron, giving a two dimensional array matching the input pattern
dimension, though many of the most interesting results for adaptive maps
occur when the array dimension and the stimulus dimensions do not match,
as we show in the second demonstration program.

     For the simulations in the first program each unit receives only 
two inputs, that is, allowable inputs are points in a two dimensional 
pattern.  During initialization, all the connections are set to small 
random values.   When a pattern is presented to all the model neurons, 
it provokes the most "activity" in one of them.  What is done is to look for
the closest response, that is, the unit whose connections most closely 
match the input stimulus.  In our program, finding this unit is a two 
step process.  First, we compute the distance between the connections 
of every unit in the array and the input stimulus in the function
Compute_activities.  Second, we find the smallest value of activity, 
which gives the unit with the closest match to the input in the function
Find_closest.

     Once the closest unit is found, we use it as the center of a 
neighborhood.  Initially the neighborhoods are very large, as large as
80% of the entire array, but get smaller as the simulation goes on,
until only the six closest array members are modified.  The modification 
rule simply multiplies the existing weights by a constant, one minus 
Alpha, and then adds Alpha times the pattern to the connections.  There 
is no need to normalize here because we are looking at relative responses.  
The modification is done in Modify_weights(): This routine is repeated 
over and over again by Iteration().

     The rest of the program is concerned with "human factors." Several 
initial two dimensional probability distributions for the input stimulus 
are provided for experimentation, a bar, a triangle, a square, and a 
circle.  A sequence of equally spaced points in the input distribution 
is tested. When tested, the program labels the unit that responds best 
when that particular stimulus point is presented.  The display can present 
the best responses of units either to the points equally spaced throughout 
the original distribution, or to points in the square that were never 
presented during learning because they were not in the input distribution.

     MAPS starts for asking for a random number seed.  Then it asks
for the learning parameter, Alpha.  A good initial choice for Alpha is 
0.1.  Then the program asks for the decay constant in iterations of the 
neighborhood decay term, 1000 or 2000 is appropriate.  The probability 
distribution of the input can be taken from a bar ('b'), a circle ('c') 
a doughnut ('d'), a square ('s') or a triangle ('t').  One of these 
letters must be chosen.  The program will then responds to letter commands 
input from the command line.  "b" displays the unit giving the best 
response to a particular input point.  "d" shows the shape of the
initial distribution and will display the points chosen for display by 'b'.  
"i" determines the number of iterations to perform.  "m" gives the 
best response of units to the missing part of the input distribution,
that is, to points in the square that the program never used.
"q" means quit the program.  "x" starts executing the program until as 
the number iterations chosen by "i" is reached.

     We can make a small modification of our mapping program to do a 
high dimensional simulation.  The main modification simply involved 
changing the definition of the struct Neuron to have 384 connections 
instead of 2 and removing the code specific to the continuous distributions.

     In the second program, MAPNUMBER, used for the high dimensional 
simulations, the commands are a little different.  As before, the program 
prompts for "Seed," followed by a prompt for "Alpha" which is the learning 
constant.  Again, 0.1 is a good initial value.  Next, the time constant 
of neighborhood decay is requested: good starting values are 1000 or 2000.  
Then the program prompts for a parameter "group size" which is used in the 
display routines: a good initial value is 100.  The final prompt is for the 
name of the input file containing the ASCII patterns (48 character) which 
are converted into 384 dimensional state vectors.  

     The program then waits for input from the command line.  Typing an 
"a" from the command line displays the best response for every point 
in the array, with the overall best responding unit for each input 
pattern marked with an '*'.  (Type "a" before any learning to see that 
the initialization truly is random.)  Next, the numbers 1,2,3,4,5, and 0, 
followed by "x" iterate for 1, 2, 3, 4, 5 and 10 times the group size.  
("x" typed alone will iterate for Group Size iterations.)  Nothing is 
displayed until "a" is typed.  This program is computation intensive.

     Examples of operation of both the high and low dimensional programs 
and the high dimensional test patterns can be seen in Chapter 14.
This appendix contains both the high and low dimensional programs and 
the three high dimensional test sets, numbers.txt, bars.txt, and 
symbols.txt.

For MAPS see page 468.  To compile:
        gcc -O -c appl.c
        gcc -o appl1 appl.o -lm
        Executable is named `appl1' instead of `MAPS'.

For MAPNUMBER see page 480.  To compile add -DNUMBER:
        gcc -O -DNUMBER -c appl.c 
        gcc -o appl2 appl.o -lm
        Executable is named `appl2' instead of `MAPNUMBER'.

*/

#if !defined NUMBER
/*----------------------------------------------------------------------
		 Maps: Two dimensional map forming program
----------------------------------------------------------------------*/

#if defined MSDOS
#include <graph.h>      /* _clearscreen _gettextposition _settextposition */
#endif
#include <math.h>       /* sqrt exp fabs */
#include <stdio.h>      /* printf scanf */
#include <stdlib.h>     /* rand srand RAND_MAX */

#define VT100           /* Remove if your terminal is not VT100 compatible */
#define TRUE 1
#define FALSE 0
#define Array_size (int)(19)

typedef struct {
    int x,y;            /* Location on 2-D map array. */
    float Activity;     /* Closeness to current stimulus. */
    float Connections[2];       /* Connection strengths. */
} Neuron;

unsigned int Old_seed;  /* seed for srand() */
long Iterate_until;
long Iteration_nr;
int Plot_missing;
char Distribution;
Neuron Current_map[Array_size][Array_size];
Neuron Closest;
float Stimulus[2];
float Size_constant;
float Alpha;

float Random(void);
void ClrScr(void);
void Erase_line(void);
void GoToXY(int x, int y);
void Initialize_map(void);
void Compute_activities(void);
void Find_closest(void);
void Modify_weights(void);
int Bars(void);
int Circle(void);
int Doughnut(void);
int Square(void);
int Triangle(void);
void Choose_stimulus(void);
void Make_stimulus(void);
void Iteration(void);
void Best_responses(void);
void Show_input_distribution(void);
void Initialize(void);
int main(void);

/*--------------------------------------------------------------------------
Returns a float number >=0 and < 1.0
--------------------------------------------------------------------------*/
float Random(void) {
    return ((float)rand())/((float)RAND_MAX+1);
}   /* Random */



/*--------------------------------------------------------------------------
Screen Control Procedure: Clear screen and move cursor to 1,1
--------------------------------------------------------------------------*/
void ClrScr(void) {
#if defined MSDOS
    _clearscreen(_GCLEARSCREEN);
#elif defined VT100
    printf("%c[f",0x1b);
    printf("%c[2J",0x1b);
#endif
}   /* ClrScr */


/*--------------------------------------------------------------------------
Screen Control Procedure: Erase current line
This routine erases the current line.  The cursor is left at the beginning
of the line.
--------------------------------------------------------------------------*/
void Erase_line(void) {
#if defined MSDOS
    int i;              /* loop index */
    struct rccoord rc;  /* initial position */
    rc=_gettextposition();
    for(i=0; i<80; i++) printf(" ");
    _settextposition(rc.row,1);
#elif defined VT100
    printf("%c[K",0x1b);
    /* Actually, I reckon that the ANSI Esc sequence should be: */
    /* Esc[0K  for erase from cursor to end of line       */
    /* Esc[1K  for erase from beginning of line to cursor */
    /* Esc[2K  for erase of entire line                   */
#endif
}   /* Erase_line */


/*--------------------------------------------------------------------------
Screen Control Procedure: Position cursor

Arguments
    x    column as integer 1..80
    y    row as integer 1..24
--------------------------------------------------------------------------*/
void GoToXY(int x, int y) {
    int x1,y1;          /* coordinates restricted to allowed range */

    x1=(x<1)? 1: (x>80)? 80: x;         /* 1<=x1<=80 */
    y1=(y<1)? 1: (y>24)? 24: y;         /* 1<=y1<=24 */
#if defined MSDOS
    _settextposition(y,x);
#elif defined VT100
    printf("%c[%d;%dH",0x1b,y1,x1);
#endif
}   /* GoToXY */


/*--------------------------------------------------------------------------
This routine initializes Current_map[j][i].
--------------------------------------------------------------------------*/
void Initialize_map(void) {
    int i,j;    /* coordinates within array */
    Neuron N;   /* used to initialize map */

    for(j=0; j<Array_size; j++)
    for(i=0; i<Array_size; i++) {
	N.x=i;
	N.y=j;
	N.Activity=(float)0;
	/* Initialize weights with small random values. */
	N.Connections[0]=(float)(0.01*(Random()-0.5));
	N.Connections[1]=(float)(0.01*(Random()-0.5));
	/* Place "neuron" in appropriate map position. */
	Current_map[j][i]=N;
    }
}   /* Initialize_map */


/*--------------------------------------------------------------------------
Compute activities of all units.  Unit with smallest distance between
synaptic values and input will be chosen.  Using Euclidean distance.
--------------------------------------------------------------------------*/
void Compute_activities(void) {
    int i,j;    /* coordinates within array */
    double X_dist,Y_dist;

    for(j=0; j<Array_size; j++)
    for(i=0; i<Array_size; i++) {
	X_dist=Current_map[j][i].Connections[0]-Stimulus[0];
	Y_dist=Current_map[j][i].Connections[1]-Stimulus[1];
	Current_map[j][i].Activity=(float)sqrt(X_dist*X_dist + Y_dist*Y_dist);
    }
}   /* Compute_activities */



/*--------------------------------------------------------------------------
This routine searches the entire array Current_map[][] looking at activities,
as calculated in Compute_activities().  The neuron with the smallest
activity is copied to the global struct Closest.
--------------------------------------------------------------------------*/
void Find_closest(void) {
    int i,j;    /* coordinates within array */
    Neuron Closest_so_far;

    Closest_so_far.Activity=(float)100000;
    for(j=0; j<Array_size; j++)
    for(i=0; i<Array_size; i++) {
	if(Current_map[j][i].Activity < Closest_so_far.Activity)
	    Closest_so_far=Current_map[j][i];
    }
    Closest=Closest_so_far;
}   /* Find_closest */


/*--------------------------------------------------------------------------
--------------------------------------------------------------------------*/
void Modify_weights(void) {
    int i,j;            /* coordinates within array */
    double Delta_I, Delta_J;
    double Size;
    double Distance_to_closest;

    /* Somewhat arbitrary formula for computing neighborhood size. */
    /* Assumes modification neighborhood starts out large and gets */
    /* small with exponential distribution.                        */
    Size= 1.5 + 0.3*Array_size*exp(-Iteration_nr/Size_constant);

    for(j=0; j<Array_size; j++)
    for(i=0; i<Array_size; i++) {
	/* Checks to see if unit is within the neighborhood of the */
	/* unit with the synaptic weights closest to the input.    */
	Delta_I=Closest.x-i;
	Delta_J=Closest.y-j;
	Distance_to_closest=sqrt(Delta_I*Delta_I + Delta_J*Delta_J);

	if(Distance_to_closest < Size) {
	    /* Weaken old connections and add on a bit of the stimulus */
	    /* to the connections.                                     */

	    /* Modify x connections. */
	    Current_map[j][i].Connections[0]=
		(1-Alpha)*Current_map[j][i].Connections[0]+Alpha*Stimulus[0];
	    /* Modify y connections. */
	    Current_map[j][i].Connections[1]=
		(1-Alpha)*Current_map[j][i].Connections[1]+Alpha*Stimulus[1];
	}
    }
}   /* Modify_weights */


/*----------------------------------------------------------------------*/
/* These functions control the distributions of input stimulus vectors. */
/* More can be added easily.                                            */
/*----------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
Returns TRUE if the Stimulus coordinates are in the region defined by
x <= -0.2 or in the region defined by x >= 0.2
--------------------------------------------------------------------------*/
int Bars(void) {
    return (Stimulus[0]<=(float)(-0.2)) || (Stimulus[0]>=(float)(0.2));
}   /* Bars */


/*--------------------------------------------------------------------------
Returns TRUE if the Stimulus coordinates are within a circle of radius 3,
centred on 0,0.
--------------------------------------------------------------------------*/
int Circle(void) {
    float Radius=0.3f;

    return (Stimulus[0]*Stimulus[0]+Stimulus[1]*Stimulus[1]<=Radius*Radius);
}   /* Circle */


/*--------------------------------------------------------------------------
Returns TRUE if the Stimulus coordinates are outside a square with sides of
length 0.5, centred on 0,0.
--------------------------------------------------------------------------*/
int Doughnut(void) {
    return !(fabs(Stimulus[0])<0.25 && fabs(Stimulus[1])<0.25);
}   /* Doughnut */


/*--------------------------------------------------------------------------
Always returns TRUE.
--------------------------------------------------------------------------*/
int Square(void) {
    return 1;
}   /* Square */


/*--------------------------------------------------------------------------
Returns TRUE if the Stimulus coordinates are within a triangle defined by
column <= row.
--------------------------------------------------------------------------*/
int Triangle(void) {
    return (Stimulus[0] <= Stimulus[1]);
}   /* Triangle */


/*--------------------------------------------------------------------------
Set both elements of Stimulus to a random variable in the range -0.5..+0.5
--------------------------------------------------------------------------*/
void Choose_stimulus(void) {
    Stimulus[0]=Random() - (float)0.5;
    Stimulus[1]=Random() - (float)0.5;
}   /* Choose_stimulus */


/*--------------------------------------------------------------------------
The general strategy is to choose points until an acceptable point from
the desired distribution appears.  The test point are chosen uniformly
from the square with sides from -0.5,0.5.

The switch statement chooses the distribution of input stimuli.  This is
not a very efficient way of doing the choice, but it ensures that equal
areas in the subset of the square distribution selected have equal
probability of being chosen.
--------------------------------------------------------------------------*/
void Make_stimulus(void) {
    switch(Distribution) {
	case 'B': /* This stimulus distribution is two bars with the */
	case 'b': /* central eight columns zero. */
	    do Choose_stimulus(); while(!Bars()); break;
	case 'C': /* This stimulus distribution is a circle centred */
	case 'c': /* on 0,0. */
	    do Choose_stimulus(); while(!Circle()); break;
	case 'D': /* This stimulus distribution is a square doughnut with no */
	case 'd': /* points chosen from a center square of side length 0.5. */
	    do Choose_stimulus(); while(!Doughnut()); break;
	case 'S': /* This stimulus distribution is uniformly covers the */
	case 's': /* full square */
	    do Choose_stimulus(); while(!Square()); break;
	case 'T': /* This stimulus distribution is a right-angled triangle */
	case 't': /* on bottom-left, with the hypotenuse along row=col */
	    do Choose_stimulus(); while(!Triangle()); break;
    }
}   /* Make_stimulus */


/*--------------------------------------------------------------------------
--------------------------------------------------------------------------*/
void Iteration(void) {
    Iteration_nr++;
    Make_stimulus();
    Compute_activities();
    Find_closest();
    Modify_weights();
}   /* Iteration */


/*--------------------------------------------------------------------------
--------------------------------------------------------------------------*/
void Best_responses(void) {
    int i,j;            /* coordinates within array */
    int Best_X, Best_Y; /* coordinate of closest match to stimulus */
    int Show;           /* flag TRUE if current position is part of distrib */

    ClrScr();
    GoToXY(1,1);
    if(Plot_missing)
	printf("               Best Responses of MISSING Input Points");
    else
	printf("                    Best Responses of Input Points");

    GoToXY(1,2);
    printf("Alpha: %5.1f Nbhd Decay: %6.0f Seed: %6u",
	Alpha,Size_constant,Old_seed);
    printf(" Distribution: ");
    if(Distribution=='B' || Distribution=='b')
	printf("Bars.");
    else if(Distribution=='C' || Distribution=='c')
	printf("Circle.");
    else if(Distribution=='D' || Distribution=='d')
	printf("Doughnut.");
    else if(Distribution=='S' || Distribution=='s')
	printf("Square.");
    else if(Distribution=='T' || Distribution=='t')
	printf("Triangle.");

    for(j=0; j<Array_size; j++)
    for(i=0; i<Array_size; i++) {
	/* Exactly Array_size points from -0.5 to +0.5 */
	Stimulus[0]=(float)i/(Array_size-1) - 0.5f;
	Stimulus[1]=(float)j/(Array_size-1) - 0.5f;
	if((Distribution=='B' || Distribution=='b') && Bars())
	    Show=1;
	else if((Distribution=='C' || Distribution=='c') && Circle())
	    Show=1;
	else if((Distribution=='D' || Distribution=='d') && Doughnut())
	    Show=1;
	else if((Distribution=='S' || Distribution=='s') && Square())
	    Show=1;
	else if((Distribution=='T' || Distribution=='t') && Triangle())
	    Show=1;
	else
	    Show=0;

	/* In the plotted array, I are columns, J are rows. */
	/* Columns are denoted by CAPITALS, rows by lower case. */

	/* If Plot_missing flag equals TRUE, only responses to points  */
	/* missing from initial distribution are displayed. */
	if(Plot_missing) Show=!Show;

	if(Show) {
	    Compute_activities();
	    Find_closest();
	    Best_X=Closest.x;
	    Best_Y=Closest.y;
	    GoToXY( (13+3*Best_X), Best_Y+3);
	    printf("%c%c",i+'A',j+'a');
	}
    }
}   /* Best_responses */


/*--------------------------------------------------------------------------
--------------------------------------------------------------------------*/
void Show_input_distribution(void) {
    int i,j;            /* coordinates within array */
    int Show;           /* flag TRUE if current position is part of distrib */

    ClrScr();
    GoToXY(1,1);
    printf("                      Distribution of Input Points");

    GoToXY(1,2);
    printf("Alpha: %5.1f Nbhd Decay: %6.0f Seed: %6u",
	Alpha,Size_constant,Old_seed);
    printf(" Distribution: ");
    if(Distribution=='B' || Distribution=='b')
	printf("Bars.");
    else if(Distribution=='C' || Distribution=='c')
	printf("Circle.");
    else if(Distribution=='D' || Distribution=='d')
	printf("Doughnut.");
    else if(Distribution=='S' || Distribution=='s')
	printf("Square.");
    else if(Distribution=='T' || Distribution=='t')
	printf("Triangle.");

    for(j=0; j<Array_size; j++)
    for(i=0; i<Array_size; i++) {
	/* Exactly Array_size points from -0.5 to +0.5 */
	Stimulus[0]=(float)i/(Array_size-1) - 0.5f;
	Stimulus[1]=(float)j/(Array_size-1) - 0.5f;
	if((Distribution=='B' || Distribution=='b') && Bars())
	    Show=1;
	else if((Distribution=='C' || Distribution=='c') && Circle())
	    Show=1;
	else if((Distribution=='D' || Distribution=='d') && Doughnut())
	    Show=1;
	else if((Distribution=='S' || Distribution=='s') && Square())
	    Show=1;
	else if((Distribution=='T' || Distribution=='t') && Triangle())
	    Show=1;
	else
	    Show=0;

	/* In the plotted array, I are columns, J are rows. */
	/* Columns are denoted by CAPITALS, rows by lower case. */
	if(Show) {
	    GoToXY( (13+3*i), j+3);
	    printf("%c%c",i+'A',j+'a');
	}
    }
}   /* Show_input_distribution */


/*--------------------------------------------------------------------------
--------------------------------------------------------------------------*/
void Initialize(void) {
    unsigned int Seed;  /* seed for srand() */
    int DistributionOK; /* Used in checking value for Distribution */

    printf("This is a demonstration of the Kohonen self-organizing  \n");
    printf("mapping algorithm.  See Chapter 14, p468.             \n\n");

    printf("Seed for random number generator (0..65535): ");
    scanf("%u",&Seed); srand(Seed);
    Old_seed=Seed;

    do {
	printf("Alpha    (0.0..1.0; 0.1 approx): ");
	scanf("%f",&Alpha);
    } while(Alpha<0 && Alpha>1);

    do {
	printf("Nbhd decay, in iterations   (>0.0; 1000 approx): ");
	scanf("%f",&Size_constant);
    } while(Size_constant<=0);

    do {
	printf("Probability distribution of input stimuli -- \n");
	printf("  B)ars, C)ircle, D)oughnut, S)quare, T)riangle: ");
	scanf("%c",&Distribution);
	DistributionOK=Distribution=='B' || Distribution=='b' ||
		       Distribution=='C' || Distribution=='c' ||
		       Distribution=='D' || Distribution=='d' ||
		       Distribution=='S' || Distribution=='s' ||
		       Distribution=='T' || Distribution=='t';
    } while(!DistributionOK);

    Iterate_until=1;
    Initialize_map();
    Plot_missing=FALSE;
}   /* Initialize */


/*--------------------------------------------------------------------------
PROGRAM Maps (Input, Output);
    For MAPS see page 468.
--------------------------------------------------------------------------*/
int main(void) {
    char Input_char;
    int Quit;

    Initialize();
    ClrScr();
    Iteration_nr=0;
    Quit=FALSE;

    do {
	GoToXY(1,23);
	Erase_line();
	printf("Iteration: %6ld.",Iteration_nr);
	printf(" B)est rsp D)ist I)t until M)iss Q)uit X)cute > ");
	scanf("%c",&Input_char);

	switch(Input_char) {
	case 'B':
	case 'b': Best_responses();
		  break;
	case 'D':
	case 'd': Show_input_distribution();
		  break;
	case 'I':
	case 'i': GoToXY(1,23); Erase_line();
		  printf("Iterate until: "); scanf("%ld",&Iterate_until);
		  break;
	case 'M':
	case 'm': Plot_missing=TRUE;
		  Best_responses();
		  Plot_missing=FALSE;
		  break;
	case 'Q':
	case 'q': Quit=TRUE;
		  break;
	case 'X':
	case 'x': GoToXY(1,23); Erase_line();
		  printf("Iterating until: %6ld",Iterate_until);
		  GoToXY(1,23);
		  do
		      Iteration();
		  while(Iteration_nr<Iterate_until);
		  break;
	}
    } while(!Quit);
    printf("\n");
    return 0;
}   /* main */




#else
/*------------------------------------------------------------------------
   Mapnumber: This program forms maps from high dimensionality inputs
------------------------------------------------------------------------*/
#if defined MSDOS
#include <graph.h>      /* _clearscreen _gettextposition _settextposition */
#include <io.h>         /* access */
#endif
#include <math.h>       /* sqrt exp fabs */
#include <stdio.h>      /* printf scanf */
#include <stdlib.h>     /* rand srand RAND_MAX */
#include <string.h>     /* strchr */

#define VT100
#define TRUE 1
#define FALSE 0
#define Array_size       (int)(19)
#define Nr_of_characters (int)(48)
#define Dimensionality   (int)(8*Nr_of_characters)

typedef float Stimulus_vector[Dimensionality];
typedef char String[80];
typedef char Char_vector[Nr_of_characters];
typedef struct {
    int x,y;            /* Location on 2-D map array. */
    float Activity;     /* Closeness to current stimulus. */
    Stimulus_vector Connections;       /* Connection strengths. */
} Neuron;

int Nr_of_stimuli;
int Group_size;
int Original_group_size;
int Delay_Parameter;
long Iteration_nr;
float Size_decay_start;
float Size_constant;
float Alpha;
String Input_file_name;
String Input_string;
FILE *Stimulus_file;
char Learn_char;
char Input_char;
Neuron Current_map[Array_size][Array_size];
Stimulus_vector Stimulus_set[99];
Stimulus_vector Current_stimulus;
Neuron Closest;
#if defined MSDOS
struct rccoord SavedCoords;     /* Used by Save_cursor and Restore_cursor */
#endif

float Random(void);
void Make_stimulus(Char_vector Input_string, /* Character string to be */
					     /* turned into a state vector. */
		   Stimulus_vector V);       /* Output state vector. */
static void Zero_byte(int B[8]);
static void Nr_to_byte(int Nr, int B[8]);
static void Add_Byte_to_Vector(Stimulus_vector V, int i, int B[8]);
void Initialize_map(void);
void Compute_activities(void);
void Find_closest(void);
void ClrScr(void);
void Erase_line(void);
void Bottom_of_block(void);
void Save_cursor(void);
void Restore_cursor(void);
void GoToXY(int x, int y);
void Modify_weights(void);
void Iteration(void);
float Neuron_activity(Neuron N);
void Display_All_Points(void);
void Read_stimuli(void);
void Initialize(void);

/*--------------------------------------------------------------------------
Returns a float number >=0 and < 1.0
--------------------------------------------------------------------------*/
float Random(void) {
    return ((float)rand())/((float)RAND_MAX+1);
}   /* Random */


/*------------------------------------------------------------------------
		Make_stimulus
This routine is called by Read_stimuli.  It takes a string containing
<Nr_of_characters> ascii characters, and for each of the
8*Number_of_characters bits sets the corresponding element of
<V> to +1 (if bit is set), or to 0 (if bit is clear).
Subsequently in Initialize(), the output vectors will be 'normalized'
by dividing each element by sqrt(Dimensionality).
Uses void Zero_byte()
     void Nr_to_byte()
     void Add_Byte_to_Vector()
Arguments
    Char_vector Input_string    string of Number_of_characters, plus null
Returns
    Stimulus_vector V           8*Number_of_characters integers, each 1 or 0
--------------------------------------------------------------------------*/
void Make_stimulus(Char_vector Input_string, Stimulus_vector V) {
    int i;      /* loop index */
    int B[8];   /* One character, expanded to an array of 8 integers */

    for(i=0; i<Nr_of_characters; i++) {
	Nr_to_byte((int)Input_string[i],B);
	if(Input_string[i]=='_') Zero_byte(B);
	Add_Byte_to_Vector(V, i ,B);
    }

    /*
    for(i=0; i<Nr_of_characters; i++) {
	printf("%c",Input_string[i]);
	for(j=0; j<8; j++) printf("%3d",V[i*8+j]);
	printf("\n");
    }
    */
}   /* Make_stimulus */

/*------------------------------------------------------------------------
Sets the array of 8 integers to zero.
Called by Make_stimulus()
------------------------------------------------------------------------*/
static void Zero_byte(int B[8]) {
    int i;      /* loop index */

    for(i=0; i<8; i++)
	B[i]=0;
}   /* Zero_byte */


/*------------------------------------------------------------------------
Copies each of the seven least significant bits of <Nr> to the elements
of B[].
Called by Make_stimulus()
------------------------------------------------------------------------*/
static void Nr_to_byte(int Nr, int B[8]) {
    int i;              /* loop index */
    int Parity=0;       /* sum of all bits */
    int mask;           /* bit mask */

    if(Nr>=128) {
	printf("Too large. ASCII conversion fails.\n"); return;
    } else if(Nr<0) {
	printf("Negative number. ASCII conversion fails.\n"); return;
    }

    /* Copy bits 6..0 of Nr to the corresponding elements of B[] */
    for(mask=0x40,i=6; i>=0; mask>>=1,i--) {
	if(Nr&mask) {
	    B[i]=1; Parity++;
	} else
	    B[i]=-1;
    }
    /* Set bit 7 such that the sum of all bits mod 2 is equal to 0 */
    B[7]=(Parity&1)? 1: -1;
}   /* Nr_to_byte */


/*------------------------------------------------------------------------
Copies 8 integers from B to sequential elements of V, starting from
element 8*i.
Called by Make_stimulus()
------------------------------------------------------------------------*/
static void Add_Byte_to_Vector(Stimulus_vector V, int i, int B[8]) {
    int j;      /* loop index */

    for(j=0; j<8; j++)
	V[i*8+j]=B[7-j];
}   /* Add_Byte_to_Vector */



/*------------------------------------------------------------------------
------------------------------------------------------------------------*/
void Initialize_map(void) {
    int i,j,k;          /* loop indices */
    Neuron N;           /* used for initializing Current_map[][] */

    for(j=0; j<Array_size; j++)
    for(i=0; i<Array_size; i++) {
	N.x=i;
	N.y=j;
	N.Activity=(float)0;
	/* Initialize weights with small random values. */
	for(k=0; k<Dimensionality; k++)
	    N.Connections[k]=(float)(0.0001*(Random()-0.5));
	/* Place "neuron" in appropriate map position. */
	Current_map[j][i]=N;
    }
}   /* Initialize_map */

/*--------------------------------------------------------------------------
Compute activities of all units.  Unit with smallest distance between
synaptic values and input will be chosen.  Using Euclidean distance.
--------------------------------------------------------------------------*/
void Compute_activities(void) {
    int i,j;    /* coordinates within array */
    int k;      /* 0..Dimensionality-1 */
    float Diff; /* difference between Connections and Current_Stimulus */
    float Sum;  /* sum of squares of differences */

    for(j=0; j<Array_size; j++)
    for(i=0; i<Array_size; i++) {
	Sum=0;
	for(k=0; k<Dimensionality; k++) {
	    Diff=Current_map[j][i].Connections[k]-Current_stimulus[k];
	    Sum+=Diff*Diff;
	}
	Current_map[j][i].Activity=(float)sqrt(Sum);
    }
}   /* Compute_activities */



/*--------------------------------------------------------------------------
This routine searches the entire array Current_map[][] looking at activities,
as calculated in Compute_activities().  The neuron with the smallest
activity is copied to the global struct Closest.
--------------------------------------------------------------------------*/
void Find_closest(void) {
    int i,j;    /* coordinates within array */
    Neuron Closest_so_far;

    Closest_so_far.Activity=(float)100000;
    for(j=0; j<Array_size; j++)
    for(i=0; i<Array_size; i++) {
	if(Current_map[j][i].Activity < Closest_so_far.Activity)
	    Closest_so_far=Current_map[j][i];
    }
    Closest=Closest_so_far;
}   /* Find_closest */


/*--------------------------------------------------------------------------
Screen Control Procedure: Clear screen and move cursor to 1,1
--------------------------------------------------------------------------*/
void ClrScr(void) {
#if defined MSDOS
    _clearscreen(_GCLEARSCREEN);
#elif defined VT100
    printf("%c[f",0x1b);
    printf("%c[2J",0x1b);
#endif
}   /* ClrScr */


/*--------------------------------------------------------------------------
Screen Control Procedure: Erase current line
This routine erases the current line.  The cursor is left at the beginning
of the line.
--------------------------------------------------------------------------*/
void Erase_line(void) {
#if defined MSDOS
    int i;              /* loop index */
    struct rccoord rc;  /* initial position */
    rc=_gettextposition();
    for(i=0; i<80; i++) printf(" ");
    _settextposition(rc.row,1);
#elif defined VT100
    printf("%c[K",0x1b);
    /* Actually, I reckon that the ANSI Esc sequence should be: */
    /* Esc[0K  for erase from cursor to end of line       */
    /* Esc[1K  for erase from beginning of line to cursor */
    /* Esc[2K  for erase of entire line                   */
#endif
}   /* Erase_line */


/*--------------------------------------------------------------------------
Screen Control Procedure: Moves cursor to line 23.
--------------------------------------------------------------------------*/
void Bottom_of_block(void) {
#if defined MSDOS
    _settextposition(23,1);
#elif defined VT100
    printf("%c[%d;%df",0x1b,23,1);
#endif
}   /* Bottom_of_block */


/*--------------------------------------------------------------------------
Screen Control Procedure: Save current cursor position
--------------------------------------------------------------------------*/
void Save_cursor(void) {
#if defined MSDOS
    SavedCoords=_gettextposition();
#elif defined VT100
    printf("%c7",0x1b);
#endif
}   /* Save_cursor */

/*--------------------------------------------------------------------------
Screen Control Procedure: Restore current cursor position
--------------------------------------------------------------------------*/
void Restore_cursor(void) {
#if defined MSDOS
    _settextposition(SavedCoords.row,SavedCoords.col);
#elif defined VT100
    printf("%c8",0x1b);
#endif
}   /* Restore_cursor */


/*--------------------------------------------------------------------------
Screen Control Procedure: Position cursor

Arguments
    x    column as integer 1..80
    y    row as integer 1..24
--------------------------------------------------------------------------*/
void GoToXY(int x, int y) {
    int x1,y1;          /* coordinates restricted to allowed range */

    x1=(x<1)? 1: (x>80)? 80: x;         /* 1<=x1<=80 */
    y1=(y<1)? 1: (y>24)? 24: y;         /* 1<=y1<=24 */
#if defined MSDOS
    _settextposition(y,x);
#else
    printf("%c[%d;%dH",0x1b,y1,x1);
#endif
}   /* GoToXY */


/*--------------------------------------------------------------------------
--------------------------------------------------------------------------*/
void Modify_weights(void) {
    int i,j;            /* coordinates within array */
    int k;              /* element index */
    double Delta_I, Delta_J;
    double Size;
    double Distance_to_closest;

    /* Somewhat arbitrary formula for computing neighborhood size. */
    /* Assumes modification neighborhood starts out large and gets */
    /* small with exponential distribution.                        */
    Size= 1.5 + 0.3*Array_size*exp(-(Iteration_nr-Size_decay_start)/Size_constant);

    for(j=0; j<Array_size; j++)
    for(i=0; i<Array_size; i++) {
	/* Checks to see if unit is within the neighborhood of the */
	/* unit with the synaptic weights closest to the input.    */
	Delta_I=Closest.x-i;
	Delta_J=Closest.y-j;
	Distance_to_closest=sqrt(Delta_I*Delta_I + Delta_J*Delta_J);

	if(Distance_to_closest < Size) {
	    /* Weaken old connections and add on a bit of the stimulus */
	    /* to the connections.                                     */
	    for(k=0; k<Dimensionality; k++)
		Current_map[j][i].Connections[k]=
		    (1-Alpha)*Current_map[j][i].Connections[k]+
		    Alpha*Current_stimulus[k];
	}
    }
}   /* Modify_weights */


/*--------------------------------------------------------------------------
--------------------------------------------------------------------------*/
void Iteration(void) {
    int i,j;    /* Byte and bit number */
    int k;      /* Element number */
    int Current_stimulus_number;

    Iteration_nr++;
    Current_stimulus_number=(int)(Nr_of_stimuli*Random());
    for(k=0; k<Dimensionality; k++)
	Current_stimulus[k]=Stimulus_set[Current_stimulus_number][k];

    /* Display progress */
    if(Iteration_nr%10==0) {
        GoToXY(1,2);
        Erase_line();
        printf("It. : %4d.  Stim: %2d",Iteration_nr,Current_stimulus_number);
        fflush(stdout);
    }

/*
    for(i=0; i<Nr_of_characters; i++) {
	printf("Current: %3d  ",Current_stimulus_number);
	for(j=0; j<8; j++)
	    printf("%6.0f",Current_stimulus[8*i+j]);
	printf("\n");
    }
*/

    Compute_activities();
    Find_closest();
    Modify_weights();
}   /* Iteration */


/*--------------------------------------------------------------------------
--------------------------------------------------------------------------*/
float Neuron_activity(Neuron N) {
    int k;      /* loop index */
    float Sum;  /* used to calculate a neuron's activity */

    Sum=0;
    for(k=0; k<Dimensionality; k++)
	Sum += (N.Connections[k]-Current_stimulus[k])*
	       (N.Connections[k]-Current_stimulus[k]);
    return (float)sqrt(Sum);
}   /* Neuron_activity */


/*--------------------------------------------------------------------------
--------------------------------------------------------------------------*/
void Display_All_Points(void) {
    int i,j,k,l;
    int Smallest_stimulus;
    Neuron Current_neuron;
    float Smallest_activity;
    float Activity;

    ClrScr();
    GoToXY(1,1);
    printf("               Best Responses  (* is maximum response)   \n");

    /* This part of the procedure finds the best matching stimulus */
    /* for all units in the map.                                   */
    for(i=0; i<Array_size; i++)
    for(j=0; j<Array_size; j++) {
	Smallest_activity=(float)100000.0;
	Smallest_stimulus=0;
	Current_neuron=Current_map[j][i];
	/* Next loop picks out stimulus that is closest to */
	/* weights of the current_neuron.                  */
	for(l=0; l<Nr_of_stimuli; l++) {
	    for(k=0; k<Dimensionality; k++)
		Current_stimulus[k]=Stimulus_set[l][k];
	    Activity=Neuron_activity(Current_neuron);
	    if(Activity < Smallest_activity) {
		Smallest_activity=Activity;
		Smallest_stimulus=l;
	    }
	    GoToXY((3*i+13),(j+3));
	    printf("%2d ",Smallest_stimulus);
	}
    }

    /* This part of the procedure picks out the maximum response to */
    /* a particular stimulus and denotes it by the symbol '*'.      */
    for(i=0; i<Nr_of_stimuli; i++) {
	for(k=0; k<Dimensionality; k++)
	    Current_stimulus[k]=Stimulus_set[i][k];
	Compute_activities();
	Find_closest();
	GoToXY((3*Closest.x+15),(Closest.y+3));
	printf("*");
    }
    GoToXY(1,2);
    Erase_line();
    printf("  Iteration: %5d",Iteration_nr);
    printf("   Data File: %s",Input_file_name);
}   /* Display_All_Points */


/*--------------------------------------------------------------------------
--------------------------------------------------------------------------*/
void Read_stimuli(void) {
    int i;                      /* loop index */
    int String_length;          /* number of characters in S */
    String S;                   /* raw line read from file */
    Char_vector Input_string;   /* processed version of S */
    int BlankLine;              /* TRUE if line empty */

    Nr_of_stimuli=0;            /* initialize stimulus set */
    Stimulus_file=fopen(Input_file_name,"rt");
    if(Stimulus_file==NULL) {
	printf("\nFile \"%s\" not found\n",Input_file_name);
	return;
    } else {
	printf("Opening file: ", Input_file_name);
    }

    while(!feof(Stimulus_file)) {
	/* Initialize Input_string by filling it with '_' characters */
	for(i=0; i<Nr_of_characters; i++)
	    Input_string[i]='_';

	/* Read one line from input file */
	fgets(S,sizeof(String),Stimulus_file);   /* read max of 79 chars */
	if(strchr(S,'\n')) *strchr(S,'\n')='\0';        /* nullify newline */
	String_length=(strlen(S)>Nr_of_characters)? Nr_of_characters:
						    strlen(S);
	BlankLine=1;                    /* initially assume line is blank */
	for(i=0; i<String_length; i++)
	    if(S[i]!=' ') BlankLine=0;  /* clear flag if character found */
	if(BlankLine) continue;

	for(i=0; i<String_length; i++)
	    Input_string[i]=S[i];
	/* printf("Input: %2d  ", Nr_of_stimuli+1); */
	/* for(i=0; i<String_length; i++)           */
	/*     printf("%c",Input_string[i]);        */
	/* printf("\n");                            */

	Make_stimulus(Input_string, Stimulus_set[Nr_of_stimuli]);
	Nr_of_stimuli++;
    }
    fclose(Stimulus_file);
}   /* Read_stimuli */

/*-------------------------------------------------------------------------
This routine allows the user to seed the random number generator, an to set
float Alpha;
float Size_constant;
int Group_size;
String Input_file_name;
-------------------------------------------------------------------------*/
void Initialize(void) {
    unsigned int Seed;  /* seed for srand() */

    printf("\nThis is a demonstration of the Kohonen self-organizing, map\n");
    printf("generating algorithm.  See chapter 14, p480.\n");
    printf("This simulation uses large weight vectors with structure.\n\n");

    printf("Seed for random number generator (0..65535): ");
    scanf("%u",&Seed); srand(Seed);

    do {
	printf("Alpha (Range=0..1, typically 0.1) : "); scanf("%f",&Alpha);
    } while(Alpha<0 || Alpha>1);

    do {
	printf("Neighbourhood decay rate (>1 iterations, typically 1000) : ");
	scanf("%f",&Size_constant);
    } while(Size_constant<=1);

    do {
	printf("Group size (>1, typically 100): ");
	scanf("%d",&Group_size);
    } while(Group_size<=1);

    /* Get file containing association strings, and threshold */
    do {
        printf("Supplied input files are: numbers.txt symbols.txt bars.txt\n");
	printf("Input file : "); scanf("%79s",Input_file_name);
    } while(access(Input_file_name,4)!=0);

    Size_decay_start=1;
    Original_group_size=Group_size;
    Initialize_map();
}   /* Initialize */

/*--------------------------------------------------------------------------
    For MAPNUMBER see page 480.
--------------------------------------------------------------------------*/
void main(main) {
    int Continue;       /* flag */
    int j;              /* iteration index */

    Initialize();
    Read_stimuli();
    ClrScr();
    Iteration_nr=0;
    Continue=TRUE;

    while(Continue) {
	GoToXY(1,23);
	Erase_line();
	printf(" Iteration: %6d. ", Iteration_nr);
        printf("A)ll G)roup size N)eighbour size (123450) Q)uit X)ecute > ");
	scanf("%c",&Input_char);

	switch(Input_char) {
	case 'A':
	case 'a': Display_All_Points();
		  break;
	case 'G':
	case 'g': GoToXY(1,23);
                  Erase_line();
		  printf("New Group Size: ");
		  scanf("%d",&Original_group_size);
		  Group_size=Original_group_size;
		  break;
	case 'N':
	case 'n': GoToXY(1,23);
                  Erase_line();
		  printf("Reset neighborhood size to original");
		  Size_decay_start=(float)Iteration_nr;
		  break;
	case 'Q':
	case 'q': Continue=FALSE;
		  break;
	case 'X':
	case 'x': GoToXY(1,23);
		  Erase_line();
		  printf(" Iterating until: %6d", Iteration_nr+Group_size);
		  for(j=0; j<Group_size; j++)
		      Iteration();
		  Group_size=Original_group_size;
		  break;
	case '1': Group_size=Group_size;
                  break;
	case '2': Group_size*=2;
                  break;
	case '3': Group_size*=3;
                  break;
	case '4': Group_size*=4;
                  break;
	case '5': Group_size*=5;
                  break;
	case '0': Group_size*=10;
                  break;
	}
    }
    printf("\n");
}   /* main */

#endif
