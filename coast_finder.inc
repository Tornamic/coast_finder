/*

coast_finder include

Author: Tornamic (Kirill Tymoshchenko)
    Discord: https://pastebin.com/raw/LMBNfFHE
    Github: https://github.com/Tornamic
    pawn.wiki https://pawn.wiki/i.php?/user/54232-tornamic/

Functions:
    FindNearestCoast(Float:near_x, Float:near_y, Float:near_z, &Float:coast_x, &Float:coast_y, &Float:coast_z);

Dependencies:
    ColAndreas plugin https://github.com/Pottus/ColAndreas
*/

#if defined t_coast_finder_defined
    #endinput
#endif
#define t_coast_finder_defined

#if !defined _colandreas_included
    #error [coast_finder] please include colandreas before coast_finder.inc
#endif

#define     RAY_COUNT               (16)
#define     RAY_ANGLE               (Float:(360.0 / RAY_COUNT))
#define     RAY_LENGTH              (Float:1000.0)
#define     POINT_ABOVE_WATER       (Float:0.1)

stock FindNearestCoast(Float:near_x, Float:near_y, Float:near_z, &Float:coast_x, &Float:coast_y, &Float:coast_z)
{
    new 
        Float:rays[RAY_COUNT][3],
        i,
        Float:e_x,
        Float:e_y,
        min_idx = -1,
        Float:min_val = FLOAT_INFINITY,
        collision
    ;
    for(; i < RAY_COUNT; ++i)
    {
        e_x = near_x + floatsin(-(RAY_ANGLE * i), degrees) * RAY_LENGTH;
        e_y = near_y + floatsin(-(RAY_ANGLE * i), degrees) * RAY_LENGTH;
        collision = CA_RayCastLine(near_x, near_y, POINT_ABOVE_WATER, e_x, e_y, POINT_ABOVE_WATER, rays[i][0], rays[i][1], rays[i][2]);
        if(collision)
        {
            CA_FindZ_For2DCoord(rays[i][0], rays[i][1], rays[i][2]);
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

static t_cf_IsPointNearWater(Float:x, Float:y, Float:z, Float:dist = 3.0, Float:height = 3.0)
{
    new Float:tmp;

	for(new i; i < 6; i++)
		if(CA_RayCastLine(x + (dist * floatsin(i * 60.0, degrees)), y + (dist * floatcos(i * 60.0, degrees)), z + height, x + (dist * floatsin(i * 60.0, degrees)), y + (dist * floatcos(i * 60.0, degrees)), z - height, tmp, tmp, tmp) == WATER_OBJECT)
			return 1;
	return 0;
}
