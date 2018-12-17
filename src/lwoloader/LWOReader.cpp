#include ".\lworeader.h"

LWOReader::LWOReader(void)
{
}

LWOReader::~LWOReader(void)
{
}
int LWOReader::readLWO(char *filename, LWOobj &object) {
	FILE  *file;
    U4	  datasize, bytesread = 0L;
    U4	  type, size;
    ID4   id;
    
	/* Open the LWO2 file */
    file = fopen(filename, "rb");
	
    /* Make sure the Lightwave file is an IFF file. */
	read_id4(id, file);
	type = MAKE_ID(id[0],id[1],id[2],id[3]);
    if (type != ID_FORM)
      error("Not an IFF file (Missing FORM tag)");

	read_u4(&datasize, 1, file);

    printf("FORM [%d]\n", datasize);

    /* Make sure the IFF file has a LWO2 form type. */
	bytesread += read_id4(id, file);
	type = MAKE_ID(id[0],id[1],id[2],id[3]);
    if (type != ID_LWO2)
      error("Not a lightwave object (Missing LWO2 tag)");

    printf("LWO2\n");

    /* Read all Lightwave chunks. */

    while (bytesread < datasize) {

	  bytesread += read_id4(id, file);
	  bytesread += read_u4(&size, 1, file);

	  type = MAKE_ID(id[0],id[1],id[2],id[3]);

      switch (type) {
        case ID_TAGS:	read_tags(size, file, object); break;
        case ID_CLIP:	read_clip(size, file, object); break;
        case ID_ENVL:	read_envl(size, file); break;
        case ID_LAYR:	read_layr(size, file); break;
        case ID_PNTS:	read_pnts(size, file, object); break;
        case ID_BBOX:	read_bbox(size, file, object); break;
        case ID_POLS:	read_pols(size, file, object); break;
        case ID_PTAG:	read_ptag(size, file, object); break;
        case ID_VMAP:	read_vmap(size, file, object); break;
		case ID_VMAD:	read_vmad(size, file, object); break;
        case ID_SURF:	read_surf(size, file, object); break;
        default:
		  seek_pad(size, file);
          //printf("%s [%d]\n", id, size);
          break;
      }

      bytesread += size;

    }
	return 1;
}