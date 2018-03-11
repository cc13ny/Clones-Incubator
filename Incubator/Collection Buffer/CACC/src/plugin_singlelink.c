/* -----------------------------------------------------------------------
 * Paramics Programmer API    (paramics-support@quadstone.com)
 * Quadstone Ltd.             Tel: +44 131 220 4491
 * 16 Chester Street          Fax: +44 131 220 4492
 * Edinburgh, EH3 7RA, UK     WWW: http://www.paramics-online.com
 * ----------------------------------------------------------------------- */  
 
/* ------------------------------------------------------------------------
 * Author: Ben Chen
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

#define FIRST_START_TIME		  35.0
#define SECOND_START_TIME                125.0

#define FIRST_LINK		         "1:3"
#define SECOND_LINK                      "3:5"
#define SLOW_DOWN_LOCATION	          100  // Vehicles will slow down 100m before the destination 

typedef struct controller_human_parameters {
	float alpha;
	float beta;
	float tau;
	float desired_speed;
	float desired_headway;
	VEHICLE* v;
} human_para;

typedef struct controller_auto_parameters {
	VEHICLE** vehicle_list;
	float desired_headway;
	float desired_speed;
} auto_para;

typedef struct VHC_DATA_s {
	int type; // 0 (leader), 1, 2, 3 (human follower), 4 (auto follower)
} VHC_DATA;

static float desired_speed = 15.0; 
static float desired_headway = 30.0 + 7.62; //the lenght of vehicle is 7.62m

static float leadSpeed(VEHICLE* v);
static float controller_human(human_para* hp);
static float controller_autonomous(auto_para* ap);

void qpx_LNK_timeStep(LINK* link)
{
	int i, j;
	int lane = 1;
	int vehicleCount; 
	int lastLeaderWithinLocation = -1;
	
	float speed, newspeed;
	
	VEHICLE* v;
	VEHICLE** vehicles;
	
	VHC_DATA* data;
	VHC_DATA* predata;
	
	human_para* humanPara;
	auto_para* autoPara;
	
	int firstleader;
	int currenttype;

	if (qpg_CFG_simulationTime() < FIRST_START_TIME) {
		return;
	}

	if(strcmp(qpg_LNK_name(link), FIRST_LINK) != 0) {
		return;
	}
	
	vehicleCount = qpg_LNK_vehicles(link, lane);
	if (vehicleCount == 0)
		return;

	vehicles = (VEHICLE**) malloc(vehicleCount * sizeof(VEHICLE*));
	
	qpg_LNK_vehicleList(link, lane, vehicleCount, vehicles);
    
	/* 1 + 3 + 1 vehicles as a platoon*/
	humanPara = (human_para*) malloc(sizeof(human_para));
    	autoPara = (auto_para*) malloc(sizeof(auto_para));
    

	v = NULL;
	firstleader = vehicleCount; // pay attention
    	for (i = 0; i < vehicleCount; i++) {
    		v = vehicles[i]; // v may be empty because all cars are within SLOW_DOWN_LOCATION
    		if (qpg_VHC_distance(v) > SLOW_DOWN_LOCATION) {
    			break;
		}

		data = (VHC_DATA*) qpg_VHC_userdata(v);
		if (data == NULL) {
			data = (VHC_DATA*)malloc(sizeof(VHC_DATA));
			if (i == 0) { // it will be wrong when the previous car runs extremely fast beyond the link. But now we can reasonbly assume that this case won't happen.
				data->type = 0;
			} else {
				predata = (VHC_DATA*)qpg_VHC_userdata(qpg_VHC_ahead(v));
				data->type = (predata->type + 1) % 5;
			}
			qps_VHC_userdata(v, data);
		}
		qps_DRW_vehicleColour(v, API_GREEN);
	}

	if (v != NULL) {
		data = (VHC_DATA*)qpg_VHC_userdata(v);
		if (data != NULL) {
			currenttype = data->type;
		} else {
			if (qpg_VHC_ahead(v) != NULL) {
				predata = (VHC_DATA*)qpg_VHC_userdata(qpg_VHC_ahead(v));
				currenttype = (predata->type + 1) % 5;
			} else {
				currenttype = 0;
			}
		}

		if (currenttype == 0) {
			firstleader = i;
		} else {
			firstleader = i + 5 - currenttype;
			for (j = i; j < MIN(firstleader, vehicleCount); j++) {
				// although v & data are reused here, they're not the same as the ones within the Big If
				v = vehicles[j];
				data = (VHC_DATA*)qpg_VHC_userdata(v);
				if (data == NULL) {
					predata = (VHC_DATA*)qpg_VHC_userdata(qpg_VHC_ahead(v));
					data = (VHC_DATA*)malloc(sizeof(VHC_DATA));
					data->type = (predata->type + 1) % 5;
					qps_VHC_userdata(v, (VHC_USERDATA*)data);
				}
				qps_DRW_vehicleColour(v, API_ORANGE);	
			} 
		}
	} 
	
	
	for (i = firstleader; i < vehicleCount; i++) {
		v = vehicles[i];
		data = (VHC_DATA*)qpg_VHC_userdata(v);
		if (data == NULL) {
			data = (VHC_DATA*)malloc(sizeof(VHC_DATA));
			if (i == firstleader) {
				data->type = 0;
			} else {
				predata = (VHC_DATA*)qpg_VHC_userdata(qpg_VHC_ahead(v));
				data->type = (predata->type + 1) % 5;
			}
			qps_VHC_userdata(v, (VHC_USERDATA*)data);
		}
		
		if (data->type == 0) { //leader : human
		        qps_DRW_vehicleColour(v, API_RED);
			newspeed = leadSpeed(v);
		} 
		else if (data->type == 4) { // follower : auto
			qps_DRW_vehicleColour(v, API_YELLOW);
			
			autoPara->vehicle_list = &vehicles[i - 4]; // it will be a problem when it can't get the previous four cars
			autoPara->desired_headway = desired_headway;
			autoPara->desired_speed = desired_speed;
			newspeed = controller_autonomous(autoPara);
			
		} 
		else { // follower : human
			qps_DRW_vehicleColour(v, API_BLUE);
			
		        humanPara->alpha = 0.15;
			humanPara->beta = 0.25;
			humanPara->tau = 0.6366;
			humanPara->desired_headway = desired_headway;
			humanPara->desired_speed = desired_speed;
			humanPara->v = v;
			newspeed = controller_human(humanPara);
		}
		qps_VHC_speed(v, newspeed); 
	}
	
	free(vehicles);
	free(humanPara);
	free(autoPara);
}

/* Adjust the speed of the leading vehicle smoothly*/
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
