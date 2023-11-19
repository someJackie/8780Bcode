#include "main.h"
#include "autoFunction.h"
#include "lemlib/api.hpp"
#include <numeric>

//Lemlib Stuff
lemlib::Drivetrain_t drivetrain {
    &leftSide,
	&rightSide,
	10.75, //track length
	3.25, // wheel diameter
	333, //wheel rpm
	2 //chase power

};
//sensors
lemlib::OdomSensors_t sensors{
	nullptr, //verticle tracking wheel 1
	nullptr, //verticle tracking wheel 2
	nullptr, //horizontal tracking wheel 
	nullptr, //another tracking wheel
	nullptr //intertial sensor
};
//forward/backward PID
lemlib::ChassisController_t lateralController{
	0.1, //kP
	0, //kD
	1, //smallErrorRange
	100, //smallErrorTimeout
	3, //largeErrorRange
	500, //largeErrorTimeout
	5 //slew rate
};
//turning PID
lemlib::ChassisController_t angularController{
	4, //kP
	40, //kD
	1, //smallErrorRange
	100, //smallErrorTimeout
	3, //largeErrorRange
	500, //largeErrorTimeout
	0 //slew rate
};

lemlib::Chassis chassis(drivetrain,lateralController,angularController,sensors);

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
	pros::Task screenTask(screen);
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
	chassis.calibrate();
	chassis.setPose(0,0,0);
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

	leftUp.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	leftDown.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	rightUp.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	rightDown.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	leftMiddle.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	rightMiddle.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);

	//chassis.turnTo(x,y,timeout,should back of robot face point,max speed) turns to face a point
	//chassis.moveTo(x,y,heading,timeout,asyncronous?) moves to a point
	//chassis.waitUntilDist(num) wait until robot has traved num distance
	//chassis.follow(path.txt,timeout,look ahead distance, backwards?)


	chassis.moveTo(0,20,0,2000,false,false,0,6,50); //tuning

	//uncomment line below to test pure pursuit 
	//chassis.follow(sameColor_txt,2000,15,true);

	/*
	if (sameColor==1){//same side auton
		chassis.follow(sameColor_txt,2000,15,true);
	}
	if (sameColor==0){//opposite side auton
		chassis.follow(oppositeColor_txt,2000,15,true);
	}
	if (sameColor==2){//forgot to choose auton just rams triball into net
		leftMiddle.move(100);
		leftDown.move(100);
		leftUp.move(100);
		rightDown.move(100);
		rightUp.move(100);
		rightMiddle.move(100);
		pros::delay(1500);
		leftMiddle.move(0);
		leftDown.move(0);
		leftUp.move(0);
		rightDown.move(0);
		rightUp.move(0);
		rightMiddle.move(0);
		pros::delay(500);
		leftMiddle.move(-100);
		leftDown.move(-100);
		leftUp.move(-100);
		rightDown.move(-100);
		rightUp.move(-100);
		rightMiddle.move(-100);
		pros::delay(1000);
		leftMiddle.move(0);
		leftDown.move(0);
		leftUp.move(0);
		rightDown.move(0);
		rightUp.move(0);
		rightMiddle.move(0);
	}
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

		int forward = controller.get_analog(ANALOG_LEFT_X);
		int turn = controller.get_analog(ANALOG_LEFT_Y);

		if (forward+turn!=0){
			leftUp.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
			leftDown.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
			rightUp.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
			rightDown.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
			leftMiddle.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
			rightMiddle.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
		}

		if (controller.get_digital(DIGITAL_L2)==true){
			speedMod = .3; // Speed at 30%
		}
		else{
			speedMod = 1; // Speed at 100%
		}

		
		double leftV = -(forward + turn)*speedMod;
		double rightV = -(forward - turn)*speedMod;
		
		std::vector<double> temps = allMotors.get_temperatures();
		double averageDriveTemps = std::reduce(temps.begin(),temps.end(),0.0)/temps.size();

		if (averageDriveTemps>60){
			leftSide.move(leftV);
			rightSide.move(rightV);
		}
		else{
			allMotors.move(0);
		}

		std::vector<double> temps = slingShotMotors.get_temperatures();
		double averageSlingTemps = std::reduce(temps.begin(),temps.end(),0.0)/temps.size();

		if (averageSlingTemps>60){
			//slingshot
			if (controller.get_digital(DIGITAL_R1)==true){
				sling1.move(127);
				sling2.move(-127);
			}
			//intake
			if (controller.get_digital(DIGITAL_A)==true){
				sling1.move(-127);
				sling2.move(127);
			}
		}
		else{
			slingShotMotors.move(0);
		}

		//turn off
		if (controller.get_digital(DIGITAL_B)==true || controller.get_digital(DIGITAL_R2)==true){
			sling1.move(0);
			sling2.move(0);
		}
		//wheellock
		if (controller.get_digital(DIGITAL_Y)==true){
			leftUp.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
			leftDown.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
			rightUp.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
			rightDown.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
			leftMiddle.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
			rightMiddle.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
		}
		
		if (controller.get_digital(DIGITAL_UP)==true){
			piston1.set_value(false);
			piston2.set_value(false);
		}
		if (controller.get_digital(DIGITAL_DOWN)==true){
			piston1.set_value(true);
			piston2.set_value(true);
		}
		pros::delay(20);
	}
}
