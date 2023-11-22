#include "main.h"

//Drive Train
//Positve: Move Forward | Negative: Move Backwards
pros::Motor leftUp(1,pros::E_MOTOR_GEAR_200,true,pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor leftMiddle(5,pros::E_MOTOR_GEAR_200,true,pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor leftDown(3,pros::E_MOTOR_GEAR_200,true,pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor rightUp(6,pros::E_MOTOR_GEAR_200,false,pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor rightMiddle(8,pros::E_MOTOR_GEAR_200,false,pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor rightDown(7,pros::E_MOTOR_GEAR_200,false,pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor_Group leftSide({leftUp,leftMiddle,leftDown});
pros::Motor_Group rightSide({rightUp,rightMiddle,rightDown});
pros::Motor_Group driveMotors({leftUp,leftMiddle,leftDown,rightUp,rightMiddle,rightDown});

//Positive: Slingshot | Negative Intake
pros::Motor sling1(11,pros::E_MOTOR_GEAR_100,false,pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor sling2(20,pros::E_MOTOR_GEAR_100,true,pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor_Group slingShotMotors({sling1,sling2});

pros::Controller controller(pros::E_CONTROLLER_MASTER);

pros::ADIDigitalIn limiter('H');
pros::ADIDigitalOut piston1('A');
pros::ADIDigitalOut piston2('B');