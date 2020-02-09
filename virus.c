#include <stdio.h>
#include <math.h>

#define MAX_PEOPLE 1000
#define MAX_DAY 1000
#define INFECTION_LENGTH 16

#define DEBUG 1

// Functions Prototype
void parseInput();

unsigned short run();

// Type declaration
typedef struct {
    unsigned short id;
    int x;
    int y;
    int toX;
    int toY;
    short status; // 0 = normal, 1 - 16 = infected, -1 = died lol
} Person;

typedef struct {
    unsigned short id;
    int x;
    int y;
} PersonInit;

typedef struct {
    unsigned short id;
    int dx;
    int dy;
} Movement;

// Configuration
short numPeople = 0; //number of people in the simulation
short daySimulated = 0; //day of infection

Movement movementTable[MAX_DAY][MAX_PEOPLE];
PersonInit peopleInitValue[MAX_PEOPLE];
Person people[MAX_PEOPLE];

unsigned short s = 0; // id of the guy that makes the city collapse

typedef struct {
    double d2;
    double d1;
    double d0;
} Poly;

unsigned short checkNear(int sx1, int sy1, int sx2, int sy2, int px1, int py1, int px2, int py2) {
    double min, max;
    double spp;

    Poly xs, ys, xr, yr;
    Poly xsr, ysr;
    Poly pxsr, pysr;
    Poly ppxysr;

    // xs = (1-t) * sx1 + sx2 * (t)
    // xs = sx1  - sx1 * (t) + sx2 * (t)
    // xs = {0, sx2 - sx1, sx1}
    xs.d2 = 0;
    xs.d1 = sx2 - sx1;
    xs.d0 = sx1;

    // ys = (1-t) * sy1 + sy2 * (t)
    // ys = sy1  - sy1 * (t) + sy2 * (t)
    // ys = {0, sy2 - sy1, sy1}
    ys.d2 = 0;
    ys.d1 = sy2 - sy1;
    ys.d0 = sy1;

    // xr = (1-t) * px1 + px2 * (t)
    // xr = px1  - px1 * (t) + px2 * (t)
    // xr = {0, px2 - px1, px1}
    xr.d2 = 0;
    xr.d1 = px2 - px1;
    xr.d0 = px1;

    // yr = (1-t) * py1 + py2 * (t)
    // yr = py1  - py1 * (t) + py2 * (t)
    // yr = {0, py2 - py1, py1}
    yr.d2 = 0;
    yr.d1 = py2 - py1;
    yr.d0 = py1;

    // xsr = xs - xr
    xsr.d2 = xs.d2 - xr.d2;
    xsr.d1 = xs.d1 - xr.d1;
    xsr.d0 = xs.d0 - xr.d0;

    // ysr = ys - yr
    ysr.d2 = ys.d2 - yr.d2;
    ysr.d1 = ys.d1 - yr.d1;
    ysr.d0 = ys.d0 - yr.d0;

    // pxsr = xsr * xsr
    pxsr.d2 = xsr.d1 * xsr.d1;
    pxsr.d1 = xsr.d1 * xsr.d0;
    pxsr.d1 += xsr.d0 * xsr.d1;
    pxsr.d0 = xsr.d0 * xsr.d0;

    // pysr = ysr * ysr

    pysr.d2 = ysr.d1 * ysr.d1;
    pysr.d1 = ysr.d1 * ysr.d0;
    pysr.d1 += ysr.d0 * ysr.d1;
    pysr.d0 = ysr.d0 * ysr.d0;

    // sqrt( (xsr * xsr) + (ysr * ysr) ) <= 10
    // (xsr * xsr) + (ysr * ysr) <= 100
    // (pxsr) + (pysr) <= 100

    ppxysr.d2 = pxsr.d2 + pysr.d2;
    ppxysr.d1 = pxsr.d1 + pysr.d1;
    ppxysr.d0 = pxsr.d0 + pysr.d0;

    // (pxsr) + (pysr) - 100 <= 0

    ppxysr.d0 = ppxysr.d0 - 100;

    // (b * b - 4 * a * c) > 0

    spp = ppxysr.d1 * ppxysr.d1 - 4 * ppxysr.d2 * ppxysr.d0;

    if (spp >= 0) {

        // min = (- b - sqrt( b * b - 4 a c ) ) / (2 a)
        // min = (- ppxysr.d1 - sqrt( ppxysr.d1 * ppxysr.d1 - 4 * ppxysr.d2 * ppxysr.d0 ) ) / ( 2 * ppxysr.d2 )

        min = (0 - ppxysr.d1 - sqrt(spp)) / (2 * ppxysr.d2);

        // max = (- b + sqrt( b * b - 4 a c ) ) / (2 a)
        // max = (- ppxysr.d1 + sqrt( ppxysr.d1 * ppxysr.d1 - 4 * ppxysr.d2 * ppxysr.d0 ) ) / ( 2 * ppxysr.d2 )

        max = (0 - ppxysr.d1 + sqrt(spp)) / (2 * ppxysr.d2);

//        return max >= 0 && min <= 1;

        if (max >= 0 && min <= 1) {
            return 1;
        } else return 0;

    } else {
//        printf("Error\n");
        return 0;
    }
}

void parseInput() {
    scanf("%hu", &numPeople);

    int p = 0;
    int q = 0;
    for (short i = 0; i < numPeople; i++) {
        scanf("%hu", &(peopleInitValue[i].id)); //scan id
        scanf("%d", &(peopleInitValue[i].x)); //scan initial x
        scanf("%d", &(peopleInitValue[i].y)); //scan initial y
    }
    scanf("%hu", &daySimulated);
    for (short date = 0; date < daySimulated; date++) {
        for (short pep = 0; pep < numPeople; pep++) {
            scanf("%hu %d %d", &(movementTable[date][pep].id), &(movementTable[date][pep].dx),
                  &(movementTable[date][pep].dy));
        }
    }

    //debug input
#if DEBUG
    printf("total pep: \n");
    printf("%hu \n", numPeople);
    printf("all id: \n");
    for (short i = 0; i < numPeople; i++) {
        printf("%hu \n", peopleInitValue[i].id);
    }
#endif
}

void init() {
    for (int i = 0; i < numPeople; i++) {
        Person *person = &people[i];
        person->x = peopleInitValue[i].x;
        person->y = peopleInitValue[i].y;
        person->id = peopleInitValue[i].id;
        person->status = 0;
    }
}

#if DEBUG
int eachDayCounter = 0;
#endif

unsigned short run() {
    for (int day = 0; day < daySimulated; day++) {

#if DEBUG
        eachDayCounter = 0;
#endif


        for (int i = 0; i < numPeople; i++) {
            Person *person = &people[i];
            if (person->status == -1) continue;

            if (day > 0) {
                person->x = person->toX;
                person->y = person->toY;

                if (person->status > 0) person->status++;
            }

            person->toX = person->x + movementTable[day][i].dx;
            person->toY = person->y + movementTable[day][i].dy;
        }


        for (int i = 0; i < numPeople; i++) {
            Person *person = &people[i];
            if (person->status == -1) continue;

#if DEBUG
            printf("ID=%d moving from (%d,%d) to (%d,%d)\n", person->id, person->x, person->y, person->toX,
                   person->toY);
#endif

            if (person->status > INFECTION_LENGTH) {
                person->status = -1;
            }

            if (person->status >= 2) {
                // Ready to spread

                for (int j = 0; j < numPeople; j++) {
                    if (j == i) continue;

                    Person *personB = &people[j];
                    if (personB->status > 0) continue;

                    if (checkNear(person->x, person->y, person->toX, person->toY,
                                  personB->x, personB->y, personB->toX, personB->toY)) {
                        personB->status = 1;
                        printf("ID=%d infected ID=%d\n", person->id, personB->id);
                    }
                }
            }
        }


#if DEBUG
        for (int i = 0; i < numPeople; i++) {
            if (people[i].status != 0) {
                eachDayCounter++;
            }
        }

        printf("Day %d ended\n", day);
        printf("Infected today: %d \n", eachDayCounter);
#endif
    }

    unsigned short counter = 0;

    for (int i = 0; i < numPeople; i++) {
        if (people[i].status != 0) {
            counter++;
        }
    }

    return counter;
}


// Code entry point
int main() {
    parseInput();
    for (int i = 0; i < numPeople; i++) {
        init();
        people[i].status = 1;
        unsigned short infected = run();

#if DEBUG
        printf("Starting with id=%d will infect %d people\n", people[i].id, infected);
#endif
    }
}