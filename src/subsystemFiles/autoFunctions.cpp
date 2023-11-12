#include "main.h"
#include "autoFunction.h"
double wheelCir = 10.21;
double robotDiameter = 11.5;
void allStop(){
    leftUp.move(0);
    leftMiddle.move(0);
    leftDown.move(0);
    rightUp.move(0);
    rightMiddle.move(0);
    rightDown.move(0);
}

void timeStop(double tiempo){
    // tiempo = time in milliseconds
    allStop();
    pros::delay(tiempo);
}

void driveE(double speed, double distance){
    //speed = speed percentage out of 100
    double degree = (distance/wheelCir)*360;
    double rpm = 2*speed;
    if (speed>100 || speed<0){
        return;
    }
    leftUp.move_relative(-degree,rpm);
    leftMiddle.move_relative(-degree,rpm);
    leftDown.move_relative(-degree,rpm);
    rightUp.move_relative(degree,rpm);
    rightMiddle.move_relative(degree,rpm);
    rightDown.move_relative(degree,rpm);

    while (!(rightDown.get_position() < degree+5) && (rightDown.get_position() > degree-5)){
        pros::delay(2);
    }
    timeStop(20);
    
}

void turnE(double speed, double rotate){
    //speed = speed percentage
    //rotate = degrees
    //diameter = 11.5 inches

    if (speed>100 || speed<0){
        return;
    }  
    double degree = 3.538*(-rotate/2); // diameter of robot / wheel diameter
    double rpm = 2*speed;

    leftUp.move_relative(degree,rpm);
    leftMiddle.move_relative(degree,rpm);
    leftDown.move_relative(degree,rpm);
    rightUp.move_relative(degree,rpm);
    rightMiddle.move_relative(degree,rpm);
    rightDown.move_relative(degree,rpm);

    while (!(rightDown.get_position() < degree+5) && (rightDown.get_position() > degree-5)){
        pros::delay(2);
    }
    timeStop(20);

}

void curveE(double out, double in, double speed, double dir){
    //out = outer arc inches
    //in = inner arc
    //speed = speed percentage
    //dir = direcition +-
    //left negative right positive
    if (speed>100 || speed<0){
        return;
    }
    if (dir!=1||dir!=-1){
        return;
    }

    double outDegree = (out/wheelCir)*360;
    double inDegree = (in/wheelCir)*360;
    double rpm = speed*2;
    double tiempo = (out/wheelCir)/(rpm/60);
    double speedIn = (in/wheelCir)/tiempo;

    if (dir==1){
        //right
        leftUp.move_relative(inDegree,speedIn);
        leftMiddle.move_relative(inDegree,speedIn);
        leftDown.move_relative(inDegree,speedIn);
        rightUp.move_relative(outDegree,rpm);
        rightMiddle.move_relative(outDegree,rpm);
        rightDown.move_relative(outDegree,rpm);
        while (!(leftDown.get_position() < inDegree+5) && (leftDown.get_position() > inDegree-5)){
            pros::delay(2);
        }
        timeStop(20);
    }
    if (dir==-1){
        //left
        leftUp.move_relative(outDegree,rpm);
        leftMiddle.move_relative(outDegree,rpm);
        leftDown.move_relative(outDegree,rpm);
        rightUp.move_relative(inDegree,speedIn);
        rightMiddle.move_relative(inDegree,speedIn);
        rightDown.move_relative(inDegree,speedIn);
        while (!(rightDown.get_position() < inDegree+5) && (rightDown.get_position() > inDegree-5)){
            pros::delay(2);
        }
        timeStop(20);
    }

    
}