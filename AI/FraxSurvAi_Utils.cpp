#include "FraxSurvAi.h"

// Derived from http://www.flipcode.com/archives/Fast_Approximate_Distance_Functions.shtml
long FraxSurvAI::GetDistance(LOCATION s, LOCATION d)
{
	if (s.x == d.x && s.y == d.y)
	{
		return 0;
	}

	int min,
		max,
		dX,
		dY,
		approxDist;

	dX = s.x - d.x;
	if (dX < 0) { dX *= -1; }
	dY = s.y - d.y;
	if (dY < 0) { dY *= -1; }


	if (dX < dY)
	{
		min = dX;
		max = dY;
	}
	else
	{
		min = dY;
		max = dX;
	}

	// Tweak these magic numbers later for better accuracy?
	approxDist = (max * 1007) + (min * 441);
	if (max < (min << 4))
	{
		approxDist -= (max * 40);
	}

	// Add 512 for rounding and then divide by 1024
	return ((approxDist + 512) >> 10);
}

void FraxSurvAI::FindNearestCommandCenter()
{
	odprintf(" FindNearestCommandCenter: Entered.\n");
	Attacking[1].TakeAllUnits(Attacking[0]);
	Attacking[0].Destroy();
	Attacking[0] = CreateFightGroup(aiNum);
	odprintf(" >Attacking[0] reinitialized.\n");

	AttackGroupAttacked = false;
	odprintf(" >Check under attack trigger reinitialized.\n");

	Unit next;
	unsigned long d;
	wayPnt *w = NULL;

	ClosestEnumerator findCC(LOCATION(66+31, 54-1));
	while (findCC.GetNext(next, d))
	{
		if (next.IsLive() && 
			next.OwnerID() != aiNum &&
			next.OwnerID() != 6 &&
			(next.GetType() == mapCommandCenter || next.GetCargo() == mapCommandCenter))
		{
			if (next.Location().y < 46)
			{
				// No point pathfinding to something right next door.  Get outta here and issue a blanket attack order.
				odprintf(" >CC found above y46.  Issuing generic attack order.\n");
				break;
			}

			// Target found.  Find nearest waypoint and move backwards along the graph.
			odprintf(" >Found target.\n");
			for (int i = 0; i < NUM_WAYPOINTS; i++)
			{
				unsigned long d2 = GetDistance(aiWaypoints[i].point, next.Location());
				if (d2 < d)
				{
					w = &(aiWaypoints[i]);
					d = d2;
				}
				odprintf(" >Found nearest waypoint to target.\n");
			}
			break;
		}
	}

	// Target (and associated waypoint) found.  Trace back to start location.
	if (w != NULL)
	{
		odprintf(" >Calculating route....\n");
		LOCATION temp[7];	// OP2 supports a maximum of 7 waypoints
		temp[0] = next.Location();
		int i = 1;
		while (w != NULL && i != 7)
		{
			temp[i] = w->point;
			w = w->next;
			i++;
		}
		odprintf(" >Route found.\n");

		if (attackRoute != NULL)
		{
			delete [] attackRoute;
			attackRoute = NULL;
		}

		// We have the attack route, but it's in backwards order.  Flip it around!
		attackRoute = new LOCATION[i];
		int j = 0;
		for (i; i > 0; i--)
		{
			attackRoute[j] = temp[i-1];
			j++;
		}
		// If not all 7 waypoints are used, the last waypoint must be -1, -1
		if (j != 7)
		{
			attackRoute[j] = LOCATION(-1, -1);
		}

		odprintf(" >Setting attack route.\n");
		PatrolRoute attackPath;
		attackPath.waypoints = attackRoute;

		Attacking[0].SetPatrolMode(attackPath);
		
	}

	// No target found, so as a fallback just enter a generic attack mode.
	else
	{
		odprintf(" >No targets found.  Entering generic attack mode.\n");
		Attacking[0].SetAttackType(mapCommandCenter);
		Attacking[0].DoAttackEnemy();
	}
		
	odprintf("FindNearestCommandCenter: Done.\n");

}

void FraxSurvAI::SafeDestroyGroup(ScGroup &group)
{
	Player[aiNum].GetDefaultGroup().TakeAllUnits(group);
	group.Destroy();
}
