#include <main.h>

static std::vector<PartitionBlock> partitionBlocks;

static uint32_t next = 0;

int part(FILE *f) {
	off_t here = ftell(f);

	PartitionBlock pb;

	if ( fread(&pb, sizeof(PartitionBlock), 1, f) <= 0 ) {
		fprintf(stderr, "rdsk:ads ad asd asd n't read partition header (%s)\n", strerror(errno));
		exit(1);
	}

#ifndef __BIG_ENDIAN__
	SwapEndianPB(pb);
#endif

	partitionBlocks.push_back(pb);

	next = pb.pb_Next;

	return (pb.pb_ID == IDNAME_PARTITION) ? 1 : 0;
}

int main(int argc, char **argv) {
	FILE *disk;

	if (argc < 2) {
		fprintf(stderr, "RDS: Usage ./RDBParser <PATH-TO-HARDFILE/DEVICE>\n");
		exit(1);
	}

	disk = fopen(argv[1], "r");
	if ( !disk ) {
		fprintf(stderr, "RDS: Can't open file (%s)\n", strerror(errno));
		exit(1);
	}

	fseek(disk, 0, SEEK_SET);

	RigidDiskBlock rdb{};

	if ( fread(&rdb, sizeof(RigidDiskBlock), 1, disk) <= 0 ) {
		fprintf(stderr, "RDSK: Can't read disk header (%s)\n", strerror(errno));
		exit(1);
	}

	if ( rdb.rdb_ID == IDNAME_RIGIDDISK) {

		printf("[%s]:\n", (unsigned char*)&rdb.rdb_ID);

#ifndef __BIG_ENDIAN__
		SwapEndianRDB(rdb);
#endif

		printf("Disk Vendor:\t%s\n", rdb.rdb_DiskVendor);
		printf("Drive Name:\t%s\n", rdb.rdb_DriveInitName);

		printf("Partition should be at block %d, Block size: %d\n", rdb.rdb_PartitionList, rdb.rdb_BlockBytes);

		next = rdb.rdb_PartitionList;

		fseek(disk, rdb.rdb_PartitionList * rdb.rdb_BlockBytes, SEEK_SET);

		do {
			printf("Next Partition:\t%d\n", next);

			if ( next != -1 ) fseek(disk, next * rdb.rdb_BlockBytes, SEEK_SET);
			else break;
		} while (part(disk));

		for ( auto p: partitionBlocks ) {
			printf("Partition ID:\t%s\n", p.pb_DriveName);
		}
	}
	return 0;
}
