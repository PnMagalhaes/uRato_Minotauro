// imports
#include <stdio.h>
#include <stdlib.h>
#include <p32xxxx.h>
#include <stdint.h>
#include <time.h>

#include "DinotauroDrivers.h"

#define OBSTACLE_IN_SIGHT 350
#define TIME_ROTATE 750
#define ROW_MAX 9
#define COL_MAX 9

#define NORTE   0
#define SUL     2
#define OESTE   3
#define ESTE    1


// prototypes
uint8_t checkObstacle();
void rotateRel_basic(int speed, double deltaAngle);
void rotate90();
void goFront();

void printMap();

// global variables
int8_t row, col = 0;
int8_t dummy_row, dummy_col = 0;
uint8_t compass_location = NORTE;   // !!important - initial position of the robot
uint8_t map[ROW_MAX][COL_MAX];
uint8_t rotate_degrees = 90;

uint8_t last_compass_location = 0;
//uint8_t last_location = 0;
uint8_t known_location = 0;

bool stop_state = false;

int main()
{
    int obs = 0;
    int groundSensor;
    //!important
    INTEnableSystemMultiVectoredInt();
    
    //init funcs
    initPIC32();
    
    //leds test
    led(1, 1);
    led(2, 1);
    led(3, 1);
    led(4, 1);    
    
    closedLoopControl( false );
    setVel2(0,0);
    delay(1000);
    
    // fill analogSensores structure
    while(1)
    {
    //    obs = checkObstacle();
        
        //if(obs == 1){
            //rotate90();
             //rotateRel_basic(50, M_PI / 2);
        //     setVel2(50, 50);
        //}
        //else{
        //    setVel2(0,0);
        //}
        
        readAnalogSensors();
        obs = (int)((analogSensors.obstSensLeft + analogSensors.obstSensFront) / 2);
        
        if(obs > 250){
            setVel2(-40,40);
            delay(300);
        }
        else{
            setVel2(50,50);
            delay(100);
        }
        
        
    }
 
        
    
    
    // start by checking the state of the Map
//    printMap();

    
    //last_compass_location = compass_location;   
    //TODO: wait for the start button
    
    /*while(1){
        goFront();
        break;

        printf("Press start to continue\n");
        while(!startButton());
      
        //TODO: function "obstacleFound()" - it needs to check if there is a wall in front of the robot
        if( !checkObstacle() )
        {
            printf("[GO_IN_FRONT state] A wall was NOT found! Heading: %d\n", compass_location);
    
            //check if the robot is heading to a known place
            printf("\t check if the robot is heading to a known place!");

            dummy_row = row;
            dummy_col = col;                
            if(compass_location == NORTE)
            {   
                dummy_row--;
                if( dummy_row < 0) dummy_row = 0;
            }
            else if(compass_location == SUL)
            {
                dummy_row++;
                if (dummy_row > ROW_MAX) dummy_row = ROW_MAX;
            }
            else if(compass_location == OESTE)
            {
                dummy_col--;
                if (dummy_col < 0) dummy_col = 0;
            }
            else if(compass_location == ESTE)
            {
                dummy_col++;
                if (dummy_col > COL_MAX) dummy_col = COL_MAX;
            }
            
            if (map[dummy_row][dummy_col] == 0 || (map[dummy_row][dummy_col] == 1 && known_location == 1))
            {
                //TODO: function "goSquareFront()" - commands the robot to go to the next square
                goFront();
                
                if (known_location > 0 )
                {
                    map[row][col] = 2;  //the robot has to go back! wrong path
                    known_location = 0;
                }
                else
                {
                    map[row][col] = 1;  //mark the position where the robot was
                }

                // it verifies where the robot is heading to know where it is going in the Map
                if(compass_location == NORTE)
                {   
                    //last_compass_location = SUL;    //this means that robot should not go back to SUL
                    row--;
                    if( row < 0) row = 0;
                }
                else if(compass_location == SUL)
                {
                    //last_compass_location = NORTE;    //this means that robot should not go back to NORTE
                    row++;
                    if (row > ROW_MAX) row = ROW_MAX;
                }
                else if(compass_location == OESTE)
                {
                    //last_compass_location = ESTE;    //this means that robot should not go back to ESTE
                    col--;
                    if (col < 0) col = 0;
                }
                else if(compass_location == ESTE)
                {
                    //last_compass_location = OESTE;    //this means that robot should not go back to OESTE
                    col++;
                    if (col > COL_MAX) col = COL_MAX;
                }
                
                //  TODO: After going to a new square, the robot needs to check the it founds the farol!
                
            }
            else if (map[dummy_row][dummy_col] == 1 && known_location == 0)
            {
                known_location++;
                printf("\tthe robot is heading to a place that it knows! heading:%d\n", compass_location);
                printMap();

                //TODO: function "rotate" - !! it rotates to the RIGHT !!
                rotate90(); //rotate_degrees=90º

                compass_location++;
                if(compass_location > OESTE) compass_location = NORTE;                
            }
            
        }
        else    // it means that a wall is in front of the robot
        {
            printf("[ROTATE_90º state] A wall was found! Heading: %d\n", compass_location);
            
            //TODO: function "rotate" - !! it rotates to the RIGHT !!
            rotate90(); //rotate_degrees=90º

            compass_location++;
            if(compass_location > OESTE) compass_location = NORTE;
        }
        
    }*/
    
    return 0;
}


uint8_t checkObstacle()
{
    int n_samples = 5;
    float tmp_val = 0;
    int mean_val = 0;
    int i = 0;
    int tmp[n_samples];
    
    for(i = 0; i< n_samples; i++)
    {
        readAnalogSensors();
        printf("Dist_left=%03d, Dist_center=%03d, Dist_right=%03d\n\n", 
              analogSensors.obstSensLeft, analogSensors.obstSensFront, 
              analogSensors.obstSensRight);
        
        //tmp[i] = (analogSensors.obstSensLeft + analogSensors.obstSensFront + analogSensors.obstSensRight) / 3;
        tmp[i] = (analogSensors.obstSensLeft + analogSensors.obstSensFront) / 2;
        delay(100);
    }
    
    mean_val = (int)((tmp[0] + tmp[1] + tmp[2] + tmp[3]+ tmp[4])/n_samples);
    printf("tmp_val:%f, mean_val=%d\n", tmp_val, mean_val);   
    
    if (mean_val > OBSTACLE_IN_SIGHT)
        return 1;
    else
        return 0;
}

void rotateRel_basic(int speed, double deltaAngle)
{
   double x, y, t;
   double targetAngle;
   double error;
   int cmdVel, errorSignOri;

   printf("1\n");
   getRobotPos(&x, &y, &t);
   printf("2\n");
   targetAngle = normalizeAngle(t + deltaAngle);
   error = normalizeAngle(targetAngle - t);
   errorSignOri = error < 0 ? -1 : 1;

   cmdVel = error < 0 ? -speed : speed;
   setVel2(-cmdVel, cmdVel);
   //setVel2(-10,10);
 
   do
   {
       getRobotPos(&x, &y, &t);
       printf("s:%f, y:%f, t:%f\n", x, y, t);
      error = normalizeAngle(targetAngle - t);
   } while (fabs(error) > 0.01 && errorSignOri * error > 0);
   setVel2(0, 0);
}


/*  Method to print the map */
void printMap()
{
    int i_row, i_col = 0;
    
    printf("\n\t MAP STATE:\n");
    printf("###########################################\n");
    for(i_row = 0; i_row < ROW_MAX; i_row++)
    {   
        printf("[\t");
        for(i_col = 0; i_col < COL_MAX; i_col++)
        {
            printf("%d\t", map[i_row][i_col]);       
        }   
        printf("]");
    }
    printf("###########################################\n");
}

void rotate90()
{
    setVel2(-40, 40);
    delay(TIME_ROTATE);
    setVel2(0, 0);
}

void goFront()
{
    setVel2(50, 50);
    delay(1300);
    setVel2(0,0);
}