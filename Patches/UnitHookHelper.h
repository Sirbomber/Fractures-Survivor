#ifndef UNITHOOKHELPER_H
#define UNITHOOKHELPER_H

#include "OP2Types\OP2Units.h"

extern OP2Unit::OP2UnitVtbl oldUnitVtbl[115], *unitVtbl[115];
extern OP2UnitInfo::OP2UnitInfoVtbl oldUnitInfoVtbl[115], *unitInfoVtbl[115];

void InitUnitVtblAddresses();

#endif