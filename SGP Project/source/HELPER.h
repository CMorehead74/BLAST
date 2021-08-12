/*
	File:				Helper.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Schadrac Francois
	Last Edited:		9/11/2012
	Purpose:			just a place to store all the constants in out game
*/

#pragma once

//friendly unit enumerations
enum AllyTypes { ALLY_GUNNER, ALLY_MEDIC, ALLY_ROCKET, ALLY_SNIPER };
enum AllyStates { STATE_ATTACK, STATE_DEFEND, STATE_FOLLOW };
enum BulletTypes { SNIPER, MG, PISTOL, SHOTGUN, ROCKET };

enum UnitType { /*eSHOTGUN,*/ eGUNNER, eMEDIC, eSNIPER, eROCKET };

//GameObjects enumerations
//enum ObjectType { OBJ_BASE, OBJ_PLAYER, OBJ_ALLY, OBJ_ENEMIES };

enum PARTICLETYPE { EMISSION, STARS };

enum EMITTERTYPE { RECTANGLE, CIRCLE, LINE };

#define MOVE_SPEED_ALLY_FOLLOWSTATE 0.025f

#define ALLY_MOVEMENT_SPEED 0.025f
#define ENEMY_MOVEMENT_SPEED 0.025f
