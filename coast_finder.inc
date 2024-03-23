/*

coast_finder include

Author: Tornamic (Kirill Tymoshchenko)
    Discord: https://pastebin.com/raw/LMBNfFHE
    Github: https://github.com/Tornamic
    pawn.wiki https://pawn.wiki/i.php?/user/54232-tornamic/
Special thanks to:
    Vladimir (NoPressF)

Functions:
	FindNearestCoast(Float:near_x, Float:near_y, Float:near_z, &Float:coast_x, &Float:coast_y, &Float:coast_z, Float:offset = 3.0, Float:height_limit = 5.0);
    IsPlayerNearCoast(playerid, Float:radius = 10.0, Float:height_limit = 10.0);
    IsVehicleNearCoast(vehicleid, Float:radius = 10.0, Float:height_limit = 10.0);
    IsPointNearCoast(Float:point_x, Float:point_y, Float:point_z, Float:radius = 10.0, Float:height_limit = 10.0);
    GetDistanceToNearestCoast(Float:near_x, Float:near_y, Float:near_z, &Float:distance);
    GetPlayerDistanceToNearestCoast(playerid, &Float:distance);
    GetVehDistanceToNearestCoast(vehicleid, &Float:distance);
    
Dependencies:
    ColAndreas plugin https://github.com/Pottus/ColAndreas
*/

// ----------------------------------------------------------------------------------------------------

#if defined t_coast_finder_defined
    #endinput
#endif
#define t_coast_finder_defined

// ----------------------------------------------------------------------------------------------------

#if !defined _colandreas_included
    #error [coast_finder] please include colandreas before coast_finder.inc
#endif

// ----------------------------------------------------------------------------------------------------

#define     RAY_COUNT               (16)
#define     RAY_ANGLE               (Float:(360.0 / RAY_COUNT))
#define     RAY_LENGTH              (Float:1000.0)
#define     POINT_ABOVE_WATER       (Float:0.1)
#define     HIGHEST_POINT           (Float:150.0)

// ----------------------------------------------------------------------------------------------------

stock FindNearestCoast(Float:near_x, Float:near_y, Float:near_z, &Float:coast_x, &Float:coast_y, &Float:coast_z, Float:offset = 3.0, Float:height_limit = 10.0)
{
    new 
        Float:rays[RAY_COUNT][3],
        i,
        Float:e_x,
        Float:e_y,
        min_idx = -1,
        Float:min_val = FLOAT_INFINITY,
        collision,
        Float:move_x,
        Float:move_y
    ;
    for(; i < RAY_COUNT; ++i)
    {
        move_x = floatsin(-(RAY_ANGLE * i), degrees);
        move_y = floatcos(-(RAY_ANGLE * i), degrees);
        e_x = near_x + move_x * RAY_LENGTH;
        e_y = near_y + move_y * RAY_LENGTH;
        collision = CA_RayCastLine(near_x, near_y, POINT_ABOVE_WATER, e_x, e_y, POINT_ABOVE_WATER, rays[i][0], rays[i][1], rays[i][2]);
        if(collision)
        {
            if(offset != 0.0)
            {
                new 
                    multiplier = -1,
                    Float:tmp    
                ;

                if(CA_RayCastLine(near_x, near_y, HIGHEST_POINT, near_x, near_y, -1.0, tmp, tmp, tmp) == WATER_OBJECT)
                    multiplier = 1;

                rays[i][0] += move_x * offset * multiplier;
                rays[i][1] += move_y * offset * multiplier;
            }
            CA_FindZ_For2DCoord(rays[i][0], rays[i][1], rays[i][2]);
            if(rays[i][2] - POINT_ABOVE_WATER > height_limit) continue;
            if(!t_cf_IsPointNearWater(rays[i][0], rays[i][1], rays[i][2])) continue;
            new Float:dist = VectorSize(rays[i][0] - near_x, rays[i][1] - near_y, rays[i][2] - near_z);
            if(dist < min_val)
            {
                min_val = dist;
                min_idx = i;
            }
        }
    }
    if(min_idx == -1)
    {
        coast_x = near_x;
        coast_y = near_y;
        coast_z = near_z;
        return 0;
    }
    coast_x = rays[min_idx][0];
    coast_y = rays[min_idx][1];
    coast_z = rays[min_idx][2];
    return 1;
}

// ----------------------------------------------------------------------------------------------------

stock IsPlayerNearCoast(playerid, Float:radius = 10.0, Float:height_limit = 10.0)
{
    new 
        Float:player_x, 
        Float:player_y, 
        Float:player_z,
        Float:coast_x,
        Float:coast_y,
        Float:coast_z
    ;
    GetPlayerPos(playerid, player_x, player_y, player_z);
    if(!FindNearestCoast(player_x, player_y, player_z, coast_x, coast_y, coast_z, 0.0, height_limit))
    {
        return 0;
    }
    return VectorSize(player_x - coast_x, player_y - coast_y, player_z - coast_z) <= radius;
}

// ----------------------------------------------------------------------------------------------------

stock IsVehicleNearCoast(vehicleid, Float:radius = 10.0, Float:height_limit = 10.0)
{
    new 
        Float:vehicle_x, 
        Float:vehicle_y, 
        Float:vehicle_z,
        Float:coast_x,
        Float:coast_y,
        Float:coast_z
    ;
    GetVehiclePos(vehicleid, player_x, player_y, player_z);
    if(!FindNearestCoast(player_x, player_y, player_z, coast_x, coast_y, coast_z, 0.0, height_limit))
    {
        return 0;
    }
    return VectorSize(vehicle_x - coast_x, vehicle_y - coast_y, vehicle_z - coast_z) <= radius;
}

// ----------------------------------------------------------------------------------------------------

stock IsPointNearCoast(Float:point_x, Float:point_y, Float:point_z, Float:radius = 10.0, Float:height_limit = 10.0)
{
    new 
        Float:coast_x,
        Float:coast_y,
        Float:coast_z
    ;
    if(!FindNearestCoast(point_x, point_y, point_z, coast_x, coast_y, coast_z, 0.0, height_limit))
    {
        return 0;
    }
    return VectorSize(point_x - coast_x, point_y - coast_y, point_z - coast_z) <= radius;
}

// ----------------------------------------------------------------------------------------------------

stock GetDistanceToNearestCoast(Float:near_x, Float:near_y, Float:near_z, &Float:distance)
{
    new 
        Float:coast_x,
        Float:coast_y,
        Float:coast_z
    ;
    if(!FindNearestCoast(near_x, near_y, near_z, coast_x, coast_y, coast_z, 0.0, HIGHEST_POINT))
    {
        return 0;
    }
    distance = VectorSize(near_x - coast_x, near_y - coast_y, near_z - coast_z);
    return 1;
}

// ----------------------------------------------------------------------------------------------------

stock GetPlayerDistanceToNearestCoast(playerid, &Float:distance)
{
    new 
        Float:player_x, 
        Float:player_y, 
        Float:player_z,
        Float:coast_x,
        Float:coast_y,
        Float:coast_z
    ;
    GetPlayerPos(playerid, player_x, player_y, player_z);
    if(!FindNearestCoast(player_x, player_y, player_z, coast_x, coast_y, coast_z, 0.0, HIGHEST_POINT))
    {
        return 0;
    }
    distance = VectorSize(player_x - coast_x, player_y - coast_y, player_z - coast_z);
    return 1;
}

// ----------------------------------------------------------------------------------------------------

stock GetVehDistanceToNearestCoast(vehicleid, &Float:distance)
{
    new 
        Float:vehicle_x, 
        Float:vehicle_y, 
        Float:vehicle_z,
        Float:coast_x,
        Float:coast_y,
        Float:coast_z
    ;
    GetVehiclePos(vehicleid, vehicle_x, vehicle_y, vehicle_z);
    if(!FindNearestCoast(vehicle_x, vehicle_y, vehicle_z, coast_x, coast_y, coast_z, 0.0, HIGHEST_POINT))
    {
        return 0;
    }
    distance = VectorSize(vehicle_x - coast_x, vehicle_y - coast_y, vehicle_z - coast_z);
    return 1;
}

// ----------------------------------------------------------------------------------------------------

static stock t_cf_IsPointNearWater(Float:x, Float:y, Float:z, Float:dist = 3.0, Float:height = 3.0)
{
    new Float:tmp;

	for(new i; i < 6; i++)
		if(CA_RayCastLine(x + (dist * floatsin(i * 60.0, degrees)), y + (dist * floatcos(i * 60.0, degrees)), z + height, x + (dist * floatsin(i * 60.0, degrees)), y + (dist * floatcos(i * 60.0, degrees)), z - height, tmp, tmp, tmp) == WATER_OBJECT)
			return 1;
	return 0;
}
