/* -----------------------------------------------------------------------
 * Paramics Programmer API    (paramics-support@quadstone.com)
 * Quadstone Ltd.             Tel: +44 131 220 4491
 * 16 Chester Street          Fax: +44 131 220 4492
 * Edinburgh, EH3 7RA, UK     WWW: http://www.paramics-online.com
 * ----------------------------------------------------------------------- */  
 
/* ------------------------------------------------------------------------
 * Author: Weinan Gao, Ben Chen, Amol
 * Contact: cc2013ex@gmail.com
 * ------------------------------------------------------------------------*/
 
/*
 * Description:
 * 
 * 	Example plugin showing how to slow down and stop a vehicle at a specific point on  a Link
 *
 * 	1. Apply to the first vehicle that is upstream of the stoping point, could be leader vehicle or not 
 *
 * 	2. So we need to check both clauses.
 *
 * 	3. The vehicle needs to be far enough from the upstream of the stoping point to stop safely.
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "programmer.h"

#define MAX(a,b)        ((a) > (b) ? (a) : (b))
#define MIN(a,b)        ((a) > (b) ? (b) : (a))

#define FIRST_START_TIME			35.0
#define SECOND_START_TIME          125.0

#define FIRST_LINK		           "1:3"
#define SECOND_LINK                "3:5"
#define SLOW_DOWN_LOCATION	        200  // Vehicles will slow down 100m before the destination 

// parameters for the controller of vehicles driven by human
typedef struct controller_human_parameters {
	float alpha;
	float beta;
	float tau;
	float desired_speed;
	float desired_headway;
	VEHICLE* v;
} human_para;

// parameters for the controller of autonomous vehicles
typedef struct controller_auto_parameters {
	VEHICLE** vehicle_list;
	float desired_headway;
	float desired_speed;
} auto_para;

// parameter setting for control
static float desired_speed = 15.0; 
static float desired_headway = 30.0 + 7.62; //the lenght of vehicle is 7.62m

//Adjust the speed of the leading vehicle
static float leadSpeed(VEHICLE* v);

// Concrete methods to control vehicles driven by human & autonomous vehicles
static float controller_human(human_para* hp);
static float controller_autonomous(auto_para* ap);

//
static int cnt = -1;
static int* cnts;





void qpx_NET_start()
{
	int nlinks = qpg_NET_links();
	cnts = (int*) malloc(nlinks * sizeof(int));
}


void qpx_LNK_timeStep(LINK* link)
{
	int lane = 1;
	int vehicleCount; // the number of vehices on a specific link
	int leader, firstleader, follower, gap; // indices of the current leader, the first leader under our control, each follower on the link, the i^th follower after the current leader (i == gap)
	int i, j;
	
	float speed, newspeed;
	const char* link_name;
	
	VEHICLE** vehicles;
	
	human_para* humanPara;
	auto_para* autoPara;
	
	link_name = qpg_LNK_name(link);

//qps_DRW_colour(API_BLUE);
// if it is not the first link, return & do nothing
	if(strcmp(link_name, FIRST_LINK) != 0)
		return;
	
	// if it is before the first starting time, return & do nothing
	if(qpg_CFG_simulationTime() < FIRST_START_TIME)
		return;
	
	vehicleCount = qpg_LNK_vehicles(link, lane);
	if (vehicleCount == 0)
		return;
	vehicles = (VEHICLE**) malloc(vehicleCount * sizeof(VEHICLE*));

    qpg_LNK_vehicleList(link, lane, vehicleCount, vehicles); // get all vehicles from the specific lane on a link, and store their pointers into the array of pointers (vehicles)
    

    firstleader = 0;
    
    if (vehicles[0] != pre_leading_vhc) {
    	cnt++;
	}
	pre_leading_vhc = vehicles[0];
	cnt = cnt % 5;
	cnt = cnt == 0 ? 5 : cnt;
    
    for (i = 5 - cnt; i < vehicleCount; i += 5) {
		if (qpg_VHC_distance(vehicles[i]) > SLOW_DOWN_LOCATION) {
    		firstleader = i;
    		break;
		}
		qps_DRW_vehicleColour(vehicles[i], API_GREEN);
		for (j = 1; j < 5; j++) {
			qps_DRW_vehicleColour(vehicles[i + j], API_GREEN);
		}
	}
	
	/* Five Cars as a group */
    humanPara = (human_para*) malloc(sizeof(human_para));
    autoPara = (auto_para*) malloc(sizeof(auto_para));
	for (leader = firstleader; leader < vehicleCount; leader += 5) {
		for (gap = 0; gap < MIN(5, vehicleCount - leader) ; gap++) {
			follower = leader + gap;
			if (gap == 0) { //leader : human
			    qps_DRW_vehicleColour(vehicles[follower], API_RED);
				newspeed = leadSpeed(vehicles[follower]);
			} else if (gap == 4) { // follower : auto
				qps_DRW_vehicleColour(vehicles[follower], API_YELLOW);
				
				autoPara->vehicle_list = &vehicles[leader]; // may be a problem
				autoPara->desired_headway = desired_headway;
				autoPara->desired_speed = desired_speed;
				newspeed = controller_autonomous(autoPara);
			} else { // follower : human
				qps_DRW_vehicleColour(vehicles[follower], API_BLUE);
				
			    humanPara->alpha = 0.15;
				humanPara->beta = 0.25;
				humanPara->tau = 0.6366;
				humanPara->desired_headway = desired_headway;
				humanPara->desired_speed = desired_speed;
				humanPara->v = vehicles[follower];
				newspeed = controller_human(humanPara);
			}
			qps_VHC_speed(vehicles[follower], newspeed); 
		}
	}
	
	free(vehicles);
	free(humanPara);
	free(autoPara);
}

float leadSpeed(VEHICLE* v)
{
	float speed;
	float sample_step = qpg_CFG_timeStep();
	
	if(qpg_VHC_stopped(v)) return 0.0; 

	speed = (1 - sample_step) * qpg_VHC_speed(v) + sample_step * (desired_speed + 0.5);
	// need acceleration for the leading car before FIRST_START_TIME?
	
	return speed;
}

float controller_human(human_para* hp)
{
	float accel;
 	float newspeed;
 	
 	VEHICLE* pre = qpg_VHC_ahead(hp->v);
 	float headway_diff = qpg_VHC_distance(hp->v) - qpg_VHC_distance(pre) - hp->desired_headway;
 	float speed_diff = qpg_VHC_speed(hp->v) - hp->desired_speed;
 	float pre_speed_diff = qpg_VHC_speed(pre) - hp->desired_speed;
 	
 	
    // Control Method: Calculate the new acceleration & speed 
 	accel = (hp->alpha / hp->tau) * headway_diff - (hp->alpha + hp->beta) * speed_diff + hp->beta * pre_speed_diff;
 	newspeed = qpg_VHC_speed(hp->v) + accel * qpg_CFG_timeStep();
 	
 	return newspeed;
}
 
float controller_autonomous(auto_para* ap)
{
 	int i, j;
 	float newspeed;
 	float accel = 0;
 	
 	float desired_speed = ap->desired_speed;
	float desired_headway = ap->desired_headway;
	
	float current_dist[5];
	float current_speed[5];
	
	float K0[5][8] = {
		{ 0, 0, 0, 0, 0, 0,  -0.136 , 0.2   },
		{ 0, 0, 0, 0, 0, 0,  -0.3163, 0.8702},
		{ 0 ,0, 0, 0, 0, 0,  -0.3033, 0.8511},
		{ 0, 0, 0, 0, 0, 0,  -0.3030, 0.8506},
		{ 0, 0, 0, 0, 0, 0,  -0.3030, 0.8506}
	};
	
	
	// Classified time_diff into 5 groups, each of which takes the parameters from one row of K0
    float time_diff = qpg_CFG_simulationTime() - FIRST_START_TIME; 
    i = time_diff / 8;
    if (i > 3) {
    	i = 4;
	}
	
	// get the current speeds & distances of 5 cars from a platoon 
	for (j = 0; j < 5; j++) {
		current_dist[j] = qpg_VHC_distance(ap->vehicle_list[j]);
		current_speed[j] = qpg_VHC_speed(ap->vehicle_list[j]);
	}
	
	// Control Method: Calculate the new acceleration & speed 
	for (j = 0; j < 4; j++) {
		accel += - K0[i][2 * j] * (current_dist[j + 1] - current_dist[j] - desired_headway) - K0[i][2 * j + 1] * (current_speed[j] - desired_speed);
	}
	newspeed = accel * qpg_CFG_timeStep() + current_speed[4];

 	return newspeed;
}
