#include "main.h"
#include "autoFunction.h"
double wheelCir = 10.21;
double robotDiameter = 11.5;
double gearRatio = 36/60;
void allStop(){
    driveMotors.move(0);
}
/*Stops drive train for certain amount of time in milliseconds*/
void timeStop(double tiempo){
    // tiempo = time in milliseconds
    allStop();
    pros::delay(tiempo);
}
/*Drives forward/backwards for a certain distance in inches and at a certain percentage of full speed*/
void driveE(double speed, double distance){
    //speed = speed percentage 
    //distance = distance in inches
    double degree =(distance/wheelCir)*360*gearRatio;
    double rpm = 2*speed;
    if (speed>100 || speed<0){
        return;
    }
    driveMotors.move_relative(degree,rpm);

    while (!(rightDown.get_position() > degree-5)){
        pros::delay(2);
    }
    timeStop(20);
    
}
/*Turns robot a certain degrees at a certain speed in percentage*/
void turnE(double speed, double rotate){
    //speed = speed percentage
    //rotate = degrees Negative: CounterClockWise | Positive ClockWise
    //diameter = 11.5 inches

    if (speed>100 || speed<0){
        return;
    }  
    double degree = 3.538*(rotate/2)*gearRatio; // diameter of robot / wheel diameter
    double rpm = 2*speed;

    leftSide.move_relative(degree,rpm);
    rightSide.move_relative(-degree,rpm);

    while (!((rightDown.get_position() < degree+5) && (rightDown.get_position() > degree-5))){
        pros::delay(2);
    }
    timeStop(20);

}
/*Makes robot turn in a circle with certain outside and inside radius, speed in percentage, and direction*/
void curveE(double out, double in, double speed, double dir){
    //out = outer arc inches
    //in = inner arc
    //speed = speed percentage
    //dir = direction +-
    //left negative right positive
    if (speed>100 || speed<0){
        return;
    }
    if (dir!= 1 || dir!=-1){
        return;
    }

    double outDegree = (out/wheelCir)*360*gearRatio;
    double inDegree = (in/wheelCir)*360*gearRatio;
    double rpm = speed*2;
    double tiempo = (out/wheelCir)/(rpm/60);
    double speedIn = (in/wheelCir)/tiempo;

    if (dir==1){
        //clockwise
        
        leftSide.move_relative(inDegree,speedIn);
        rightSide.move_relative(outDegree,rpm); 

        while (!(leftDown.get_position() < inDegree+5) && (leftDown.get_position() > inDegree-5)){
            pros::delay(2);
        }
        timeStop(20);
    }
    if (dir==-1){
        //counter-clockwise

        leftSide.move_relative(outDegree,rpm);
        rightSide.move_relative(inDegree,speedIn);

        while (!(rightDown.get_position() < inDegree+5) && (rightDown.get_position() > inDegree-5)){
            pros::delay(2);
        }
        timeStop(20);
    }

    
}