#include <Outpost2DLL.h>
#include <windows.h>
#include <memory.h>
#include <hfl.h>
#include <OP2Types.h>
#include "UnitHookHelper.h"
#include "..\functions.h"

OP2Unit::OP2UnitVtbl oldUnitVtbl[115], *unitVtbl[115];
OP2UnitInfo::OP2UnitInfoVtbl oldUnitInfoVtbl[115], *unitInfoVtbl[115];


void InitUnitVtblAddresses()
{
	static bool unitVtblAddressesInited = false;

	if (unitVtblAddressesInited || !isInited)
		return;
	unitVtblAddressesInited = true;

	DWORD unitVtblOffset[115];
	DWORD unitInfoVtblOffset[115];

	memset(&unitVtblOffset, 0, sizeof(unitVtblOffset));
	memset(&unitInfoVtblOffset, 0, sizeof(unitInfoVtblOffset));
	memset(&unitVtbl, 0, sizeof(unitVtbl));
	memset(&unitInfoVtbl, 0, sizeof(unitInfoVtbl));

	unitVtblOffset[mapCargoTruck] = 0xCF4A8;
	unitVtblOffset[mapConVec] = 0xCF990;
	unitVtblOffset[mapSpider] = 0xD0710;
	unitVtblOffset[mapScorpion] = 0xD07C8;

	unitVtblOffset[mapLynx] = 0xD0880;
	unitVtblOffset[mapPanther] = 0xD0938;
	unitVtblOffset[mapTiger] = 0xD05C0;

	unitVtblOffset[mapRoboSurveyor] = 0xD61A0;
	unitVtblOffset[mapRoboMiner] = 0xD0A10;
	unitVtblOffset[mapGeoCon] = 0xD0AC8;
	unitVtblOffset[mapScout] = 0xD0B80;
	unitVtblOffset[mapRoboDozer] = 0xCF7F8;
	unitVtblOffset[mapEvacuationTransport] = 0xD0C58;
	unitVtblOffset[mapRepairVehicle] = 0xD0D10;
	unitVtblOffset[mapEarthworker] = 0xD0DC8;
	unitVtblOffset[mapSmallCapacityAirTransport] = 0xD0E80;

	unitVtblOffset[mapTube] = 0xD0F20;
	unitVtblOffset[mapWall] = 0xD1040;
	unitVtblOffset[mapLavaWall] = 0xD1180;
	unitVtblOffset[mapMicrobeWall] = 0xD12A0;

	unitVtblOffset[mapCommonOreMine] = 0xD48A8;
	unitVtblOffset[mapRareOreMine] = 0xD13C0;

	unitVtblOffset[mapGuardPost] = 0xCFFB0;
	unitVtblOffset[mapLightTower] = 0xD1510;
	unitVtblOffset[mapCommonStorage] = 0xCF5F8;
	unitVtblOffset[mapRareStorage] = 0xCF6F8;
	unitVtblOffset[mapForum] = 0xD1670;
	unitVtblOffset[mapCommandCenter] = 0xD1790;
	unitVtblOffset[mapMHDGenerator] = 0xD18B0;
	unitVtblOffset[mapResidence] = 0xD19D0;
	unitVtblOffset[mapRobotCommand] = 0xD1AF0;
	unitVtblOffset[mapTradeCenter] = 0xD1C10;
	unitVtblOffset[mapBasicLab] = 0xD1D30;
	unitVtblOffset[mapMedicalCenter] = 0xD1E50;
	unitVtblOffset[mapNursery] = 0xD1F70;
	unitVtblOffset[mapSolarPowerArray] = 0xD2090;
	unitVtblOffset[mapRecreationFacility] = 0xD21B0;
	unitVtblOffset[mapUniversity] = 0xD22D0;
	unitVtblOffset[mapAgridome] = 0xCF0F0;
	unitVtblOffset[mapDIRT] = 0xD2410;
	unitVtblOffset[mapGarage] = 0xCFCA0;
	unitVtblOffset[mapMagmaWell] = 0xD2550;
	unitVtblOffset[mapMeteorDefense] = 0xD2680;
	unitVtblOffset[mapGeothermalPlant] = 0xD27A0;
	unitVtblOffset[mapArachnidFactory] = 0xD28C0;
	unitVtblOffset[mapConsumerFactory] = 0xD2B10;
	unitVtblOffset[mapStructureFactory] = 0xD2C48;
	unitVtblOffset[mapVehicleFactory] = 0xD2D80;
	unitVtblOffset[mapStandardLab] = 0xD2EB8;
	unitVtblOffset[mapAdvancedLab] = 0xD0260;
	unitVtblOffset[mapObservatory] = 0xD2FF8;
	unitVtblOffset[mapReinforcedResidence] = 0xD3118;
	unitVtblOffset[mapAdvancedResidence] = 0xD3238;
	unitVtblOffset[mapCommonOreSmelter] = 0xD3358;
	unitVtblOffset[mapSpaceport] = 0xD3478;
	unitVtblOffset[mapRareOreSmelter] = 0xD35B0;
	unitVtblOffset[mapGORF] = 0xD36D0;
	unitVtblOffset[mapTokamak] = 0xD37F0;

	unitVtblOffset[mapAcidCloud] = 0xCF058;
	unitVtblOffset[mapEMP] = 0xD6BF0;
	unitVtblOffset[mapLaser] = 0xD6C88;
	unitVtblOffset[mapMicrowave] = 0xD6D20;
	unitVtblOffset[mapRailGun] = 0xD6DB8;
	unitVtblOffset[mapRPG] = 0xD6E50;
	unitVtblOffset[mapStarflare] = 0xD6EE8;
	unitVtblOffset[mapSupernova] = 0xD7148;
	unitVtblOffset[mapStarflare2] = 0xD70B0;
	unitVtblOffset[mapSupernova2] = 0xD7148;
	unitVtblOffset[mapNormalUnitExplosion] = 0xD7018;
	unitVtblOffset[mapESG] = 0xD5F60;
	unitVtblOffset[mapStickyfoam] = 0xD71E0;
	unitVtblOffset[mapThorsHammer] = 0xD63A8;
	unitVtblOffset[mapEnergyCannon] = 0xD7310;

	unitVtblOffset[mapBlast] = 0xD3BF0;
	unitVtblOffset[0x4B] = 0xD7278;

	unitVtblOffset[mapLightning] = 0xD0388;
	unitVtblOffset[mapVortex] = 0xD6460;
	unitVtblOffset[mapEarthquake] = 0xCFB10;
	unitVtblOffset[mapEruption] = 0xD7430;
	unitVtblOffset[mapMeteor] = 0xD4710;

	unitVtblOffset[mapMiningBeacon] = 0xCF218;
	unitVtblOffset[mapMagmaVent] = 0xCF2D0;
	unitVtblOffset[mapFumarole] = 0xCF368;

	unitVtblOffset[mapWreckage] = 0xCF400;

	unitVtblOffset[mapDisasterousBuildingExplosion] = 0xD3E98;
	unitVtblOffset[mapCatastrophicBuildingExplosion] = 0xD3F60;
	unitVtblOffset[mapAtheistBuildingExplosion] = 0xD4028;

	unitVtblOffset[mapEDWARDSatellite] = 0x0A0A0A;
	unitVtblOffset[mapSolarSatellite] = 0x0A0A0A;
	unitVtblOffset[mapIonDriveModule] = 0x0A0A0A;
	unitVtblOffset[mapFusionDriveModule] = 0x0A0A0A;
	unitVtblOffset[mapCommandModule] = 0x0A0A0A;
	unitVtblOffset[mapFuelingSystems] = 0x0A0A0A;
	unitVtblOffset[mapHabitatRing] = 0x0A0A0A;
	unitVtblOffset[mapSensorPackage] = 0x0A0A0A;
	unitVtblOffset[mapSkydock] = 0x0A0A0A;
	unitVtblOffset[mapStasisSystems] = 0x0A0A0A;
	unitVtblOffset[mapOrbitalPackage] = 0x0A0A0A;
	unitVtblOffset[mapPhoenixModule] = 0x0A0A0A;

	unitVtblOffset[mapRareMetalsCargo] = 0x0A0A0A;
	unitVtblOffset[mapCommonMetalsCargo] = 0x0A0A0A;
	unitVtblOffset[mapFoodCargo] = 0x0A0A0A;
	unitVtblOffset[mapEvacuationModule] = 0x0A0A0A;
	unitVtblOffset[mapChildrenModule] = 0x0A0A0A;

	unitVtblOffset[mapSULV] = 0xD4300;
	unitVtblOffset[mapRLV] = 0xD43A8;
	unitVtblOffset[mapEMPMissile] = 0xD4450;

	unitVtblOffset[mapImpulseItems] = 0x0A0A0A;
	unitVtblOffset[mapWares] = 0x0A0A0A;
	unitVtblOffset[mapLuxuryWares] = 0x0A0A0A;

	unitVtblOffset[mapInterColonyShuttle] = 0xD45A8;
	unitVtblOffset[mapSpider3Pack] = 0x0A0A0A;
	unitVtblOffset[mapScorpion3Pack] = 0x0A0A0A;

	unitVtblOffset[mapPrettyArt] = 0xD4670;

	for (int i = 0; i < numof(unitVtbl); i ++)
	{
		if (unitVtblOffset[i] != NULL)
		{
			if (unitVtblOffset[i] != 0x0A0A0A)
			{
				unitVtbl[i] = (OP2Unit::OP2UnitVtbl*)(imageBase + unitVtblOffset[i]);
				oldUnitVtbl[i] = *unitVtbl[i];
			}
			unitInfoVtbl[i] = unitInfoArray[i]->vtbl;
			oldUnitInfoVtbl[i] = *unitInfoVtbl[i];
		}
	}
}