# coast_finder include

## Author: 
Tornamic (Kirill Tymoshchenko)
   * Discord: https://pastebin.com/raw/LMBNfFHE
   * Github: https://github.com/Tornamic
   * pawn.wiki https://pawn.wiki/i.php?/user/54232-tornamic/

## Special thanks:
Vladimir (NoPressF)

## Functions:
    FindNearestCoast(Float:near_x, Float:near_y, Float:near_z, &Float:coast_x, &Float:coast_y, &Float:coast_z);
    
## Dependencies:
  ColAndreas plugin https://github.com/Pottus/ColAndreas
## Example:
```pawn
CMD:coast(playerid)
{	
	new 
		Float:player_x, Float:player_y, Float:player_z,
		Float:coast_x, Float:coast_y, Float:coast_z
	;
	
	GetPlayerPos(playerid, player_x, player_y, player_z);

	if(FindNearestCoast(player_x, player_y, player_z, coast_x, coast_y, coast_z))
	{
		SetPlayerPos(playerid, coast_x, coast_y, coast_z + 1.0);
		SendClientMessage(playerid, -1, "Вы успешно телепортировались к ближайшему берегу");
		return 1;
	}

	SendClientMessage(playerid, -1, "Сервер не смог найти ближайший берег");
	
	return 1;
}
```
