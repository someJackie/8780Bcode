#include "main.h"

//Motors
pros::Motor leftUp(1,pros::E_MOTOR_GEAR_200,false,pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor leftMiddle(5,pros::E_MOTOR_GEAR_200,false,pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor leftDown(3,pros::E_MOTOR_GEAR_200,false,pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor rightUp(6,pros::E_MOTOR_GEAR_200,false,pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor rightMiddle(8,pros::E_MOTOR_GEAR_200,false,pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor rightDown(7,pros::E_MOTOR_GEAR_200,false,pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor_Group leftSide({leftUp,leftMiddle,leftDown});
pros::Motor_Group rightSide({rightUp,rightMiddle,rightDown});



pros::Motor sling1(11,pros::E_MOTOR_GEAR_100,false,pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor sling2(20,pros::E_MOTOR_GEAR_100,false,pros::E_MOTOR_ENCODER_DEGREES);

pros::Controller controller(pros::E_CONTROLLER_MASTER);

pros::ADIDigitalIn limiter('h');