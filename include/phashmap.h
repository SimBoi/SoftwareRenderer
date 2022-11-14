/******************************************************************************
* IrtPHashMap.h - Header file for simple pointers hash map.		      *
*******************************************************************************
* (C) Gershon Elber, Technion, Israel Institute of Technology                 *
*******************************************************************************
* Written by Fady Massarwa, Jul. 2016.					      *
******************************************************************************/

#ifndef MISC_P_HASH_MAP_H
#define MISC_P_HASH_MAP_H

typedef struct MiscPHashMapStruct *MiscPHashMap;
typedef void *MiscPHashMapKey;
typedef void *MiscPHashMapValue;
typedef void (*MiscPHashMapCBFunc)(MiscPHashMap HMap, 
				   MiscPHashMapKey Key, 
				   MiscPHashMapValue Value);

typedef enum {
    MISCP_HMSUCCESS = 0,
    MISCP_HMFAILURE
} MiscPHMResult;

MiscPHashMap MiscPHashMapCreate(int HashSize);
void MiscPHashMapDelete(MiscPHashMap HMap);
MiscPHMResult MiscPHashMapSet(MiscPHashMap HMap, 
			      MiscPHashMapKey Key, 
			      MiscPHashMapValue Value);
MiscPHMResult MiscPHashMapGet(MiscPHashMap HMap, 
			      MiscPHashMapKey Key, 
			      MiscPHashMapValue *Value);
MiscPHMResult MiscPHashMapRemove(MiscPHashMap HMap, MiscPHashMapKey Key);
void MiscPHashMapPrint(MiscPHashMap HMap);
void MiscPHashMapIterate(MiscPHashMap HMap, MiscPHashMapCBFunc CBFunc);

#endif /* IRT_P_HASH_MAP_H */
