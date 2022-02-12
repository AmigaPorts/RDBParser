#ifndef RDBTEST_MAIN_H
#define RDBTEST_MAIN_H


#include <devices/hardblocks.h>

#include <cstring>
#include <cerrno>
#include <cstdlib>
#include <netinet/in.h>
#include <vector>

#include <IConfig.h>


#define MAKE_ID(a, b, c, d)		((int32_t) ((int32_t) (d) << 24) | ((int32_t) (c) << 16) | ((int32_t) (b) <<  8) | (int32_t)(a))

#ifdef IDNAME_RIGIDDISK
#undef IDNAME_RIGIDDISK
#endif //IDNAME_RIGIDDISK
#define IDNAME_RIGIDDISK		MAKE_ID('R','D','S','K')

#ifdef IDNAME_PARTITION
#undef IDNAME_PARTITION
#endif //IDNAME_PARTITION
#define IDNAME_PARTITION		MAKE_ID('P','A','R','T')

#define SwapEndian(x)			x = ntohl(x)

#define SwapEndianPBlock(x)		SwapEndian(pBlock.x)

inline void SwapEndianRDB(RigidDiskBlock &pBlock);

inline void SwapEndianRDB(RigidDiskBlock &pBlock) {
	//SwapEndianPBlock(rdb_ID);
	SwapEndianPBlock(rdb_SummedLongs);
	SwapEndianPBlock(rdb_ChkSum);
	SwapEndianPBlock(rdb_HostID);
	SwapEndianPBlock(rdb_BlockBytes);
	SwapEndianPBlock(rdb_Flags);
	SwapEndianPBlock(rdb_BadBlockList);
	SwapEndianPBlock(rdb_PartitionList);
	SwapEndianPBlock(rdb_FileSysHeaderList);


	SwapEndianPBlock(rdb_DriveInit);
	for ( int i = 0; i < 6; i++ )
		SwapEndianPBlock(rdb_Reserved1[i]);

	SwapEndianPBlock(rdb_Cylinders);
	SwapEndianPBlock(rdb_Sectors);
	SwapEndianPBlock(rdb_Heads);
	SwapEndianPBlock(rdb_Interleave);
	SwapEndianPBlock(rdb_Park);

	for ( int i = 0; i < 3; i++ )
		SwapEndianPBlock(rdb_Reserved2[i]);

	SwapEndianPBlock(rdb_WritePreComp);
	SwapEndianPBlock(rdb_ReducedWrite);
	SwapEndianPBlock(rdb_StepRate);

	for ( int i = 0; i < 5; i++ )
		SwapEndianPBlock(rdb_Reserved3[i]);

	SwapEndianPBlock(rdb_RDBBlocksLo);
	SwapEndianPBlock(rdb_RDBBlocksHi);
	SwapEndianPBlock(rdb_LoCylinder);
	SwapEndianPBlock(rdb_HiCylinder);
	SwapEndianPBlock(rdb_CylBlocks);
	SwapEndianPBlock(rdb_AutoParkSeconds);
	SwapEndianPBlock(rdb_HighRDSKBlock);
	SwapEndianPBlock(rdb_Reserved4);

	/*
	for ( int i = 0; i < 10; i++ )
		SwapEndianPBlock(rdb_Reserved5[i]);
	 */
}


inline void SwapEndianPB(PartitionBlock &pBlock);

inline void SwapEndianPB(PartitionBlock &pBlock) {
	//SwapEndianPBlock(pb_ID);
	SwapEndianPBlock(pb_SummedLongs);
	SwapEndianPBlock(pb_ChkSum);
	SwapEndianPBlock(pb_HostID);
	SwapEndianPBlock(pb_Next);
	SwapEndianPBlock(pb_Flags);

	for ( int i = 0; i < 2; i++ )
		SwapEndianPBlock(pb_Reserved1[i]);

	SwapEndianPBlock(pb_DevFlags);

	for ( int i = 0; i < 15; i++ )
		SwapEndianPBlock(pb_Reserved2[i]);

	for ( int i = 0; i < 17; i++ )
		SwapEndianPBlock(pb_Environment[i]);

	for ( int i = 0; i < 15; i++ )
		SwapEndianPBlock(pb_EReserved[i]);
}

#endif //RDBTEST_MAIN_H