
#pragma once

#include <cstdint>

static constexpr uintptr_t MapObjectVtbl[] = {
  0x00000000,  // mapNone
  0x004CF4A8,  // mapCargoTruck
  0x004CF990,  // mapConVec
  0x004D0710,  // mapSpider
  0x004D07C8,  // mapScorpion

  0x004D0880,  // mapLynx
  0x004D0938,  // mapPanther
  0x004D05C0,  // mapTiger

  0x004D61A0,  // mapRoboSurveyor
  0x004D0A10,  // mapRoboMiner
  0x004D0AC8,  // mapGeoCon
  0x004D0B80,  // mapScout
  0x004CF7F8,  // mapRoboDozer
  0x004D0C58,  // mapEvacuationTransport
  0x004D0D10,  // mapRepairVehicle
  0x004D0DC8,  // mapEarthworker
  0x004D0E80,  // mapSmallCapacityAirTransport

  0x004D0F20,  // mapTube
  0x004D1040,  // mapWall
  0x004D1180,  // mapLavaWall
  0x004D12A0,  // mapMicrobeWall

  0x004D48A8,  // mapCommonOreMine
  0x004D13C0,  // mapRareOreMine

  0x004CFFB0,  // mapGuardPost
  0x004D1510,  // mapLightTower
  0x004CF5F8,  // mapCommonStorage
  0x004CF6F8,  // mapRareStorage
  0x004D1670,  // mapForum
  0x004D1790,  // mapCommandCenter
  0x004D18B0,  // mapMHDGenerator
  0x004D19D0,  // mapResidence
  0x004D1AF0,  // mapRobotCommand
  0x004D1C10,  // mapTradeCenter
  0x004D1D30,  // mapBasicLab
  0x004D1E50,  // mapMedicalCenter
  0x004D1F70,  // mapNursery
  0x004D2090,  // mapSolarPowerArray
  0x004D21B0,  // mapRecreationFacility
  0x004D22D0,  // mapUniversity
  0x004CF0F0,  // mapAgridome
  0x004D2410,  // mapDIRT
  0x004CFCA0,  // mapGarage
  0x004D2550,  // mapMagmaWell
  0x004D2680,  // mapMeteorDefense
  0x004D27A0,  // mapGeothermalPlant
  0x004D28C0,  // mapArachnidFactory
  0x004D2B10,  // mapConsumerFactory
  0x004D2C48,  // mapStructureFactory
  0x004D2D80,  // mapVehicleFactory
  0x004D2EB8,  // mapStandardLab
  0x004D0260,  // mapAdvancedLab
  0x004D2FF8,  // mapObservatory
  0x004D3118,  // mapReinforcedResidence
  0x004D3238,  // mapAdvancedResidence
  0x004D3358,  // mapCommonOreSmelter
  0x004D3478,  // mapSpaceport
  0x004D35B0,  // mapRareOreSmelter
  0x004D36D0,  // mapGORF
  0x004D37F0,  // mapTokamak

  0x004CF058,  // mapAcidCloud
  0x004D6BF0,  // mapEMP
  0x004D6C88,  // mapLaser
  0x004D6D20,  // mapMicrowave
  0x004D6DB8,  // mapRailGun
  0x004D6E50,  // mapRPG
  0x004D6EE8,  // mapStarflare
  0x004D7148,  // mapSupernova
  0x004D70B0,  // mapStarflare2
  0x004D7148,  // mapSupernova2
  0x004D7018,  // mapNormalExplosion
  0x004D5F60,  // mapESG
  0x004D71E0,  // mapStickyfoam
  0x004D63A8,  // mapThorsHammer
  0x004D7310,  // mapEnergyCannon

  0x004D3BF0,  // mapBlast
  0x004D7278,  // mapBFG

  0x004D0388,  // mapLightning
  0x004D6460,  // mapVortex
  0x004CFB10,  // mapEarthquake
  0x004D7430,  // mapEruption
  0x004D4710,  // mapMeteor

  0x004CF218,  // mapMiningBeacon
  0x004CF2D0,  // mapMagmaVent
  0x004CF368,  // mapFumarole

  0x004CF400,  // mapWreckage

  0x004D3E98,  // mapDisasterousBuildingExplosion
  0x004D3F60,  // mapCatastrophicBuildingExplosion
  0x004D4028,  // mapAtheistBuildingExplosion

  0x00000000,  // mapEDWARDSatellite
  0x00000000,  // mapSolarSatellite
  0x00000000,  // mapIonDriveModule
  0x00000000,  // mapFusionDriveModule
  0x00000000,  // mapCommandModule
  0x00000000,  // mapFuelingSystems
  0x00000000,  // mapHabitatRing
  0x00000000,  // mapSensorPackage
  0x00000000,  // mapSkydock
  0x00000000,  // mapStasisSystems
  0x00000000,  // mapOrbitalPackage
  0x00000000,  // mapPhoenixModule

  0x00000000,  // mapRareMetalsCargo
  0x00000000,  // mapCommonMetalsCargo
  0x00000000,  // mapFoodCargo
  0x00000000,  // mapEvacuationModule
  0x00000000,  // mapChildrenModule

  0x004D4300,  // mapSULV
  0x004D43A8,  // mapRLV
  0x004D4450,  // mapEMPMissile

  0x00000000,  // mapImpulseItems
  0x00000000,  // mapWares
  0x00000000,  // mapLuxuryWares

  0x004D45A8,  // mapInterColonyShuttle
  0x00000000,  // mapSpider3Pack
  0x00000000,  // mapScorpion3Pack

  0x004D4670   // mapPrettyArt
};
