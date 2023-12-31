#include "main.h"
#include "autoFunction.h"
#include "lemlib/api.hpp"
#include <numeric>
#include "api.h"
#include "pros/misc.h"
#include "ARMS/config.h"

//Lemlib Stuff
lemlib::Drivetrain drivetrain {
    &leftSide,
	&rightSide,
	10.75, //track length
	3.25, // wheel diameter
	333, //wheel rpm
	2 //chase power

};
//sensors
lemlib::OdomSensors sensors{
	nullptr, //verticle tracking wheel 1
	nullptr, //verticle tracking wheel 2
	nullptr, //horizontal tracking wheel 
	nullptr, //another tracking wheel
	nullptr //intertial sensor
};
//forward/backward PID
lemlib::ControllerSettings linearController{
	10, //kP
	30, //kD
	1, //smallErrorRange
	100, //smallErrorTimeout
	3, //largeErrorRange
	500, //largeErrorTimeout
	20 //slew rate
};
//turning PID
lemlib::ControllerSettings angularController{
	2, //kP
	10, //kD
	1, //smallErrorRange
	100, //smallErrorTimeout
	3, //largeErrorRange
	500, //largeErrorTimeout
	20 //slew rate
};

lemlib::Chassis chassis(drivetrain,linearController,angularController,sensors);

//print lemlib position
void screen(){
	while (true){
		lemlib::Pose pose = chassis.getPose();
		pros::lcd::print(0,"x: %f",pose.x);
		pros::lcd::print(1,"y: %f",pose.y);
		pros::lcd::print(2,"heading: %f",pose.theta);
		pros::delay(10);
	}
}

void armsScreen(){
	pros::lcd::set_text(0,"X: " + std::to_string(arms::odom::getPosition().x));
	pros::lcd::set_text(1,"Y: " + std::to_string(arms::odom::getPosition().y));
	pros::lcd::set_text(2,"H: " + std::to_string(arms::odom::getHeading()));
	pros::lcd::set_text(3,"Left: " + std::to_string(arms::odom::getLeftEncoder()));
	pros::lcd::set_text(4,"Right: " + std::to_string(arms::odom::getRightEncoder()));
	pros::delay(10);
}

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}
/**
 * sling shot Code
 */
void slingShot(){

	sling1.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	sling2.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

	sling1.move(127);
	sling2.move(-127);


	while (true){
		if (limiter.get_value()==true){
			sling1.move(0);
			sling2.move(0);
			return;
		}
		if (controller.get_digital(DIGITAL_X)){
			sling1.move(0);
			sling2.move(0);
			return;

		pros::delay(20);
		}
	}

}

/*Temperature Control*/
bool motorTemps(pros::Motor_Group motorGroup) {
		std::vector<double> temps = motorGroup.get_temperatures();
		double averageTemps = std::reduce(temps.begin(),temps.end(),0.0)/temps.size();

		if (averageTemps>60){
			return true;
		}
		else{
			return false;
		}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);

	pros::Controller controller(pros::E_CONTROLLER_MASTER);

	leftUp.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	leftDown.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	rightUp.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	rightDown.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	leftMiddle.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	rightMiddle.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	
	sling1.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	sling2.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

	chassis.calibrate();
	chassis.setPose(0,0,0);
	//pros::Task screenTask(screen);
	pros::Task screenTask(armsScreen);
	arms::init();
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

int sameColor = 2;
/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
	/*
	while (true){ //auton selector
		if (controller.get_digital(DIGITAL_LEFT)==true){
			sameColor = 1;
			controller.print(2,0,"Same Color");
			pros::delay(50);
		}
		if (controller.get_digital(DIGITAL_RIGHT)==true){
			sameColor = 0;
			controller.print(2,0,"Opposite Color");
			pros::delay(50);
		}
	}
	*/
}

ASSET(sameColor_txt);
ASSET(oppositeColor_txt);

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {

	pros::ADIEncoder leftE ();
	pros::ADIEncoder rightE ();
	pros::ADIEncoder bottomE ();

	float leftRad; // All in Inches
	float rightRad;
	float downRad;

	float realX = 0;
	float realY = 0;
	float realTheta;
	pros::lcd::set_text(1, "Auton");

	driveMotors.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);

	switch(arms::selector::auton){
		case 1: //red side auton

		break;
		case 3:

		break;

	}

	//chassis.turnTo(x,y,timeout,should back of robot face point,max speed) turns to face a point
	//chassis.moveTo(x,y,heading,timeout,asyncronous?) moves to a point
	//chassis.waitUntilDist(num) wait until robot has traved num distance
	//chassis.follow(path.txt,timeout,look ahead distance, backwards?)

	
	chassis.calibrate();
	chassis.setPose(0,0,0);
	//chassis.turnTo();
	//chassis.moveTo(0,30,0,20000); //tuning
	
	

	//uncomment line below to test pure pursuit 
	//chassis.follow(sameColor_txt,3,15000,true);

	/*
	if (sameColor==1){//same side auton
		chassis.follow(sameColor_txt,2000,15,true);
	}
	if (sameColor==0){//opposite side auton
		chassis.follow(oppositeColor_txt,2000,15,true);
	}
	*/

	//  Same Color
		/*
		driveMotors.move(-100);
		pros::delay(1300);
		driveMotors.move(0);
		pros::delay(500);
		driveMotors.move(100);
		pros::delay(500);
		driveMotors.move(0);
		*/
	//	Opposite Color
		/*
		driveMotors.move(-100);
		pros::delay(650);
		driveMotors.move(0);
		*/
	//	Skills Auton
		/*
		slingShotMotors.move(127);
		driveMotors.move(-20);
		*/
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	
	double speedMod = 1; // Speed Modifer 
	bool driveOverTemp;
	bool slingOverTemp;

	while (true) {
	
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);

		//Temperature Regulation system		

		int forward = controller.get_analog(ANALOG_LEFT_Y);
		int turn = controller.get_analog(ANALOG_LEFT_X);

		if (forward+turn!=0){
			driveMotors.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);
		}

		if (controller.get_digital(DIGITAL_L2)==true){
			speedMod = .3; // Speed at 30%
		}
		else{
			speedMod = 1; // Speed at 100%
		}

		double leftV = (forward + turn)*speedMod;
		double rightV = (forward - turn)*speedMod;
		
		std::vector<double> dTemps = driveMotors.get_temperatures();
		double averageDriveTemps = std::reduce(dTemps.begin(),dTemps.end(),0.0)/dTemps.size();

		if (averageDriveTemps<55){
			leftSide.move(leftV);
			rightSide.move(rightV);
		}
		else{
			driveMotors.move(0);
		}

		std::vector<double> sTemps = slingShotMotors.get_temperatures();
		double averageSlingTemps = std::reduce(sTemps.begin(),sTemps.end(),0.0)/sTemps.size();

		if (averageSlingTemps<55){
			//slingshot
			if (controller.get_digital(DIGITAL_R1)==true){
				slingShotMotors.move(-127);
			}
			//intake
			if (controller.get_digital(DIGITAL_A)==true){
				slingShotMotors.move(127);
			}
		}
		else{
			slingShotMotors.move(0);
		}

		//turn off
		if (controller.get_digital(DIGITAL_B)==true || controller.get_digital(DIGITAL_R2)==true){
			slingShotMotors.move(0);
		}
		//wheellock
		if (controller.get_digital(DIGITAL_Y)==true){
			driveMotors.set_brake_modes(pros::E_MOTOR_BRAKE_BRAKE);
		}
		
		if (controller.get_digital(DIGITAL_UP)==true){
			piston1.set_value(false);
			piston2.set_value(false);
		}
		if (controller.get_digital(DIGITAL_DOWN)==true){
			piston1.set_value(true);
			piston2.set_value(true);
		}
		if (controller.get_digital(DIGITAL_LEFT)==true){
			leftSide.move(127);
			rightSide.move(127);
		}
		pros::delay(20);
	}
}
